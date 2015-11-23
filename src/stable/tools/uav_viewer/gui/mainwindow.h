#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_compass.h>
#include <qwt_analog_clock.h>
#include <QPalette>
#include <QKeyEvent>
#include "attitudeindicator.h"
#include "speedometer.h"
#include "../sensors/sensors.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	    
	public:
		explicit MainWindow(QWidget *parent = 0);
		void setSensors(Sensors* sensors);
		void updateThreadGUI();    
		~MainWindow();
	    
	signals:
		void signal_updateGUI();
	    
	private slots:
		void keyPressEvent( QKeyEvent *k );

		void keyReleaseEvent( QKeyEvent *k );

		void updateGUI_recieved();

		void on_takeoff_button_clicked();

		void on_land_button_clicked();

		void on_reset_button_clicked();

		void on_toggleCam_pressed();

		void on_linZU_pressed();

		void on_linZU_released();

		void on_linZD_pressed();

		void on_linZD_released();

		void on_angZL_pressed();

		void on_angZL_released();

		void on_angZR_pressed();

		void on_angZR_released();

		void on_linXF_pressed();

		void on_linXF_released();

		void on_linXB_pressed();

		void on_linXB_released();

		void on_linYL_pressed();

		void on_linYL_released();

		void on_linYR_pressed();

		void on_linYR_released();

		void on_tabWidget_currentChanged(int index);

	private:
		void sendVelocitiesToUAV();
		void drawYawValues(float degress);
		void drawAltd(float meters);
		void drawPitchRollValues(float pitch,float roll); 
		static float quatToRoll(float, float, float, float);
		static float quatToPitch(float, float, float, float);
		static float quatToYaw(float, float, float, float);    
		float rad2deg(float r);
        void initButtons();
		void drawVelocitiesValues(float vx,float vy,float vz); 
		void setDroneStatus(int state);    
		void printNavdata(jderobot::NavdataDataPtr data);		  
		float vx,vy,vz,pitch,roll,yaw;    
		float linx,liny,linz;
		QwtCompass *createCompass(int pos);
		QwtDial *createDial(int pos);
		Ui::MainWindow *ui;
		SpeedoMeter *d_speedo;
		AttitudeIndicator *d_ai;
		QwtDial *velLinZ;
		QwtDial *velLinY;
		QwtDial *velLinX;
		QwtCompass *compass;
		QwtDial *altd;
		QwtDial *horizon;
		Sensors* sensors;
};

#endif // MAINWINDOW_H
