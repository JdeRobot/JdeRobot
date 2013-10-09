#include "gui.h"

GUI::GUI(Sensors* sensors)
{

    this->sensors = sensors;

    QGridLayout* mainLayout = new QGridLayout();

    labelImage = new QLabel();

    mainLayout->addWidget(labelImage, 0, 0);

    setLayout(mainLayout);

    setVisible(true);

    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    show();

}

void GUI::updateThreadGUI()
{
    emit signal_updateGUI();
}

void GUI::on_updateGUI_recieved()
{
    cv::Mat frame = this->sensors->getImage();

    QImage imageQt = QImage((const unsigned char*)(frame.data),
                            frame.cols,
                            frame.rows,
                            frame.step,
                            QImage::Format_RGB888);

    labelImage->setPixmap(QPixmap::fromImage(imageQt));
}

