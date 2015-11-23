#ifndef NAVIGATORCAMERA_SHARER_H
#define NAVIGATORCAMERA_SHARER_H

#include <pthread.h>
#include <jderobot/pose3d.h>
#include <cv.h>
#include "quaternion.h"

namespace navigatorCamera {

	/** This class provides a shared memory point.
	 *
	 * All methods of an instance of this class are thread-safe via a mutex and
	 * provide or save a copy of the related data.
	 *
	 */
	class Sharer {
	public:

		/// Default constructor.
		Sharer();

		/// Default destructor.
		virtual ~Sharer();

		/** Set method to control the flag 'guiVisible'.
		 *
		 * @param status	<code>true</code> if the gui is visible,
		 * 					<code>false</code> otherwise.
		 */
		void setGuiVisible(bool status);

		/** Set method to control the flag 'controlActive'.
		 *
		 * @param status	<code>true</code> if the control is active,
		 * 					<code>false</code> otherwise.
		 */
		void setControlActive(bool status);

		/** Set method to put the current Pose3D.
		 *
		 * @param p		current Pose3D Pointer.
		 */
		void setPose3D(jderobot::Pose3DDataPtr p);

		/** Set method to put the current RGB Image.
		 *
		 * @param image		current RGB Image.
		 */
		void setImage(cv::Mat image);

		/** Set method to put the current translation step, the distance
		 *  to move when is used the translation controls of GUI.
		 *
		 * @param step	current translation step.
		 */
		void setTranslationStep(double step);

		/** Set method to put the current rotation step, the angle
		 *  to move when is used the rotation controls of GUI.
		 *
		 * @param step	current rotation step.
		 */
		void setRotationStep(double step);
		void setSpeedX(double step);
		void setSpeedY(double step);

		/** Get method to know the status of flag 'guiVisible'.
		 *
		 * @return	<code>true</code> if the gui is visible,
		 * 			<code>false</code> otherwise.
		 */
		bool getGuiVisible();

		/** Get method to know the status of flag 'controlActive'.
		 *
		 * @return	<code>true</code> if the control is active,
		 * 			<code>false</code> otherwise.
		 */
		bool getControlActive();

		/** Get method to know the current Pose3D.
		 *
		 * @return	current Pose3D Pointer.
		 */
		jderobot::Pose3DDataPtr getPose3D();

		/** Get method to know the current RGB Image.
		 *
		 * @return	current RGB image.
		 */
		cv::Mat getImage();

		/** Get method to know the current translation step.
		 *
		 * @return	current translation step.
		 */
		double getTranslationStep();

		/** Get method to know the current rotation step.
		 *
		 * @return	current rotation step.
		 */
		double getRotationStep();
		double getSpeedX();
		double getSpeedY();

		/** Method to change the translation of current Pose3D.
		 *
		 * @param sX	sign to apply at translation step for 'x' component.
		 * @param sY	sign to apply at translation step for 'y' component.
		 * @param sZ	sign to apply at translation step for 'z' component.
		 */
		void changePose3dTranslation(double sX, double sY, double sZ);
		void changePose3dTranslationSpeed();

		/** Method to change the rotation of current Pose3D.
		 *
		 * @param sY	sign to apply at rotation step for yaw angle.
		 * @param sP	sign to apply at rotation step for pitch angle.
		 * @param sR	sign to apply at rotation step for roll angle.
		 */
		void changePose3dRotation(double sY, double sP, double sR);

		/** Method to restart the current Pose3D.
		 *
		 */
		void restartPose3D();


	private:

		pthread_mutex_t synch;			///< Mutex for thread-safe access to internal data.
		pthread_mutex_t synchFlags;		///< Mutex for thread-safe access to internal flags.
		pthread_mutex_t synchPose3D;	///< Mutex for thread-safe access to pose3d related data.
		pthread_mutex_t synchCamera;	///< Mutex for thread-safe access to camera related data.

		bool guiVisible;		///< Flag for the visibility of the GUI.
		bool controlActive;		///< Flag for the status of the control threads.

		jderobot::Pose3DDataPtr pose3d;				///< Current Pose3D.
		cv::Mat RGBimage;							///< Current RGB image

		double trlnStp;		///< Current translation step to move when is used the translation controls of GUI.
		double rtnStp;		///< Current rotation step to move when is used the rotation controls of GUI.

		double speed_x;
		double speed_y;


	}; /* class Sharer */

} /* namespace navigatorCamera */
#endif /* NAVIGATORCAMERA_SHARER_H */
