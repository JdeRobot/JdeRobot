#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <QString>
#include <qwt_dial.h>

class SpeedoMeter: public QwtDial
{
public:
    SpeedoMeter(QWidget *parent = NULL);

    void setLabel(const QString &);
    QString label() const;

protected:
    virtual void drawScaleContents(QPainter *painter,
        const QPoint &center, int radius) const;

private:
    QString d_label;
};

#endif // SPEEDOMETER_H
