JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR-IPP = -I../../services/graphics_gtk -I/opt/intel/ipp/5.3/ia32/include -I/opt/intel/ipp/5.1/ia32/include/ 
LIB-DIR-IPP =  -L/opt/intel/ipp/5.3/ia32/lib -L/opt/intel/ipp/5.3/ia32/sharedlib  -L/opt/intel/ipp/5.2/ia32/lib -L/opt/intel/ipp/5.2/ia32/sharedlib  -L/opt/intel/ipp/5.1/ia32/lib -L/opt/intel/ipp/5.1/ia32/sharedlib
LIB-IPP =-lippaca6 -lippacpx -lippac -lippact7 -lippacv8 -lippacw7 -lippcca6 -lippccpx -lippcc -lippcct7 -lippccv8 -lippccw7 -lippcha6 -lippchpx -lippch -lippcht7 -lippchv8 -lippchw7 -lippcore -lippcva6 -lippcvpx -lippcv -lippcvt7 -lippcvv8 -lippcvw7 -lippdca6 -lippdcpx -lippdc -lippdct7 -lippdcv8 -lippdcw7 -lippia6 -lippipx -lippi -lippit7 -lippiv8 -lippiw7 -lippja6 -lippjpx -lippj -lippjt7 -lippjv8 -lippjw7 -lippma6 -lippmpx -lippm -lippmt7 -lippmv8 -lippmw7 -lippsa6 -lippsca6 -lippscpx -lippsc -lippsct7 -lippscv8 -lippscw7 -lippspx -lippsra6 -lippsrpx -lippsr -lippsrt7 -lippsrv8 -lippsrw7 -lipps -lippst7 -lippsv8 -lippsw7 -lippvca6 -lippvcpx -lippvc -lippvct7 -lippvcv8 -lippvcw7 -lippvma6 -lippvmpx -lippvm -lippvmt7 -lippvmv8 -lippvmw7
INC-DIR = -I../../base -I../../drivers/graphics_gtk -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC-DIRA) $(INC-DIR-IPP)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA) $(LIB-DIR-IPP)
GCC = gcc
CFLAGS = -g -Wall $(INC-DIR)
LIBS = -lm -lX11 $(LIB-IPP)

FILTERS = filter_pass.o filter_opflow.o filter_conv.o filter_canny.o filter_corner.o

ipp_demo: ipp_demo.o $(FILTERS)
	$(GCC) -shared -Wl,-soname,ipp_demo.so $(LIB-DIR) -o ipp_demo.so ipp_demo.o $(FILTERS)  $(LIBS)

ipp_demo.o: ipp_demo.c
	$(GCC) $(CFLAGS) -fPIC -c ipp_demo.c

filter_pass.o: filters.h filter_pass.c
	$(GCC) $(CFLAGS) -fPIC -c filter_pass.c

filter_opflow.o: filters.h filter_opflow.c
	$(GCC) $(CFLAGS) -fPIC -c filter_opflow.c

filter_conv.o: filters.h filter_conv.c
	$(GCC) $(CFLAGS) -fPIC -c filter_conv.c

filter_canny.o: filters.h filter_canny.c
	$(GCC) $(CFLAGS) -fPIC -c filter_canny.c

filter_corner.o: filters.h filter_corner.c
	$(GCC) $(CFLAGS) -fPIC -c filter_corner.c	

clean:
	rm -f *.o ipp_demo.so

release:
	tar -zcvf ipp_demo.tgz *c *h *glade Makefile

