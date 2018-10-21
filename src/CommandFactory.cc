// Commandfactory.cpp
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


#include "CommandFactory.h"
#include "CommandError.h"

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

#include <dlfcn.h>

extern Command* returnCommand(void); 

using namespace std;
using namespace command;

CommandFactory * CommandFactory::s_instance = 0;


CommandFactory *CommandFactory::instance()
{
	if(s_instance == 0)
	{
		s_instance = new CommandFactory();
	}
	return s_instance;
}
Command *CommandFactory::buildCommand(const char *commandname)
{
	if(commandname && commandname[0] != 0)
	{
		void *handle;
		char *error;
		Command* (*returnCommand)(void);

		if(strchr(commandname,'/'))
		{
			command::CommandError *err = new command::CommandError(__FILE__, __LINE__, "Illegal Characters in Lib Name");
			return err;
		}

		string commandpath;
		if(d_libdir == "")
		{
			commandpath=commandname;
		}
		else
		{
			commandpath = d_libdir + "/lib" + commandname + ".so";
		}

		handle=::dlopen(commandpath.c_str(),RTLD_LAZY);
		if(error = ::dlerror())
		{
			// try to load it from public directories
			//
			commandpath = "/usr/lib/rude/commands/";
			commandpath += "lib";
			commandpath += commandname;
			commandpath += ".so";
			
			handle=::dlopen(commandpath.c_str(), RTLD_LAZY);
			if(error = ::dlerror())
			{
				std::string message = "Could not load command library '";
				message += commandname;
				message += "' at path '";
				message +=  commandpath;
				message += "'\n";
				message += error;
				message += "\n";
	
				command::CommandError *err = new command::CommandError( __FILE__, __LINE__, message.c_str());
				return err;
			}
		}

		d_dlhandles.push_back(handle);	

		returnCommand = (Command*(*)(void)) dlsym(handle,"returnCommand");
	
		if(error = ::dlerror())
		{
			std::string message = "Could not obtain address of returnObject function for '";
			message += commandname;
			message += "' at path '";
			message +=  commandpath;
			message += "'\n";
			message += error;
			message += "\n";

			command::CommandError *err = new command::CommandError( __FILE__, __LINE__, message.c_str());
			return err;
		}

		Command *returncommand = returnCommand();

		return returncommand;

		//::dlclose(handle); // Do this in the destructor - cannot delete before we use the object!
	}
	return 0;
}


void CommandFactory::setLibDir(const char *path)
{
	d_libdir = path ? path : "";
}

CommandFactory::CommandFactory()
{
	d_libdir="";
}

CommandFactory::~CommandFactory()
{
	int len=d_dlhandles.size();
	for(int x=0; x< len; x++)
	{
		::dlclose(d_dlhandles[x]); 
	}
}
