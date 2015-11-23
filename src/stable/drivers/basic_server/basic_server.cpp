#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "myInterface.h"
#include <signal.h>




class myClassI: virtual public jderobot::myInterface {
	public:
	myClassI()
	{

		//...............
		replyTask = new ReplyTask();
		this->control=replyTask->start();//my own thread
	}

	~myClassI(){
		this->replyTask->destroy();
		this->control.join();
	}

	virtual void test(const Ice::Current&){

	}


	
	private:
		class ReplyTask: public IceUtil::Thread{
		public:
			ReplyTask():_done(false){

		  }

		virtual void run(){
			while(!(_done)){
				//thread code
				std::cout << "running" << std::endl;
				usleep(100000);
			}
		}

		virtual void destroy(){
			this->_done=true;
		}
		private:
		bool _done;
		};

	typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
	ReplyTaskPtr replyTask;
    IceUtil::ThreadControl control;
	

};


Ice::CommunicatorPtr ic;
bool killed;
myClassI* interface1;


void exitApplication(int s){

	killed=true;

	if (interface1 != NULL)
		delete interface1;

	ic->shutdown();
	exit(0);
}




int main(int argc, char** argv){


	killed=false;
	struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = exitApplication;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);

	Ice::PropertiesPtr prop;
	std::string componentPrefix("myComponent");

	
	

	try{
			ic = Ice::initialize(argc,argv);
			prop = ic->getProperties();
	}
	catch (const Ice::Exception& ex) {
			std::cerr << ex << std::endl;
			return 1;
	}
	catch (const char* msg) {
			std::cerr <<"Error :" << msg << std::endl;
			return 1;
	}

	std::string Endpoints = prop->getProperty(componentPrefix + ".Endpoints");
	Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints(componentPrefix, Endpoints);

	// for each interface:

	std::string objPrefix="myInterface";
	std::string Name = "pointcloud1";
	std::cout << "Creating pointcloud1 " << Name << std::endl;
	interface1 = new myClassI();
	adapter->add(interface1, ic->stringToIdentity(Name));

	//starting the adapter
	adapter->activate();
	ic->waitForShutdown();

	if (!killed)
		exitApplication(1);
   
   return 0;

}

	

