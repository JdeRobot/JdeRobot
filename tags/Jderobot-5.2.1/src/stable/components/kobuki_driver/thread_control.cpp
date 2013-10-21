#include "thread_control.h"

Thread_control::Thread_control(Ice::CommunicatorPtr ic, KobukiManager* kobuki_manager)
{
    this->ic = ic;
    prop = ic->getProperties();

    this->kobuki_manager = kobuki_manager;
    initMotors();
    initEncoders();
}

void Thread_control::initEncoders()
{
    std::string motorsControl_string = "Encoders";
    encoders = new Encoders(kobuki_manager);

    std::string Endpoints = prop->getProperty("kobuki.Encoders.Endpoints");

    Ice::ObjectAdapterPtr adapter =
        ic->createObjectAdapterWithEndpoints(motorsControl_string, Endpoints);

    adapter->add(encoders, ic->stringToIdentity(std::string(motorsControl_string)));
    adapter->activate();
}


void Thread_control::initMotors()
{
    std::string motorsControl_string = "Motors";
    motors = new Motors(kobuki_manager);

    std::string Endpoints = prop->getProperty("kobuki.Motors.Endpoints");

    Ice::ObjectAdapterPtr adapter =
        ic->createObjectAdapterWithEndpoints(motorsControl_string, Endpoints);

    adapter->add(motors, ic->stringToIdentity(std::string(motorsControl_string)));
    adapter->activate();
}

void Thread_control::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        this->kobuki_manager->update();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_control)
            diff = cycle_control;
        else
            diff = cycle_control - diff;


        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}
