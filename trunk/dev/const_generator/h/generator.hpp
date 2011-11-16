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

#ifndef __CONST_GENERATOR__GENERATOR__INCLUDED__
#define __CONST_GENERATOR__GENERATOR__INCLUDED__

#include <const_generator/h/cfg.hpp>

namespace const_generator {

//! Генерирует файлы типов на основании cfg.
void
generate( 
	//! Информация для генерации.
	const cfg_t & cfg );

} /* namespace const_generator */

#endif
