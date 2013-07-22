#Editables
NAOPATH=$AL_DIR
JDEROBOTDIR="$HOME/jde-5.0/"
DESTINO=$JDEROBOTDIR/"bin"
PYPATH="/usr/include/python2.6"
JDEINCLUDE=$JDEROBOTDIR/include/jderobot
JDELIB=$JDEROBOTDIR/lib/jderobot/
#Generales
GCCRUTA="/usr/lib/gcc/$(g++ -dumpmachine)/$(g++ -dumpversion)"
#FILES="adapter.h adapter.cpp naobody.h naobody.cpp"
FILES="naobody.cpp adapters/*.h adapters/*.cpp"
NAME="naobody"

# LINKADO

g++ $FILES -c -I /home/frivas/svn/jderobot/jderobot/branches/5.0/build//include/jderobot -I/usr/include/gearbox/ `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` -pthread -I/usr/include/X11 -DWITH_NOIDREF -I$JDEINCLUDE -I$NAOPATH -I$NAOPATH/include -I$NAOPATH/include/alproxies -I$NAOPATH/include/alcommon -I$NAOPATH/include/alcore -I$NAOPATH/include/libthread -I$NAOPATH/include/alvalue -I$NAOPATH/include/altools -I$NAOPATH/include/alfactory -I$NAOPATH/include/alvision

### ENLAZADO

libtool --mode=link g++ -g -O -shared -nostdlib -o naobody *.o $JDEROBOTDIR/lib/jderobot/libJderobotIce.la $JDEROBOTDIR/lib/jderobot/libJderobotUtil.la $JDEROBOTDIR/lib/jderobot/libcolorspacesmm.la $JDEROBOTDIR/lib/jderobot/libJderobotInterfaces.la $JDEROBOTDIR/lib/jderobot/libprogeo.la $GCCRUTA/../../../../lib/crt1.o $GCCRUTA/crtbeginS.o -L/usr/local/lib/../lib -L$GCCRUTA -L$GCCRUTA/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/local/lib -L. -L$GCCRUTA/../../.. -lstdc++ -lm -lc -lgcc_s $GCCRUTA/crtendS.o $GCCRUTA/../../../../lib/crtn.o  -Wl,-soname -Wl,$NAME -o $NAME -rdynamic -ldl -lutil -lpthread -L$JDELIB -lgsl -lgslcblas -L$NAOPATH/lib -L$NAOPATH/lib/python2.6 $NAOPATH/lib/libalvalue.so $NAOPATH/lib/libalcommon.so `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --libs opencv` -lgsl -lgslcblas


### INSTALACION

#cp $NAME $DESTINO/$NAME











#g++ -g -O -o naobody naobody.o -pthread  /home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot/libJderobotIce.so /home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot/libJderobotUtil.so /home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot/libcolorspacesmm.so /home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot/libJderobotInterfaces.so /home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot/libprogeo.so /usr/lib/libglademm-2.4.so /usr/lib/libgtkmm-2.4.so /usr/lib/libglade-2.0.so /usr/lib/libatkmm-1.6.so /usr/lib/libgdkmm-2.4.so /usr/lib/libgiomm-2.4.so /usr/lib/libpangomm-1.4.so /usr/lib/libglibmm-2.4.so /usr/lib/libcairomm-1.0.so /usr/lib/libsigc-2.0.so /usr/lib/libgtk-x11-2.0.so /usr/lib/libxml2.so /usr/lib/libgdk-x11-2.0.so /usr/lib/libatk-1.0.so /usr/lib/libgio-2.0.so /usr/lib/libpangoft2-1.0.so /usr/lib/libgdk_pixbuf-2.0.so -lm /usr/lib/libpangocairo-1.0.so /usr/lib/libcairo.so /usr/lib/libpango-1.0.so /usr/lib/libfreetype.so -lfontconfig /usr/lib/libgobject-2.0.so /usr/lib/libgmodule-2.0.so /usr/lib/libgthread-2.0.so -lrt /usr/lib/libglib-2.0.so -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy /usr/local/lib/libgsl.so /usr/local/lib/libgslcblas.so -pthread -Wl,-rpath -Wl,/home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot -Wl,-rpath -Wl,/home/frivas/svn/jderobot/jderobot/branches/5.0/build/lib/jderobot
