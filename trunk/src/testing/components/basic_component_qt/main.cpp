//Qt
#include <QtGui>

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "gui/threadgui.h"
#include "gui/gui.h"
#include "sensors/sensors.h"
#include "sensors/threadsensors.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Ice::CommunicatorPtr ic;
    try{
        ic = Ice::initialize(argc, argv);

        Sensors* sensors = new Sensors(ic);
        threadGUI* gui = new threadGUI(sensors);
        ThreadSensors* threadSensors = new ThreadSensors(sensors);

        gui->start();
        threadSensors->start();

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }
    return a.exec();

}
