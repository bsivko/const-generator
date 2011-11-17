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

#include <tclap/CmdLine.h>
#include <tclap/UnlabeledMultiArg.h>
#include <tclap/SwitchArg.h>

#include <const_generator/h/cmd_parser.hpp>

#include <stdexcept>
#include <string>

namespace const_generator {

//! Split incoming string to parts by delimiter.
/*!
	\return vector of values in order of parsing.

	\note delimiters should be deleted.
*/
std::vector<std::string>
split_by_delimiter( 
	const std::string & value, 
	const std::string delimiter = std::string(",") )
{
	std::vector<std::string> result;

	// Find all delimiters.
	size_t pos = 0;
	std::vector<size_t> poses;
	while( value.size() > pos )
	{
		size_t to_find = value.find( delimiter, pos );
		if ( to_find == std::string::npos )
			break;

		poses.push_back( to_find );

		pos = to_find + delimiter.size();
	}

	// Split by delimiters.
	size_t start = 0;
	for( unsigned int i = 0; i < poses.size(); ++i )
	{
		result.push_back( value.substr( start, poses[i] - start ) );
		start = poses[i] + delimiter.size();
	}
	result.push_back( value.substr( start, value.size() - start ) );

	return result;
}

cfg_t
tclap_parse( int argc, char ** argv )
{
	cfg_t result;

	try
	{
		TCLAP::CmdLine cmd( "Constant Generator.", 
			' ', "1.0.0", true );

		TCLAP::ValueArg<std::string> namespace_arg(
			"n",
			"namespace",
			"Namespace of values. Namespaces must be split by '::'", 
			// not required.
			false, 
			"", 
			"string" );
		cmd.add( namespace_arg );

		TCLAP::ValueArg<std::string> root_name_arg(
			"r",
			"root-class-name",
			"name of root class (without '_t') as class name (without spaces, tabs, start at symbol, etc.).",
			// not required.
			false, 
			"root", 
			"string" );
		cmd.add( root_name_arg );

		TCLAP::SwitchArg include_root_in_type_arg(
			"i",
			"include-root-in-type",
			"include name of root class in generated type names" );
		cmd.add( include_root_in_type_arg );

		TCLAP::UnlabeledMultiArg<std::string> types_args( 
			"types", 
			"child types at generation; one value - one type.",
			true,
			"string" );
		cmd.add( types_args );

		cmd.parse( argc, argv );

		result.m_namespaces = split_by_delimiter( namespace_arg.getValue(), "::" );
		result.m_root_class_name = root_name_arg.getValue();
		result.m_include_root_in_type = include_root_in_type_arg.getValue();
		result.m_types = types_args.getValue();
	}
	catch ( const TCLAP::ArgException &e )
	{
		throw std::runtime_error( 
			"cmd args parse error: '" + e.error() + "'"
			" for arg '" + e.argId() + "'" );
	}

	return result;
}

} /* namespace const_generator */
