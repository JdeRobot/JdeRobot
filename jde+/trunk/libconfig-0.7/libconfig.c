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

#include "libconfig.h"
#include "config.tab.h"
#include "scanner.h"
#include "private.h"

#include <stdlib.h>

#define PATH_TOKENS ":./"
#define CHUNK_SIZE 10

#define _new(T) calloc(sizeof(T), 1) // zeroed
#define _delete(P) free((void *)(P))

// ---------------------------------------------------------------------------

static const char *__io_error = "file I/O error";

static void __config_list_destroy(config_list_t *list);
extern int libconfig_yyparse(void *scanner, struct parse_context *ctx);

// ---------------------------------------------------------------------------

/*
static int __config_setting_compare(const void *a, const void *b)
  {
  const config_setting_t **s1 = (const config_setting_t **)a;
  const config_setting_t **s2 = (const config_setting_t **)b;

  return(strcmp((*s1)->name, (*s2)->name));
  }
*/

// ---------------------------------------------------------------------------

/*
static int __config_name_compare(const void *a, const void *b)
  {
  const char *name = (const char *)a;
  const config_setting_t **s = (const config_setting_t **)b;
  const char *p, *q;

  for(p = name, q = (*s)->name; ; p++, q++)
    {
    int pd = ((! *p) || strchr(PATH_TOKENS, *p));
    int qd = ((! *q) || strchr(PATH_TOKENS, *q));

    if(pd && qd)
      break;
    else if(pd)
      return(-1);
    else if(qd)
      return(1);
    else if(*p < *q)
      return(-1);
    else if(*p > *q)
      return(1);
    }
  
  return(0);
  }
*/

static int __config_name_compare(const char *a, const char *b)
  {
  const char *p, *q;

  for(p = a, q = b; ; p++, q++)
    {
    int pd = ((! *p) || strchr(PATH_TOKENS, *p));
    int qd = ((! *q) || strchr(PATH_TOKENS, *q));

    if(pd && qd)
      break;
    else if(pd)
      return(-1);
    else if(qd)
      return(1);
    else if(*p < *q)
      return(-1);
    else if(*p > *q)
      return(1);
    }
  
  return(0);
  }

// ---------------------------------------------------------------------------

static void __config_value_print(const config_value_t *value, int type,
                                 FILE *stream)
  {
  switch(type)
    {
    case CONFIG_TYPE_BOOL:
      fputs(value->bval ? "TRUE" : "FALSE", stream);
      break;

    case CONFIG_TYPE_INT:
      fprintf(stream, "%ld", value->ival);
      break;

    case CONFIG_TYPE_FLOAT:
      fprintf(stream, "%.8f", value->fval);
      break;

    case CONFIG_TYPE_STRING:
      {
      char *p;

      fputc('\"', stream);

      if(value->sval)
        {
        for(p = value->sval; *p; p++)
          {
          switch(*p)
            {
            case '\"':
            case '\\':
              fputc('\\', stream);
              fputc(*p, stream);
              break;
              
            case '\n':
              fputs("\\n", stream);
              break;
              
            case '\r':
              fputs("\\r", stream);
              break;
              
            case '\f':
              fputs("\\f", stream);
              break;
              
            case '\t':
              fputs("\\t", stream);
              break;
              
            default:
              fputc(*p, stream);
            }
          }
        }
      fputc('\"', stream);
      break;
      }

    default:
      fputs("???", stream);
      break;
    }
  }

// ---------------------------------------------------------------------------

static void __config_list_add(config_list_t *list, config_setting_t *setting)
  {
  if(list->length == list->capacity)
    {
    list->capacity += CHUNK_SIZE;
    list->elements = (config_setting_t **)realloc(
      list->elements, list->capacity * sizeof(config_setting_t *));
    }

  list->elements[list->length] = setting;
  list->length++;
  }

// ---------------------------------------------------------------------------

