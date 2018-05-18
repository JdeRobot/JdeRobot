#include "gui.h"

GUI::GUI(Robot* robot, Config::Properties props)
{

    this->robot = robot;

    QGridLayout* mainLayout = new QGridLayout();
    QGridLayout* layoutControl = new QGridLayout();
    QVBoxLayout* layoutButtons = new QVBoxLayout();


    camerasWidget = new CamerasWidget(robot);

    buttonStopRobot = new QPushButton("Stop Robot");
    checkLaser = new QCheckBox("Laser");
    checkCameras = new QCheckBox("Cameras");

	InfoCurrentV = new QLabel("Current v (m/s):");
	InfoCurrentW = new QLabel("Current w (rad/s):");
	currentV = new QLabel("0");
	currentW = new QLabel("0");

    canvasVW = new controlVW();
    canvasVW->setProps(props);
    laserWidget =new LaserWidget();


    //int indiceFilaGui = 0;
    layoutControl->addWidget(canvasVW, 0, 0);

    layoutButtons->addWidget(InfoCurrentV, 0);
    layoutButtons->addWidget(currentV, 1);
    layoutButtons->addWidget(InfoCurrentW, 2);
    layoutButtons->addWidget(currentW, 3);

	QSpacerItem *item = new QSpacerItem(0,200, QSizePolicy::Expanding, QSizePolicy::Fixed);
	layoutButtons->addItem(item);

    layoutButtons->addWidget(buttonStopRobot, 2);
    layoutButtons->addWidget(checkCameras, 3);
    layoutButtons->addWidget(checkLaser, 4);



    mainLayout->addLayout(layoutControl, 0, 0);
    mainLayout->addLayout(layoutButtons, 0, 1);

    setLayout(mainLayout);

    setVisible(true);

    adjustSize();

    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    connect(buttonStopRobot, SIGNAL(clicked()),this, SLOT(on_buttonStopRobot_clicked()) );

    connect(canvasVW, SIGNAL(VW_changed(float,float)), this, SLOT(on_update_canvas_recieved(float, float)));

    connect(checkLaser, SIGNAL(stateChanged(int)), this, SLOT(on_checks_changed()));
    connect(checkCameras, SIGNAL(stateChanged(int)), this, SLOT(on_checks_changed()));

    show();

}

void GUI::on_checks_changed()
{
    camerasWidget->setVisible(checkCameras->isChecked());
    laserWidget->setVisible(checkLaser->isChecked());
}

void GUI::on_update_canvas_recieved(float v, float w)
{

    this->robot->getActuators()->setMotorV((float)v);
    this->robot->getActuators()->setMotorW((float)w);
}

void GUI::on_buttonStopRobot_clicked()
{
    canvasVW->Stop();
}

void GUI::updateThreadGUI()
{
    emit signal_updateGUI();
}

void GUI::on_updateGUI_recieved()
{
    camerasWidget->update();
    laserWidget->update(this->robot->getSensors()->getLaserData());
	currentV->setText( QString::number(canvasVW->getV()));
	currentW->setText( QString::number(canvasVW->getW()));
}

