/* ----------------------------------------------------------------------------
   libconfig - A structured configuration file parsing library
   Copyright (C) 2005  Mark A Lindner
 
   This file is part of libconfig.
    
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
    
   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
   ----------------------------------------------------------------------------
*/

#include "libconfig.h++"

using namespace libconfig;

// ---------------------------------------------------------------------------

void Config::ConfigDestructor(void *arg)
  {
  delete reinterpret_cast<Setting *>(arg);
  }

// ---------------------------------------------------------------------------

Config::Config()
  {
  config_init(& _config);
  config_set_destructor(& _config, ConfigDestructor);
  }

// ---------------------------------------------------------------------------

Config::~Config()
  {
  config_destroy(& _config);
  }

// ---------------------------------------------------------------------------

void Config::read(FILE *stream) throw(ParseException)
  {
  if(! config_read(& _config, stream))
    throw ParseException(config_error_line(& _config),
                         config_error_text(& _config));
  }

// ---------------------------------------------------------------------------

void Config::write(FILE *stream) const
  {
  config_write(& _config, stream);
  }

// ---------------------------------------------------------------------------

void Config::loadFile(const char *fname) throw(FileIOException,
                                               ParseException)
  {
  FILE *f = fopen(fname, "rt");
  if(f == NULL)
    throw FileIOException();
  try
    {
    read(f);
    fclose(f);
    }
  catch(ParseException& p)
    {
    fclose(f);
    throw p;
    }
  }

// ---------------------------------------------------------------------------

void Config::saveFile(const char *fname) throw(FileIOException)
  {
  if(! config_save_file(& _config, fname))
    throw FileIOException();
  }

// ---------------------------------------------------------------------------

Setting & Config::lookup(const std::string& path) const
  throw(SettingNotFoundException)
  {
  return(lookup(path.c_str()));
  }

// ---------------------------------------------------------------------------

Setting & Config::lookup(const char *path) const
  throw(SettingNotFoundException)
  {
  config_setting_t *s = config_lookup(& _config, path);
  if(! s)
    throw SettingNotFoundException();

  return(Setting::wrapSetting(s));
  }

// ---------------------------------------------------------------------------

Setting & Config::getRoot() const
  {
  return(Setting::wrapSetting(config_root_setting(& _config)));
  }

// ---------------------------------------------------------------------------

Setting::Setting(config_setting_t *setting)
  : _setting(setting)
  {
  switch(config_setting_type(setting))
    {
    case CONFIG_TYPE_GROUP:
      _type = TypeGroup;
      break;

    case CONFIG_TYPE_INT:
      _type = TypeInt;
      break;

    case CONFIG_TYPE_FLOAT:
      _type = TypeFloat;
      break;

    case CONFIG_TYPE_STRING:
      _type = TypeString;
      break;

    case CONFIG_TYPE_BOOL:
      _type = TypeBoolean;
      break;

    case CONFIG_TYPE_ARRAY:
      _type = TypeArray;
      break;

    case CONFIG_TYPE_NONE:
    default:
      _type = TypeNone;
      break;
    }
  }

// ---------------------------------------------------------------------------

Setting::~Setting()
  {
  _setting = NULL;
  }

// ---------------------------------------------------------------------------

Setting::operator bool() const throw(SettingTypeException) 
  {
  assertType(TypeBoolean);

  return(config_setting_get_bool(_setting) ? true : false);
  }

// ---------------------------------------------------------------------------

Setting::operator long() const throw(SettingTypeException)
  {
  assertType(TypeInt);

  return(config_setting_get_int(_setting));
  }

// ---------------------------------------------------------------------------

Setting::operator int() const throw(SettingTypeException)
  {
  assertType(TypeInt);

  // may cause loss of precision:
  return(static_cast<int>(config_setting_get_int(_setting)));
  }

// ---------------------------------------------------------------------------

Setting::operator double() const throw(SettingTypeException)
  {
  assertType(TypeFloat);

  return(config_setting_get_float(_setting));
  }

// ---------------------------------------------------------------------------

Setting::operator float() const throw(SettingTypeException)
  {
  assertType(TypeFloat);

  // may cause loss of precision:
  return(static_cast<float>(config_setting_get_float(_setting)));
  }

// ---------------------------------------------------------------------------

Setting::operator const char *() const throw(SettingTypeException)
  {
  assertType(TypeString);

  return(config_setting_get_string(_setting));
  }

// ---------------------------------------------------------------------------

bool Setting::operator=(bool const& value) throw(SettingTypeException)
  {
  assertType(TypeBoolean);

  config_setting_set_bool(_setting, value);

  return(value);
  }

// ---------------------------------------------------------------------------

long Setting::operator=(long const& value) throw(SettingTypeException)
  {
  assertType(TypeInt);

  config_setting_set_int(_setting, value);

  return(value);
  }

// ---------------------------------------------------------------------------

