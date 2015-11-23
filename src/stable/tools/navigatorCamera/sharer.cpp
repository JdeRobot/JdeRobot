#include "sharer.h"

namespace navigatorCamera {

	Sharer::Sharer()
	{
		pthread_mutex_init(&this->synch, NULL);
		pthread_mutex_init(&this->synchFlags, NULL);
		pthread_mutex_init(&this->synchPose3D, NULL);
		pthread_mutex_init(&this->synchCamera, NULL);

		guiVisible = false;
		controlActive = false;
		speed_x = 0.0;
		speed_y = 0.0;
	}

	Sharer::~Sharer()
	{
		pthread_mutex_destroy(&this->synch);
		pthread_mutex_destroy(&this->synchFlags);
		pthread_mutex_destroy(&this->synchPose3D);
		pthread_mutex_destroy(&this->synchCamera);
	}

	void Sharer::setGuiVisible(bool status)
	{
		pthread_mutex_lock(&this->synchFlags);
		this->guiVisible = status;
		pthread_mutex_unlock(&this->synchFlags);
	}

	void Sharer::setControlActive(bool status)
	{
		pthread_mutex_lock(&this->synchFlags);
		this->controlActive = status;
		pthread_mutex_unlock(&this->synchFlags);
	}

	void Sharer::setPose3D(jderobot::Pose3DDataPtr p)
	{
		pthread_mutex_lock(&this->synchPose3D);
		this->pose3d = p;
		pthread_mutex_unlock(&this->synchPose3D);
	}

	void Sharer::setImage(cv::Mat image)
	{
		pthread_mutex_lock(&this->synchCamera);
		this->RGBimage = image;
		pthread_mutex_unlock(&this->synchCamera);
	}

	void Sharer::setTranslationStep(double step)
	{
		pthread_mutex_lock(&this->synch);
		this->trlnStp = step;
		pthread_mutex_unlock(&this->synch);
	}

	void Sharer::setRotationStep(double step)
	{
		pthread_mutex_lock(&this->synch);
		this->rtnStp = step;
		pthread_mutex_unlock(&this->synch);
	}

	void Sharer::setSpeedX(double spx)
	{
		pthread_mutex_lock(&this->synch);
		this->speed_x = spx;
		pthread_mutex_unlock(&this->synch);
	}

	void Sharer::setSpeedY(double spy)
	{
		pthread_mutex_lock(&this->synch);
		this->speed_y = spy;
		pthread_mutex_unlock(&this->synch);
	}

	bool Sharer::getGuiVisible()
	{
		pthread_mutex_lock(&this->synchFlags);
		bool status = this->guiVisible;
		pthread_mutex_unlock(&this->synchFlags);

		return status;
	}

	bool Sharer::getControlActive()
	{
		pthread_mutex_lock(&this->synchFlags);
		bool status = this->controlActive;
		pthread_mutex_unlock(&this->synchFlags);

		return status;
	}

	jderobot::Pose3DDataPtr Sharer::getPose3D()
	{
		pthread_mutex_lock(&this->synchPose3D);
		jderobot::Pose3DDataPtr retVal = this->pose3d;
		pthread_mutex_unlock(&this->synchPose3D);

		return retVal;
	}

	cv::Mat Sharer::getImage()
	{
		pthread_mutex_lock(&this->synchCamera);
		cv::Mat retVal = this->RGBimage;
		pthread_mutex_unlock(&this->synchCamera);

		return retVal;
	}

	double Sharer::getTranslationStep()
	{
		pthread_mutex_lock(&this->synch);
		double retVal = this->trlnStp;
		pthread_mutex_unlock(&this->synch);

		return retVal;
	}

	double Sharer::getRotationStep()
	{
		pthread_mutex_lock(&this->synch);
		double retVal = this->rtnStp;
		pthread_mutex_unlock(&this->synch);

		return retVal;
	}

	double Sharer::getSpeedX()
	{
		pthread_mutex_lock(&this->synch);
		double retVal = this->speed_x;
		pthread_mutex_unlock(&this->synch);

		return retVal;
	}

	double Sharer::getSpeedY()
	{
		pthread_mutex_lock(&this->synch);
		double retVal = this->speed_y;
		pthread_mutex_unlock(&this->synch);

		return retVal;
	}

