#include "formats.h"

#include <string.h>

/*each entry have to match with PixelFormat enums*/
static const Format const formatTable[] = {
  {"NONE",FORMAT_NONE,0,0}, /*0*/
  {"RGB_888",FORMAT_RGB_888,3,24}, /*1*/
  {"YUY2",FORMAT_YUY2,2,16}, /*2*/
  {"L_8",FORMAT_L_8,1,8} /*3*/
};
  
const Format* getFormatTable(int *nEntries){
  if (nEntries!=0)
    *nEntries = sizeof(formatTable)/sizeof(formatTable[0]);
  
  return formatTable;
}


/*FIXME: use strncmp to avoid problems-> define MAX_BUFFER somewhere*/
const Format* searchFormat(const char *format_name){
  int n,i;
  const Format *fmtT = getFormatTable(&n);
  
  for (i=0; i<n; i++){
    if (strcmp(format_name,fmtT[i].format_name) == 0)
      return &fmtT[i];
  }
  return 0;
}

int Format_snprintf(char *str, size_t size, const Format * const self){
  return snprintf(str,size, "fmt(%s;%d;%d)",
		  self->format_name,self->pixelSize,self->bitsPerPixel);
}

int Format_fprintf(FILE *stream, const Format * const self){
  char str[512];

  Format_snprintf(str,512,self);
  return fprintf(stream,"%s",str);
}

int Format_printf(const Format * const self){
  return Format_fprintf(stdout,self);
}
