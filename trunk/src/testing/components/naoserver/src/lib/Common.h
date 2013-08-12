/**
 * @file Math/Common.h
 *
 * This contains some often used mathematical definitions and functions.
 *
 * @author <a href="mailto:martin.kallnik@gmx.de">Martin Kallnik</a>
 * @author Max Risler
 */


#ifndef __Math_Common_h__
#define __Math_Common_h__

#include <cmath>
#include <cstdlib>
#include <string>

#ifdef WEBOTS
#define USE_SENSOR	false
#else
#define USE_SENSOR	true
#endif


/**
* defines the sign of a (-1, 0 or 1)
*/
#ifndef sgn
#define sgn(a)   ( (a) < 0 ? -1 : ((a)==0) ? 0 : 1 )
#endif

/**
* defines the sign of a (-1 or 1)
*/
#ifndef sign
#define sign(a)   ( (a) < 0 ? -1 : 1 )
#endif

/**
* defines the square of a value
*/
#ifndef sqr
#define sqr(a) ( (a) * (a) )
#endif

inline double sec(const double a){return 1/cos(a);}

inline double cosec(const double a){return 1/sin(a);}

/** @name constants for some often used angles */
///@{
/** constant for a half circle*/
const double pi = 3.1415926535897932384626433832795;
/** constant for a full circle*/
const double pi2 = 2.0*3.1415926535897932384626433832795;
/** constant for three quater circle*/
const double pi3_2 = 1.5*3.1415926535897932384626433832795;
/** constant for a quarter circle*/
const double pi_2 = 0.5*3.1415926535897932384626433832795;
/** constant for a 1 degree*/
const double pi_180 = 3.1415926535897932384626433832795/180;
/** constant for a 1/8 circle*/
const double pi_4 = 3.1415926535897932384626433832795*0.25;
/** constant for a 3/8 circle*/
const double pi3_4 = 3.1415926535897932384626433832795*0.75;
/** constant for an expression used by the gaussian function*/
const double sqrt2pi = sqrt(2.0*pi);
///@}

// std output with color
const std::string begin_red = "\033[1;31m";
const std::string begin_green = "\033[1;32m";
const std::string begin_yellow = "\033[1;33m";
const std::string begin_blue = "\033[1;34m";
const std::string begin_purple = "\033[1;35m";
const std::string begin_lblue = "\033[1;36m";
const std::string begin_grey = "\033[1;37m";
const std::string end_color = "\033[0m";



/**
 * Converts angle from rad to degrees.
 * \param angle code in rad
 * \return angle coded in degrees
 */
inline double toDegrees(double angle){return angle * 180.0 / pi;}

/** Converts angle from degrees to rad.
 * \param degrees angle coded in degrees
 * \return angle coded in rad
 */
inline double fromDegrees(double degrees){return degrees * pi_180;}

/** Converts angle from degrees to rad.
 * \param degrees angle coded in degrees
 * \return angle coded in rad
 */
inline double toRadians(double degrees){return degrees * pi_180;}

/**
* reduce angle to [-pi..+pi[
* \param data angle coded in rad
* \return normalized angle coded in rad
*/
inline double normalizePi(double data)
{
  if (data < pi && data >= -pi) return data;
  double ndata = data - ((int )(data / pi2))*pi2;
  while (ndata >= pi)
  {
    ndata -= pi2;
  }
  while (ndata < -pi)
  {
    ndata += pi2;
  }
  return ndata;
}

/**
* The function returns a random number in the range of [0..1].
* @return The random number.
*/
inline double randomDouble() {return double(rand()) / RAND_MAX;}

/**
* The function returns a random integer number in the range of [0..n-1].
* @param n the number of possible return values (0 ... n-1)
* @return The random number.
*/
inline int random(int n) {return (int)(randomDouble()*n*0.999999);}

/** constant, cast before execution*/
const double RAND_MAX_DOUBLE = static_cast<double>(RAND_MAX);

/**
* The function returns a random integer number in the range of [0..n].
* @param n the number of possible return values (0 ... n)
* @return The random number.
*/
inline int randomFast(int n)
{
  return static_cast<int>((rand()*n) / RAND_MAX_DOUBLE);
}

/**
* Round to the next integer
* @param d A number
* @return The number as integer
*/
inline int roundNumberToInt(double d)
{
  return static_cast<int>(floor(d+0.5));
}

/**
* Round to the next integer but keep type
* @param d A number
* @return The number
*/
inline double roundNumber(double d)
{
  return floor(d+0.5);
}

inline float normalAng(float x, float mu, float st)
{
	float diff = normalizePi(x-mu);
	float d1 = st*sqrt(2.0*M_PI);
	float d2 = 2*st*st;

	if(d1==0.0) d1 = 0.00001;
	if(d2==0.0) d2 = 0.00001;

	return (1.0/d1)*exp(-(diff*diff)/d2);
}
inline float normalDist(float x, float mu, float st)
{
	float diff = x-mu;
	float d1 = st*sqrt(2.0*M_PI);
	float d2 = 2*st*st;

	if(d1==0.0) d1 = 0.00001;
	if(d2==0.0) d2 = 0.00001;

	return (1.0/d1)*exp(-(diff*diff)/d2);
}

#endif // __Math_Common_h__
