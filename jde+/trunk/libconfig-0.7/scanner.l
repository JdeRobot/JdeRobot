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

%option reentrant
%option noyywrap
%option yylineno
%option nounput
%option bison-bridge
%option header-file="scanner.h"
%option outfile="lex.yy.c"

%{

#include <stdlib.h>
#include <string.h>
#include "config.tab.h"

/* this is somewhat kludgy, but I wanted to avoid building strings
  dynamically during scanning */

static char *make_string(char *s)
{
  char *r = ++s;
  char *p, *q = r;
  size_t len = strlen(r);
  int esc = 0;

  *(r + --len) = 0;

  for(p = r; *p; p++)
  {
    if(*p == '\\')
    {
      if(! esc)
      {
        esc = 1;
	continue;
      }
    }
     
    if(esc)
    {
      if(*p == 'n')
        *(q++) = '\n';
      else if(*p == 'r')
        *(q++) = '\r';
      else if(*p == 'f')
        *(q++) = '\f';
      else if(*p == 't')
        *(q++) = '\t';
      else
        *(q++) = *p;

      esc = 0;
    }
    else
      *(q++) = *p;
  }

  *q = 0;

  return(r);
}

%}

ws               [ \t\r\n]+
equals           \=
comma            ,
group_start      \{
group_end        \}(;)?
true             TRUE
false            FALSE
name             [A-Za-z][-A-Za-z0-9_]*
quote            \"
integer          [-+]?[0-9]+
float            [-+]?([0-9]*)?\.[0-9]+([eE][-+]?[0-9]+)?
string           {quote}([^\"]|(\\\"))*{quote}
end              ;
array_start      \[
array_end        \]
comment          (#|\/\/).*$

%x COMMENT

%%

\/\*          { BEGIN COMMENT; }
<COMMENT>\*\/ { BEGIN INITIAL; }
<COMMENT>.    { /* ignore */ }

{ws}          { /* skip */ }

{equals}      { return(EQUALS); }
{comma}       { return(COMMA); }
{group_start} { return(GROUP_START); }
{group_end}   { return(GROUP_END); }
{true}        { yylval->ival = 1; return(BOOLEAN); }
{false}       { yylval->ival = 0; return(BOOLEAN); }
{name}        { yylval->sval = strdup(yytext); return(NAME); }
{float}       { yylval->fval = atof(yytext); return(FLOAT); }
{integer}     { yylval->ival = atoi(yytext); return(INTEGER); }
{string}      { yylval->sval = strdup(make_string(yytext)); return(STRING); }
{array_start} { return(ARRAY_START); }
{array_end}   { return(ARRAY_END); }
{end}         { return(END); }
{comment}     { /* ignore */ }
.             { return(GARBAGE); }
