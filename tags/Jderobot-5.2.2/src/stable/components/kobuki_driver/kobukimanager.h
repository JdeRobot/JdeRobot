#ifndef KOBUKIMANAGER_H
#define KOBUKIMANAGER_H
/*****************************************************************************
 Includes
 ****************************************************************************/
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <csignal>
#include <ecl/time.hpp>
#include <ecl/sigslots.hpp>
#include <ecl/geometry/pose2d.hpp>
#include <ecl/linear_algebra.hpp>
#include "kobuki_driver/kobuki.hpp"

//boost
#include <boost/signals2/mutex.hpp>

/*****************************************************************************
 Classes
*****************************************************************************/

class KobukiManager {
public:
  KobukiManager();

  ~KobukiManager();

  void update();

  void setV(float v);
  void setW(float w);

  double getRobotX();
  double getRobotY();
  double getRobotTheta();

  void processMotion();

  ecl::Pose2D<double> getPose();

private:
    double dx, dth;
    ecl::Pose2D<double> pose;
    kobuki::Kobuki kobuki;
    ecl::Slot<> slot_stream_data;
    private: boost::signals2::mutex mutex; ///< Mutex for thread-safe access to internal data.

    float v;
    float w;

};
#endif // KOBUKIMANAGER_H