static config_setting_t *__config_list_search(config_list_t *list,
                                              const char *name,
                                              int *index)
  {
  config_setting_t **found = NULL;
  int i;

  if(! list)
    return(NULL);
  
  for(i = 0, found = list->elements; i < list->length; i++, found++)
    {
    if(! __config_name_compare(name, (*found)->name))
      {
      if(index)
        *index = i;
      
      return(*found);
      }
    }
  
  return(NULL);
  }

// ---------------------------------------------------------------------------

static void __config_list_remove(config_list_t *list, int index)
  {
  int offset = (index * sizeof(config_setting_t *));
  int len = list->length - 1 - index;
  void *base = (void *)list->elements + offset;

  memmove(base, base + sizeof(config_setting_t *),
          len * sizeof(config_setting_t *));

  list->length--;

  if((list->capacity - list->length) >= CHUNK_SIZE)
    {
    // realloc smaller?
    }
  }

// ---------------------------------------------------------------------------

static void __config_setting_destroy(config_setting_t *setting)
  {
  if(setting)
    {
    if(setting->name)
      _delete(setting->name);

    if(setting->type == CONFIG_TYPE_STRING)
      _delete(setting->value.sval);
    
    else if((setting->type == CONFIG_TYPE_GROUP)
            || (setting->type == CONFIG_TYPE_ARRAY))
      {
      if(setting->value.list)
        __config_list_destroy(setting->value.list);
      }

    if(setting->hook && setting->config->destructor)
      setting->config->destructor(setting->hook);
    
    _delete(setting);
    }
  }

// ---------------------------------------------------------------------------

static void __config_list_destroy(config_list_t *list)
  {
  config_setting_t **p;
  int i;

  if(! list)
    return;

  if(list->elements)
    {
    for(p = list->elements, i = 0; i < list->length; p++, i++)
      __config_setting_destroy(*p);
    
    _delete(list->elements);
    }

  _delete(list);
  }

// ---------------------------------------------------------------------------

static int __config_array_checktype(const config_setting_t *array, int type)
  {
  // if the array is empty, then it has no type yet
  
  if(! array->value.list)
    return(CONFIG_TRUE);
  
  if(array->value.list->length == 0)
    return(CONFIG_TRUE);

  // otherwise the first element added determines the type of the array
  
  config_setting_t *elem = array->value.list->elements[0];
  return(elem->type == type);
  }

// ---------------------------------------------------------------------------

int config_read(config_t *config, FILE *stream)
  {
  yyscan_t scanner;
  struct parse_context ctx;
  int r;
  
  // Reinitialize the config (keep the destructor)
  void (*destructor)(void *) = config->destructor;
  config_destroy(config);
  config_init(config);
  config->destructor = destructor;

  ctx.config = config;
  ctx.group = config->root;
  ctx.setting = config->root; // was NULL

  libconfig_yylex_init(&scanner);
  libconfig_yyrestart(stream, scanner);
  r = libconfig_yyparse(scanner, &ctx);
  libconfig_yylex_destroy(scanner);

  return(r == 0);
  }

// ---------------------------------------------------------------------------

static void __config_write(const config_setting_t *setting, FILE *stream,
                           int depth)
  {
  config_list_t *list;
  
  if(depth > 0)
    {
    fprintf(stream, "%*s", depth * 2, " ");
    fprintf(stream, "%s", setting->name);
    }

  switch(setting->type)
    {
    case CONFIG_TYPE_GROUP:
      {
      if(depth > 0)
        {
        fprintf(stream, "\n");
        fprintf(stream, "%*s", depth * 2, " ");
        fprintf(stream, "{\n");
        }

      list = setting->value.list;
      
      if(list)
        {
        int len = list->length;
        config_setting_t **s;

        for(s = list->elements; len--; s++)
          __config_write(*s, stream, depth + 1);
        }

      if(depth > 0)
        {
        fprintf(stream, "%*s", depth * 2, " ");
        fprintf(stream, "}\n");
        }
      break;
      }

    case CONFIG_TYPE_ARRAY:
      {
      fprintf(stream, " = [ ");

      list = setting->value.list;
      
      if(list)
        {
        int len = list->length;
        config_setting_t **s;

        for(s = list->elements; len--; s++)
          {
          __config_value_print(&((*s)->value), (*s)->type, stream);

          if(len)
            fputc(',', stream);
          
          fputc(' ', stream);
          }
        }
      
      fprintf(stream, "];\n");
      break;
      }

    default:
      fputs(" = ", stream);
      __config_value_print(&(setting->value), setting->type, stream);
      fputs(";\n", stream);
    }
  }

