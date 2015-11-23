#include <iostream>
#include <QTimer>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#if QT_VERSION < 0x040000
typedef QColorGroup Palette;
#else
typedef QPalette Palette;
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	connect(this, SIGNAL(signal_updateGUI()), this, SLOT(updateGUI_recieved()));
	ui->setupUi(this);
	ui->battery->setValue(60.0);

	vx=vy=vz=pitch=roll=yaw=0.0;
	linx=liny=linz=0.0;

	compass=createCompass(1);
	compass->setGeometry(QRect(970,50,120,120));

	compass->setReadOnly(true);
	//compass->setValue(200.0);

	altd=createDial(2);

	altd->setGeometry(QRect(970,200,115,135));


	velLinZ = createDial(1);
	velLinZ->setGeometry(QRect(1050,400,150,150));


	velLinY = createDial(1);
	velLinY->setGeometry(QRect(900,400,150,150));

	velLinX = createDial(1);
	velLinX->setGeometry(QRect(750,400,150,150));


	horizon=createDial(0);
	horizon->setGeometry(QRect(750,80,200,200));

	ui->tabWidget->setCurrentIndex(0);

    this->initButtons();
}

void MainWindow::initButtons(){
    QPixmap rotateLeft(":images/rotate_left.png");
    QIcon rotateLeftIcon(rotateLeft);
    this->ui->angZL->setIcon(rotateLeftIcon);

    QPixmap rotateRight(":images/rotate_right.png");
    QIcon rotateRightIcon(rotateRight);
    this->ui->angZR->setIcon(rotateRightIcon);

    QPixmap up(":images/up.png");
    QIcon upIcon(up);
    this->ui->linZU->setIcon(upIcon);

    QPixmap down(":images/down.png");
    QIcon downIcon(down);
    this->ui->linZD->setIcon(downIcon);

    QPixmap toggle(":images/toggle_cam.png");
    QIcon toggleIcon(toggle);
    this->ui->toggleCam->setIcon(toggleIcon);

    QPixmap forward(":images/forward.png");
    QIcon forwardIcon(forward);
    this->ui->linXF->setIcon(forwardIcon);

    QPixmap backward(":images/backward.png");
    QIcon backwardIcon(backward);
    this->ui->linXB->setIcon(backwardIcon);

    QPixmap left(":images/left.png");
    QIcon leftIcon(left);
    this->ui->linYL->setIcon(leftIcon);

    QPixmap right(":images/right.png");
    QIcon rightIcon(right);
    this->ui->linYR->setIcon(rightIcon);
}


MainWindow::~MainWindow()
{
	delete sensors;
	delete compass;
	delete altd;
	delete velLinY;
	delete velLinZ;
	delete velLinX;
	delete horizon;
	delete d_ai;
	delete d_speedo;
	delete ui;
}

void MainWindow::updateThreadGUI()
{
	emit signal_updateGUI();
}

void MainWindow::updateGUI_recieved()
{
	cv::Mat image = this->sensors->getImage();

	QImage imageQt = QImage((const unsigned char*)(image.data),
		    image.cols,
		    image.rows,
		    image.step,
		    QImage::Format_RGB888);
		    
		    
	int camera=0;
    	//640x360 ArDrone2
    	//320x240 ArDrone1 frontal
    	//174x144 ArDrone 1 ventral
    int x=0;
    int y=0;
    if(image.cols==IMAGE_COLS_MAX){
        x=20;
    }else{
        x=(IMAGE_COLS_MAX+20)/2-(image.cols/2);
    }
    if(image.rows==IMAGE_ROWS_MAX){
        y=40;
    }else{
        y=(IMAGE_ROWS_MAX+40)/2-(image.rows/2);
    }


	QSize size(image.cols,image.rows);
	ui->imageLLabel->move(x,y);
	ui->imageLLabel->resize(size);

	ui->imageLLabel->setHidden(false);
	ui->imageLLabel->setPixmap(QPixmap::fromImage(imageQt));

	jderobot::NavdataDataPtr navdata=this->sensors->getNavdata();
	//this->printNavdata(navdata);
	jderobot::Pose3DDataPtr pose3DDataPtr = this->sensors->getPose3DData();	
	int batteryPer=navdata->batteryPercent;
	ui->battery->setValue(batteryPer);
	float qw = pose3DDataPtr->q0;
	float qx = pose3DDataPtr->q1;
	float qy = pose3DDataPtr->q2;
	float qz = pose3DDataPtr->q3;
	float yawd=this->rad2deg(quatToYaw(qw, qx, qy, qz));
	float pitchd=this->rad2deg(quatToPitch(qw, qx, qy, qz));
	float rolld=this->rad2deg(quatToRoll(qw, qx, qy, qz));	    
	drawYawValues(yawd);
	drawPitchRollValues(-pitchd,-rolld);
		        	    	
    double altd=pose3DDataPtr->z;
	drawAltd(altd);

	drawVelocitiesValues(navdata->vx,navdata->vy,navdata->vz);

	this->setDroneStatus(navdata->state);

}

