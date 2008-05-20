JDEDIR = /usr/local
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I/usr/X11R6/include/X11 -I/opt/intel/ipp/5.2/ia32/include -I/opt/intel/ipp/5.1/ia32/include/ -I../../drivers/graphics_xforms
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -L/opt/intel/ipp/5.2/ia32/lib -L/opt/intel/ipp/5.2/ia32/sharedlib -L/opt/intel/ipp/5.1/ia32/sharedlib
IPP_LIBS = -lippaca6 -lippacpx -lippac -lippact7 -lippacv8 -lippacw7 -lippcca6 -lippccpx -lippcc -lippcct7 -lippccv8 -lippccw7 -lippcha6 -lippchpx -lippch -lippcht7 -lippchv8 -lippchw7 -lippcore -lippcva6 -lippcvpx -lippcv -lippcvt7 -lippcvv8 -lippcvw7 -lippdca6 -lippdcpx -lippdc -lippdct7 -lippdcv8 -lippdcw7 -lippia6 -lippipx -lippi -lippit7 -lippiv8 -lippiw7 -lippja6 -lippjpx -lippj -lippjt7 -lippjv8 -lippjw7 -lippma6 -lippmpx -lippm -lippmt7 -lippmv8 -lippmw7 -lippsa6 -lippsca6 -lippscpx -lippsc -lippsct7 -lippscv8 -lippscw7 -lippspx -lippsra6 -lippsrpx -lippsr -lippsrt7 -lippsrv8 -lippsrw7 -lipps -lippst7 -lippsv8 -lippsw7 -lippvca6 -lippvcpx -lippvc -lippvct7 -lippvcv8 -lippvcw7 -lippvma6 -lippvmpx -lippvm -lippvmt7 -lippvmv8 -lippvmw7
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms $(IPP_LIBS)

opflow: opflowgui.o opflow.o
	$(GCC) -shared -Wl,-soname,opflow.so $(LIB-DIR) -o opflow.so opflow.o opflowgui.o $(LIBS)

opflowgui.o: opflowgui.c opflowgui.h
	$(GCC) $(CFLAGS) -fPIC -c opflowgui.c

opflowgui.c: opflowgui.fd
	fdesign -convert opflowgui.fd

opflow.o: opflow.c opflow.h
	$(GCC) $(CFLAGS) -fPIC -c opflow.c

grabar: opflowgui.o
	$(GCC) $(CFLAGS) -D GRABAR -fPIC -c opflow.c
	$(GCC) -shared -Wl,-soname,opflow.so $(LIB-DIR) -o opflow.so opflow.o opflowgui.o $(LIBS)
	$(GCC) $(CFLAGS) -fPIC -c opflow.c

clean:
	rm -f *.o opflow.so opflowgui.c opflowgui.h

install:
	cp myschema.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf opflow`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile
