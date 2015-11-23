#ifndef IGUI_H
#define IGUI_H

#include "../shared.h"

class IGui
{
public:

     //virtual ~Gui() {};
     virtual int runGui(basic_component::Shared* sm) = 0; 
     virtual void update() = 0;

};

extern "C" IGui* create_gui(int, char**);
extern "C" void destroy_gui(IGui*);

#endif // GUI_H
