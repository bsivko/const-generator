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

//! Расширение файла для интерфейса.
const std::string gc_hpp_ext = "hpp";
//! Расширение файла для реализации.
const std::string gc_cpp_ext = "cpp";

//! Создать имя класса с учетом всех префиксов и постфиксов.
inline std::string
create_class_name( const std::string & name )
{
	return name + "_t";
}

//! Создать имя modes-класса.
inline std::string
create_modes_class_name( const std::string & root_name )
{
	return create_class_name( root_name + "_" + "modes" );;
}

//! Собрать строку из вектора по разделителям.
/*!
	\note сборка идет вида 
		prefix + value[0] + delimiter + 
		prefix + value[1] + delimiter + 
		...
		prefix + value[n-1]

	\return строка в заданном формате.
*/
std::string
collect_by_delimiters( 
	//! Элементы вставки.
	const std::vector<std::string> & values, 
	//! Префикс перед каждым элементом.
	const std::string & prefix, 
	//! Разделитель между элементами.
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

//! Открывает файл и проверяет, что с ним все в порядке при открытии.
/*!
	\thow std::exception в случае ошибок.
*/
void
open_and_check( 
	//! Объект на открытие.
	std::ofstream & file, 
	//! Имя файла.
	const std::string & name )
{
	file.open( name.c_str(), std::ios::out );

	if ( !file.good() )
	{
		throw std::runtime_error( 
			"File open error to write. Filename:'" + name + "'" );
	}
}

void
make_head_namespaces( 
	//! Поток-приемник формирования.
	std::ostream & o, 
	//! Когфигурация для сборки.
	const cfg_t & cfg )
{
	o << 
		collect_by_delimiters( cfg.m_namespaces, "namespace ", " { \n\n" ) << 
		" {\n\n" << std::endl;
}

void
make_foot_namespaces( 
	//! Поток-приемник формирования.
	std::ostream & o, 
	//! Когфигурация для сборки.
	const cfg_t & cfg )
{
	cfg_t::namespaces_t namespaces;
	
	for( unsigned int i = cfg.m_namespaces.size(); i > 0; --i )
		namespaces.push_back( cfg.m_namespaces[i-1] );

	o << 
		collect_by_delimiters( namespaces, "} /* namespace ", " */\n\n" ) << 
		" */\n\n" << std::endl;
}

//! Получить hpp-класс как стринг.
/*!
	\return строка для hpp-описания типа.
*/
std::string
make_hpp_classtype_as_string( 
	//! Имя класса, из которого получаем.
	const std::string & name,
	//! Имя базового типа.
	const std::string & root_name,
	//! Когфигурация для сборки.
	const cfg_t & cfg )
{
	std::ostringstream result;

	const std::string root_postfix = 
		cfg.m_include_root_in_type ? "" : "_" + root_name;

	const std::string class_name = 
		create_class_name(name + root_postfix);

	result << 
		"//! Описание типа.\n"
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

//! Создать объединящий функциональный класс.
std::string
make_hpp_modes_type( 
	//! Имя базового типа.
	const std::string & root_name,
	//! Когфигурация для сборки.
	const cfg_t & cfg )
{

	std::ostringstream result;

	const std::string modes_name = create_modes_class_name( root_name );

	result << 
		"//! Тип множества всех возможных типов.\n"
		"class " << modes_name << " {\n"
		"\tprotected:\n"
		"\t\t//! Static class.\n"
		"\t\t" << modes_name << "();\n\n"
		"\tpublic:\n"
		"\t\tvirtual\n"
		"\t\t~" << modes_name << "();\n\n"
		"\t\t//! Сформировать список " << root_name << " через разделитель.\n"
		"\t\tstatic std::string\n"
		"\t\tget_list( const std::string & delimiter = \", \" );\n\n"
		"\t\t//! Проверка на присутствие типа по строке.\n"
		"\t\tstatic bool\n"
		"\t\texist_type( const std::string & name );\n\n"
		"\t\t//! Фабрика для " << root_name << " через именованную строку.\n"
		"\t\tstatic const " << create_class_name( root_name ) << " *\n" 
		"\t\tfactory( const std::string & name );\n\n"
		"\tprotected:\n\n"
		"\t\t//! Сформировать множество пар всех возможных типов.\n" 
		"\t\tstatic void\n"
		"\t\tinit();\n\n"
		"\t\t//! Произвести очистку типов.\n"
		"\t\tstatic void\n"
		"\t\tdeinit();\n\n"
		"\tprivate:\n"
		"\t\ttypedef std::map <std::string, " << create_class_name( root_name ) << " *> container_t;\n\n"
		"\t\t//! Множество связок имя типа - экземпляр, описывающий тип.\n"
		"\t\tstatic container_t m_container;\n"
		"};\n\n";

	return result.str();
}

//! Сформировать hpp в поток-приемник.
void
make_hpp_file( 
	//! Поток-приемник формирования.
	std::ostream & o, 
	//! Конфигурация сборки.
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

	o << "// При добавлении нового типа " << cfg.m_root_class_name << ":\n"
		"// 1. Добавить класс-наследник от " << cfg.m_root_class_name << "_t.\n"
		"//    В нем прописать type() с соответствующим именем.\n"
		"// 2. Добавить в функцию init() вставку в контейнер по образцу.\n" <<
		std::endl;

	o << 
		"#include <string>\n"
		"#include <map>\n\n";

	make_head_namespaces( o, cfg );

	o << 
		"//! Базовый тип для констант.\n" 
		"class " << cfg.m_root_class_name << "_t\n"
		"{\n"
		"\tpublic:\n"
		"\t\t//! Стринговое представление типа.\n"
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

//! Сформировать cpp в поток-приемник.
void
make_cpp_file( 
	//! Поток-приемник формирования.
	std::ostream & o, 
	//! Конфигурация сборки.
	const cfg_t & cfg )
{
	const std::string modes_name = create_modes_class_name( cfg.m_root_class_name );

	const std::string activator_name = cfg.m_root_class_name + "_activator";
	const std::string activator_class_name = create_modes_class_name( activator_name );

	{
		std::string include_def = 
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

	o << "//! Класс-объект для автоматического создания "
		"и уничтожения множеств типов.\n"
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
