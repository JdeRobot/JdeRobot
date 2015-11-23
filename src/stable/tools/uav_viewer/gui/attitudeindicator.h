#ifndef ATTITUDEINDICATOR_H
#define ATTITUDEINDICATOR_H

#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <QKeyEvent>

class AttitudeIndicatorNeedle: public QwtDialNeedle
{
public:
    AttitudeIndicatorNeedle(const QColor &);
    virtual void draw(QPainter *, const QPoint &, int length,
        double direction, QPalette::ColorGroup) const;
};

class AttitudeIndicator: public QwtDial
{
    Q_OBJECT

public:
    AttitudeIndicator(QWidget *parent = NULL);
    double angle() const { return value(); }
    double gradient() const { return d_gradient; }

public slots:
    void setGradient(double);
    void setAngle(double angle) { setValue(angle); }

protected:
    virtual void keyPressEvent(QKeyEvent *e);

    virtual void drawScale(QPainter *, const QPoint &center,
        int radius, double origin, double arcMin, double arcMax) const;

    virtual void drawScaleContents(QPainter *painter,
        const QPoint &center, int radius) const;

private:
    double d_gradient;
};

#endif // ATTITUDEINDICATOR_H