void MainWindow::printNavdata(jderobot::NavdataDataPtr data)
{
	std::cout << "-----------------Navdata---------------" << std::endl;
	std::cout << "Vehicle " << data->vehicle << std::endl;
	std::cout << "State:\t"<< data->state << "\tTimestamp:\t"<< (data->tm/1000000) << "(s)"<< std::endl;		
	std::cout << "Battery:\t"<< data->batteryPercent << "\tAltitude:\t"<< data->altd << std::endl;

	std::cout << "RotX:\t"<< data->rotX <<std::endl;
	std::cout << "RotY:\t"<< data->rotY << std::endl;
	std::cout << "RotZ:\t"<< data->rotZ << "\n" << std::endl;		

	std::cout << "Vx:\t"<< data->vx <<std::endl;	
	std::cout << "Vy:\t"<< data->vy << std::endl;
	std::cout << "Vz:\t"<< data->vz << "\n" << std::endl;	
	std::cout << "Ax:\t"<< data->ax <<std::endl;	
	std::cout << "Ay:\t"<< data->ay << std::endl;
	std::cout << "Az:\t"<< data->az << "\n" << std::endl;	
	std::cout << "MagX:\t"<< data->magX << std::endl;	
	std::cout << "MagY:\t"<< data->magY << std::endl;
	std::cout << "MagZ:\t"<< data->magZ << "\n" << std::endl;
	std::cout << "Pressure:\t"<< data->pressure << "\tTemp:\t"<< data->temp << "\n" << std::endl;	

	std::cout << "WindSpeed:\t"<< data->windSpeed << std::endl;	
	std::cout << "WindAngle:\t"<< data->windAngle << std::endl;
	std::cout << "WindCompAngle:\t"<< data->windCompAngle << "\n" << std::endl;
	std::cout << "\t--Tag Detection--" << std::endl;
	std::cout << "\tTagCount: "<< data->tagsCount << std::endl;
	if(data->tagsCount!=0){
		for(int i=0;i<data->tagsType.size();i++)
		{
			std::cout << "\t"<<data->tagsType[i] << std::endl;
		}
		for(int i=0;i<data->tagsXc.size();i++)
		{
			std::cout << "\t"<<data->tagsXc[i] << std::endl;
		}
		for(int i=0;i<data->tagsYc.size();i++)
		{
			std::cout << "\t"<<data->tagsYc[i] << std::endl;
		}	
		for(int i=0;i<data->tagsWidth.size();i++)
		{
			std::cout << "\t"<<data->tagsWidth[i] << std::endl;
		}	
		for(int i=0;i<data->tagsHeight.size();i++)
		{
			std::cout << "\t"<<data->tagsHeight[i] << std::endl;
		}
		for(int i=0;i<data->tagsOrientation.size();i++)
		{
			std::cout << "\t"<<data->tagsOrientation[i] << std::endl;
		}
		for(int i=0;i<data->tagsDistance.size();i++)
		{
			std::cout << "\t"<<data->tagsDistance[i] << std::endl;
		}				
	}
	std::cout << "---------------------------------------" << std::endl;
}

void MainWindow::sendVelocitiesToUAV()
{
	this->sensors->sendVelocitiesToUAV(linx,liny,linz,roll,pitch,yaw);
}

void MainWindow::drawYawValues(float degress)
{
	//Rot Z
	//-180,180
	//- right,+ left
	//Compass 360º
	QString value=QString::number(degress,'f',2);
	ui->yawValue->setText(QString(value));
	compass->setValue(degress);
}

