#include <formats.h>
#include <string.h>

/*each entry have to match with PixelFormat enums*/
static FormatEntry const pixelFormatInfos[] = {
  {"NONE",{0,0,{0,0,0,0},0}}, /*0*/
  {"RGBA_8888",{4,32,{0xFF000000,0xFF0000,0xFF00,0xFF},RGBA}}, /*1*/
  {"RGBX_8888",{4,24,{0,0xFF0000,0xFF00,0xFF},RGB}}, /*2*/
  {"RGB_888",{3,24,{0,0xFF0000,0xFF00,0xFF},RGB}}, /*3*/
  {"RGB_565",{2,16,{0,0xF800,0x7E0,0x1F},RGB}}, /*4*/
  {"RGB_332",{1,8,{0,0xE0,0x1C,0x3},RGB}}, /*5*/
  {"L_8",{1,8,{0,0xFF,0xFF,0xFF},LUMINANCE}}, /*6*/
  {"YUV2",{1,16,{0xFF,0xFF,0xFF,0},YUV}}, /*7*/
};

const FormatEntry* getPixelFormatTable(size_t *nEntries){
  if (nEntries!=0)
    *nEntries = sizeof(pixelFormatInfos)/sizeof(pixelFormatInfos[0]);
  
  return pixelFormatInfos;
}


/*FIXME: use strncmp to avoid problems-> define MAX_BUFFER somewhere*/
const Format* searchPixelFormat(const char *format_name){
  unsigned int n,i;
  const FormatEntry *fmtT = getPixelFormatTable(&n);

  for (i=0; i<n; i++){
    if (strcmp(format_name,fmtT[i].format_name) == 0)
      return &(fmtT[i].format);
  }
  return &(fmtT[0].format);/*NONE*/
}
