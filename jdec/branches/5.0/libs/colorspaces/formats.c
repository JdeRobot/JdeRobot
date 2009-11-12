#include <formats.h>

static Format const pixelFormatInfos[] = {
  {0,0,{0,0,0,0},0}, /*PIXEL_FORMAT_NONE*/
  {4,32,{0xFF000000,0xFF0000,0xFF00,0xFF},RGBA}, /*PIXEL_FORMAT_RGBA_8888*/
  {4,24,{0,0xFF0000,0xFF00,0xFF},RGB}, /*PIXEL_FORMAT_RGBX_8888*/
  {3,24,{0,0xFF0000,0xFF00,0xFF},RGB}, /*PIXEL_FORMAT_RGB_888*/
  {2,16,{0,0xF800,0x7E0,0x1F},RGB}, /*PIXEL_FORMAT_RGB_565*/
  {1,8,{0,0xE0,0x1C,0x3},RGB}, /*PIXEL_FORMAT_RGB_332*/
  {1,8,{0,0xFF,0xFF,0xFF},LUMINANCE}, /*PIXEL_FORMAT_L_8*/
  {1,16,{0xFF,0xFF,0xFF,0},YUV}, /*PIXEL_FORMAT_YUV2*/
  {1,16,{0xFF,0xFF,0xFF,0},YUV}, /*PIXEL_FORMAT_YUYV*/
};

const Format* getPixelFormatTable(size_t *nEntries){
  if (nEntries!=0)
    *nEntries = sizeof(pixelFormatInfos)/sizeof(pixelFormatInfos[0]);
  
  return pixelFormatInfos;
}
