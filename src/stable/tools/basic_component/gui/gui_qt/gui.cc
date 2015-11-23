#include "gui.h"

QApplication *app = 0;
int argc; 
char** argv;


extern "C" IGui* create_gui(int a, char** v)
{
   argc = a;
   argv = v;
   app = new QApplication(argc, argv);
   return new Gui();
}

extern "C" void destroy_gui( IGui* object )
{
  delete object;
}

//default constructor
Gui::Gui() { }

//default destructor
Gui::~Gui() { }

int Gui::runGui(basic_component::Shared* sm)
{
    this->sm = sm;
    pthread_t t_gui;

    setWindowTitle("Basic Component Qt");
    //Creation and setup of Gui elements
    QGridLayout* mainLayout = new QGridLayout();
    labelImage = new QLabel();
    mainLayout->addWidget(labelImage, 0, 0);

    setLayout(mainLayout);
    setVisible(true);


    //We connect a signat to a slot to be able to do something from an incoming signal.
    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    int ret = app->exec();

    this->sm->setClosed(true);
    return ret;
}



void Gui::update()
{
   //emit the signal to update the gui
    emit signal_updateGUI(); 
}

void Gui::on_updateGUI_recieved()
{
    
    //Create and displays the image taken from the shared memory.
    cv::Mat frame = this->sm->getImage();
    QImage imageQt = QImage((const unsigned char*)(frame.data),
                            frame.cols,
                            frame.rows,
                            frame.step,
                            QImage::Format_RGB888);
  
    labelImage->setPixmap(QPixmap::fromImage(imageQt));
}



