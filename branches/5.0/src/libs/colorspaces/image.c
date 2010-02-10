#include "image.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*min and max macros*/
#define max(a,b)				\
  ({ typeof (a) _a = (a);			\
    typeof (b) _b = (b);			\
    _a > _b ? _a : _b; })

#define min(a,b)				\
  ({ typeof (a) _a = (a);			\
    typeof (b) _b = (b);			\
    _a < _b ? _a : _b; })


void FPyuv2rgb(const unsigned char y, const unsigned char u, const unsigned char v,
	       unsigned char * const r, unsigned char * const g, unsigned char * const b){
  int _y,_u,_v,_r,_g,_b;
  _y = (int)y;
  _u = (int)u;
  _v = (int)v;
  
  _r = min((9535 * (_y - 16) + 13074 * (_v - 128)) >> 13, 255);
  _g = min((9535 * (_y - 16) -  6660 * (_v - 128) - 3203 * (_u - 128)) >> 13, 255);
  _b = min((9535 * (_y - 16) + 16531 * (_u - 128)) >> 13, 255);

  _r = max(_r,0);
  _g = max(_g,0);
  _b = max(_b,0);
  
  *r = (unsigned char)_r;
  *g = (unsigned char)_g;
  *b = (unsigned char)_b;
}

void FPrgb2yuv(const unsigned char r, const unsigned char g, const unsigned char b,
	       unsigned char * const y, unsigned char * const u, unsigned char * const v){
  unsigned int _y,_u,_v,_r,_g,_b;

  _r = (unsigned int)r;
  _g = (unsigned int)g;
  _b = (unsigned int)b;

  _y = min(abs(_r * 2104 + _g * 4130 + _b * 802 + 4096 + 131072) >> 13, 235);
  _u = min(abs(_r * -1214 + _g * -2384 + _b * 3598 + 4096 + 1048576) >> 13, 240);
  _v = min(abs(_r * 3598 + _g * -3013 + _b * -585 + 4096 + 1048576) >> 13, 240);
  *y = (unsigned char)_y;
  *u = (unsigned char)_u;
  *v = (unsigned char)_v;
}

ImageDescription* new_ImageDescription(const int width, 
				       const int height,
				       const int size,
				       const Format *fmt){
  ImageDescription *idesc;
  
  idesc = calloc(1,sizeof(ImageDescription));
  ImageDescription_initialize(idesc,width,height,size,fmt);
  return idesc;
}

void ImageDescription_initialize(ImageDescription* const self,
				 const int width,
				 const int height,
				 const int size,
				 const Format *fmt){
  assert(self!=0);
  self->width = width;
  self->height = height;
  self->size = size;
  if (fmt==0)
    self->fmt = &getFormatTable(0)[FORMAT_NONE];
  else
    self->fmt = fmt;
};

void ImageDescription_deinitialize(ImageDescription* const self){
  assert(self!=0);
}

void delete_ImageDescription(ImageDescription * const self){
  if (self != 0){
    ImageDescription_deinitialize(self);
    free(self);
  }
}

int ImageDescription_snprintf(char *str, size_t size, const ImageDescription * const self){
  char fmtStr[256];

  Format_snprintf(fmtStr,256, self->fmt);
  return snprintf(str,size, "imgDesc(%d;%d;%d;%s)",
		  self->width,self->height,self->size,fmtStr);
}

int ImageDescription_fprintf(FILE *stream, const ImageDescription * const self){
  char str[512];

  ImageDescription_snprintf(str,512,self);
  return fprintf(stream,"%s",str);
}

int ImageDescription_printf(const ImageDescription * const self){
  return ImageDescription_fprintf(stdout,self);
}

Image* new_Image(const ImageDescription* desc, char * const data){
  Image *i;
  
  i = calloc(1,sizeof(Image));
  Image_initialize(i,desc,data);
  return i;
}

void Image_initialize(Image* const self,
		      const ImageDescription* desc,
		      char * const data){
  assert(self!=0);
  self->description = *desc;
  if (data==0){
    Image_ownData(self);
    self->imageData = (char*)calloc(desc->size,sizeof(char));
  }else{
    Image_disownData(self);
    self->imageData = data;
  }
}