void MainWindow::drawAltd(float meters)
{
	int cent=0;
	float result=0.0;
	if(meters>=10 && meters<100){
		cent=(int)meters/10;
		result=meters-(cent*10);
		altd->setValue(result);
	}else if((meters>=100 && meters<1000)){
		cent=(int)meters/100;
		result=meters-(cent*100);
		altd->setValue(result);
	}else{
		altd->setValue(meters);
	}


	QString value=QString::number(meters,'f',2);
	ui->altdValue->setText(value);
}

void MainWindow::drawPitchRollValues(float pitch,float roll)
{
	float resultP=0.0;
	if(pitch>0 && pitch<=90){
		resultP=pitch/90;
		resultP=-resultP;
	}else if(pitch<0 && pitch>=-90){
		resultP=pitch/-90;
	}else{
		resultP=0.0;
	}

	//rotY
	d_ai->setGradient(resultP);
	//rotX
	d_ai->setAngle(roll);
	QString pString=QString::number(pitch,'f',2);
	QString rString=QString::number(roll,'f',2);    

	ui->pitchValue->setText(pString);
	ui->rollValue->setText(rString);

}

void MainWindow::drawVelocitiesValues(float vlx,float vly,float vlz)
{
	//mm/sec to m/s
	float result=0.0;
	if(vlx<0.0){
		ui->velXLabel->setStyleSheet("QLabel {background-color: red}");
	}else{
		ui->velXLabel->setStyleSheet("QLabel {background-color: green}");
	}
	vlx=std::abs(vlx);
	vlx=vlx/1000.0;
	result=(0.6*vx)+((1-0.6)*vlx);
	velLinX->setValue(result);
	vx=vlx;

	if(vly<0.0){
		ui->velYLabel->setStyleSheet("QLabel {background-color: red}");
	}else{
		ui->velYLabel->setStyleSheet("QLabel {background-color: green}");
	}
	vly=std::abs(vly);
	vly=vly/1000.0;
	result=(0.6*vy)+((1-0.6)*vly);
	velLinY->setValue(result);
	vy=vly;

	if(vlz<0.0){
		ui->velZLabel->setStyleSheet("QLabel {background-color: red}");
	}else{
		ui->velZLabel->setStyleSheet("QLabel {background-color: green}");
	}
	vlz=std::abs(vlz);
	vlz=vlz/1000.0;
	result=(0.6*vz)+((1-0.6)*vlz);
	velLinZ->setValue(result);
	vz=vlz;
}

void MainWindow::setSensors(Sensors* sensors){
	this->sensors=sensors; 

}

QwtCompass *MainWindow::createCompass(int pos)
{
	int c;

	Palette colorGroup;
	for ( c = 0; c < Palette::NColorRoles; c++ )
	colorGroup.setColor((Palette::ColorRole)c, QColor());

	#if QT_VERSION < 0x040000
		colorGroup.setColor(Palette::Base, backgroundColor().light(120));
	#else
		colorGroup.setColor(Palette::Base,
		palette().color(backgroundRole()).light(120));
	#endif
	colorGroup.setColor(Palette::Foreground,
	colorGroup.color(Palette::Base));

	QwtCompass *compass = new QwtCompass(this);
	compass->setLineWidth(4);
	compass->setFrameShadow(
	pos <= 2 ? QwtCompass::Sunken : QwtCompass::Raised);

	switch(pos)
	{
		case 0:
		{
			/*
			A windrose, with a scale indicating the main directions only
			*/
			QMap<double, QString> map;
			map.insert(0.0, "N");
			map.insert(90.0, "E");
			map.insert(180.0, "S");
			map.insert(270.0, "W");

			compass->setLabelMap(map);

			QwtSimpleCompassRose *rose = new QwtSimpleCompassRose(4, 1);
			compass->setRose(rose);

			compass->setNeedle(
			new QwtCompassWindArrow(QwtCompassWindArrow::Style2));
			compass->setValue(60.0);
			break;
		}
		case 1:
		{
			/*
			A compass showing another needle
			*/
			QMap<double, QString> map;
			map.insert(0.0, "");
			map.insert(90.0, "");
			map.insert(180.0, "");
			map.insert(270.0, "");

			compass->setLabelMap(map);

			compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
			compass->setScaleTicks(0, 0, 3);

			compass->setNeedle(new QwtCompassMagnetNeedle(
			QwtCompassMagnetNeedle::TriangleStyle, Qt::white, Qt::red));
			compass->setValue(220.0);
			break;
		}
	}

	QPalette newPalette = compass->palette();
	for ( c = 0; c < Palette::NColorRoles; c++ )
	{
		if ( colorGroup.color((Palette::ColorRole)c).isValid() )
		{
			for ( int cg = 0; cg < QPalette::NColorGroups; cg++ )
			{
				newPalette.setColor(
					(QPalette::ColorGroup)cg,
					(Palette::ColorRole)c,
					colorGroup.color((Palette::ColorRole)c));
			}
		}
	}

	for ( int i = 0; i < QPalette::NColorGroups; i++ )
	{
		QPalette::ColorGroup cg = (QPalette::ColorGroup)i;

		const QColor light =
		    newPalette.color(cg, Palette::Base).light(170);
		const QColor dark = newPalette.color(cg, Palette::Base).dark(170);
		const QColor mid = compass->frameShadow() == QwtDial::Raised
		    ? newPalette.color(cg, Palette::Base).dark(110)
		    : newPalette.color(cg, Palette::Base).light(110);

		newPalette.setColor(cg, Palette::Dark, dark);
		newPalette.setColor(cg, Palette::Mid, mid);
		newPalette.setColor(cg, Palette::Light, light);
	}
	compass->setPalette(newPalette);

	return compass;
}