// ---------------------------------------------------------------------------

void config_write(const config_t *config, FILE *stream)
  {
  __config_write(config->root, stream, 0);
  }

// ---------------------------------------------------------------------------

int config_load_file(config_t *config, const char *fname)
  {
  int ret;
  FILE *f = fopen(fname, "rt");
  if(! f)
    {
    config->error_text = __io_error;
    return(0);
    }
  
  ret = config_read(config, f);
  fclose(f);
  return(ret);
  }

// ---------------------------------------------------------------------------

int config_save_file(config_t *config, const char *fname)
  {
  FILE *f = fopen(fname, "wt");
  if(! f)
    {
    config->error_text = __io_error;
    return(0);
    }
  
  config_write(config, f);
  fclose(f);
  return(1);
  }

// ---------------------------------------------------------------------------

void config_destroy(config_t *config)
  {
  __config_setting_destroy(config->root);

  memset((void *)config, 0, sizeof(config_t));
  }

// ---------------------------------------------------------------------------

void config_init(config_t *config)
  {
  memset((void *)config, 0, sizeof(config_t));

   config->root = _new(config_setting_t);
   config->root->type = CONFIG_TYPE_GROUP;
   config->root->config = config;
  }

// ---------------------------------------------------------------------------

static config_setting_t *config_setting_create(config_setting_t *parent,
                                               const char *name, int type)
  {
  config_setting_t *setting;
  config_list_t *list;

  if((parent->type != CONFIG_TYPE_GROUP)
     && (parent->type != CONFIG_TYPE_ARRAY))
    return(NULL);

  setting = _new(config_setting_t);
  setting->parent = parent;
  setting->name = (name == NULL) ? NULL : strdup(name);
  setting->type = type;
  setting->config = parent->config;
  setting->hook = NULL;

  list = parent->value.list;
  
  if(! list)
    list = parent->value.list = _new(config_list_t);

  __config_list_add(list, setting);

  return(setting);
  }

// ---------------------------------------------------------------------------

long config_setting_get_int(const config_setting_t *setting)
  {
  return((setting->type == CONFIG_TYPE_INT) ? setting->value.ival : 0);
  }

// ---------------------------------------------------------------------------

int config_setting_set_int(config_setting_t *setting, long value)
  {
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_INT;
  else if(setting->type != CONFIG_TYPE_INT)
    return(CONFIG_FALSE);
  
  setting->value.ival = value;
  return(CONFIG_TRUE);
  }

// ---------------------------------------------------------------------------

double config_setting_get_float(const config_setting_t *setting)
  {
  return((setting->type == CONFIG_TYPE_FLOAT) ? setting->value.fval : 0.0);
  }

// ---------------------------------------------------------------------------

int config_setting_set_float(config_setting_t *setting, double value)
  {
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_FLOAT;
  if(setting->type != CONFIG_TYPE_FLOAT)
    return(CONFIG_FALSE);
  
  setting->value.fval = value;
  return(CONFIG_TRUE);
  }

// ---------------------------------------------------------------------------

int config_setting_get_bool(const config_setting_t *setting)
  {
  return((setting->type == CONFIG_TYPE_BOOL) ? setting->value.bval : 0);
  }

// ---------------------------------------------------------------------------

int config_setting_set_bool(config_setting_t *setting, int value)
  {
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_BOOL;
  if(setting->type != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);
  
  setting->value.bval = value;
  return(CONFIG_TRUE);
  }

// ---------------------------------------------------------------------------

const char *config_setting_get_string(const config_setting_t *setting)
  {
  return((setting->type == CONFIG_TYPE_STRING) ? setting->value.sval : NULL);
  }

// ---------------------------------------------------------------------------

