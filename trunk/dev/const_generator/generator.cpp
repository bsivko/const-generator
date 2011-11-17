/*
	Copyright (c) 2011
	All rights reserved.

	Author: bsivko@gmail.com (Boris Sivko)

	This file is part of Const Generator.

	Const Generator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <const_generator/h/generator.hpp>

#include <stdexcept>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

namespace const_generator {

//! Ext of file for interface.
const std::string gc_hpp_ext = "hpp";
//! Ext of file for impl.
const std::string gc_cpp_ext = "cpp";

//! Create name of class with using prefixes and postfixes.
inline std::string
create_class_name( const std::string & name )
{
	return name + "_t";
}

//! Create name of overall class.
inline std::string
create_modes_class_name( const std::string & root_name )
{
	return create_class_name( root_name + "_" + "modes" );;
}

//! Make string from vector by delimiters.
/*!
	\note result by next view: 
		prefix + value[0] + delimiter + 
		prefix + value[1] + delimiter + 
		...
		prefix + value[n-1]

	\return string in described format.
*/
std::string
collect_by_delimiters( 
	//! Elements to insert.
	const std::vector<std::string> & values, 
	//! Prefix before every element.
	const std::string & prefix, 
	//! Delimiter between elements.
	const std::string & delimiter )
{
	std::string result;

	for( unsigned int i = 0; i < values.size(); ++i )
	{
		result += prefix + values[i];
		if ( i != values.size() - 1 )
			result += delimiter;
	}

	return result;
}

//! Open file and check that it ok.
/*!
	\thow std::exception if open fail.
*/
void
open_and_check( 
	//! File to open.
	std::ofstream & file, 
	//! Name of file.
	const std::string & name )
{
	file.open( name.c_str(), std::ios::out );

	if ( !file.good() )
	{
		throw std::runtime_error( 
			"File open error to write. Filename:'" + name + "'" );
	}
}

//! Make namespaces of head of file.
void
make_head_namespaces( 
	//! Stream-receiver.
	std::ostream & o, 
	//! Cfg to make.
	const cfg_t & cfg )
{
	o << 
		collect_by_delimiters( cfg.m_namespaces, "namespace ", " { \n\n" ) << 
		" {\n\n" << std::endl;
}

//! Make namespaces of foot of file.
void
make_foot_namespaces( 
	//! Stream-receiver.
	std::ostream & o, 
	//! Cfg to make.
	const cfg_t & cfg )
{
	cfg_t::namespaces_t namespaces;
	
	for( unsigned int i = cfg.m_namespaces.size(); i > 0; --i )
		namespaces.push_back( cfg.m_namespaces[i-1] );

	o << 
		collect_by_delimiters( namespaces, "} /* namespace ", " */\n\n" ) << 
		" */\n\n" << std::endl;
}

//! Get hpp-class as string.
/*!
	\return string for hpp-description of type.
*/
std::string
make_hpp_classtype_as_string( 
	//! Name of class.
	const std::string & name,
	//! Name of base type.
	const std::string & root_name,
	//! Cfg of creating.
	const cfg_t & cfg )
{
	std::ostringstream result;

	const std::string root_postfix = 
		cfg.m_include_root_in_type ? "" : "_" + root_name;

	const std::string class_name = 
		create_class_name(name + root_postfix);

	result << 
		"//! Description of type.\n"
		"class " << class_name << " : public " << create_class_name(root_name) << "\n"
		"{\n" << 
		"\tpublic:\n" <<
		"\t\tvirtual std::string\n" <<
		"\t\ttype() const \n" <<
		"\t\t{\n"
		"\t\t\treturn \"" << name << "\";\n" <<
		"\t\t}\n"
		"};\n\n";

	return result.str();
}

