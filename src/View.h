// View.h
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

#ifndef INCLUDED_View_H
#define INCLUDED_View_H

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif


class View{
   std::string d_message;
	std::string d_programurl;
	std::string d_secureurl;
	std::string d_title;
	std::string d_charset;
	bool d_translate;

protected:

	void setTitle(const char *title);
	void requiresTranslation(bool doesit);
	void printSafeHTML(const char *text, std::ostream &);
	void setMessage(const char *file, int line, const char *messagetype, const char *messagecode,  const char *message,  const char *diagnostic);
   int checkPermissions(const char *username, const char *perms);
   int checkResourcePermissions(const char *resourcetype, const char *resourceid, const char *username, const char *perms);
		
	View();

public:

	virtual void setProgramUrl(const char *url);
	virtual void setSecureProgramUrl(const char *url);
	virtual void setCharset(const char *charset);
	virtual void printHeaders();

	void traceMessage(const char *file, int line , const char *message);
	virtual const char *getMessage();
	
	// Returns true if xslt translation needs to occur (pass ostream to display)
	// Returns false if no translation needs to occur (pass std::cout to display)
	
	virtual bool requiresTranslation();
	
	virtual void display(std::ostream &) = 0;

	virtual ~View();

	const char *getProgramUrl();
	const char *getSecureProgramUrl();
	virtual const char *getTitle();
	virtual const char *getCharset();
};

#endif

