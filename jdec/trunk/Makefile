DRIVERS = firewire imagefile  video4linux

all:
	CURDIR=`pwd`;
	cd core && make
	for i in $(DRIVERS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd drivers/$$i && make || exit ; pwd;\
	done;	

clean:
	CURDIR=`pwd`;
	cd core && make clean
	for i in $(DRIVERS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd drivers/$$i && make clean || exit ; pwd;\
	done;	


