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

#ifndef __CONST_GENERATOR__CFG__INCLUDED__
#define __CONST_GENERATOR__CFG__INCLUDED__

#include <vector>

#include <stdexcept>

namespace const_generator {

//! Generation configuration.
struct cfg_t
{
	//! Name of the root class.
	std::string m_root_class_name;

	typedef std::vector<std::string> types_t;
	//! Constants for generation.
	types_t m_types;

	typedef std::vector<std::string> namespaces_t;
	//! User namespace.
	namespaces_t m_namespaces;

	//! Include in name root type or not. 
	/*!
		true - include
	*/
	bool m_include_root_in_type;

	//! Check this structure - correct or not.
	/*!
		\throw std::exception if incorrect.

		\note automatic do normalize()
	*/
	void
	check();

	//! Normalize data to internal requirements.
	/*!
		\note call must be after forming all cfg data.
	*/
	void
	normalize();

	cfg_t();
};

} /* namespace const_generator */

#endif
