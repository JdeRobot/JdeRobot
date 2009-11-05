#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <glib.h>

/*jdec includes*/
#include <loader.h>
#include <schema.h>

/* The names of functions that actually do the manipulation. */
int com_list(char *);
int com_listf(char *);
//int com_listi(char *);
int com_view(char *);
int com_rename(char *);
int com_stat(char *);
int com_pwd(char *);
int com_delete(char *);
int com_help(char *);
int com_cd(char *);
int com_exit(char *);

int com_load(char *);
//int com_sfactory(char *);
int com_finstance(char *);

/* A structure which contains information on the commands this program
   can understand. */


typedef struct {
  const char *name;		/* User printable name of the function. */
  rl_icpfunc_t *func;		/* Function to call to do the job. */
  const char *doc;		/* Documentation for this function.*/
} COMMAND;

enum STATES { BASE, FACTORY, INSTANCE };
typedef struct {
  STATES state;
  COMMAND *commands;
  char prompt[256];
  void *pdata;
} PSTATE;


COMMAND bcommands[] = {
  { "cd", com_cd, "Change to directory DIR" },
  { "help", com_help, "Display this text" },
  { "?", com_help, "Synonym for `help'" },
  { "list", com_list, "List files" },
  { "ls", com_list, "Synonym for `list'" },
  { "listf", com_listf, "List loaded factories" },
  { "lf", com_listf, "Synonym for `listf'" },
  { "listi", com_listi, "List created instances" },
  { "li", com_listi, "Synonym for `listi'" },
  { "load", com_load, "Load shared object" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "exit", com_exit, "Quit using jdeC" },
  { "view", com_view, "View the contents of FILE" },
  { (const char *)NULL, (rl_icpfunc_t *)NULL, (const char *)NULL }
};
const char *bprompt = "jdeC $> ";

COMMAND fcommands[] = {
  { "help", com_help, "Display this text" },
  { "instance", com_finstance, "Create a new instance" },
  { "listi", com_listi, "List created instances of this factory" },
  { "li", com_listi, "Synonym for `listi'" },
  { "?", com_help, "Synonym for `help'" },
  { "exit", com_exit, "Exit factory mode" },
  { (const char *)NULL, (rl_icpfunc_t *)NULL, (const char *)NULL }
};

/*format strings used for factory cmds and prompts*/
const char *fcmd = "%s@%s";
const char *fprompt = "jdeC[%s@%s] $> ";

//COMMAND sf_cmd = { "", com_sfactory, "" };

/* Forward declarations. */
char *stripwhite (char *string);
COMMAND *find_command (const char *name);
void initialize_readline ();
int execute_line (char *line);

/* When non-zero, this global means the user is done using this program. */
int done;

/* Parsing state.
   Global var modified in:
       main()
       execute_line()
       com_exit()
*/
PSTATE pstate;

int
main (int argc,char **argv){
  char *line, *s;

  initialize_readline ();	/* Bind our completer. */

  /*initialize pstate*/
  pstate.state = BASE;
  pstate.commands = bcommands;
  strncpy(pstate.prompt,bprompt,256);
  pstate.prompt[255] = '\0';
  pstate.pdata = NULL;

  /* Loop reading and executing lines until the user quits. */
  while(done == 0){
    line = readline (pstate.prompt);
    
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
      //printf("%s\n",s);
      free (line);
    }
  }
  printf("Bye!\n");
  exit (0);
}

/* Execute a command line. */
int
execute_line (char *line){
  COMMAND *command;
  char *word;
  const GList *sf_list_index;
  char str[256];

  /* Isolate the command word.
     line is modified if token was found, see strsep(3) */
  word = strsep(&line," ");

  command = find_command (word);
  if (command)
    return ((*(command->func)) (line));

  /*search in loaded factories*/
  sf_list_index = SFactory_get_factories();
  while( sf_list_index != 0 ) {
    sprintf(str,fcmd,((SFactory*)sf_list_index->data)->schema_name,
	    ((SFactory*)sf_list_index->data)->interface_name);
    if (strcmp (str, word) == 0) {
      pstate.state = FACTORY;
      pstate.commands = fcommands;
      snprintf(pstate.prompt,256,fprompt,
	       ((SFactory*)sf_list_index->data)->schema_name,
	       ((SFactory*)sf_list_index->data)->interface_name);
      pstate.pdata = sf_list_index->data;
      return 0;
    }
    sf_list_index = g_list_next(sf_list_index);
  }

  /* no command or factory name found*/
  fprintf (stderr, "%s: No such command for jdeC.\n", word);
  return (-1);
}


