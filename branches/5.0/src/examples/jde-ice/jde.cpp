#include <Ice/Ice.h>
#include <schema.h>
#include <iostream>

using namespace std;
using namespace jde;

int main(int argc, char* argv[])
{
  int status = 0;
  struct timeval tlast,tnow;
  long diff;
  int i;
  img v;
  Ice::CommunicatorPtr ic;


  try {
    ic = Ice::initialize(argc, argv);
    Ice::ObjectPrx base = ic->stringToProxy("SimplesA:default -h 193.147.184.196 -p 10000");
    SchemaPrx schema = SchemaPrx::checkedCast(base);
    if (!schema)
      throw "Invalid proxy";

    /* frame rate computing */   
    for(;;) {
      /* printf("cronos iteration\n"); */
      gettimeofday(&tnow,NULL); 
      diff = (tnow.tv_sec-tlast.tv_sec)*1000000+tnow.tv_usec-tlast.tv_usec;
      v = schema->getData();
      tlast=tnow;
      cout << "FPS: "<< (float)1000000./(float)diff << endl;
      //usleep(10000);
    }
  } catch (const Ice::Exception& ex) {
    cerr << ex << endl;
    status = 1;
  } catch (const char* msg) {
    cerr << msg << endl;
    status = 1;
  }
  if (ic)
    ic->destroy();
  return status;
}
