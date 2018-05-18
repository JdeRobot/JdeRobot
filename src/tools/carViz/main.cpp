#include "robot/robot.h"
#include "gui/threadupdategui.h"

#include "jderobot/config/config.h"
#include "jderobot/comm/communicator.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    try {

        Config::Properties props = Config::load(argc, argv);



         //-----------------Comm----------------//
         Comm::Communicator* jdrc = new Comm::Communicator(props);

         // Variables Compartidas
         // Robot -> Sensores, navegacion, actuadores
         Robot *robot = new Robot(jdrc);


         ThreadUpdateGUI* thread_update_gui = new ThreadUpdateGUI(robot, props);
         thread_update_gui->start();

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }

    app.exec();
}
