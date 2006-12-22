#include <stdio.h>
#include <stdlib.h>
#include "libconfig.h"

int main(int argc, char **argv)
  {
  config_t cfg;
  FILE *fp = fopen("test.cfg", "r");
  int i, x;

  config_init(&cfg);
  x =config_read(&cfg, fp);

  fclose(fp);

  if(! x)
    {
    printf("error on line %d: %s\n", cfg.error_line, cfg.error_text);
    exit(1);
    }

  /*
  config_setting_t *s = config_lookup(cfg, "top.group1.group2.zzz");

  if(! s)
    puts("NOT FOUND");
  else
    printf("FOUND! name = %s, type = %d\n", s->name, s->type);
  */

  const char *s = config_lookup_string(&cfg, "application.group1.group2.zzz");

  if(! s)
    puts("NOT FOUND");
  else
    printf("found: %s\n", s);

  const config_setting_t *array = config_lookup(&cfg, "application.group1.my_array");
  if(array)
    {
    for(i = 0; i < config_setting_length(array); i++)
      {
      long val = config_setting_get_int_elem(array, i);
      printf("value [%i]: %ld\n", i, val);
      }
    }

  {
  config_setting_t *root = config_root_setting(&cfg);

  config_setting_t *new = config_setting_add(root, "foobar", CONFIG_TYPE_FLOAT);

  config_setting_set_float(new, 3.141592654);

  config_setting_t *old = config_lookup(&cfg, "application.group1");

  printf("old is: %p\n", old);
  config_setting_remove(old, "group2");
  }



  config_write(&cfg, stdout);

  
  config_destroy(&cfg);

  
  return(0);
  }
