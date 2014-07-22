#include "gui.h"
namespace basic_component_qt {
Gui::Gui(Shared* sm)
{

    this->sm = sm;

    //Creation and setup of Gui elements
    QGridLayout* mainLayout = new QGridLayout();
    labelImage = new QLabel();
    mainLayout->addWidget(labelImage, 0, 0);
    setLayout(mainLayout);
    setVisible(true);

    //We connect a signat to a slot to be able to do something from an incoming signal.
    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    show();

}


void Gui::updateThreadGUI()
{
    //emit the signal to update the gui
    emit signal_updateGUI();
}

void Gui::on_updateGUI_recieved()
{
    //Create and displays the image taken from the shared memory.

    //cv::Mat
    cv::Mat frame = this->sm->getImage();
    QImage imageQt = QImage((const unsigned char*)(frame.data),
                            frame.cols,
                            frame.rows,
                            frame.step,
                            QImage::Format_RGB888);
/*  
    //IplImage
    IplImage* frame = this->sm->getImage();
    QImage imageQt = QImage((const unsigned char*)(frame->imageData),
                            frame->width,
                            frame->height,
                            QImage::Format_RGB888);
*/
  
    labelImage->setPixmap(QPixmap::fromImage(imageQt));
}

}