QwtDial *MainWindow::createDial(int pos)
{
    QwtDial *dial = NULL;
    switch(pos)
    {
        case 0:
        {
            d_ai = new AttitudeIndicator(this);
            dial = d_ai;
            break;
        }
        case 1:
        {
            d_speedo = new SpeedoMeter(this);
            d_speedo->setRange(0.0,8.0);
            d_speedo->setLabel("m/s");
            d_speedo->setOrigin(180);
            d_speedo->setScaleArc(0.0,270.0);
            d_speedo->setScale(-1, 2, 1.0);
            dial = d_speedo;
            break;
        }
        case 2:
        {
            d_speedo = new SpeedoMeter(this);
            d_speedo->setRange(0.0,10.0);
            d_speedo->setLabel("m");
            d_speedo->setOrigin(-90);
            d_speedo->setScaleArc(0.0,360.0);
            d_speedo->setScale(-1, 2, 1);
            dial = d_speedo;
            break;
        }

    }

    if ( dial )
    {
        dial->setReadOnly(true);
        dial->scaleDraw()->setPenWidth(3);
        dial->setLineWidth(4);
        dial->setFrameShadow(QwtDial::Sunken);
    }
    return dial;
}

void MainWindow::on_takeoff_button_clicked()
{
    qDebug() << "take off";
    this->sensors->takeOff();
}

void MainWindow::on_land_button_clicked()
{
    qDebug() << "land";
    this->sensors->land();
}

void MainWindow::on_toggleCam_pressed()
{
    qDebug() << "Toggle Cam";
	this->sensors->toggleCam();
}

void MainWindow::on_reset_button_clicked()
{
    qDebug() << "reset";
    this->sensors->reset();
}

