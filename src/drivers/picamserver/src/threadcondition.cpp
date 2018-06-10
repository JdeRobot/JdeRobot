#include "threadcondition.h"

namespace piCam {

        ThreadCondition::ThreadCondition() throw ( piCam::Exception ) {
            ready=false;
        }


        void ThreadCondition::Wait(std::unique_lock<std::mutex>& lck) throw ( piCam::Exception ) {
            ready=false;
             while ( !ready ) cv.wait ( lck );
        }


        void ThreadCondition::BroadCast() throw ( piCam::Exception ) {
            ready = true;
            cv.notify_all();

        }
  

}
