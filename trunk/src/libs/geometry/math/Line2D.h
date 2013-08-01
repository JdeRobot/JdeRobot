/*Represents a 2D line with the general equation of the line: Ax+By+C = 0*/

#ifndef LINE2D_H
#define LINE2D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>

class Line2D{
public:
  Line2D();
  Line2D(double p1x, double p1y, double p2x, double p2y);
  Line2D(Eigen::Vector2d p1, Eigen::Vector2d p2);
  //Line2D(Point2D p1, Point2D p2);
  Line2D(double va, double vb, double vc);
  Line2D(Eigen::Vector3d v);

  Eigen::Vector3d getVector();

  /*Calculate line from 2 2D points*/
  Eigen::Vector3d getLine(double p1x, double p1y, double p2x, double p2y);
  Eigen::Vector3d getLine(Eigen::Vector2d p1, Eigen::Vector2d p2);

  /*Calculate a 2D normal line from current 2D line and a 2D point*/
  Line2D getNormalLine(double px, double py);
  
private:

  Eigen::Vector3d v;
    
};

#endif
