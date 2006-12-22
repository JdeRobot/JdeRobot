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

%defines
%output="y.tab.c"
%pure-parser
%lex-param{void *scanner}
%parse-param{void *scanner}
%parse-param{struct parse_context *ctx}

%{
#include <stdio.h>
#include <string.h>
#include "libconfig.h"
#include "private.h"

static const char *err_array_elem_type = "mismatched element type in array";

#define IN_ARRAY() \
  (ctx->setting->type == CONFIG_TYPE_ARRAY)

void libconfig_yyerror(void *scanner, struct parse_context *ctx,
                      char const *s)
  {
  ctx->config->error_line = libconfig_yyget_lineno(scanner);;
  ctx->config->error_text = s;
  }


%}

%union
  {
  long ival;
  double fval;
  char *sval;
  }

%token <ival> BOOLEAN INTEGER
%token <fval> FLOAT
%token <sval> STRING NAME
%token EQUALS NEWLINE ARRAY_START ARRAY_END COMMA GROUP_START GROUP_END END GARBAGE

%%

settings:
    /* empty */
  | settings setting
  ;
  
setting:
    assignment
  | group
  ;

array:
  ARRAY_START { ctx->setting->type = CONFIG_TYPE_ARRAY; }
  list
  ARRAY_END { }
  ;

value:
    simple_value
  | array
  ;

simple_value:
    BOOLEAN
  {
  if(IN_ARRAY())
    {
    if(! config_setting_set_bool_elem(ctx->setting, -1, (int)$1))
      {
      libconfig_yyerror(scanner, ctx, err_array_elem_type);
      YYABORT;
      }
    }
  else
    config_setting_set_bool(ctx->setting, (int)$1);
  }
  | INTEGER
  {
  if(IN_ARRAY())
    {
    if(! config_setting_set_int_elem(ctx->setting, -1, $1))
      {
      libconfig_yyerror(scanner, ctx, err_array_elem_type);
      YYABORT;
      }
    }
  else
    {
    config_setting_set_int(ctx->setting, $1);
    }
  }
  | FLOAT
  {
  if(IN_ARRAY())
    {
    if(! config_setting_set_float_elem(ctx->setting, -1, $1))
      {
      libconfig_yyerror(scanner, ctx, err_array_elem_type);
      YYABORT;
      }
    }
  else
    config_setting_set_float(ctx->setting, $1);
  }
  | STRING
  {
  if(IN_ARRAY())
    {
    if(! config_setting_set_string_elem(ctx->setting, -1, $1))
      {
      free($1);
      libconfig_yyerror(scanner, ctx, err_array_elem_type);
      YYABORT;
      }
    else
      free($1);
    }
  else
    {
    config_setting_set_string(ctx->setting, $1);
    free($1);
    }
  }
  ;

list:
    /* empty */
  | simple_value
  | list COMMA simple_value
  ;

assignment:
  NAME
  {
  ctx->setting = config_setting_add(ctx->group, $1, CONFIG_TYPE_NONE);
  free($1);
  }

  EQUALS value END
  { ctx->setting = NULL; }
  ;

group:
  NAME
  {
  ctx->group = config_setting_add(ctx->group, $1, CONFIG_TYPE_GROUP);
  free($1);
  }
  GROUP_START settings GROUP_END
  {
  if(ctx->group)
    ctx->group = ctx->group->parent;
  }
  ;

%%
