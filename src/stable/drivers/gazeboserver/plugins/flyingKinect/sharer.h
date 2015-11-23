#ifndef __GAZEBO_SHARER_H__
#define __GAZEBO_SHARER_H__

#include "pose3d.h"

namespace gazebo
{

	class Sharer
	{
		public:

			/** Default destructor. */
			virtual ~Sharer();

			/** Method for controlling the access to the single instance
			 *  of this class with singleton pattern.
			 */
			static Sharer* getInstance();

			/** Method to set the pointer to gazebo model in this sharer.
			 *
			 * @param model	the pointer to the gazebo model of flyingKinect.
			 */
			void setGzModel(const physics::ModelPtr model);

			/** Method to get the pointer to gazebo model in this sharer.
			 *
			 * @return	the pointer to the gazebo model of flyingKinect.
			 */
			physics::ModelPtr getGzModel();

			/** Method to set the current pose3d of the model
			 *  when update the depth image and the point cloud.
			 */
			void setCurrentPose3d();

			/** Method to get the last saved current pose3d of the model.
			 *
			 * @return	the last saved pose3d position of the model.
			 */
			math::Pose getCurrentPose3d();

		private:

			Sharer();	///< Private so that it can not be called.

			Sharer(const Sharer &);	///< Copy constructor is private.

			Sharer& operator=(const Sharer &);	///< Assignment operator is private.

			/** Delete the sharer instance */
			static void DestroySharer();


			static Sharer* pInstance;	///< Pointer to the single instance of this class with singleton pattern.

			pthread_mutex_t mutex;	///< Mutex to coordinate access to 'position' variable.

			physics::ModelPtr gzModel;	///< Model pointer to flyingKinect model.

			math::Pose positionLast;		///< Saved pose3d position of the last time that the sensor plugin updates the depth image.
			math::Pose positionPrevious;	///< Saved pose3d position of the previous time that the sensor plugin updates the depth image.

	};

} /* gazebo */
#endif /* __GAZEBO_SHARER_H__ */
