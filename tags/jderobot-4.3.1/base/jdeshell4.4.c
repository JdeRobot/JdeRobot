/*
 *
 *  Copyright (C) 1997-2009 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Jos� Mar�a Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 *            David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include <jde.h>
#include <hierarchy.h>
#include <loader.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef int rl_cmdfunc_t (char *);

const char * thisrelease = "jderobot 4.4.0";

typedef struct {
  const char *name;		/* User printable name of the function. */
  rl_cmdfunc_t *func;		/* Function to call to do the job. */
  const char *doc;		/* Documentation for this function.*/
} COMMAND;


enum STATE { BASE, SCHEMA };
typedef struct {
  enum STATE state;
  COMMAND *commands;
  char prompt[256];
  void *pdata;
} SHELLSTATE;

enum GENERATOR_ST { COMMANDS = 1, SCHEMAS = 2 };

/* Forward declarations. */
char *stripwhite (char *string);
COMMAND *find_command (const char *name);
void initialize_readline ();
int execute_line (char *line);
/*commads functions declaration*/
int com_cd(char *);
int com_help(char *);
int com_list(char *);
int com_dir(char *);
int com_load_driver(char *);
int com_load_schema(char *);
int com_load_schema2(char*);
int com_ps(char *);
int com_pwd(char *);
int com_exit(char *);
int com_view(char *);
int com_run(char *);
int com_stop(char *);
int com_show(char *);
int com_hide(char *);
int com_init(char *);
int com_terminate(char *);
int com_zoom(char *);
int com_license(char *);

COMMAND bcommands[] = {
  { "help", com_help, "Display this help text" },
  { "cd", com_cd, "Change the working directory to DIR" },
  { "dir", com_dir, "List files at the working directory" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "view", com_view, "View the contents of FILE" },
  { "ls", com_list, "List loaded schemas" },
  { "load_driver", com_load_driver, "Load driver" },
  { "load_service", com_load_driver, "Load service" },
  { "load_schema", com_load_schema, "Load schema" },
  { "load_schema2", com_load_schema2, "Load schema. New implementation" },
  { "ps", com_ps, "Print schemas states" },
  { "run", com_run, "Run schema" },
  { "stop", com_stop, "Stop schema" },
  { "show", com_show, "Show schema gui" },
  { "hide", com_hide, "Hide schema gui" },
  { "zoom", com_zoom, "Change to schema mode" },
  { "show_license", com_license, "Show JDE license" },
  { "?", com_help, "Synonym for 'help'" },
  { "exit", com_exit, "Quit using jderobot" },
  { "quit", com_exit, "Synonym for 'exit'" },
  { (const char *)NULL, (rl_cmdfunc_t *)NULL, (const char *)NULL }
};

COMMAND scommands[] = {
  { "help", com_help, "Display this text" },
  { "cd", com_cd, "Change to directory DIR" },
  { "dir", com_dir, "List files" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "view", com_view, "View the contents of FILE" },
  { "run", com_run, "Run schema" },
  { "stop", com_stop, "Stop schema" },
  { "show", com_show, "Show schema gui" },
  { "hide", com_hide, "Hide schema gui" },
  { "init", com_init, "Init schema" },
  { "terminate", com_terminate, "Terminate schema" },
  { "resume", com_run, "Synonym for 'run'" },
  { "suspend", com_stop, "Synonym for 'stop'" },
  { "guiresume", com_show, "Synonym for 'show'" },
  { "guisuspend", com_hide, "Synonym for 'hide'" },
  { "?", com_help, "Synonym for 'help'" },
  { "exit", com_exit, "Exit schema mode" },
  { (const char *)NULL, (rl_cmdfunc_t *)NULL, (const char *)NULL }
};

/*format strings used for prompts*/
const char *bprompt = "jderobot $> ";
const char *sprompt = "jderobot[%s] $> ";

/*history file*/
const char *histfile_name = ".jderobot";

/* When non-zero, this global means the user is done using this program. */
int done;
SHELLSTATE shstate;
enum GENERATOR_ST generator_state;

JDEHierarchy* myhierarchy = 0;


void signal_handler(int sig){
  if (myhierarchy != 0)
    delete_JDEHierarchy(myhierarchy);
  exit(sig);
}

/**
 * Jde main function
 * @param argc Number of arguments
 * @param argv Array with the params
 * @return The end status
 */