Image* Image_copy(Image* const self){
  Image *outImg = new_Image(&self->description,0);

  memmove(outImg->imageData,self->imageData,self->description.size);
  return outImg;
}

void Image_swapDataOwner(Image* const oldOwner, Image* const newOwner){
  if (oldOwner->imageDataOwned == 1){
    Image_disownData(oldOwner);
    Image_ownData(newOwner);
  }
}

void Image_ownData(Image* const self){
  self->imageDataOwned = 1;
}

void Image_disownData(Image* const self){
  self->imageDataOwned = 0;
}
  

void Image_deinitialize(Image* const self){
  assert(self!=0);
  if (self->imageDataOwned!=0)
    free(self->imageData);
}

void delete_Image(Image * const self){
  if (self!=0){
    Image_deinitialize(self);
    free(self);
  }
}

Image* Image_toL8fromRGB888(const Image * const in){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_L_8]);
  int nPixels,i;

  nPixels = in->description.width*in->description.height;
  outDesc = new_ImageDescription(in->description.width,
				 in->description.height,
				 nPixels,
				 outFmt);
  
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i++){
    unsigned char r,g,b;
    unsigned int j,l;
    j=i*in->description.fmt->pixelSize; /*RGB image iterator*/
    r = (unsigned char)in->imageData[j];
    g = (unsigned char)in->imageData[j+1];
    b = (unsigned char)in->imageData[j+2];
    l = (((unsigned int)r) * 11 + ((unsigned int)g) * 16 + ((unsigned int)b) * 5)>>5;/* /32 */
    out->imageData[i] = (unsigned char)l;
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toL8fromYUY2(const Image * const in){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_L_8]);
  int nPixels,i;

  nPixels = in->description.width*in->description.height;
  outDesc = new_ImageDescription(in->description.width,
				 in->description.height,
				 nPixels,
				 outFmt);
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i++){
    int j;
    j=i*in->description.fmt->pixelSize; /*YUV image iterator*/
    out->imageData[i] = in->imageData[j];/*Y*/
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toL8(const Image * const self){
  assert(self!=0);

  if (self->description.fmt->format == FORMAT_L_8)
    return new_Image(&self->description,self->imageData);
  else if (self->description.fmt->format == FORMAT_RGB_888)
    return Image_toL8fromRGB888(self);
  else if (self->description.fmt->format == FORMAT_YUY2)
    return Image_toL8fromYUY2(self);
  else
    return 0;
}

Image* Image_toYUY2fromRGB888(const Image *const self){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_YUY2]);
  int nPixels,i;

  nPixels = self->description.width*self->description.height;
  outDesc = new_ImageDescription(self->description.width,
				 self->description.height,
				 nPixels*outFmt->pixelSize,
				 outFmt);
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i++){
    int j = i*3;/*byte index for rgb24*/
    int k = i*2;/*byte index for YUV2*/
    unsigned char y,u,v;
    FPrgb2yuv((unsigned char)self->imageData[j],
	      (unsigned char)self->imageData[j+1],
	      (unsigned char)self->imageData[j+2],
	      &y,&u,&v);
    out->imageData[k] = (char)y;
    if (i%2 == 0)/*even pixels use U*/
      out->imageData[k+1] = (char)u;
    else
      out->imageData[k+1] = (char)v;
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toYUY2fromL8(const Image *const self){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_YUY2]);
  int nPixels,i;

  nPixels = self->description.width*self->description.height;
  outDesc = new_ImageDescription(self->description.width,
				 self->description.height,
				 nPixels*outFmt->pixelSize,
				 outFmt);
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i++){
    int j = i*2;/*byte index for YUV2*/
    unsigned char y,u,v;
    FPrgb2yuv((unsigned char)self->imageData[i],
	      (unsigned char)self->imageData[i],
	      (unsigned char)self->imageData[i],
	      &y,&u,&v);
    out->imageData[j] = (char)y;
    if (i%2 == 0)/*even pixels use U*/
      out->imageData[j+1] = (char)u;
    else
      out->imageData[j+1] = (char)v;
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toYUY2(const Image *const self){
  assert(self!=0);

  if (self->description.fmt->format == FORMAT_YUY2)
    return new_Image(&self->description,self->imageData);
  else if (self->description.fmt->format == FORMAT_RGB_888)
    return Image_toYUY2fromRGB888(self);
  else if (self->description.fmt->format == FORMAT_L_8)
    return Image_toYUY2fromL8(self);
  else
    return 0;
}

Image* Image_toRGB888fromYUY2(const Image *const self){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_RGB_888]);
  int nPixels,i;

  nPixels = self->description.width*self->description.height;
  outDesc = new_ImageDescription(self->description.width,
				 self->description.height,
				 nPixels*outFmt->pixelSize,
				 outFmt);
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i+=2){
    int j = i*3;/*byte index for rgb24*/
    int k = i*2;/*byte index for YUV2*/
    unsigned char r,g,b;
    FPyuv2rgb((unsigned char)self->imageData[k],/*y0*/
	      (unsigned char)self->imageData[k+1],/*u*/
	      (unsigned char)self->imageData[k+3],/*v*/
	      &r,&g,&b);
    out->imageData[j] = (char)r;
    out->imageData[j+1] = (char)g;
    out->imageData[j+2] = (char)b;
    FPyuv2rgb((unsigned char)self->imageData[k+2],/*y1*/
	      (unsigned char)self->imageData[k+1],/*u*/
	      (unsigned char)self->imageData[k+3],/*v*/
	      &r,&g,&b);
    out->imageData[j+3] = (char)r;
    out->imageData[j+4] = (char)g;
    out->imageData[j+5] = (char)b;
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toRGB888fromL8(const Image *const self){
  Image *out = 0;
  ImageDescription *outDesc;
  const Format *outFmt = &(getFormatTable(0)[FORMAT_RGB_888]);
  int nPixels,i;

  nPixels = self->description.width*self->description.height;
  outDesc = new_ImageDescription(self->description.width,
				 self->description.height,
				 nPixels*outFmt->pixelSize,
				 outFmt);
  out = new_Image(outDesc,0);
  for (i=0; i<nPixels; i++){
    int j = i*3;/*byte index for RGB888*/
    out->imageData[j] = out->imageData[j+1] = out->imageData[j+2] = self->imageData[i];
  }
  delete_ImageDescription(outDesc);
  return out;
}

Image* Image_toRGB888(const Image *const self){
  assert(self!=0);

  if (self->description.fmt->format == FORMAT_RGB_888)
    return new_Image(&self->description,self->imageData);
  else if (self->description.fmt->format == FORMAT_YUY2)
    return Image_toRGB888fromYUY2(self);
  else if (self->description.fmt->format == FORMAT_L_8)
    return Image_toRGB888fromL8(self);
  else
    return 0;
}

Image* Image_convert(const Image *const self, const Format *outFmt){
  if (outFmt->format == FORMAT_RGB_888)
    return Image_toRGB888(self);
  else if (outFmt->format == FORMAT_YUY2)
    return Image_toYUY2(self);
  else if (outFmt->format == FORMAT_L_8)
    return Image_toL8(self);
  else
    return 0;
}
   

int Image_snprintf(char *str, size_t size, const Image * const self){
  char descStr[512];
  int i,j;

  ImageDescription_snprintf(descStr,512, &self->description);
  i = snprintf(str,size, "img(%s;data(",descStr);
  j = 0;
  while (i<size && j<self->description.size){
    i += snprintf(str+i,size-i,"%hhx,",(unsigned char)self->imageData[j]);
    j++;
  }

  if (i<size){
    i--;/*remove last ,*/
    i += snprintf(str+i,size-i,"))");
  }
  return i;
}

int Image_fprintf(FILE *stream, const Image * const self){
  char str[1024];

  Image_snprintf(str,1024,self);
  return fprintf(stream,"%s",str);
}

int Image_printf(const Image * const self){
  return Image_fprintf(stdout,self);
}
