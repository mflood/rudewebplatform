// CommandError.h
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


#ifndef INCLUDED_CommandError_H
#define INCLUDED_CommandError_H

#ifndef INCLUDED_COMMAND_H
#include "Command.h"
#endif

#include <string>

namespace command{

class CommandError: public Command{
	std::string d_message;

public:
	CommandError(const char *filename, int linenumber, const char *errormessage);
	bool execute();

};
}
#endif

