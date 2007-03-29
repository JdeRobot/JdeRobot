DRIVERS = firewire  imagefile  pantilt  player video4linux networkclient networkserver
SCHEMAS = hsituner  introrob  myschema  opengldemo  papito  myperceptive followball
DOCS = manual 

all: 
	CURDIR=`pwd`;
	cd core && make
	cd $(CURDIR)

	CURDIR=`pwd`;
	for i in $(DRIVERS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd drivers/$$i && make || exit ; pwd;\
	done;

	CURDIR=`pwd`;
	for i in $(SCHEMAS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd schemas/$$i && make || exit ; pwd;\
	done;

	CURDIR=`pwd`;		
	for i in $(DOCS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd docs/$$i && make || exit ; pwd;\
	done;

clean:
	CURDIR=`pwd`;
	cd core && make clean
	for i in $(DRIVERS); do (echo "Cleaning " $$i); \
	cd $(CURDIR) ;\
	cd drivers/$$i && make clean || exit ; pwd;\
	done;	
	for i in $(SCHEMAS); do (echo "Cleaning " $$i); \
	cd $(CURDIR) ;\
	cd schemas/$$i && make clean || exit ; pwd;\
	done;	
	for i in $(DOCS); do (echo "Cleaning " $$i); \
	cd $(CURDIR) ;\
	cd docs/$$i && make clean || exit ; pwd;\
	done;