int main(int argc, char** argv) {
  char *line, *s;/* shell buffers*/
  int n=1; /* argument number in the console for the configuration file parameter */
  char configfile[MAX_BUFFER] = {'\0'};
  char histfile_path[MAX_BUFFER];

  signal(SIGTERM, &signal_handler); /* kill interrupt handler */
  signal(SIGINT, &signal_handler); /* control-C interrupt handler */
  signal(SIGABRT, &signal_handler); /* failed assert handler */
  signal(SIGPIPE, SIG_IGN);
  
  /* Pablo Barrera: Por alguna raz�n libforms hace que fprintf("%f") ponga 
    una coma en vez de un punto como separador si las locales son espa�olas. 
    Con esto las ponemos a POSIX. Al salir el entorno es el normal */
  /*unsetenv("LANG");*/
  setenv("LC_ALL","POSIX",1);

  printf ("\n");
  printf (" <jderobot> Copyright (C) 1997-2009 JDE Developers Team \n");
  printf ("   This is free software, and you are welcome to redistribute it \n");
  printf ("   under certain conditions; type `show_license' for details. \n\n");

  
  printf("%s\n\n",thisrelease);

  /*histfile path*/
  strncpy(histfile_path,getenv("HOME"),MAX_BUFFER-1);
  histfile_path[MAX_BUFFER-1] = '\0';
  strncat(histfile_path,"/",MAX_BUFFER-1);
  histfile_path[MAX_BUFFER-1] = '\0';
  strncat(histfile_path,histfile_name,MAX_BUFFER-1);
  histfile_path[MAX_BUFFER-1] = '\0';

  n=1;
  while(argv[n]!=NULL) {
    if ((strcmp(argv[n],"--help")==0) ||
	(strcmp(argv[n],"-h")==0))
      {fprintf(stdout,"Use: jde [config.file]\n\n\t[config.file] Sets an specific config file. Don't use this option to read default configuration.\n\n");
      exit(0);}
    else if ((strcmp(argv[n],"--version")==0) ||
	     (strcmp(argv[n],"-v")==0))
      {fprintf(stdout,"%s\n",thisrelease);
      exit(0);}
    else if ((strcmp(argv[n],"--gui")==0) ||
	     (strcmp(argv[n],"-g")==0)){
      fprintf(stdout,"Not valid command line argument \"--gui\". If you want to \nauto activate the gui use the configuration file.");
    }
    else 
      {
	sprintf(configfile,"%s",argv[n]);
      }
    n++;
  }
  
  myhierarchy = new_JDEHierarchy(argc,argv,configfile);
  
  if (myhierarchy==0){
    fprintf(stdout,"Initialization failed...\n");
    exit(-1);
  }

  /* read commands from keyboard */
  fprintf(stdout,"Starting shell...\n");
  initialize_readline ();	/* Bind our completer. */
  read_history(histfile_path);

  /*initialize shstate*/
  shstate.state = BASE;
  shstate.commands = bcommands;
  strncpy(shstate.prompt,bprompt,256);
  shstate.prompt[255] = '\0';
  shstate.pdata = NULL;

  /* Loop reading and executing lines until the user quits. */
  while(done == 0){
    line = readline (shstate.prompt);
    
    if (!line)
      com_exit(NULL);
    else {
      /* Remove leading and trailing whitespace from the line.
	 Then, if there is anything left, add it to the history list
	 and execute it. */
      s = stripwhite (line);
    
      if (*s) {
	add_history (s);
	execute_line (s);
      }
      /*printf("%s\n",s);*/
      free (line);
    }
  }
  write_history(histfile_path);
  delete_JDEHierarchy(myhierarchy);
  pthread_exit(0); 
  /* If we don't need this thread anymore, 
     but want the others to keep running */
}

/**
 * Execute a command line.
 * @param line string to be parsed and executed
 * @return 0 on success,or -1 otherwise
 */
int
execute_line (char *line){
  COMMAND *command;
  char *word;
  JDESchema *s;

  /* Isolate the command word.
     line is modified if token was found, see strsep(3) */
  word = strsep(&line," ");

  command = find_command (word);
  if (command)
    return ((*(command->func)) (line));

  /*search in loaded schemas*/
  if ((s=JDEHierarchy_find_schema(myhierarchy,word)) != 0){/*cmd is a schema name->run it*/
    JDESchema_run(s,0);
    return 0;
  }

  /* no command or schema name found*/
  fprintf (stderr, "%s: No such command for jderobot.\n", word);
  return (-1);
}

