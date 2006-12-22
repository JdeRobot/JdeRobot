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

#ifndef __libconfig_hpp
#define __libconfig_hpp

#include <stdio.h>
#include <string>
#include <map>

namespace libconfig {

#include <libconfig.h>

enum SettingType
  {
  TypeNone = 0,
  TypeGroup,
  TypeInt,
  TypeFloat,
  TypeString,
  TypeBoolean,
  TypeArray
  };

class ConfigException
  {
  };

class SettingTypeException : public ConfigException
  {
  };

class SettingNotFoundException : public ConfigException
  {
  };

class SettingExistsException : public ConfigException
  {
  };

class FileIOException : public ConfigException
  {
  };

class ParseException : public ConfigException
  {
  friend class Config;

  private:

  int _line;
  const char *_error;

  ParseException(int line, const char *error)
    : _line(line), _error(error) {}

  public:

  virtual ~ParseException() { }

  inline int getLine() { return(_line); }
  inline const char *getError() { return(_error); }
  };

class Setting
  {
  friend class Config;

  private:

  config_setting_t *_setting;
  SettingType _type;

  Setting(config_setting_t *setting);

  void assertType(SettingType type) const
    throw(SettingTypeException);
  static Setting & wrapSetting(config_setting_t *setting);
  
  public:

  virtual ~Setting();
  
  inline SettingType getType() const { return(_type); }

  operator bool() const throw(SettingTypeException);
  operator long() const throw(SettingTypeException);
  operator int() const throw(SettingTypeException);
  operator double() const throw(SettingTypeException);
  operator float() const throw(SettingTypeException);
  operator const char *() const throw(SettingTypeException);

  bool operator=(bool const& value) throw(SettingTypeException);
  long operator=(long const& value) throw(SettingTypeException);
  long operator=(int const& value) throw(SettingTypeException);
  double operator=(double const& value) throw(SettingTypeException);
  double operator=(float const& value) throw(SettingTypeException);
  const char *operator=(const char *value) throw(SettingTypeException);
  const char *operator=(const std::string & value) throw(SettingTypeException)
    {
    return(operator=(value.c_str()));
    }
  
  Setting & operator[](const char * key) const
    throw(SettingTypeException, SettingNotFoundException);

  Setting & operator[](const std::string & key) const
    throw(SettingTypeException, SettingNotFoundException)
    {
    return(operator[](key.c_str()));
    }

  Setting & operator[](int index) const
    throw(SettingTypeException, SettingNotFoundException);

  void remove(const char *name)
    throw(SettingTypeException, SettingNotFoundException);

  void remove(const std::string & name)
    throw(SettingTypeException, SettingNotFoundException)
    {
    remove(name.c_str());
    }

  Setting & add(const std::string & name, SettingType type)
    throw(SettingTypeException, SettingExistsException)
    {
    return(add(name.c_str(), type));
    }
  
  Setting & add(const char *name, SettingType type)
    throw(SettingTypeException, SettingExistsException);

  Setting & add(SettingType type)
    throw(SettingTypeException);
  
  int getLength() const; // for arrays & structures
  const char *getName() const;
  };

class Config
  {
  private:
  
  config_t _config;

  static void ConfigDestructor(void *arg);

  public:

  Config();
  virtual ~Config();

  void read(FILE *stream) throw(ParseException);
  void write(FILE *stream) const;

  void loadFile(const char *fname) throw(FileIOException, ParseException);
  void saveFile(const char *fname) throw(FileIOException);

  Setting & lookup(const std::string & path) const
    throw(SettingNotFoundException);
  Setting & lookup(const char *path) const
    throw(SettingNotFoundException);

  Setting & getRoot() const;
  };

} // namespace libconfig

#endif // __libconfig_hpp
