//ViewFactory.cpp
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


#include "ViewFactory.h"
#include <dlfcn.h>
#include "View.h"
#include "ViewError.h"
#include <string>
#include <string.h>
#include <ctype.h>

extern View* returnObject(void); 

extern View* returnDecorator(View*);  


using namespace std;

ViewFactory * ViewFactory::s_instance = 0;


ViewFactory *ViewFactory::instance()
{
	if(s_instance == 0)
	{
		s_instance = new ViewFactory();
	}
	return s_instance;
}



View *ViewFactory::buildView(const char *name)
{
	///////////////
	// Given a name and a default name to use if name is empty, 
	// and a path or no path if the system paths are to be used,
	// we attempt to load a view object
	// If we fail, we return the built in error-view object with debug info
	// If the view object requires translation, we add the view name to 
	// the list of XSL templates to load. (The list is a map - to avoid loading same template twice!!)
	// 
	////////////////////////////
	
	void *handle;
	char *error;
	View* (*returnObject)(void);

	string viewname = name ? name : ""; 


	// string leading and trailing whitespace from the name
	while( isspace(viewname[0]) )
	{
		int length=viewname.size();
		viewname = viewname.substr(1, length);
	}

	while( isspace(viewname[ viewname.size() - 1 ]))
	{
		viewname = viewname.substr(0, viewname.size() - 1);
	}

	// If the name they passwed in was empty, use the default viewname 
	// NOTE: the default starts as empty and must be set 
	//
	if(viewname == "")
	{
		viewname=d_defaultlib;
	}

	// If the name is STILL empty, send back the default error view
	//
	if(viewname == "")
	{
		view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, "No view name provided and no default set");
		return errview;
	}

	// Make sure the name is not tainted
	// If it is, send back the built-in error view
	//
	if(strchr(viewname.c_str(),'/'))
	{
		view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, "Illegal Characters in Lib Name");
		return errview;
	}

	
	//////////////////////////////////////////
	// Figure out the path to the library
	// If libdir was not set, we look in default system directories
	// 
	string viewpath;
	if(d_libdir == "")
	{
		// If libdir is not set, it means we are searching in the default locations ( set in the ENV )
		// and since the system knows how to search, we need to leave off the "lib" and ".so"
		// parts of the filename!!!
		//
		viewpath=viewname;
	}
	else
	{
		// If libdir IS set, then we are loading an exact path and need to send the complete libname explicitly
		//
		viewpath = d_libdir + "/lib" + viewname + ".so";
	}

	// Attempt to load the library
	//
	handle=::dlopen(viewpath.c_str(),RTLD_LAZY);
	if(error = ::dlerror())
	{
		// If loading failed, return the built-in error view
		// with debug info
		//
		std::string message = "Could not load view library '";
		message += viewname;
		message += "' at path '";
		message +=  viewpath;
		message += "'\n";
		message += error;
		message += "\n";

		view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, message.c_str());
		return errview;
	}

	// When we are done with the view, we need to destroy it, so we need to store the handle 
	// in our handle vector so we can kill it  (dlclose it) in our destructor
	//
	d_dlhandles.push_back(handle);	

	// Obtain the handle (address) of the exported (extern) function "retrunObject"
	// Note that this does not execute the function, but merely returns a pointer to the function
	//
	returnObject = (View*(*)(void)) dlsym(handle,"returnObject");

	// If there was an error obtaining the handle of the function, 
	// we might be trying to open a different kind of object!!!
	// Return the built-in error view with debug info
	//
	if(error = ::dlerror())
	{
		std::string message = "Could not obtain address of returnObject function for '";
		message += viewname;
		message += "' at path '";
		message +=  viewpath;
		message += "'\n";
		message += error;
		message += "\n";

		view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, message.c_str());
		return errview;
	}

	// Everything is going acccording to plan!
	// Execute the "returnObject()" function we have a handle for, 
	// which creates the View Object by calling new, and returns it to us
	//
	View *returnview = returnObject();

	
	//::dlclose(handle); // Do this in the destructor!! - cannot delete before we use the object!

	// Since we loaded the view successfully, we will need to load the template as well....
	// So we add the viewname to the list of XSLTemplates that require loading....
	// NOTE: But only if the view requires translation!!!
	//
	if(returnview->requiresTranslation())
	{
		addXSLTemplate(viewname.c_str());
	}
	
	return returnview;
}




