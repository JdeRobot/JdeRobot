#include "controlvw.h"

controlVW::controlVW(QWidget *parent) :
    QWidget(parent)
{

    QGridLayout* mainLayout = new QGridLayout();

    setLayout(mainLayout);

    setAutoFillBackground(true);

//    show();

    QPalette Pal(palette());
    // set black background
    Pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(Pal);

    setMinimumSize(400, 400);
    qimage.load(":/images/pelota.png");

	setMouseTracking(true);
	this->v = 0;
	this->w = 0;

}

void controlVW::setProps(Config::Properties props)
{
	this->props = props;

    QString svmax = QString::fromUtf8(props.asStringWithDefault("carViz.Vmax", "5").c_str());
    QString swmax = QString::fromUtf8(props.asStringWithDefault("carViz.Wmax", "0.5").c_str());

    this->v_max = svmax.toFloat();
    this->w_max = swmax.toFloat();
}

void controlVW::Stop()
{
    line = QPointF(0, 0);
    repaint();

}


void controlVW::mouseMoveEvent ( QMouseEvent * event )
{

    if(event->buttons()==Qt::LeftButton ){

        int x = event->x()-width()/2;
        int y = event->y()-height()/2;

        line = QPointF(x, y);

		//Show tooltip with the current v and w
		QString aa = "v: " + QString::number(v) + " , w: " + QString::number(w);
		QToolTip::showText(QPoint(QCursor::pos()), aa);

        repaint();

    }

}

float controlVW::getV()
{
	return this->v;
}

float controlVW::getW()
{
	return this->w;
}

void controlVW::paintEvent(QPaintEvent *)
{

   int _width = width();
   int _height = height();


   int width = 2;
   QPen pen;

   QPainter painter(this);
   painter.setPen(pen);

   pen = QPen(Qt::blue, width);
   painter.setPen(pen);

   // Centro del widget
   painter.translate(QPoint(_width/2, _height/2));

   // eje
   painter.drawLine(QPointF(-_width, 0),
                    QPointF( _width, 0));

   painter.drawLine(QPointF(0, -_height),
                    QPointF(0, _height));

   // con el raton
   pen = QPen(Qt::red, width);
   painter.setPen(pen);

   painter.drawLine(QPointF(line.x(), -_height),
                    QPointF(line.x(), _height));

   painter.drawLine(QPointF(-_width, line.y()),
                    QPointF( _width, line.y()));

//   std::cout << "x: " << line.x() << " y: " << -line.y() << std::endl;

   //float k = 0.01;
   //float p = 0;

   //float v_normalized = 40 * (k * line.y() + p)*(-1);
   //float w_normalized = 20 * (k * line.x() + p)*(-1);

	float v_div = 200/(v_max)*1.0;
	float w_div = 200/(w_max)*1.0;

	float v_normalized = (line.y()/v_div)*(-1);
   	float w_normalized = (line.x()/w_div)*(-1);

	this->v = v_normalized;
	this->w = w_normalized;

//   std::cout << "v_normalized: " << v_normalized << " w_normalized: " << w_normalized << std::endl;

   emit VW_changed(v_normalized, w_normalized);

   painter.drawImage(line.x()-qimage.width()/2, line.y()-qimage.height()/2, qimage);

}