	void Sharer::changePose3dTranslation(double sX, double sY, double sZ)
	{

		pthread_mutex_lock(&this->synch);
		double step = this->trlnStp;
		pthread_mutex_unlock(&this->synch);

		// Obtain the rotation matrix (R) with the quaternion of the current Pose3D.
		pthread_mutex_lock(&this->synchPose3D);
		jderobot::math::Quaterniond q(this->pose3d->q0, this->pose3d->q1, this->pose3d->q2, this->pose3d->q3);
		pthread_mutex_unlock(&this->synchPose3D);
		Eigen::Matrix3d R(q.toRotationMatrix());

		// Put in vector the translation relative.
		Eigen::Vector3d vRel((sX * step), (sY * step), (sZ * step));

		// Apply the rotation at vector relative to obtain the translation absolute.
		Eigen::Vector3d vAbs = R * vRel;

		pthread_mutex_lock(&this->synchPose3D);
		this->pose3d->x += vAbs[0];
		this->pose3d->y += vAbs[1];
		this->pose3d->z += vAbs[2];
		pthread_mutex_unlock(&this->synchPose3D);
	}

	void Sharer::changePose3dTranslationSpeed()
	{

		pthread_mutex_lock(&this->synch);
		double step = this->trlnStp;
		pthread_mutex_unlock(&this->synch);

		// Obtain the rotation matrix (R) with the quaternion of the current Pose3D.
		pthread_mutex_lock(&this->synchPose3D);
		jderobot::math::Quaterniond q(this->pose3d->q0, this->pose3d->q1, this->pose3d->q2, this->pose3d->q3);
		pthread_mutex_unlock(&this->synchPose3D);
		Eigen::Matrix3d R(q.toRotationMatrix());

		// Put in vector the translation relative.
		Eigen::Vector3d vRel((1. * speed_x), (1. * speed_y), (0. * step));

		// Apply the rotation at vector relative to obtain the translation absolute.
		Eigen::Vector3d vAbs = R * vRel;

		pthread_mutex_lock(&this->synchPose3D);
		this->pose3d->x += vAbs[0];
		this->pose3d->y += vAbs[1];
		this->pose3d->z += vAbs[2];
		pthread_mutex_unlock(&this->synchPose3D);
	}

	void Sharer::changePose3dRotation(double sY, double sP, double sR)
	{
		pthread_mutex_lock(&this->synch);
		double step = this->rtnStp;
		pthread_mutex_unlock(&this->synch);

		pthread_mutex_lock(&this->synchPose3D);
		jderobot::math::Quaterniond preQ(this->pose3d->q0, this->pose3d->q1, this->pose3d->q2, this->pose3d->q3);
		pthread_mutex_unlock(&this->synchPose3D);

		// Obtain the equivalent rotation quaternion.
		jderobot::math::Quaterniond q1(Eigen::AngleAxisd(sY*step, preQ*Eigen::Vector3d::UnitZ()));
		jderobot::math::Quaterniond q2(Eigen::AngleAxisd(sP*step, preQ*Eigen::Vector3d::UnitY()));
		jderobot::math::Quaterniond q3(Eigen::AngleAxisd(sR*step, preQ*Eigen::Vector3d::UnitX()));
		jderobot::math::Quaterniond curQ = q1 * q3 * q2 * preQ;
		curQ.normalize();

		pthread_mutex_lock(&this->synchPose3D);
		this->pose3d->q0 = curQ.w();
		this->pose3d->q1 = curQ.x();
		this->pose3d->q2 = curQ.y();
		this->pose3d->q3 = curQ.z();
		pthread_mutex_unlock(&this->synchPose3D);
	}

	void Sharer::restartPose3D()
	{
		pthread_mutex_lock(&this->synchPose3D);
		this->pose3d->x =  0;
		this->pose3d->y =  0;
		this->pose3d->z =  0;
		this->pose3d->h =  1;
		this->pose3d->q0 =  1;
		this->pose3d->q1 =  0;
		this->pose3d->q2 =  0;
		this->pose3d->q3 =  0;
		pthread_mutex_unlock(&this->synchPose3D);
	}

} /* namespace navigatorCamera */
