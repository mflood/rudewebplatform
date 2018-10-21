//ViewFactory.h
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

#ifndef INCLUDED_ViewFactory_H
#define INCLUDED_ViewFactory_H

#include <string>
#include <map>
#include <vector>
class View;

class ViewFactory{
	std::string d_libdir;
	std::string d_defaultlib;
	
	std::vector<void*> d_dlhandles;
	static ViewFactory *s_instance;
	std::map<std::string, int> d_xsltemplate_map;	
protected:

	virtual void addXSLTemplate(const char *filename);
	ViewFactory();
public:
	static ViewFactory *instance();
	virtual ~ViewFactory();
	
	View *buildDecoratedView(const char *viewname, const char *decoratorname);
	View *decorateView(View *returnview, const char *decoratorname);
	virtual View *buildView(const char *viewname);

	virtual const char *getXSLTemplateAt(int index);
	virtual int getNumXSLTemplates();
	void setLibDir(const char *path);
	void setDefaultLib(const char *libname);
};
#endif