View *ViewFactory::buildDecoratedView(const char *childviewname, const char *decoratorname)
{
	// This is the safe way to decorate a view, 
	// because if the view is standalone, it wont be decorated
	//

   // First we build the child view...
	//
	View *returnview = buildView(childviewname);


	
	if(returnview->requiresTranslation())
	{
		return decorateView(returnview, decoratorname);
	}
	else
	{
		return returnview;
	}

}



View *ViewFactory::decorateView(View *returnview, const char *decoratorname)
{

		void *handle;
		char *error;
		View* (*returnDecorator)(View *);
		
		string libname = decoratorname ? decoratorname : ""; 

		if(libname == "")
		{
			view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, "No decorator name provided");
			return errview;
		}


		addXSLTemplate(libname.c_str());

		//////////////////////////////////////////
		// Figure out the path to the library
		// If libdir was not set, we look in default system directories
		// 
		string viewpath;
		if(d_libdir == "")
		{
			// If libdir is not set, it means we are searching in the default locations ( set in the ENV )
			// and since the system knows how to search, we need to leave off the "lib" and ".so"
			// parts of the filename!!!
			//
			viewpath=decoratorname;
		}
		else
		{
			// If libdir IS set, then we are loading an exact path and need to send the complete libname explicitly
			//
			viewpath = d_libdir + "/lib" + decoratorname + ".so";
		}

		// Attempt to load the library
		//
		handle=::dlopen(viewpath.c_str(),RTLD_LAZY);
		
		if(error = ::dlerror())
		{
			// If loading failed, return the built-in error view
			// with debug info
			//
			std::string message = "Could not load decorator library '";
			message += decoratorname;
			message += "' at path '";
			message +=  viewpath;
			message += "'\n";
			message += error;
			message += "\n";

			view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, message.c_str());
			return errview;
		}
		else
		{
			// When we are done with the decorator, we need to destroy it, so we need to store the handle 
			// in our handle vector so we can kill it  (dlclose it) in our destructor
			//
			d_dlhandles.push_back(handle);

		}
		
		// Obtain the handle (address) of the exported (extern) function "returnDecorator"
		// Note that this does not execute the function, but merely returns a pointer to the function
		//
		returnDecorator =	(View*(*)(View*)) dlsym(handle,"returnDecorator");

		// If there was an error obtaining the handle of the function, 
		// we might be trying to open a different kind of object!!!
		// Return the built-in error view with debug info
		//
		if(error = ::dlerror())
		{
			std::string message = "Could not get address of returnDecorator function for '";
			message += decoratorname;
			message += "' at path '";
			message +=  viewpath;
			message += "'\n";
			message += error;
			message += "\n";
	
			view::ViewError *errview = new view::ViewError( __FILE__, __LINE__, message.c_str());
			return errview;
		}


		// Everything is going acccording to plan!
		// Execute the "returnDecorator( View *someview )" function we have a handle for, 
		// which creates the Decorator and Passes it the View that was passed in to us.
		//
		returnview = returnDecorator(returnview);

		//::dlclose(handle); // Do this in the destructor - cannot delete before we use the object!
	
	return returnview;

}


void ViewFactory::addXSLTemplate(const char *filename)
{
	if(filename)
	{
		string mytemplate = filename;
		d_xsltemplate_map[mytemplate]=1;
	}
}

const char *ViewFactory::getXSLTemplateAt(int index)
{
	map<string,int>::iterator iter;
	int x=0;
	for(iter=d_xsltemplate_map.begin(); iter != d_xsltemplate_map.end(); iter++)
	{
		if(x == index)
		{
			return (iter->first).c_str();
		}
		x++;
	}
	return "";
}

int ViewFactory::getNumXSLTemplates()
{
	return d_xsltemplate_map.size();
}

void ViewFactory::setLibDir(const char *path)
{
	d_libdir = path ? path : "";
}

void ViewFactory::setDefaultLib(const char *libname)
{
	d_defaultlib = libname ? libname : "";
}

ViewFactory::ViewFactory()
{
	d_libdir="";
	d_defaultlib="";
}

ViewFactory::~ViewFactory()
{
	int len=d_dlhandles.size();
	for(int x=0; x< len; x++)
	{
		::dlclose(d_dlhandles[x]); 
	}
}

