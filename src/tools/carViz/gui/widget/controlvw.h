#ifndef CONTROLVW_H
#define CONTROLVW_H

#include <QtWidgets>

#include <iostream>
#include <string>

#include <jderobot/config/properties.hpp>

class controlVW : public QWidget
{
    Q_OBJECT
public:
    explicit controlVW(QWidget *parent = 0);
    
    void Stop();
    void setProps(Config::Properties props);
	float getV();
	float getW();

private:
    QPointF line;
    QImage qimage;

	float v, w, v_max, w_max;
	Config::Properties props;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent ( QMouseEvent * event );

signals:
    void VW_changed(float, float);

public slots:
    
};

#endif // CONTROLVW_H