/**
 * Look up NAME as the name of a command
 * @param name name to be found
 * @return a pointer to the command if found, null otherwise
 */
COMMAND *
find_command (const char *name){
  register int i;
    
  for (i = 0; shstate.commands[i].name; i++)
    if (strcmp (name, shstate.commands[i].name) == 0)
      return (&shstate.commands[i]);

  return ((COMMAND *)NULL);
}

/**
 * Strip whitespace from the start and end of STRING
 * @param string string to be striped
 * @return a pointer into STRING
 */
char *
stripwhite (char *string){
  register char *s, *t;

  for (s = string; isspace (*s); s++)
    ;
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && isspace (*t))
    t--;
  *++t = '\0';

  return s;
}

/* **************************************************************** */
/*                                                                  */
/*                  Interface to Readline Completion                */
/*                                                                  */
/* **************************************************************** */

char *command_generator(const char *, int);
char **command_completion(const char *, int, int);

/**
 * Tell the GNU Readline library how to complete.  We want to try to complete
 * on command names if this is the first word in the line and schema
 * names or filenames on second word.
 * @return void
*/
void
initialize_readline (){
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "jdec";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = command_completion;
}

/**
 * Attempt to complete on the contents of TEXT. START and END bound the
 * region of rl_line_buffer that contains the word to complete.
 * We can use the entire contents of rl_line_buffer in case we 
 * want to do some simple parsing.
 * @param text the word to complete
 * @param start text start index on rl_line_buffer
 * @param end text end index on rl_line_buffer
 * @return Return the array of matches or NULL if there aren't any.
 */
char **
command_completion (const char *text,int start, int end){
  char **matches;
  char cmd[256];

  matches = (char **)NULL;

  /* We set generator state depending upon the place we are
     inside the line */
  if (start == 0){/*line start*/
    switch(shstate.state){
    case BASE:
      generator_state = (COMMANDS|SCHEMAS);
      break;
    case SCHEMA:
      generator_state = COMMANDS;
      break;
    default:
      generator_state = 0;
    }
  }else {/*some commands have a schema name after*/
    if (sscanf(rl_line_buffer,"%s",cmd)){
      if ((strcmp (cmd,"run") == 0) || 
	  (strcmp (cmd,"stop") == 0) ||
	  (strcmp (cmd,"show") == 0) ||
	  (strcmp (cmd,"hide") == 0) ||
	  (strcmp (cmd,"zoom") == 0) )
	generator_state = SCHEMAS;
      else
	generator_state = 0;/*reset state*/
    }
  }
  /*completion_matches for mac headers...*/
  matches = rl_completion_matches (text, command_generator);

  return (matches);
}

/**
 * Generator function for command completion. 
 * @param text command to complete
 * @param state lets us know whether
 * @return one possible completion
 */
char *
command_generator (const char *text,int state){
  static int list_index, len;
  static int schema_index;
  static int schema_list_index,schema_list_size;
  const char *name;
  char str[256];

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
      list_index = 0;
      schema_index = 0;
      schema_list_index = 0;
      pthread_mutex_lock(&(myhierarchy->mutex));
      schema_list_size = list_size(&(myhierarchy->schema_list));
      pthread_mutex_unlock(&(myhierarchy->mutex));
      len = strlen (text);
  }

  /* Return the next name which partially matches from the command
     list. */
  if (generator_state & COMMANDS) {
    while ((name = shstate.commands[list_index].name)) {
      list_index++;
      if (strncmp (name, text, len) == 0)
	return (strdup(name));
    }
  }

  /*search on schema names*/
  if (generator_state & SCHEMAS) {
    while( schema_list_index < schema_list_size ){
      pthread_mutex_lock(&(myhierarchy->mutex));
      JDESchema *s = list_get_at(&(myhierarchy->schema_list),
				 schema_list_index);
      pthread_mutex_unlock(&(myhierarchy->mutex));
      schema_list_index++;
      if (s!=0){
	sprintf(str,"%s",s->name);
	if (strncmp (str, text, len) == 0)
	  return (strdup(str));
      }
    }

    /*search in old hierarchy 'all'*/
    while( schema_index < num_schemas) {
      sprintf(str,"%s",all[schema_index].name);
      schema_index++;
      if (strncmp (str, text, len) == 0)
	return (strdup(str));
    }

  }

  
  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/* **************************************************************** */
