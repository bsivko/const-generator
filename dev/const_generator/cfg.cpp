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

#include <const_generator/h/cfg.hpp>

#include <stdexcept>
#include <ctype.h>
#include <algorithm>
#include <string>

namespace const_generator {

cfg_t::cfg_t() : 
	m_root_class_name("root"),
	m_include_root_in_type( true )
{
}

//! Check name of type.
void
check_type( 
	//! Check this value.
	const std::string & value, 
	//! Description of error.
	const std::string & desc )
{
	if ( value.empty() )
	{
		throw std::runtime_error( 
			"Command args value error( " + desc + " ) " + 
			"Value is empty." );
	}

	if ( value.find( "\t" ) != std::string::npos )
	{
		throw std::runtime_error( 
			"Command args value error( " + desc + " ) "
			"Value have a tab at name." );
	}

	if ( value.find( "\n" ) != std::string::npos | 
		value.find( "\r" ) != std::string::npos )
	{
		throw std::runtime_error( 
			"Command args value error( " + desc + " ) " + 
			"Value have a \\n or \\r at name." );
	}

	if ( value.find( " " ) != std::string::npos )
	{
		throw std::runtime_error( 
			"Command args value error( " + desc + " ) " + 
			"Value have space at name." );
	}

	if ( isdigit(value[0]) )
	{
		throw std::runtime_error( 
			"Command args value error( " + desc + " ) " + 
			"Value have digit symbol at first element." );
	}
}

void
cfg_t::normalize()
{
	std::sort( m_types.begin(), m_types.end() );
}

void
cfg_t::check()
{
	normalize();

	for( unsigned int i = 0; i < m_namespaces.size(); ++i )
	{
		if ( m_namespaces[i].empty() )
		{
			throw std::runtime_error( 
				"Command args value error. "
				"One of namespace values is empty." );
		}
	}

	if ( m_types.empty() )
	{
		throw std::runtime_error( 
			"Command args value error. "
			"Must be one type at last." );
	}


	check_type( m_root_class_name, "root_class_name" );

	for( unsigned int i = 0; i < m_types.size(); ++i )
		check_type( m_types[i], m_types[i] );

	// Check to the same elements.
	for( unsigned int i = 0; i < m_types.size()-1; ++i )
	{
		if ( m_types[i] == m_types[i+1] )
		{
			throw std::runtime_error( 
				"Command args value error. "
				"Two equal types found: "
				"'" + m_types[i] + "'" );
		}
	}
}

} /* namespace const_generator */

