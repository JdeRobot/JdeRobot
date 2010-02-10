#include <colorspaces/colorspaces.h>
#include <colorspaces/colorspaces++.h>
#include <iostream>
#include <tr1/memory>

using namespace colorspaces;

int main(int argc, char **argv){
  /*formats.h & formats++.h*/
  {
    const Format *fmtT;
    const char *fmt_name;
    int k,nFmt;
    
    fmtT = getFormatTable(&nFmt);
    
    for(k=0; k<nFmt; k++){
      Format_printf(&fmtT[k]);
      printf(" |C\n");
      std::cout << fmtT[k] << " |C++\n";
      fmt_name = fmtT[k].format_name;
    }
    
    const Format *f;
    f = searchFormat(fmt_name);
    std::cout << *f << std::endl;
  }

  /*image.h & image++.h*/
  {
    ImageDescription *iDesc;
    
    iDesc = new_ImageDescription(2,2,4,&(getFormatTable(0)[FORMAT_L_8]));
    ImageDescription_printf(iDesc);
    printf(" |C\n");
    delete_ImageDescription(iDesc);
    
    Image *i;
    iDesc = new_ImageDescription(2,2,4,&(getFormatTable(0)[FORMAT_L_8]));
    std::cout << *iDesc << " |C++\n";
    i = new_Image(iDesc,0);
    Image_printf(i);
    printf(" |C\n");
    delete_Image(i);
    
    Image *j;
    i = new_Image(iDesc,0);
    j = new_Image(iDesc,i->imageData);
    std::cout << *i << " |C++\n";
    std::cout << *j << " |C++\n";
    Image_swapDataOwner(i,j);
    delete_Image(i);
    delete_Image(j);
    delete_ImageDescription(iDesc);

    std::tr1::shared_ptr<ImageDescriptionpp> 
      iiDesc(new ImageDescriptionpp(2,2,4,&(getFormatTable(0)[FORMAT_L_8])));
    std::tr1::shared_ptr<Imagepp> ii(new Imagepp(*iiDesc,0));

    ii.reset(new Imagepp(*iiDesc,0));
    std::tr1::shared_ptr<Imagepp> jj(new Imagepp(*iiDesc,ii->imageData));
    Image_swapDataOwner(ii.get(),jj.get());

    i = new_Image(iiDesc.get(),0);
    ii.reset(new Imagepp(*i));/*data copied*/
    ii.reset();
    delete_Image(i);

    i = new_Image(iiDesc.get(),0);
    ii.reset(new Imagepp(i->description,i->imageData));
    Image_swapDataOwner(i,ii.get());/*get data ownership*/
    delete_Image(i);

    /*exercise RGB->YUY2 and RGB->L8 routines*/
    std::cout << "RGB conversion routines\n";
    const Format *rgbFmt = &getFormatTable(0)[FORMAT_RGB_888];
    char dataRGB[] = {255,255,255,0,0,0,128,128,128,100,200,5};/*4 pixel rgb*/
    iiDesc.reset(new ImageDescriptionpp(2,2,rgbFmt->pixelSize*2*2,rgbFmt));
    std::cout << *iiDesc << " |C++\n";

    ii.reset(new Imagepp(*iiDesc,dataRGB));
    std::cout << *ii << " |C++\n";

    j = Image_toL8(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toL8();
    std::cout << *jj << " |C++\n";

    j = Image_toYUY2(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toYUY2();
    std::cout << *jj << " |C++\n";

    j = Image_toRGB888(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toRGB888();
    std::cout << *jj << " |C++\n";
    
    /*exercise YUY2->RGB and YUY2->L8 routines*/
    std::cout << "YUY2 conversion routines\n";
    const Format *yuy2Fmt = &getFormatTable(0)[FORMAT_YUY2];
    char dataYUY2[] = {16,235,100,50};/*2 pixel yuy2*/
    iiDesc.reset(new ImageDescriptionpp(2,1,yuy2Fmt->pixelSize*2*1,yuy2Fmt));
    std::cout << *iiDesc << " |C++\n";

    ii.reset(new Imagepp(*iiDesc,dataYUY2));
    std::cout << *ii << " |C++\n";

    j = Image_toL8(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toL8();
    std::cout << *jj << " |C++\n";

    j = Image_toRGB888(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toRGB888();
    std::cout << *jj << " |C++\n";

    j = Image_toYUY2(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toYUY2();
    std::cout << *jj << " |C++\n";
    
    /*exercise L8->YUY2 and L8->RGB routines*/
    std::cout << "L8 conversion routines\n";
    const Format *l8Fmt = &getFormatTable(0)[FORMAT_L_8];
    char dataL8[] = {5,4};/*2 pixel L8*/
    iiDesc.reset(new ImageDescriptionpp(2,1,l8Fmt->pixelSize*2*1,l8Fmt));
    std::cout << *iiDesc << " |C++\n";

    ii.reset(new Imagepp(*iiDesc,dataL8));
    std::cout << *ii << " |C++\n";

    j = Image_toRGB888(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toRGB888();
    std::cout << *jj << " |C++\n";

    j = Image_toYUY2(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toYUY2();
    std::cout << *jj << " |C++\n";

    j = Image_toL8(ii.get());
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->toL8();
    std::cout << *jj << " |C++\n";


    /*test convert routine*/
    std::cout << "Convert routine\n";

    iiDesc.reset(new ImageDescriptionpp(2,1,l8Fmt->pixelSize*2*1,l8Fmt));
    std::cout << *iiDesc << " |C++\n";

    ii.reset(new Imagepp(*iiDesc,dataL8));
    std::cout << *ii << " |C++\n";

    j = Image_convert(ii.get(),rgbFmt);
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->convert(rgbFmt);
    std::cout << *jj << " |C++\n";

    j = Image_convert(ii.get(),yuy2Fmt);
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->convert(yuy2Fmt);
    std::cout << *jj << " |C++\n";

    j = Image_convert(ii.get(),l8Fmt);
    std::cout << *j << " |C++\n";
    delete_Image(j);

    jj = ii->convert(l8Fmt);
    std::cout << *jj << " |C++\n";


    /*imagetest routines*/
    i = Image_createTestHline(10,10,/*10x10*/
			      1,/*1 pixel width*/
			      0,/*start in row 0*/
			      0,0);/*use default colors*/
    std::cout << *i << " |C++\n";
    delete_Image(i);
        
    i = Image_createTestVline(10,10,
			      1,/*1 pixel width*/
			      0,/*start in col 0*/
			      0,0);/*use default colors*/
    std::cout << *i << " |C++\n";
    delete_Image(i);

    i = Image_createTestSquare(10,10,
			       2,/*2 pixel length*/
			       0,0,/*start in 0,0*/
			       0,0);/*use default colors*/
    std::cout << *i << " |C++\n";
    delete_Image(i);
    
    /*c++ wrapper*/
    ii = colorspaces::Imagepp::createTestHline(10,10,
					       1,/*1 pixel width*/
					       0,/*start in row 0*/
					       0,0);/*use default colors*/
    std::cout << *ii << " |C++\n";
    ii = colorspaces::Imagepp::createTestVline(10,10,
					       1,/*1 pixel width*/
					       0,/*start in row 0*/
					       0,0);/*use default colors*/
    std::cout << *ii << " |C++\n";

    ii = colorspaces::Imagepp::createTestSquare(10,10,
						2,/*2 pixel length*/
						0,0,/*start in 0,0*/
						0,0);/*use default colors*/
    std::cout << *ii << " |C++\n";
  }
}
