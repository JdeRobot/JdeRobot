#ifndef ENCODERS_H
#define ENCODERS_H

//boost
#include <boost/signals2/mutex.hpp>

//Encoders
#include <jderobot/encoders.h>

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "../kobukimanager.h"

class Encoders: virtual public jderobot::Encoders
{
public:
    Encoders(KobukiManager* kobuki);

    virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&);

    virtual void setEncodersData(const jderobot::EncodersDataPtr&  encodersData,
                                 const Ice::Current&);

    private: boost::signals2::mutex mutex; ///< Mutex for thread-safe access to internal data.

    private: ;

    private: KobukiManager* kobuki;

};

#endif // ENCODERS_H