int config_setting_set_string(config_setting_t *setting, const char *value)
  {
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_STRING;
  if(setting->type != CONFIG_TYPE_STRING)
    return(CONFIG_FALSE);
  
  if(setting->value.sval)
    _delete(setting->value.sval);
    
  setting->value.sval = strdup(value);
  return(CONFIG_TRUE);
  }

// ---------------------------------------------------------------------------

config_setting_t *config_lookup(const config_t *config, const char *path)
  {
  const char *p = path;
  config_setting_t *setting = config->root, *found;

  for(;;)
    {
    while(*p && strchr(PATH_TOKENS, *p))
      p++;

    if(! *p)
      break;

    found = config_setting_get_member(setting, p);

    if(! found)
      break;

    setting = found;

    while(! strchr(PATH_TOKENS, *p))
      p++;    
    }
      
  return(*p ? NULL : setting);
  }

// ---------------------------------------------------------------------------

const char *config_lookup_string(const config_t *config, const char *path)
  {
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(NULL);

  return(config_setting_get_string(s));
  }

// ---------------------------------------------------------------------------

long config_lookup_int(const config_t *config, const char *path)
  {
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(0);

  return(config_setting_get_int(s));
  }

// ---------------------------------------------------------------------------

double config_lookup_float(const config_t *config, const char *path)
  {
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(0.0);

  return(config_setting_get_float(s));
  }

// ---------------------------------------------------------------------------

int config_lookup_bool(const config_t *config, const char *path)
  {
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(0);

  return(config_setting_get_bool(s));
  }

// ---------------------------------------------------------------------------

long config_setting_get_int_elem(const config_setting_t *array, int index)
  {
  const config_setting_t *element = config_setting_get_elem(array, index);

  if(! element)
    return(0);
  
  if(element->type != CONFIG_TYPE_INT)
    return(0);
  
  return(element->value.ival);
  }

// ---------------------------------------------------------------------------

int config_setting_set_int_elem(config_setting_t *array, int index, long value)
  {
  config_setting_t *element = NULL;

  if(array->type != CONFIG_TYPE_ARRAY)
    return(CONFIG_FALSE);
  
  if(index < 0)
    {
    if(! __config_array_checktype(array, CONFIG_TYPE_INT))
      return(CONFIG_FALSE);
    
    element = config_setting_create(array, NULL, CONFIG_TYPE_INT);
    }
  else
    {
    element = config_setting_get_elem(array, index);

    if(! element)
      return(CONFIG_FALSE);
    }

  return(config_setting_set_int(element, value));
  }

// ---------------------------------------------------------------------------

double config_setting_get_float_elem(const config_setting_t *array, int index)
  {
  config_setting_t *element = config_setting_get_elem(array, index);

  if(! element)
    return(0.0);
  
  if(element->type != CONFIG_TYPE_FLOAT)
    return(0.0);

  return(element->value.fval);
  }

// ---------------------------------------------------------------------------

int config_setting_set_float_elem(config_setting_t *array, int index,
                                  double value)
  {
  config_setting_t *element = NULL;

  if(array->type != CONFIG_TYPE_ARRAY)
    return(CONFIG_FALSE);

  if(index < 0)
    {
    if(! __config_array_checktype(array, CONFIG_TYPE_FLOAT))
      return(CONFIG_FALSE);
    
    element = config_setting_create(array, NULL, CONFIG_TYPE_FLOAT);
    }
  else
    element = config_setting_get_elem(array, index);

  if(! element)
    return(CONFIG_FALSE);

  return(config_setting_set_float(element, value));
  }

// ---------------------------------------------------------------------------

int config_setting_get_bool_elem(const config_setting_t *array, int index)
  {
  config_setting_t *element = config_setting_get_elem(array, index);

  if(! element)
    return(CONFIG_FALSE);
  
  if(element->type != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);

  return(element->value.bval);
  }

// ---------------------------------------------------------------------------

