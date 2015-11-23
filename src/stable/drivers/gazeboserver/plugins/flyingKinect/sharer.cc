#include "sharer.h"
#include "pose3d.h"

namespace gazebo
{

	Sharer* Sharer::pInstance = NULL; /* Initializes the pointer. */

	Sharer* Sharer::getInstance()
	{
		if ( pInstance == NULL ) // Only allow one instance of class to be generated.
		{
			pInstance = new Sharer();
			atexit(&DestroySharer);		// At exit, destroy the sharer.
		}
		return pInstance;
	}

	Sharer::Sharer()
	{
		pthread_mutex_init(&mutex, NULL);
	}

	void Sharer::DestroySharer()
	{
		if ( pInstance != NULL )
			delete pInstance;
	}

	Sharer::~Sharer()
	{
		pthread_mutex_destroy(&mutex);
	}

	void Sharer::setGzModel(const physics::ModelPtr model)
	{
		pthread_mutex_lock(&mutex);
		pInstance->gzModel = model;
		pthread_mutex_unlock(&mutex);
	}

	physics::ModelPtr Sharer::getGzModel()
	{
		pthread_mutex_lock(&mutex);
		physics::ModelPtr retVal = pInstance->gzModel;
		pthread_mutex_unlock(&mutex);

		return retVal;
	}

	void Sharer::setCurrentPose3d()
	{
		if ( pInstance->gzModel != NULL )
		{
			pthread_mutex_lock(&mutex);
			pInstance->positionPrevious = pInstance->positionLast;
			pInstance->positionLast = pInstance->gzModel->GetWorldPose();
			pthread_mutex_unlock(&mutex);
		}
	}

	math::Pose Sharer::getCurrentPose3d()
	{
		pthread_mutex_lock(&mutex);
		math::Pose retVal = pInstance->positionPrevious;
		pthread_mutex_unlock(&mutex);

		return retVal;
	}

} /* gazebo */
