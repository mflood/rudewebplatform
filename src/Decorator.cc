// Decorator.cpp
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

#include "Decorator.h"
#include "ViewFactory.h"


#include <string>

using namespace std;

namespace view{

Decorator::Decorator(View *childview)
{
	d_childview = childview;
	d_message = "";
}

Decorator::~Decorator()
{
	delete d_childview;
}


void Decorator::insertDecorator(const char *decoratorname)
{
	ViewFactory *factory = ViewFactory::instance();
	View *newview = factory->decorateView(d_childview, decoratorname);
   if(newview)
	{
           d_childview = newview;
	}
	else
	{
          setMessage(__FILE__, __LINE__, "error", "LIB", "Could not insert decorator", decoratorname);
	}
}


const char * Decorator::getMessage()
{
	d_message = View::getMessage();
	if(d_childview)
	{
	 	d_message += d_childview->getMessage();
   }
	return d_message.c_str();
}

void Decorator::display(std::ostream & outstream)
{
	if(d_childview)
	{
		d_childview->display(outstream);
	}
}

void Decorator::setProgramUrl(const char * url)
{
	View::setProgramUrl(url);
	if(d_childview)
	{
		d_childview->setProgramUrl(url);
	}
}
void Decorator::setSecureProgramUrl(const char * url)
{
	View::setSecureProgramUrl(url);
	if(d_childview)
	{
		d_childview->setSecureProgramUrl(url);
	}
}

const char *Decorator::getTitle()
{
	if(d_childview)
	{
		return d_childview->getTitle();
	}
	else
	{
		return View::getTitle();
	}
}

void Decorator::setCharset(const char *charset)
{
	View::setCharset(charset);
	if(d_childview)
	{
		d_childview->setCharset(charset);
	}
}

const char * Decorator::getCharset()
{
	
	if(d_childview)
	{
		return d_childview->getCharset();
	}
	else
	{
      return View::getCharset();
	}
}

void Decorator::printHeaders()
{
	if(d_childview)
	{
		d_childview->printHeaders();
	}
}

bool Decorator::requiresTranslation()
{
	if(d_childview)
	{
		return d_childview->requiresTranslation();
	}
	else
	{
		return View::requiresTranslation();
	}
}

}

