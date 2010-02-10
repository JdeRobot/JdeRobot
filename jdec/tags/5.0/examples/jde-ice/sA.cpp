#include <Ice/Ice.h>
#include <schema.h>
#include <iostream>

using namespace std;
using namespace jde;

class sA : public Schema {
public:
  virtual int start(const Ice::Current&);
  virtual int stop(const Ice::Current&);
  virtual int iteration(const Ice::Current&);
  virtual img getData(const Ice::Current&);
};

int sA::start(const Ice::Current&) {
  cout << "Starting sA\n";
  return 0;
}

int sA::stop(const Ice::Current&) {
  cout << "Stoping sA\n";
  return 0;
}

int sA::iteration(const Ice::Current&) {
  cout << "Executing iteration on sA\n";
  return 0;
}

img sA::getData(const Ice::Current&) {
  img v = img::vector(640*480);
  return v;
}

int main(int argc, char* argv[]) {
  int status = 0;
  Ice::CommunicatorPtr ic;

  try {
    ic = Ice::initialize(argc,argv);
    Ice::ObjectAdapterPtr adapter = 
      ic->createObjectAdapterWithEndpoints("SimplesAAdapter",
					   "default -h 193.147.184.196 -p 10000");
    Ice::ObjectPtr object = new sA;
    adapter->add(object,
		 ic->stringToIdentity("SimplesA"));
    adapter->activate();
    ic->waitForShutdown();
  } catch (const Ice::Exception& e) {
    cerr << e << endl;
    status = 1;
  } catch (const char* msg) {
    cerr << msg << endl;
    status = 1;
  }
  if (ic) {
    try {
      ic->destroy();
    } catch (const Ice::Exception& e) {
      cerr << e << endl;
      status = 1;
    }
  }
  return status;
}
