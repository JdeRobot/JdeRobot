#ifndef _PICAM_THREADTHREADCONDITION_H
#define _PICAM_THREADTHREADCONDITION_H
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "exceptions.h"
namespace piCam {

        class ThreadCondition
        {
            public:
            ThreadCondition() throw ( piCam::Exception );


            void Wait(std::unique_lock<std::mutex>& lck) throw ( piCam::Exception );


            void BroadCast() throw ( piCam::Exception );

            private:
            std::mutex mtx;
            std::condition_variable cv;
            bool ready  ;

        };

}		
#endif