/*                                                                  */
/*                       jdec Commands                              */
/*                                                                  */
/* **************************************************************** */

/**
 * String to pass to system (). 
 * This is for the LIST, VIEW and RENAME commands.
 */
static char syscom[1024];

/**
 * Check if a command requires args
 * @param caller calling command
 * @param arg argument string
 * @return non-zero if ARG is a valid argument for CALLER, else print
 * an error message and return zero.
 */
int
valid_argument (const char *caller, const char *arg){
  if (!arg || !*arg)
    {
      fprintf (stderr, "%s: Argument required.\n", caller);
      return (0);
    }

  return (1);
}

/**
 * List files
 * @param arg arguments. If not null ls is done over this filename
 * @return -1 on error, and the return status of ls otherwise
 */
int
com_dir (char *arg){
  const char *a = arg;

  if (!a)
    a = "";

  sprintf (syscom, "ls -FClg %s", a);
  return system (syscom);
}

/**
 * List loaded schemas
 * @param arg arguments. Ignored
 * @return 0
 */
int
com_list (char *ignore){
/*   int i; */

/*   for(i=0;i<num_schemas;i++) */
/*     printf("%s\n",all[i].name); */

  pthread_mutex_lock(&(myhierarchy->mutex));
  list_iterator_start(&(myhierarchy->schema_list));
  while(list_iterator_hasnext(&(myhierarchy->schema_list))){
    JDESchema *s = 
      (JDESchema*)list_iterator_next(&(myhierarchy->schema_list));
    printf("%s\n",s->name);
  }
  list_iterator_stop(&(myhierarchy->schema_list));
  pthread_mutex_unlock(&(myhierarchy->mutex));
  return 0;
}

/**
 * View contents of a file
 * @param arg arguments. File to be viewed
 * @return -1 on error and the return status of the viewer otherwise
 */
int
com_view (char *arg){
  if (!valid_argument ("view", arg))
    return -1;

#if defined (__MSDOS__)
  /* more.com doesn't grok slashes in pathnames */
  sprintf (syscom, "less %s", arg);
#else
  sprintf (syscom, "more %s", arg);
#endif
  return (system (syscom));
}

/**
 * Print out JDE license
 * @param arg arguments.Command or null
 * @return 0
 */

int
com_license (char *arg)
{

	
	char license[] = "\n  Copyright (C) 1997-2009 JDE Developers Team \n\n" \
  "This program is free software: you can redistribute it and/or modify \n" \
  "it under the terms of the GNU General Public License as published by \n" \
  "the Free Software Foundation, either version 3 of the License, or \n" \
  "(at your option) any later version. \n" \
  "\n" \
  "This program is distributed in the hope that it will be useful, \n" \
  "but WITHOUT ANY WARRANTY; without even the implied warranty of \n" \
  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \n" \
  "GNU General Public License for more details. \n" \
  "\n" \
  "You should have received a copy of the GNU General Public License \n" \
  "along with this program.  If not, see http://www.gnu.org/licenses/. \n" \
  "\n" \
  "You can download the source code in http://jde.gsyc.es/index.php/Downloads\n\n";

  fprintf(stdout, license);

	return (0);

}

/**
 * Print out help for a command or for all if no command provided
 * @param arg arguments.Command or null
 * @return 0
 */
int
com_help (char *arg){
  register int i;
  int printed = 0;

  for (i = 0; shstate.commands[i].name; i++){
    if (!arg || (strcmp (arg, shstate.commands[i].name) == 0)) {
      fprintf (stdout,"%s\t\t%s.\n", shstate.commands[i].name, shstate.commands[i].doc);
      printed++;
    }
  }

  if (!printed) {
    fprintf (stdout,"No commands match `%s'.  Possibilties are:\n", arg);

    for (i = 0; shstate.commands[i].name; i++) {
      /* Print in six columns. */
      if (printed == 6)	{
	printed = 0;
	printf ("\n");
      }

      fprintf (stdout,"%s\t", shstate.commands[i].name);
      printed++;
    }

    if (printed)
      fprintf (stdout,"\n");
  }
  return (0);
}