//! Create overall class.
std::string
make_hpp_modes_type( 
	//! Name of base class.
	const std::string & root_name,
	//! Cfg of making.
	const cfg_t & cfg )
{

	std::ostringstream result;

	const std::string modes_name = create_modes_class_name( root_name );

	result << 
		"//! Type of variety all possible types.\n"
		"class " << modes_name << " {\n"
		"\tprotected:\n"
		"\t\t//! Static class.\n"
		"\t\t" << modes_name << "();\n\n"
		"\tpublic:\n"
		"\t\tvirtual\n"
		"\t\t~" << modes_name << "();\n\n"
		"\t\t//! Form list " << root_name << " by delimiter.\n"
		"\t\tstatic std::string\n"
		"\t\tget_list( const std::string & delimiter = \", \" );\n\n"
		"\t\t//! Check of presenting type by string.\n"
		"\t\tstatic bool\n"
		"\t\texist_type( const std::string & name );\n\n"
		"\t\t//! Factory of creating " << root_name << " by name-string.\n"
		"\t\tstatic const " << create_class_name( root_name ) << " *\n" 
		"\t\tfactory( const std::string & name );\n\n"
		"\tprotected:\n\n"
		"\t\t//! Form variety of pairs all possible types.\n" 
		"\t\tstatic void\n"
		"\t\tinit();\n\n"
		"\t\t//! Make clearing of types.\n"
		"\t\tstatic void\n"
		"\t\tdeinit();\n\n"
		"\tprivate:\n"
		"\t\ttypedef std::map <std::string, " << create_class_name( root_name ) << " *> container_t;\n\n"
		"\t\t//! Variety of links 'name type - description instance'.\n"
		"\t\tstatic container_t m_container;\n"
		"};\n\n";

	return result.str();
}

//! Form hpp in stream-receiver.
void
make_hpp_file( 
	//! Stream-receiver.
	std::ostream & o, 
	//! Cfg of make.
	const cfg_t & cfg )
{
	{
		std::string header_def = 
			collect_by_delimiters( cfg.m_namespaces, "__", "" ) + "__" +
			cfg.m_root_class_name + "_" + gc_hpp_ext + "__INCLUDED__";

		std::transform( header_def.begin(), header_def.end(), header_def.begin(), ::toupper );
		header_def = 
			"#ifndef " + header_def + "\n" +
			"#define " + header_def + "\n";
		o << header_def << std::endl;
	}

	o << "// To add new type " << cfg.m_root_class_name << ":\n"
		"// 1. Add sub-class from " << cfg.m_root_class_name << "_t.\n"
		"//    Create virtual method type() and all control methods (if need).\n"
		"// 2. Add in function " << 
			make_hpp_modes_type( cfg.m_root_class_name, cfg ) << 
			"::init() inserting to container by sample.\n" <<
		std::endl;

	o << 
		"#include <string>\n"
		"#include <map>\n\n";

	make_head_namespaces( o, cfg );

	o << 
		"//! Base type of constants.\n" 
		"class " << cfg.m_root_class_name << "_t\n"
		"{\n"
		"\tpublic:\n"
		"\t\t//! String representation of type.\n"
		"\t\tvirtual std::string\n"
		"\t\ttype() const = 0;\n"
		"\t\tvirtual\n"
		"\t\t~" + cfg.m_root_class_name + "_t();\n"
		"};\n\n";

	for( cfg_t::types_t::const_iterator it = cfg.m_types.begin(), 
		it_end = cfg.m_types.end(); it != it_end; ++it )
	{
		o << make_hpp_classtype_as_string( *it, cfg.m_root_class_name, cfg );
	}

	o << make_hpp_modes_type( cfg.m_root_class_name, cfg );

	make_foot_namespaces( o, cfg );

	o << "#endif" << std::endl;
}

