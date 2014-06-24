/*
 * poolWriteImages.cpp
 *
 *  Created on: 03/05/2013
 *      Author: frivas
 */

#include "poolWriteImages.h"
#include <libgen.h>

namespace recorder{
poolWriteImages::poolWriteImages(jderobot::CameraPrx prx, int freq, int poolSize, int cameraID,  std::string imageFormat, std::vector<int> compression_params, MODE mode, int bufferSeconds, std::string videoMode) {
	// TODO Auto-generated constructor stub
	pthread_mutex_init(&(this->mutex), NULL);
	pthread_mutex_init(&(this->mModeMutex), NULL);

	this->poolSize=poolSize;
	this->compression_params=compression_params;
	this->cameraID=cameraID;
	this->active=true;
	this->imageFormat=imageFormat;
	this->prx=prx;
	this->freq=freq;

	mBufferSeconds = bufferSeconds;
	mMode = mode;
	mBuffer = NULL;
	mLastSecondsLog = 5;
	mNameLog = "alarm1";
	mVideoMode = videoMode;

	if (mMode == SAVE_BUFFER)
	{
		jderobot::Logger::getInstance()->info("Recorder run as buffer mode, with a buffer = " + boost::lexical_cast<std::string>(mBufferSeconds) + " seconds.");
		mBuffer = new RingBuffer(mBufferSeconds*1000);
	}

	mCamType = this->prx->getImageData()->description->format;

	std::stringstream filePath;
	filePath << "data/images/camera" << this->cameraID << "/cameraData.jde";
	this->cycle = 1000.0/freq;
	this->outfile.open(filePath.str().c_str());
	gettimeofday(&lastTime,NULL);
}

poolWriteImages::~poolWriteImages() {
	this->outfile.close();
	if (mBuffer)
		delete(mBuffer);
}

bool poolWriteImages::getActive(){
	return this->active;
}

bool poolWriteImages::startCustomVideo(std::string path, std::string name, int seconds)
{
	mNamePathVideo = path;
	return startCustomLog(name, seconds);
}


bool poolWriteImages::startCustomLog (std::string name, int seconds)
{
	bool ret;

	pthread_mutex_lock(&(this->mModeMutex));

	if (mMode != SAVE_BUFFER)
	{
		jderobot::Logger::getInstance()->warning("Can't handle recording '" + name + "' because there is a recording active!" );
		ret = false;
	}
	else
	{
		mNameLog = name;
		mLastSecondsLog = seconds;
		mMode = WRITE_BUFFER;

		ret = true;
	}
	pthread_mutex_unlock(&(this->mModeMutex));

	return ret;
}

void poolWriteImages::consumer_thread(){
//	while(this->active){
		//std::cout << "consumidor entro" << std::endl;

		pthread_mutex_lock(&(this->mutex));
		if (this->images.size()>0){
			//std::cout << " camara: " << cameraID <<  this->images.size()  << std::endl;
			cv::Mat img2Save;

			this->images.begin()->copyTo(img2Save);
			this->images.erase(this->images.begin());

			long long int relative;
			relative=*(this->its.begin());
			this->its.erase(this->its.begin());
			pthread_mutex_unlock(&(this->mutex));

			std::stringstream buff;//create a stringstream
			buff << "data/images/camera" << cameraID << "/" << relative << "." << imageFormat;


			std::stringstream dataPath;
			dataPath << "data-" << mNameLog << "/images/camera" << this->cameraID << "/";

			MODE currentMode;
			pthread_mutex_lock(&(this->mModeMutex));
			currentMode = mMode;
			pthread_mutex_unlock(&(this->mModeMutex));

			// Normal mode
			if (currentMode == WRITE_FRAME)
			{

				cv::imwrite(buff.str(), img2Save,this->compression_params);
				this->outfile << relative<< std::endl;

			}
			else
			{
				// Save buffer in memory mode == SAVE_BUFFER
				cv::Mat saveImage;

				if (mVideoMode.compare("Video")==0)
				{
					if (mCamType.compare("RGB8")==0)
					{
						saveImage.create(img2Save.size(), CV_8UC3);
						cv::cvtColor(img2Save,saveImage,CV_BGR2RGB);
					}
					else if (mCamType.compare("DEPTH8_16")==0)
					{
						saveImage.create(img2Save.size(), CV_8UC3);
						std::vector<cv::Mat> layers;
						cv::split(img2Save, layers);
						cv::cvtColor(layers[0],saveImage,CV_GRAY2RGB);

					}
					else
					{
						jderobot::Logger::getInstance()->warning("mCamType not recognized " + mCamType);
					}
				}
				else if (mVideoMode.compare("Log")==0)
				{
					saveImage.create(img2Save.size(), img2Save.type());
					img2Save.copyTo(saveImage);
				}


				RingBuffer::RingNode node;
				node.cameraId = cameraID;
				node.relativeTime = relative;

				saveImage.copyTo(node.frame);
				mBuffer->addNode(node);

				if (currentMode == WRITE_BUFFER)
				{

					//Create dir
					boost::filesystem::path dir(dataPath.str());
					boost::filesystem::create_directories(dir);

					std::stringstream filePath;
					filePath << "data-" << mNameLog << "/images/camera" << this->cameraID << "/cameraData.jde";
					this->logfile.open(filePath.str().c_str());


					jderobot::Logger::getInstance()->info("Init recording log: " + mNameLog + " (camera" + boost::lexical_cast<std::string>(this->cameraID) +  " ) with "
							+ boost::lexical_cast<std::string>(mBufferSeconds)
							+ " buffer seconds and " + boost::lexical_cast<std::string>(mLastSecondsLog) + " at the end!" );

					mBuffer->write(mNameLog, this->compression_params);

					pthread_mutex_lock(&(this->mModeMutex));
					mMode = WRITE_END_LOG;
					pthread_mutex_unlock(&(this->mModeMutex));

					mFinalInit = boost::posix_time::second_clock::local_time();

				}
				// Save the final seconds of recording and save 'data' file
				else if (currentMode == WRITE_END_LOG)
				{

					mFinalEnd = boost::posix_time::second_clock::local_time();
					boost::posix_time::time_duration total = mFinalEnd - mFinalInit;

					if (total.seconds() > mLastSecondsLog)
					{
						std::vector<int> res;

						boost::filesystem::directory_iterator end_itr;
						for ( boost::filesystem::directory_iterator itr( dataPath.str() ); itr != end_itr; ++itr )
						{
							if ( itr->path().generic_string().find("png") == std::string::npos )
								continue;

							unsigned begin = itr->path().generic_string().find_last_of("/") + 1;
							unsigned end = itr->path().generic_string().find_last_of(".");
							if (begin == std::string::npos || end == std::string::npos)
							{
								jderobot::Logger::getInstance()->warning("Error while parsed file " + itr->path().generic_string());
								continue;
							}

							res.push_back( atoi(itr->path().generic_string().substr(begin, end-begin).c_str()) );
						}

						std::sort(res.begin(),res.end());
						for (std::vector<int>::iterator it = res.begin(); it < res.end(); it++)
						{
							this->logfile << *it << std::endl;
						}

						this->logfile.close();
						jderobot::Logger::getInstance()->info("End recording log: " + mNameLog );

						// Save the video
						if (mVideoMode.compare("Video")==0)
						{
							std::stringstream basePath, fileData, fileImage, command, command_video, fileVideo, tmpFileVideo;
							basePath << "data-" << mNameLog << "/images/camera" << this->cameraID << "/";
							fileData << basePath.str() << "cameraData.jde";
							fileImage << basePath.str() << "list.txt";

							command << "cat " << fileData.str() << " | sed -e 's/$/.png/g' > " << fileImage.str();

							system(command.str().c_str());

							fileVideo << mNamePathVideo << "/" << mNameLog << "-" << this->cameraID << "-" << mCamType << ".avi";
							tmpFileVideo << mNamePathVideo << "/" << mNameLog << "-" << this->cameraID << "-" << mCamType << ".mp4";

							command_video << "cd " << basePath.str() << ";" ;
							command_video << "mencoder mf://@list.txt -mf w=320:h=240:fps=10:type=png -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o " << tmpFileVideo.str() << ";" ;
							command_video << "ffmpeg -y -i " << tmpFileVideo.str() << " -vcodec libx264 " << fileVideo.str() << ";" ;
							command_video << "rm -f " << tmpFileVideo.str() << ";" ;
							command_video << "cd -; rm -rf " << "data-" << mNameLog;

							std::cout << command_video.str() << std::endl;

							system(command_video.str().c_str());
						}

						pthread_mutex_lock(&(this->mModeMutex));
						mMode = SAVE_BUFFER;
						pthread_mutex_unlock(&(this->mModeMutex));
					}
					else
					{
						std::stringstream path;
						path << "data-" << mNameLog << "/images/camera" << cameraID << "/" << relative << "." << imageFormat;
						cv::imwrite(path.str(), saveImage ,this->compression_params);
					}
				}
			}

		}
		else
			pthread_mutex_unlock(&(this->mutex));
		usleep(1000);
		//std::cout << "consumidor salgo" << std::endl;

//	}
}

void poolWriteImages::producer_thread( struct timeval inicio){
//(cv::Mat image, long long int it){
	//std::cout << "productor entro" << std::endl;
//	while(this->active){
		jderobot::ImageDataPtr imageData=this->prx->getImageData();
		colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(imageData->description->format);
		cv::Mat image;
		image.create(cv::Size(imageData->description->width, imageData->description->height), CV_8UC3);
		memcpy((unsigned char *) image.data ,&(imageData->pixelData[0]), image.cols*image.rows * 3);
		struct timeval now;
		gettimeofday(&now,NULL);
		long long int relative;
		relative=((now.tv_sec*1000000+now.tv_usec) - (inicio.tv_sec*1000000+inicio.tv_usec))/1000;
		pthread_mutex_lock(&(this->mutex));
		while (this->images.size() > this->poolSize){
			pthread_mutex_unlock(&(this->mutex));
			usleep(100);
			pthread_mutex_lock(&(this->mutex));
		}
		this->images.push_back(image);
		this->its.push_back(relative);
		pthread_mutex_unlock(&(this->mutex));
		gettimeofday(&now,NULL);

		long long int totalNow=now.tv_sec*1000000+now.tv_usec;
		long long int totalLast=lastTime.tv_sec*1000000+lastTime.tv_usec;

		float sleepTime =this->cycle - (totalNow-totalLast)/1000.;

		//std::cout << "productor: " << this->cameraID << ", sleep: " << sleepTime << std::endl;
		if(sleepTime < 0 )
			sleepTime = 0;
		usleep(sleepTime*1000);
		gettimeofday(&lastTime,NULL);
		//std::cout << "productor salgo" << std::endl;
//	}
}


} //namespace