int config_setting_set_bool_elem(config_setting_t *array, int index,
                                 int value)
  {
  config_setting_t *element = NULL;

  if(array->type != CONFIG_TYPE_ARRAY)
    return(CONFIG_FALSE);

  if(index < 0)
    {
    if(! __config_array_checktype(array, CONFIG_TYPE_BOOL))
      return(CONFIG_FALSE);
    
    element = config_setting_create(array, NULL, CONFIG_TYPE_BOOL);
    }
  else
    element = config_setting_get_elem(array, index);

  if(! element)
    return(CONFIG_FALSE);

  return(config_setting_set_bool(element, value));
  }

// ---------------------------------------------------------------------------

const char *config_setting_get_string_elem(const config_setting_t *array,
                                           int index)
  {
  config_setting_t *element = config_setting_get_elem(array, index);

  if(! element)
    return(NULL);
  
  if(element->type != CONFIG_TYPE_STRING)
    return(NULL);

  return(element->value.sval);
  }

// ---------------------------------------------------------------------------

int config_setting_set_string_elem(config_setting_t *array, int index,
                                   const char *value)
  {
  config_setting_t *element = NULL;

  if(array->type != CONFIG_TYPE_ARRAY)
    return(CONFIG_FALSE);

  if(index < 0)
    {
    if(! __config_array_checktype(array, CONFIG_TYPE_STRING))
      return(CONFIG_FALSE);
    
    element = config_setting_create(array, NULL, CONFIG_TYPE_STRING);
    }
  else
    element = config_setting_get_elem(array, index);

  if(! element)
    return(CONFIG_FALSE);

  return(config_setting_set_string(element, value));
  }

// ---------------------------------------------------------------------------

config_setting_t *config_setting_get_elem(const config_setting_t *array,
                                          int index)
  {
  config_list_t *list = array->value.list;
  
  if(((array->type != CONFIG_TYPE_ARRAY)
      && (array->type != CONFIG_TYPE_GROUP)) || ! list)
    return(NULL);

  if((index < 0) || (index >= list->length))
    return(NULL);

  return(list->elements[index]);
  }

// ---------------------------------------------------------------------------

config_setting_t *config_setting_get_member(const config_setting_t *setting,
                                            const char *name)
  {
  if(setting->type != CONFIG_TYPE_GROUP)
    return(NULL);

  return(__config_list_search(setting->value.list, name, NULL));
  }

// ---------------------------------------------------------------------------

void config_set_destructor(config_t *config, void (*destructor)(void *))
  {
  config->destructor = destructor;
  }

// ---------------------------------------------------------------------------

int config_setting_length(const config_setting_t *setting)
  {
  if((setting->type != CONFIG_TYPE_GROUP)
     && (setting->type != CONFIG_TYPE_ARRAY))
    return(0);
  
  if(! setting->value.list)
    return(0);

  return(setting->value.list->length);
  }

// ---------------------------------------------------------------------------

void config_setting_set_hook(const config_setting_t *setting, void *hook)
  {
  ((config_setting_t *)setting)->hook = hook;
  }

// ---------------------------------------------------------------------------

config_setting_t *config_setting_add(config_setting_t *parent,
                                     const char *name, int type)
  {
  if((type < CONFIG_TYPE_NONE) || (type > CONFIG_TYPE_ARRAY))
    return(NULL);

  if(! parent)
    return(NULL);

  if(parent->type == CONFIG_TYPE_ARRAY)
    name = NULL;

  if(name)
    if(config_setting_get_member(parent, name) != NULL)
      return(NULL); // already exists

  return(config_setting_create(parent, name, type));
  }

// ---------------------------------------------------------------------------

int config_setting_remove(config_setting_t *parent, const char *name)
  {
  int index;
  config_setting_t *setting;
  
  if(! parent)
    return(CONFIG_FALSE);

  if(parent->type != CONFIG_TYPE_GROUP)
    return(CONFIG_FALSE);

  if(! (setting = __config_list_search(parent->value.list, name, &index)))
    return(CONFIG_FALSE);

  //printf("removing setting %p at index %d\n", setting, index);
  __config_setting_destroy(setting);
  
  __config_list_remove(parent->value.list, index);

  return(CONFIG_TRUE);
  }

// ---------------------------------------------------------------------------
// eof
