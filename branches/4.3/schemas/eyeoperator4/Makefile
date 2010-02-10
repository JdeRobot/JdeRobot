JDEDIR = /usr/local
INC-DIR = -I../../base -I. -I../opflow -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I/opt/intel/ipp/5.2/ia32/include -I../../drivers/graphics_xforms
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -L/opt/intel/ipp/5.2/ia32/lib -L/opt/intel/ipp/5.2/ia32/sharedlib
IPP_LIBS = -lippaca6 -lippacpx -lippac -lippact7 -lippacv8 -lippacw7 -lippcca6 -lippccpx -lippcc -lippcct7 -lippccv8 -lippccw7 -lippcha6 -lippchpx -lippch -lippcht7 -lippchv8 -lippchw7 -lippcore -lippcva6 -lippcvpx -lippcv -lippcvt7 -lippcvv8 -lippcvw7 -lippdca6 -lippdcpx -lippdc -lippdct7 -lippdcv8 -lippdcw7 -lippia6 -lippipx -lippi -lippit7 -lippiv8 -lippiw7 -lippja6 -lippjpx -lippj -lippjt7 -lippjv8 -lippjw7 -lippma6 -lippmpx -lippm -lippmt7 -lippmv8 -lippmw7 -lippsa6 -lippsca6 -lippscpx -lippsc -lippsct7 -lippscv8 -lippscw7 -lippspx -lippsra6 -lippsrpx -lippsr -lippsrt7 -lippsrv8 -lippsrw7 -lipps -lippst7 -lippsv8 -lippsw7 -lippvca6 -lippvcpx -lippvc -lippvct7 -lippvcv8 -lippvcw7 -lippvma6 -lippvmpx -lippvm -lippvmt7 -lippvmv8 -lippvmw7
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms $(IPP_LIBS)
GRABAR = -D GRABAR

eyeoperator4: eyeoperator4gui.o eyeoperator4.o
	$(GCC) -shared -Wl,-soname,eyeoperator4.so $(LIB-DIR) -o eyeoperator4.so eyeoperator4.o  eyeoperator4gui.o $(LIBS)

eyeoperator4gui.o: eyeoperator4gui.c eyeoperator4gui.h
	$(GCC) $(CFLAGS) -fPIC -c eyeoperator4gui.c

eyeoperator4gui.c: eyeoperator4gui.fd
	fdesign -convert eyeoperator4gui.fd

eyeoperator4.o: eyeoperator4.c eyeoperator4.h eyeoperator4gui.o
	$(GCC) $(CFLAGS) -fPIC -c eyeoperator4.c

grabar: eyeoperator4gui.o
	$(GCC) $(CFLAGS) $(GRABAR) -fPIC -c eyeoperator4.c
	$(GCC) -shared -Wl,-soname,eyeoperator4.so $(LIB-DIR) -o eyeoperator4.so eyeoperator4.o  eyeoperator4gui.o $(LIBS)
	$(GCC) $(CFLAGS) -fPIC -c eyeoperator4.c
		
clean:
	rm -f *.o eyeoperator4.so eyeoperator4gui.c eyeoperator4gui.h

install:
	cp eyeoperator4.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf eyeoperator4`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf eyeoperator4`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

