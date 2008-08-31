JDEDIR = /usr/local
INC_DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB_DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC_DIR = -I../../base -I../../drivers/graphics_gtk -I../../services/graphics_gtk -I/opt/intel/ipp/5.1/ia32/include/ -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC_DIRA)
LIB_DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -L/opt/intel/ipp/5.3.3.075/ia32/lib -L/opt/intel/ipp/5.3.3.075/ia32/sharedlib -L/opt/intel/ipp/5.1/ia32/lib -L/opt/intel/ipp/5.1/ia32/sharedlib/ $(LIB_DIRA)
IPP_LIBS = -lippacpx -lippac -lippact7 -lippacv8 -lippacw7 -lippcca6 -lippccpx -lippcc -lippcct7 -lippccv8 -lippccw7 -lippcha6 -lippchpx -lippch -lippcht7 -lippchv8 -lippchw7 -lippcore -lippcva6 -lippcvpx -lippcv -lippcvt7 -lippcvv8 -lippcvw7 -lippdca6 -lippdcpx -lippdc -lippdct7 -lippdcv8 -lippdcw7 -lippia6 -lippipx -lippi -lippit7 -lippiv8 -lippiw7 -lippja6 -lippjpx -lippj -lippjt7 -lippjv8 -lippjw7 -lippma6 -lippmpx -lippm -lippmt7 -lippmv8 -lippmw7 -lippsa6 -lippsca6 -lippscpx -lippsc -lippsct7 -lippscv8 -lippscw7 -lippspx -lippsra6 -lippsrpx -lippsr -lippsrt7 -lippsrv8 -lippsrw7 -lipps -lippst7 -lippsv8 -lippsw7 -lippvca6 -lippvcpx -lippvc -lippvct7 -lippvcv8 -lippvcw7 -lippvma6 -lippvmpx -lippvm -lippvmt7 -lippvmv8 -lippvmw7
GCC = gcc
CFLAGS = -g -pedantic -Wall -I/opt/intel/ipp/5.3.3.075/ia32/include $(INC_DIR)
LIBS = -lm -lX11 $(IPP_LIBS)

prueba_ipp: prueba_ipp.o
	$(GCC) -shared -Wl,-soname,prueba_ipp.so $(LIB_DIR) -o prueba_ipp.so prueba_ipp.o $(LIBS)

prueba_ipp.o: prueba_ipp.c
	$(GCC) $(CFLAGS) -fPIC -c prueba_ipp.c

clean:
	rm -f *.o prueba_ipp.so

release:
	tar -zcvf prueba_ipp.tgz *c *h *glade Makefile
