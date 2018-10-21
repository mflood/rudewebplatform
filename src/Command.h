// Command.h
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

#ifndef INCLUDED_COMMAND_H
#define INCLUDED_COMMAND_H

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif


namespace rude{
namespace server{
class Message;
}}

class Command{

	std::string d_message;
	
protected:

   void setError(const char *error);
	
	void setMessage(const char *message);
	
	Command();
		
	void setMessage(const char *file, int line, const char *type, const char *code, const char *message, const char *diagnostic);

	void traceMessage(const char *file, int line , const char *message);
	
	char *fixTextareaData(const char *data);
	
   int checkPermissions(const char *username, const char *perms);
   int checkResourcePermissions(const char *resourcetype, const char *resourceid, const char *username, const char *perms);


public:

	virtual bool execute() = 0;

	const char *getError() const;
	
	const char *getMessage() const; 

	virtual ~Command();
};

#endif