/**
 * Change to the directory ARG.
 * @param arg arguments. Dir to change
 * @return -1 on error, 0 otherwise
 */
int
com_cd (char *arg){
  if (chdir (arg) == -1)
    {
      perror (arg);
      return -1;
    }

  com_pwd (0);
  return (0);
}

/**
 * Print out the current working directory
 * @param arg arguments. Ignored
 * @return -1 on error, 0 otherwise
 */
int
com_pwd (char *ignore){
  char dir[1024], *s;

  s = getcwd (dir, sizeof(dir) - 1);
  if (s == 0)
    {
      fprintf (stderr, "Error getting pwd: %s\n", dir);
      return -1;
    }

  fprintf (stdout,"Current directory is %s\n", dir);
  return 0;
}

/**
 * The user wishes to quit using this program or exit from SCHEMA mode
 * Just set DONE non-zero or return to BASE mode
 * @param arg arguments. Ignored
 * @return 0
 */
int
com_exit (char *ignored){
  switch(shstate.state){
  case BASE:
    done = 1;
    break;
  case SCHEMA:
  default:
    break;
  }
  shstate.state = BASE;
  shstate.commands = bcommands;
  strncpy(shstate.prompt,bprompt,256);
  shstate.prompt[255] = '\0';
  shstate.pdata = NULL;
  printf ("\n");

  return (0);
}

/**
 * Load driver/service shared object
 * @param arg arguments. File to be loaded
 * @return -1 on error, 0 otherwise
 */
int
com_load_driver (char *arg){
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  char *cf;
  int words;
  JDEDriver* d;

  if (!valid_argument ("load_driver", arg))
    return -1;

  words=sscanf(arg,"%s %s",word,word2);
  if (words == 1){
    cf = get_configfile();
  }else if (words==2){
    cf = word2;
  }else{
    fprintf (stderr, "load_driver command accept 1 or 2 args only: load_driver <driver.so> [<config file>]");	
    return -1;
  }
  
  d = jde_loaddriver(word);
  if (d == 0){
    fprintf(stderr,"Driver/Service loading failed\n");
    return -1;
  }
  JDEDriver_init(d,cf);
  return 0;
}

/**
 * Load schema shared object
 * @param arg arguments. File to be loaded
 * @return -1 on error, 0 otherwise
 */
int
com_load_schema (char *arg){
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int words;
  char *cf;
  JDESchema *s;

  if (!valid_argument ("load_schema", arg))
    return -1;
  
  words=sscanf(arg,"%s %s",word,word2);
  if (words == 1){
    cf = get_configfile();
  }else if (words==2){
    cf = word2;
  }else{
    fprintf (stderr, "load_schema command accept 1 or 2 args only: load_schema <schema.so> [<config file>]");	
    return -1;
  }

  s = jde_loadschema(word);
  if (s == 0){
    fprintf(stderr,"Schema loading failed\n");
    return -1;
  }
  JDEHierarchy_add_schema(myhierarchy,s);
  JDESchema_init(s,cf);
  return 0;
}

/**
 * Load a module
 * @param arg arguments. File to be loaded
 * @return -1 on error, 0 otherwise
 */
int
com_load_schema2 (char *arg){
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int words;
  char *cf;
  JDESchema *s;

  if (!valid_argument ("load_schema2", arg))
    return -1;
  
  words=sscanf(arg,"%s %s",word,word2);
  if (words == 1){
    cf = get_configfile();
  }else if (words==2){
    cf = word2;
  }else{
    fprintf (stderr, "load_schema2 command accept 1 or 2 args only: load_schema2 <module> [<config file>]");	
    return -1;
  }

  s = load_schema(word,cf);
  if (s==0){
    fprintf(stderr,"Module loading failed\n");
    return -1;
  }
  JDEHierarchy_add_schema(myhierarchy,s);
  JDESchema_init(s,cf);
  return 0;
}

/**
 * Print all schema state
 * @param arg arguments. Ignored
 * @return 0
 */
