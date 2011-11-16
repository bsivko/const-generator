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

#include <iostream>
#include <string>

#include <stdexcept>

#include <const_generator/h/cfg.hpp>
#include <const_generator/h/cmd_parser.hpp>
#include <const_generator/h/generator.hpp>

int
main( int argc, char ** argv )
{
	try
	{
		const_generator::cfg_t cfg = 
			const_generator::tclap_parse( argc, argv );

		cfg.check();

		const_generator::generate( cfg );
	}
	catch ( const std::exception & ex )
	{
		std::cerr << "Error: '" << ex.what() << "'" << std::endl;
	}
}