long Setting::operator=(int const& value) throw(SettingTypeException)
  {
  assertType(TypeInt);

  long cvalue = static_cast<long>(value);
  
  config_setting_set_int(_setting, cvalue);

  return(cvalue);
  }

// ---------------------------------------------------------------------------

double Setting::operator=(double const& value) throw(SettingTypeException)
  {
  assertType(TypeFloat);

  config_setting_set_float(_setting, value);

  return(value);
  }

// ---------------------------------------------------------------------------

double Setting::operator=(float const& value) throw(SettingTypeException)
  {
  assertType(TypeFloat);

  double cvalue = static_cast<double>(value);

  config_setting_set_float(_setting, cvalue);

  return(cvalue);
  }

// ---------------------------------------------------------------------------

const char *Setting::operator=(const char *value) throw(SettingTypeException)
  {
  assertType(TypeString);

  config_setting_set_string(_setting, value);

  return(value);
  }

// ---------------------------------------------------------------------------

Setting & Setting::operator[](int i) const
  throw(SettingTypeException, SettingNotFoundException)
  {
  if((_type != TypeArray) && (_type != TypeGroup))
    throw SettingTypeException();
  
  config_setting_t *setting = config_setting_get_elem(_setting, i);

  if(! setting)
    throw SettingNotFoundException();

  return(wrapSetting(setting));
  }

// ---------------------------------------------------------------------------

Setting & Setting::operator[](const char *key) const
  throw(SettingTypeException, SettingNotFoundException)
  {
  assertType(TypeGroup);

  config_setting_t *setting = config_setting_get_member(_setting, key);

  if(! setting)
    throw SettingNotFoundException();

  return(wrapSetting(setting));
  }

// ---------------------------------------------------------------------------

int Setting::getLength() const
  {
  return(config_setting_length(_setting));
  }

// ---------------------------------------------------------------------------

const char * Setting::getName() const
  {
  return(config_setting_name(_setting));
  }

// ---------------------------------------------------------------------------

void Setting::remove(const char *name)
  throw(SettingTypeException, SettingNotFoundException)
  {
  assertType(TypeGroup);
  
  if(! config_setting_remove(_setting, name))
    throw SettingNotFoundException();
  }

// ---------------------------------------------------------------------------

Setting & Setting::add(const char *name, SettingType type)
  throw(SettingTypeException, SettingExistsException)
  {
  assertType(TypeGroup);

  int typecode;

  switch(type)
    {
    case TypeGroup:
      typecode = CONFIG_TYPE_GROUP;
      break;

    case TypeInt:
      typecode = CONFIG_TYPE_INT;
      break;

    case TypeFloat:
      typecode = CONFIG_TYPE_FLOAT;
      break;

    case TypeString:
      typecode = CONFIG_TYPE_STRING;
      break;

    case TypeBoolean:
      typecode = CONFIG_TYPE_BOOL;
      break;

    case TypeArray:
      typecode = CONFIG_TYPE_ARRAY;
      break;
      
    default:
      throw SettingTypeException();
    }

  config_setting_t *setting = config_setting_add(_setting, name, typecode);

  if(! setting)
    throw SettingExistsException();

  return(wrapSetting(setting));
  }

// ---------------------------------------------------------------------------

Setting & Setting::add(SettingType type) throw(SettingTypeException)
  {
  assertType(TypeArray);

  if(getLength() > 0)
    {
    SettingType atype = operator[](0).getType();
    if(type != atype)
      throw SettingTypeException();
    }
  else
    {
    if((type != TypeInt) && (type != TypeFloat) && (type != TypeString)
       && (type != TypeBoolean))
      throw SettingTypeException();
    }

  int tp = CONFIG_TYPE_NONE;

  if(type == TypeInt)
    tp = CONFIG_TYPE_INT;
  else if(type == TypeFloat)
    tp = CONFIG_TYPE_FLOAT;
  else if(type == TypeString)
    tp = CONFIG_TYPE_STRING;
  else
    tp = CONFIG_TYPE_BOOL;

  config_setting_t *s = config_setting_add(_setting, NULL, tp);

  Setting &ns = wrapSetting(s);

  switch(type)
    {
    case TypeInt:
      ns = 0;
      break;

    case TypeFloat:
      ns = 0.0;

    case TypeString:
      ns = (char *)NULL;

    case TypeBoolean:
      ns = false;
    }

  return(ns);
  }

// ---------------------------------------------------------------------------

void Setting::assertType(SettingType type) const throw(SettingTypeException)
  {
  if(type != _type)
    throw SettingTypeException();
  }

// ---------------------------------------------------------------------------

Setting & Setting::wrapSetting(config_setting_t *s)
  {
  Setting *setting = NULL;
  
  void *hook = config_setting_get_hook(s);
  if(! hook)
    {
    setting = new Setting(s);
    config_setting_set_hook(s, reinterpret_cast<void *>(setting));
    }
  else
    setting = reinterpret_cast<Setting *>(hook);

  return(*setting);
  }

// ---------------------------------------------------------------------------
// eof
