// Command.cpp
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

#include "Command.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <rude/database.h>

using namespace std;

Command::Command()
{
 	d_message = "";}

char *Command::fixTextareaData(const char *data)
{
	//int length=strlen(data);
	//char *retdata=new char[length +1];
   //strcpy(retdata, data);
	//return retdata;

	if(data)
	{
		int length=strlen(data);
		char *uncrlf=new char[length +1];
		char *ptr=uncrlf;
		while(*data)
		{
			if(*data == 0x0a)
			{
				data++;
			}
			*ptr = *data;
			ptr++;
			if(*data)
			{
				data++;
			}
		}
		*ptr=0;
		return uncrlf;
	}
	else
	{
		char *ret=new char[1];
		ret[0]=0;
		return ret;
	}
}


const char *Command::getError() const
{
	return d_message.c_str();
}

void Command::setMessage(const char *message)
{
	setMessage("Unknown", 0, "message", "0", message, "");
}

void Command::setError(const char *error)
{
	setMessage("Unknown", 0, "error", "0", error, "");
}

const char *Command::getMessage() const
{
	return d_message.c_str();
}

void Command::traceMessage(const char *file, int line , const char *message)
{
    setMessage(file, line, "trace", "0", message, "");
}

void Command::setMessage(const char *file, int line, const char *messagetype, const char *messagecode,  const char *message,  const char *diagnostic)
{
   char buffer[10];

	sprintf(buffer, "%d", line);
	
   d_message += "<message type='";
	d_message += messagetype ? messagetype : "unknown";
	d_message += "' code='";
	d_message += messagecode ? messagecode : "0";
	d_message += "' file='";
	d_message += file ? file : "0";
	d_message += "' line='";
	d_message += buffer ? buffer : "0";
	d_message += "'>";
	
	d_message += "<description>";
	d_message += message ? message : "";
	d_message += "</description>";
	
	d_message += "<diagnostic><![CDATA[";
	d_message += diagnostic ? diagnostic : "";
	d_message += "]]></diagnostic>";
	d_message += "</message>";
}

Command::~Command()
{

}

int Command::checkResourcePermissions(const char *resourcetype, const char *resourceid, const char *username, const char *perms)
{
   return checkPermissions(username, perms);
}

int Command::checkPermissions(const char *username, const char *perms)
{
	std::string theuser = username? username: "";
	if(theuser == "")
	{
		return 0;
	}
	else
	{
		return 1;
	}

    std::string permstring=perms ? perms : "";	
	size_t location;

	// erase all spaces
	//
	while( (location = permstring.find(" ",0)) != string::npos )
	{
		permstring.erase(location);
	}

	// erase all tabs
	//
	while( (location = permstring.find("\t",0)) != string::npos )
	{
		permstring.erase(location);
	}
	
	// replace all ',' with '","'
	//
	int position = 0;
	while( (location = permstring.find(",",position)) != string::npos )
	{
 		// location points right at the ' '
		permstring.replace(location, 1, "','");
		position = (int) location + 3;
	}
	if(permstring=="")
	{
      return 0;
	}
    
	rude::Database *database = rude::Database::instance("sec");

        std::string sql = "SELECT  ";
        sql += "IF( sec_userperms.override='negative', 'no',  ";
        sql += "IF(sec_userperms.override='positive', 'yes',  ";
        sql += "IF( COUNT(sec_groupperms.group_id), 'yes','no') )) ";
        sql += "FROM sec_perms, sec_users  ";
        sql += "LEFT JOIN sec_userperms ON sec_userperms.user_id=sec_users.user_id ";
        sql += "AND sec_userperms.perm_id=sec_perms.perm_id ";
        sql += "LEFT JOIN sec_groupmembers ON sec_groupmembers.user_id=sec_users.user_id ";
        sql += "LEFT JOIN sec_groupperms ON ( sec_groupperms.group_id=sec_groupmembers.group_id OR sec_groupperms.group_id=1) "; // INCLUDE GROUP EVERYONE
        sql += "AND sec_groupperms.perm_id=sec_perms.perm_id ";
        sql += "WHERE sec_perms.code IN ('";
        sql += permstring;
        sql += "') AND sec_users.login='";
        sql += username ? username : "";
        sql += "' ";
        sql += "GROUP BY sec_perms.perm_id ";

       int count=0;
       if(database->storeResultQuery(sql.c_str()))
       {
		       std::string answer;
             while(database->nextRow())
             {
                   answer=database->column(0);
                   if(answer == "yes")
                   {
                      count++;
                   }
             }
             database->freeResult();
       }
       else
       {
            setMessage(__FILE__,  __LINE__, "system", "0003",  database->getError() , sql.c_str());
       }
       return count;
}