//! Form cpp in stream-receiver.
void
make_cpp_file( 
	//! Stream-receiver.
	std::ostream & o, 
	//! Cfg to make.
	const cfg_t & cfg )
{
	const std::string modes_name = 
		create_modes_class_name( cfg.m_root_class_name );

	const std::string activator_name = 
		cfg.m_root_class_name + "_activator";
	const std::string activator_class_name = 
		create_modes_class_name( activator_name );

	{
		const std::string include_def = 
			"#include <" + collect_by_delimiters( cfg.m_namespaces, "", "/" ) + 
			"/h/" + cfg.m_root_class_name + "." + gc_hpp_ext + ">\n" + 
			"#include <stdexcept>\n\n";

		o << include_def << std::endl;
	}

	make_head_namespaces( o, cfg );

	o <<
		"" + cfg.m_root_class_name + "_t::~" + cfg.m_root_class_name + "_t() \n"
		"{\n"
		"}\n\n";

	o << 
		modes_name << "::container_t " << 
		modes_name << "::m_container;\n\n";

	o << "//! Instance for automatic creating "
		"and deleting variety of types.\n"
		"class " << activator_class_name << " : public " << modes_name << "\n"
		"{\n"
		"} " << activator_name << ";\n\n";

	o << 
		modes_name << "::" << modes_name << "()\n" <<
		"{\n" 
		"\tinit();\n"
		"}\n" << 
		std::endl;

	o << 
		modes_name << "::~" << modes_name << "()\n" <<
		"{\n" 
		"\tdeinit();\n"
		"}\n" << 
		std::endl;

	o <<
		"void\n" <<
		modes_name << "::deinit()\n"
		"{\n"
		"\tfor( container_t::iterator it = m_container.begin(), \n"
		"\t\tit_end = m_container.end(); it != it_end; ++it )\n"
		"\t{\n"
		"\t\tif ( it->second != 0 )\n"
		"\t\t{\n"
		"\t\t\tdelete it->second;\n"
		"\t\t}\n"
		"\t}\n"
		"}\n\n";

	{
		o <<
			"void\n" << 
			modes_name << "::init()\n"
			"{\n";

		for( cfg_t::types_t::const_iterator it = cfg.m_types.begin(), 
			it_end = cfg.m_types.end(); it != it_end; ++it )
		{
			const std::string root_postfix = 
				cfg.m_include_root_in_type ? "" : "_" + cfg.m_root_class_name;

			const std::string class_name = 
				create_class_name(*it + root_postfix);

			o << "\tm_container[ " << class_name << "().type() ] = \n"
				"\t\tnew " << class_name << "();" << std::endl;
		}

		o << "}\n\n";
	}

	o << 
		"std::string\n" <<
		modes_name << "::get_list( const std::string & delimiter )\n"
		"{\n"
		"\tstd::string result;\n\n"
		"\t\tfor( container_t::const_iterator it_begin = m_container.begin(),\n"
		"\t\tit_end = m_container.end(), it = it_begin; it != it_end; ++it )\n"
		"\t{\n"
		"\t\tif ( it != it_begin )\n"
		"\t\t\tresult += delimiter;\n\n"
		"\t\tresult += it->second->type();\n"
		"\t}\n\n"
		"\treturn result;\n"
		"}\n\n";

	o <<
		"bool\n" <<
		modes_name << "::exist_type( const std::string & name )\n"
		"{\n"
		"\treturn m_container.find( name ) != m_container.end();\n"
		"}\n\n";

	o << 
		"const " << create_class_name(cfg.m_root_class_name) << " *\n" << 
		modes_name << "::factory( const std::string & name )\n"
		"{\n"
		"\tif ( " << modes_name << "::exist_type( name ) )\n"
		"\t\treturn m_container.find( name )->second;\n"
		"\telse throw std::runtime_error( \n"
		"\t\t\"Unknown type of " << cfg.m_root_class_name << 
			" :'\" + name + \"'\" );\n\n"
		"}\n\n";

	make_foot_namespaces( o, cfg );
}

void
generate( const cfg_t & cfg )
{
	std::ofstream hpp_file;
	open_and_check( hpp_file, cfg.m_root_class_name + "." + gc_hpp_ext );

	std::ofstream cpp_file;
	open_and_check( cpp_file, cfg.m_root_class_name + "." + gc_cpp_ext );

	make_hpp_file( hpp_file, cfg );

	make_cpp_file( cpp_file, cfg );
}

} /* namespace const_generator */