/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */
COMMAND *
find_command (const char *name){
  register int i;
    
  for (i = 0; pstate.commands[i].name; i++)
    if (strcmp (name, pstate.commands[i].name) == 0)
      return (&pstate.commands[i]);

  return ((COMMAND *)NULL);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char *
stripwhite (char *string){
  register char *s, *t;

  for (s = string; whitespace (*s); s++)
    ;
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
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

/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
void
initialize_readline (){
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "jdeC";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = command_completion;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char **
command_completion (const char *text,int start, int end){
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = rl_completion_matches (text, command_generator);

  return (matches);
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char *
command_generator (const char *text,int state){
  static int list_index, len;
  static const GList *sf_list_index, *i_list_index;
  const char *name;
  char str[256];

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
      list_index = 0;
      sf_list_index = SFactory_get_factories();
      //i_list_index = Schema_get_instances();
      len = strlen (text);
  }

  /* Return the next name which partially matches from the command
     list. */
  while ((name = pstate.commands[list_index].name)) {
    list_index++;
	
    if (strncmp (name, text, len) == 0)   
      return (strdup(name));
  }

  /*search on factory names*/
  while( pstate.state != FACTORY && sf_list_index != 0) {
    sprintf(str,fcmd,((SFactory*)sf_list_index->data)->schema_name,
	    ((SFactory*)sf_list_index->data)->interface_name);
    sf_list_index = g_list_next(sf_list_index);
    if (strncmp (str, text, len) == 0)
      return (strdup(str));
  }

  /*search on instances names*/
  while( i_list_index != 0 ) {
    sprintf(str,"%s@%s",((SFactory*)i_list_index->data)->schema_name,
	    ((SFactory*)i_list_index->data)->interface_name);
    i_list_index = g_list_next(i_list_index);
    if (strncmp (str, text, len) == 0)
      return (strdup(str));
  }


  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/* **************************************************************** */
/*                                                                  */
/*                       jdeC Commands                              */
/*                                                                  */
/* **************************************************************** */

/* String to pass to system ().  This is for the LIST, VIEW and RENAME
   commands. */
static char syscom[1024];

/* Return non-zero if ARG is a valid argument for CALLER, else print
   an error message and return zero. */
int
valid_argument (const char *caller, const char *arg){
  if (!arg || !*arg)
    {
      fprintf (stderr, "%s: Argument required.\n", caller);
      return (0);
    }

  return (1);
}

/* List the file(s) named in arg. */
int
com_list (char *arg){
  const char *a = arg;

  if (!a)
    a = "";

  sprintf (syscom, "ls -FClg %s", a);
  return system (syscom);
}

/* List the file(s) named in arg. */
int
com_listf (char *arg){
  const GList* i;

  for (i = SFactory_get_factories(); i != 0; i = g_list_next(i))
    printf("%s implementing %s\n",
	   ((SFactory*)i->data)->schema_name,
	   ((SFactory*)i->data)->interface_name);

  return 0;
}

int
com_view (char *arg){
  if (!valid_argument ("view", arg))
    return 1;

#if defined (__MSDOS__)
  /* more.com doesn't grok slashes in pathnames */
  sprintf (syscom, "less %s", arg);
#else
  sprintf (syscom, "more %s", arg);
#endif
  return (system (syscom));
}


/* Print out help for ARG, or for all of the commands if ARG is
   not present. */
int
com_help (char *arg){
  register int i;
  int printed = 0;

  for (i = 0; pstate.commands[i].name; i++){
    if (!arg || (strcmp (arg, pstate.commands[i].name) == 0)) {
      printf ("%s\t\t%s.\n", pstate.commands[i].name, pstate.commands[i].doc);
      printed++;
    }
  }

  if (!printed) {
    printf ("No commands match `%s'.  Possibilties are:\n", arg);

    for (i = 0; pstate.commands[i].name; i++) {
      /* Print in six columns. */
      if (printed == 6)	{
	printed = 0;
	printf ("\n");
      }

      printf ("%s\t", pstate.commands[i].name);
      printed++;
    }

    if (printed)
      printf ("\n");
  }
  return (0);
}

/* Change to the directory ARG. */
int
com_cd (char *arg){
  if (chdir (arg) == -1)
    {
      perror (arg);
      return 1;
    }

  com_pwd (0);
  return (0);
}

/* Print out the current working directory. */
int
com_pwd (char *ignore){
  char dir[1024], *s;

  s = getcwd (dir, sizeof(dir) - 1);
  if (s == 0)
    {
      printf ("Error getting pwd: %s\n", dir);
      return 1;
    }

  printf ("Current directory is %s\n", dir);
  return 0;
}

/* The user wishes to quit using this program.  Just set DONE
   non-zero. */
int
com_exit (char *arg){
  switch(pstate.state){
  case BASE:
    done = 1;
    break;
  case FACTORY:
    break;
  case INSTANCE:
    break;
  }
  pstate.state = BASE;
  pstate.commands = bcommands;
  strncpy(pstate.prompt,bprompt,256);
  pstate.prompt[255] = '\0';
  pstate.pdata = NULL;
  printf ("\n");

  return (0);
}

/* Load ARG so. */
int
com_load (char *arg){
  if (!valid_argument ("load", arg))
    return 1;

  if (load_so (arg) != 0)
    return 1;

  return 0;
}

//int com_sfactory(char *fname){
  
int com_finstance(char *arg) {
  Schema* i;

  if (pstate.state == FACTORY && pstate.pdata )
    i = SFactory_create((SFactory*)pstate.pdata);
  return 0;
}
