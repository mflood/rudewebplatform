// Decorator.h
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

#ifndef INCLUDED_DECORATOR_H
#define INCLUDED_DECORATOR_H

#ifndef INCLUDED_VIEW_H
#include "View.h"
#endif

namespace view{

class Decorator: public View
{
	View *d_childview;
	std::string d_message;

protected:
	Decorator(View *childview);
		
public:
	virtual ~Decorator();

	void insertDecorator(const char *decoratorname);
	
	virtual void setProgramUrl(const char *url);
	virtual void setSecureProgramUrl(const char *url);
	virtual void setCharset(const char *charset);
	virtual const char * getCharset();
	virtual void printHeaders();
	virtual bool requiresTranslation();	
	virtual void display(std::ostream &);
	virtual const char *getTitle();
	virtual const char *getMessage();
};

}

#endif

