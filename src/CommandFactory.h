//Commandfactory.h
//
// Copyright (C) 2005 Matthew Flood
// See file AUTHORS for contact information
// 
// This file is part of RudeWebPlatform.
// 
// RudeWebPlatform is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// RudeWebPlatform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with RudeWebPlatform; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
// 
//------------------------------------------------------------------------

#ifndef INCLUDED_COMMANDFACTORY_H
#define INCLUDED_COMMANDFACTORY_H

#include "Command.h"
#include <string>
#include <vector>

class CommandFactory{

	std::string d_libdir;
	std::vector<void*> d_dlhandles;
	static CommandFactory *s_instance;
	CommandFactory();
public:
	static CommandFactory *instance();
	~CommandFactory();
	Command *buildCommand(const char *commandname);
	void setLibDir(const char *path);

};

#endif

