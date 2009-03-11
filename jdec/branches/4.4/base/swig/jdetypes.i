%module jdetypes

%include cpointer.i
%include carrays.i

%pointer_class(char,charp)
%pointer_class(int,intp)
%pointer_class(float,floatp)
%pointer_class(double,doublep)

%array_class(char,charArray)
%array_class(int,intArray)
%array_class(float,floatArray)
%array_class(double,doubleArray)

%pointer_cast(void*,char*,voidp_2_charp)
%pointer_cast(void*,int*,voidp_2_intp)
%pointer_cast(void*,float*,voidp_2_floatp)
%pointer_cast(void*,double*,voidp_2_doublep)