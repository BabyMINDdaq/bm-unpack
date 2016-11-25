/* This file is part of BabyMINDdaq software package. This software
 * package is designed for internal use for the Baby MIND detector
 * collaboration and is tailored for this use primarily.
 *
 * BabyMINDdaq is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BabyMINDdaq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BabyMINDdaq.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _MDARGUMENTS_H__
#define _MDARGUMENTS_H__

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <list>
#include <algorithm> // for transform
#include <cctype> // for tolower

typedef enum MDargumentStatus_t {
  MDARGUMENT_STATUS_OK             = 0,
  MDARGUMENT_STATUS_NOT_FOUND      = 1,
  MDARGUMENT_STATUS_FORMAT_ERROR   = 2,
  MDARGUMENT_STATUS_ERROR          = 3
} MDargumentStatus_t ;

typedef enum MDargumentType_t {
  MDARGUMENT_TYPE_VALUE    = 0,
  MDARGUMENT_TYPE_SWITCH   = 1,
  MDARGUMENT_TYPE_NAME     = 2,
  MDARGUMENT_TYPE_ERROR    = -1
} MDargumentType_t ;

typedef enum MDargumentFormatID_t {
  MDARGUMENT_FORMAT_NOVALUE = 0,
  MDARGUMENT_FORMAT_INT     = 1,
  MDARGUMENT_FORMAT_DOUBLE  = 2,
  MDARGUMENT_FORMAT_STRING  = 3,
  MDARGUMENT_FORMAT_ERROR   = -1
} MDargumentFormatID_t ;


using namespace std;

class MDargument{
public:
  MDargument();
  MDargument( string aName="", string aDescr="", string aSwitch="", string aFormat="", string aDefault = "")
  :_name(aName),_description(aDescr),_switch(aSwitch),_value(aDefault),_format(aFormat) {};

  virtual ~MDargument(){};

  string GetName()         { return _name; };
  string GetDescription()  { return _description; };
  string GetSwitch()       { return _switch; };
  string GetDefault()      { return _value; };
  string GetFormat()       { return _format; };
  MDargumentFormatID_t GetFormatID() {
    if ( _format == "" )              return MDARGUMENT_FORMAT_NOVALUE; 
    else if ( _format == "<int>" )    return MDARGUMENT_FORMAT_INT;
    else if ( _format == "<double>" ) return MDARGUMENT_FORMAT_DOUBLE;
    else if ( _format == "<string>" ) return MDARGUMENT_FORMAT_STRING;
    else                              return MDARGUMENT_FORMAT_ERROR;
  };


  const char * GetValue(){ return _value.c_str(); };
  const char * GetValue( string & aVal ){ aVal.assign( _value.c_str() ); return _value.c_str(); };
  void SetValue( string aValue ){ _value = aValue; };
  void SetValue( const char * aValue ){ _value = aValue; };

private:
  //data 
  string _name;
  string _description;
  string _switch;
  string _value;
  string _format; 
};

typedef list< MDargument * > ArgList;
typedef list< MDargument * >::iterator ArgListIter;

//----------------------------------------------------------------------------------------------

class MDargumentHandler
{
public:
  MDargumentHandler( string aDescription="" );
  virtual ~MDargumentHandler(){};
  
  void AddArgument( string aName, string aDescription, string aSwitch, string aFormat="", string aDefault ="" );
  void Usage();
  int ProcessArguments( int argc, char **argv );

  bool GetValue( string aName );
  MDargumentStatus_t GetValue( string aName, string & aVal );
  MDargumentStatus_t GetValue( string aName, int & aVal );
  MDargumentStatus_t GetValue( string aName, double & aVal );
  
private:
  MDargument * Find( string aNameOrSwitch );
  bool       IsArgName(const char * str);
  bool       IsArgSwitch(const char * str);
  bool       IsArgValue(const char * str);

  MDargumentType_t  ArgumentType(const char * str);

  // data
  ArgList    _argList;
  string     _name;        // Program name - extracted form argv[0]
  string     _description; // Program description
};


#endif
