/** @file quaternion.h
 *
 *  @par License
 *
 *  @par
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  @par
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  @par
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  @date   24/07/2014
 *  @author L. Roberto Morales Iglesias <lr.morales.iglesias@gmail.com>
 *
 */
#ifndef JDEROBOT_QUATERNION_H_
#define JDEROBOT_QUATERNION_H_

#include <cmath>
#include <eigen3/Eigen/Geometry>

namespace jderobot {

	namespace math {

		template<typename Scalar, int Options = Eigen::AutoAlign>
		class Quaternion: public Eigen::Quaternion<Scalar, Options> {
			public:


				typedef Eigen::Quaternion<Scalar,Options> Base;
				typedef typename Base::AngleAxisType AngleAxisType;

				/// Default constructor leaving the quaternion uninitialized.
				Quaternion(void):Eigen::Quaternion<Scalar, Options>() {}

				/** Constructs and initializes the quaternion $w+xi+yj+zk$
				 *  from its four coefficients w, x, y and z.
				 *
				 * @warning Note the order of the arguments:
				 *          the real w coefficient first, while internally
				 *          the coefficients are stored in the following
				 *          order: [x, y, z, w]
				 */
				Quaternion(const Scalar & w, const Scalar & x, const Scalar &y,
						const Scalar & z)
					:Eigen::Quaternion<Scalar, Options>(w,x,y,z){}

				/// Copy constructor
				template<typename OtherDerived>
				Quaternion(const Eigen::QuaternionBase<OtherDerived>& other)
					: Eigen::Quaternion<Scalar, Options>(other) { }

				/// Constructs and initializes a quaternion from the angle-axis aa
				Quaternion(const AngleAxisType& aa)
					: Eigen::Quaternion<Scalar, Options>(aa) {}

				static Eigen::Quaternion<Scalar, Options> FromEuler(const Eigen::Matrix<Scalar, 3, 1 >& v){
					double w,x,y,z;
					double phi, theta, psi;

					phi = v.x() / 2.0;
					theta = v.y() / 2.0;
					psi = v.z() / 2.0;
					w = std::cos(phi) * std::cos(theta) * std::cos(psi)
						+ std::sin(phi) * std::sin(theta) * std::sin(psi);
					x = std::sin(phi) * std::cos(theta) * std::cos(psi)
						- std::cos(phi) * std::sin(theta) * std::sin(psi);
					y = std::cos(phi) * std::sin(theta) * std::cos(psi)
						+ std::sin(phi) * std::cos(theta) * std::sin(psi);
					z = std::cos(phi) * std::cos(theta) * std::sin(psi)
						- std::sin(phi) * std::sin(theta) * std::cos(psi);


					return Eigen::Quaternion<Scalar, Options>(w,x,y,z).normalized();
				}

				Eigen::Matrix<Scalar, 3, 1 > getEulerVector() const{
					Scalar x,y,z;
					Scalar ww,xx,yy,zz;

					ww = this->w() * this->w();
					xx = this->x() * this->x();
					yy = this->y() * this->y();
					zz = this->z() * this->z();

					x = std::atan2(2 * (this->y()*this->z() + this->w()*this->x()), ww - xx - yy + zz);

					Scalar sarg = -2 * (this->x()*this->z() - this->w() * this->y());
					y = (sarg <= -1.0) ? -0.5*M_PI : ((sarg >= 1.0) ? 0.5*M_PI : std::asin(sarg));

					z = std::atan2(2 * (this->x()*this->y() + this->w()*this->z()), ww + xx - yy - zz);


					return Eigen::Matrix<Scalar, 3, 1 >(x,y,z);
				}



				template<typename OtherDerived>
				Quaternion & operator= (const Eigen::QuaternionBase <OtherDerived>& other) {
					this->Base::operator=(other);
					return *this;
				}
		};

		/// Floating point with simple precision based quaternion
		typedef Quaternion<float> Quaternionf;
		/// Floating point with double precision based quaternion
		typedef Quaternion<double> Quaterniond;

	}
}

#endif /* JDEROBOT_QUATERNION_H_ */
