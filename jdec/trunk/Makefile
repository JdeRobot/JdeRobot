DRIVERS = firewire imagefile player networkclient networkserver mplayer graphics_gtk graphics_xforms pantilt
SCHEMAS = eyeoperator  followball  hsvtuner  introrob  opengldemo  opflow  recorder  teleoperator
DOCS = manual progeo-manual howto-ipp howto-gazebo 
EXAMPLES = fuzzylib myschema papito 
UTILS = fuzzylib progeo colorspaces

all: 
	CURDIR=`pwd`;
	cd base && make
	cd $(CURDIR)

	CURDIR=`pwd`;
	cd docs && doxygen jdec.doxygen;
	cd $(CURDIR)

	CURDIR=`pwd`;	
	for i in $(UTILS); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd utils/$$i && make || exit ; pwd;\
	done;

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

	CURDIR=`pwd`;			
	for i in $(EXAMPLES); do (echo "Building " $$i); \
	cd $(CURDIR) ;\
	cd examples/$$i && make || exit ; pwd;\
	done;	

clean:
	CURDIR=`pwd`;
	cd base && make clean
	cd $(CURDIR)/docs && rm -rf html
	for i in $(UTILS); do (echo "Cleaning " $$i); \
	cd $(CURDIR) ;\
	cd utils/$$i && make clean || exit ; pwd;\
	done;
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
	for i in $(EXAMPLES); do (echo "Cleaning " $$i); \
	cd $(CURDIR) ;\
	cd examples/$$i && make clean || exit ; pwd;\
	done;