int
com_ps (char *ignored){
  /* int i,j,k; */


  /* for(i=0;i<num_schemas;i++){ */
/*     if ((all[i].state==winner) || */
/* 	(all[i].state==notready) || */
/* 	(all[i].state==ready)) { */
/*       fprintf(stdout,"%s: %.0f ips, ",all[i].name,all[i].fps); */
/*       if (all[i].state==winner) { */
/* 	fprintf(stdout,"winner ( "); */
/* 	k=0; */
/* 	for (j=0;j<num_schemas;j++) */
/* 	  if (all[i].children[j]==TRUE) { */
/* 	    if (k==0) { */
/* 	      fprintf(stdout,"\b");k++; */
/* 	    } */
/* 	    fprintf(stdout,"%s ",all[j].name); */
/* 	  } */
/* 	fprintf(stdout,"\b)"); */
/*       } */
/*       else if (all[i].state==slept) */
/* 	fprintf(stdout,"slept"); */
/*       else if (all[i].state==notready) */
/* 	fprintf(stdout,"notready"); */
/*       else if (all[i].state==ready) */
/* 	fprintf(stdout,"ready"); */
/*       else if (all[i].state==forced) */
/* 	fprintf(stdout,"forced"); */
/*       fprintf(stdout,"\n"); */
/*     } */
/*   } */

  pthread_mutex_lock(&(myhierarchy->mutex));
  list_iterator_start(&(myhierarchy->schema_list));
  while(list_iterator_hasnext(&(myhierarchy->schema_list))){
    JDESchema *s = 
      (JDESchema*)list_iterator_next(&(myhierarchy->schema_list));
    int state = JDESchema_get_state(s);
    fprintf(stdout,"%s: %.0f ips, %s\n",s->name,s->fps,states_str[state]);
  }
  list_iterator_stop(&(myhierarchy->schema_list));
  pthread_mutex_unlock(&(myhierarchy->mutex));
  return 0;
}

/**
 * Run a schema
 * @param arg arguments. Schema to run
 * @return -1 on error, 0 otherwise
 */
int
com_run (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("run", arg))
      return -1;
    if ((s=JDEHierarchy_find_schema(myhierarchy,arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return -1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return -1;

  JDESchema_run(s,0);
  return 0;
}

/**
 * Stop a schema
 * @param arg arguments. Schema to stop
 * @return -1 on error, 0 otherwise
 */
int
com_stop (char *arg){
  JDESchema *s;
  
  if (shstate.state == BASE){
    if (!valid_argument ("stop", arg))
      return -1;
    if ((s=JDEHierarchy_find_schema(myhierarchy,arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return -1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return -1;
  JDESchema_stop(s);
  return 0;
}

/**
 * Show a schema
 * @param arg arguments. Schema to show
 * @return -1 on error, 0 otherwise
 */
int
com_show (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("show", arg))
      return -1;
    if ((s=JDEHierarchy_find_schema(myhierarchy,arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return -1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return -1;
  
  JDESchema_show(s);
  return 0;
}

/**
 * Hide a schema
 * @param arg arguments. Schema to hide
 * @return -1 on error, 0 otherwise
 */
int
com_hide (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("hide", arg))
      return -1;
    if ((s=JDEHierarchy_find_schema(myhierarchy,arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return -1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return -1;

  JDESchema_hide(s);
  return 0;
}

/**
 * Init a schema
 * @param arg arguments. Config file or null
 * @return 0
 */
int
com_init (char *arg){
  JDESchema *s = (JDESchema*)shstate.pdata;

  if (arg==0 || *arg == '\0')/*no args, use global configfile*/
    JDESchema_init(s,get_configfile());
  else
    JDESchema_init(s,arg);
  return 0;
}

/**
 * Terminate a schema
 * @param arg arguments. Ignored
 * @return 0
 */
int
com_terminate (char *ignored){
  JDESchema *s = (JDESchema*)shstate.pdata;
  
  JDESchema_terminate(s);
  return 0;
}

/**
 * Go to SCHEMA mode
 * @param arg arguments. Schema name
 * @return -1 on error, 0 otherwise
 */
int com_zoom(char *arg){
  JDESchema *s;

  if (!valid_argument ("zoom", arg))
    return -1;
  if ((s=JDEHierarchy_find_schema(myhierarchy,arg)) == 0){
    fprintf (stderr,"%s: unknown schema\n",arg);
    return -1;
  }
  
  shstate.state = SCHEMA;
  shstate.commands = scommands;
  snprintf(shstate.prompt,256,sprompt,s->name);
  shstate.pdata = (void*)s;
  return 0;
}