void MainWindow::on_linZU_pressed()
{
    qDebug() << "Up pressed";
    this->linz=LINZ;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linZU_released()
{
    qDebug() << "Up released";
    this->linz=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linZD_pressed()
{
    qDebug() << "Down pressed";
    this->linz=-LINZ;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linZD_released()
{
    qDebug() << "Down released";
    this->linz=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_angZL_pressed()
{
    qDebug() << "Rotate Left pressed";
    this->yaw=ANGZ;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_angZL_released()
{
    qDebug() << "Rotate Left released";
    this->yaw=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_angZR_pressed()
{
    qDebug() << "Rotate Right pressed";
    this->yaw=-ANGZ;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_angZR_released()
{
    qDebug() << "Rotate Right released";
    this->yaw=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linXF_pressed()
{
    qDebug() << "Forward pressed";
    this->linx=LINX;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linXF_released()
{
    qDebug() << "Forward released";
    this->linx=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linXB_pressed()
{
    qDebug() << "Backward pressed";
    this->linx=-LINX;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linXB_released()
{
    qDebug() << "Backward released";
    this->linx=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linYL_pressed()
{
    qDebug() << "Left pressed";
    this->liny=LINY;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linYL_released()
{
    qDebug() << "Left released";
    this->liny=0.0;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linYR_pressed()
{
    qDebug() << "Right pressed";
    this->liny=-LINY;
    this->sendVelocitiesToUAV();
}

void MainWindow::on_linYR_released()
{
    qDebug() << "Right released";
    this->liny=0.0;
    this->sendVelocitiesToUAV();
}


void MainWindow::keyPressEvent( QKeyEvent *k )
{
    switch ( tolower(k->key()) ) {
        case 'w':
            on_linZU_pressed();
            break;
        case 'd':
            on_angZR_pressed();
            break;
        case 's':
            on_linZD_pressed();
            break;
        case 'a':
            on_angZL_pressed();
            break;
        case '2':
            on_linXB_pressed();
            break;
        case '8':
            on_linXF_pressed();
            break;
        case '6':
            on_linYR_pressed();
            break;
        case '4':
            on_linYL_pressed();
            break;
        case 'r':
            on_reset_button_clicked();
            break;
        case 't':
            on_takeoff_button_clicked();
            break;
        case 'l':
            on_land_button_clicked();
            break;
        case 'c':
        	on_toggleCam_pressed();
        	break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *k )
{
    switch ( tolower(k->key()) ) {
        case 'w':
            on_linZU_released();
            break;
        case 'd':
            on_angZR_released();
            break;
        case 's':
            on_linZD_released();
            break;
        case 'a':
            on_angZL_released();
            break;
        case '2':
            on_linXB_released();
            break;
        case '8':
            on_linXF_released();
            break;
        case '6':
            on_linYR_released();
            break;
        case '4':
            on_linYL_released();
            break;
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index==0){
        compass->show();
        altd->show();
        velLinX->show();
        velLinY->show();
        velLinZ->show();
        horizon->show();
    }else{
        compass->hide();
        altd->hide();
        velLinX->hide();
        velLinY->hide();
        velLinZ->hide();
        horizon->hide();
    }

}
void MainWindow::setDroneStatus(int state){
	switch(state){
		case 1:
			ui->statusValue->setText("Initied");
			break;
		case 2:
			ui->statusValue->setText("Landed");
			break;
		case 3:
			ui->statusValue->setText("Flying");
			break;
		case 4:
			ui->statusValue->setText("Hovering");
			break;
		case 5:
			ui->statusValue->setText("Test");
			break;
		case 6:
			ui->statusValue->setText("Tacking off");
			break;
		case 7:
			ui->statusValue->setText("Flying");
			break;
		case 8:
			ui->statusValue->setText("Landing");
			break;
		case 9:
			ui->statusValue->setText("Looping");
			break;	
		default:								
			ui->statusValue->setText("Unknown");																	
	}

}

float MainWindow::rad2deg(float r)
{
	float degree=(r * 180) / M_PI;
	return degree;
}

float MainWindow::quatToRoll(float qw, float qx, float qy, float qz) 
{

	double rotateXa0 = 2.0*(qy*qz + qw*qx);
	double rotateXa1 = qw*qw - qx*qx - qy*qy + qz*qz;
	float rotateX = 0.0;
	if (rotateXa0 != 0.0 && rotateXa1 != 0.0) 
		rotateX = atan2(rotateXa0, rotateXa1);

	return rotateX;
}

float MainWindow::quatToPitch(float qw, float qx, float qy, float qz) 
{

	double rotateYa0 = -2.0*(qx*qz - qw*qy);
	float rotateY = 0.0;
	if( rotateYa0 >= 1.0 )
		rotateY = M_PI/2.0;
	else if( rotateYa0 <= -1.0 )
		rotateY = -M_PI/2.0;
	else rotateY = asin(rotateYa0);

	return rotateY;
}

float MainWindow::quatToYaw(float qw, float qx, float qy, float qz) 
{

	double rotateZa0 = 2.0*(qx*qy + qw*qz);
	double rotateZa1 = qw*qw + qx*qx - qy*qy - qz*qz;
	float rotateZ = 0.0;
	if (rotateZa0 != 0.0 && rotateZa1 != 0.0)
		rotateZ = atan2(rotateZa0, rotateZa1);

	return rotateZ;
}
