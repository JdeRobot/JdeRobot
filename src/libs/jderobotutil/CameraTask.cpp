/*
 * CameraTask.cpp
 *
 *  Created on: 29/6/2015
 *      Author: frivas
 */

#include "CameraTask.h"
#include <openssl/md5.h>
#include <zlib.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <logger/Logger.h>
#include <iomanip>




namespace jderobot {

CameraTask::CameraTask(const jderobot::Camera* camera, int fps):mycamera(camera),_done(false) {
  // TODO Auto-generated constructor stub
  this->fps=fps;
}

CameraTask::~CameraTask() {
  // TODO Auto-generated destructor stub
}


void CameraTask::pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format){
  IceUtil::Mutex::Lock sync(requestsMutex);
  requests.push_back(std::make_pair(cb, format));
}

void CameraTask::print_md5_sum(unsigned char* md) {
  int i;
  for(i=0; i <MD5_DIGEST_LENGTH; i++) {
    printf("%02x",md[i]);
  }
}

void CameraTask::run(){


   reply=jderobot::ImageDataPtr(new jderobot::ImageData());
   reply->description = jderobot::ImageDescriptionPtr(new jderobot::ImageDescription());


  float cycle; // duración del ciclo

  cycle=(float)(1/(float)fps)*1000000;

  IceUtil::Time lastIT=IceUtil::Time::now();
  while(!(_done)){
    IceUtil::Time t = IceUtil::Time::now();
    reply->timeStamp.seconds = (long)t.toSeconds();
    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;


    cv::Mat image;
    createCustomImage(image);
    if (image.empty())
      continue;



    {//critical region start
      IceUtil::Mutex::Lock sync(requestsMutex);
      while(!requests.empty()){
        jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front().first;
        std::string format = requests.front().second;
        requests.pop_front();
        reply->description->format=format;
        sendImage(cb, format, image);
      }

    }//critical region end


    int process = IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds();

    if (process > (int)cycle ){
      jderobot::Logger::getInstance()->warning("-------- Camera timeout-" );
    }
    else{
      int delay = (int)cycle - process;
      if (delay <1 || delay > (int)cycle)
        delay = 1;

      usleep(delay);
    }

    lastIT=IceUtil::Time::now();
  }
}

void CameraTask::sendImage(jderobot::AMD_ImageProvider_getImageDataPtr cb, std::string& format, const cv::Mat& image){
  reply->description->format=format;
  reply->description->height=image.size().height;
  reply->description->width=image.size().width;

  if (format == colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name)
  {
    if (image.channels() != 3){
        jderobot::Logger::getInstance()->error("Error, image with FORMAT_RGB8_Z must have 3 channels");
        return;
    }
    unsigned long source_len = image.rows*image.cols*3;
    unsigned long compress_len = compressBound(source_len);
    reply->pixelData.resize(compress_len);
    unsigned char* compress_buf = (unsigned char *) malloc(compress_len);

    int r = compress((Bytef *) compress_buf, (uLongf *) &compress_len, (const Bytef *) &(image.data[0]), (uLong)source_len );

    if(r != Z_OK) {
      jderobot::Logger::getInstance()->error("Compression Error");
      switch(r) {
        case Z_MEM_ERROR:
          jderobot::Logger::getInstance()->error("Compression Error: Not enough memory to compress");
          break;
        case Z_BUF_ERROR:
          jderobot::Logger::getInstance()->error("Compression Error: Target buffer too small.");
          break;
        case Z_STREAM_ERROR:
          jderobot::Logger::getInstance()->error("Compression Error: Invalid compression level.");
          break;
      }
    }
    else
    {
      reply->description->format = colorspaces::ImageRGB8::FORMAT_RGB8_Z.get()->name;
      memcpy(&(reply->pixelData[0]),  &(compress_buf[0]), compress_len);

      // md5sum
      unsigned char *md5hash;

      md5hash = MD5((const unsigned char*) &(compress_buf[0]), compress_len, NULL);

      std::stringstream buf;
      for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        buf << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(static_cast<unsigned char>(md5hash[i]));
      reply->description->md5sum = buf.str();

    }

    if (compress_buf)
      free(compress_buf);

  }
  else if (format == colorspaces::ImageRGB8::FORMAT_RGB8.get()->name)
  {
    if (image.channels() != 3){
        jderobot::Logger::getInstance()->error("Error, image with FORMAT_RGB8_Z must have 3 channels");
        return;
    }

    reply->description->format = colorspaces::ImageRGB8::FORMAT_RGB8.get()->name;
    reply->pixelData.resize(image.rows*image.cols * 3);
    memcpy(&(reply->pixelData[0]),(unsigned char *) image.data, image.rows*image.cols * 3);

    // md5sum
    unsigned char *md5hash;
    md5hash = MD5((const unsigned char*) (unsigned char *) image.data, image.rows*image.cols * 3, NULL);

    std::stringstream buf;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
      buf << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(static_cast<unsigned char>(md5hash[i]));

    reply->description->md5sum = buf.str();

  }
  else if (format == colorspaces::ImageGRAY8::FORMAT_GRAY8_Z.get()->name)
  {
    if (image.channels() != 1){
        jderobot::Logger::getInstance()->error("Error, image with FORMAT_GRAY8_Z must have 1 channels");
        return;
    }

    size_t source_len = image.rows*image.cols;
    size_t compress_len = compressBound(source_len);
    reply->pixelData.resize(compress_len);

    unsigned char* compress_buf = (unsigned char *) malloc(compress_len);

    int r = compress2((Bytef *) compress_buf, (uLongf *) &compress_len, (Bytef *) &(image.data[0]), (uLong)source_len , 9);
    if(r != Z_OK) {
      jderobot::Logger::getInstance()->error("Compression Error");
      switch(r) {
      case Z_MEM_ERROR:
        jderobot::Logger::getInstance()->error("Compression Error: Not enough memory to compress");
        break;
      case Z_BUF_ERROR:
        jderobot::Logger::getInstance()->error("Compression Error: Target buffer too small");
        break;
      case Z_STREAM_ERROR:    // Invalid compression level
        jderobot::Logger::getInstance()->error("Compression Error: Invalid compression level");
        break;
      }
    }

    reply->description->format=colorspaces::ImageGRAY8::FORMAT_GRAY8_Z.get()->name;
    reply->pixelData.resize(compress_len);
    memcpy(&(reply->pixelData[0]), (unsigned char *) compress_buf, compress_len);

    if (compress_buf)
      free(compress_buf);
  }
  else if (format == colorspaces::ImageGRAY8::FORMAT_GRAY8.get()->name)
  {
    if (image.channels() != 1){
        jderobot::Logger::getInstance()->error("Error, image with FORMAT_GRAY8_Z must have 1 channels");
        return;
    }
    reply->pixelData.resize(image.rows*image.cols);
    memcpy(&(reply->pixelData[0]),(unsigned char *) image.data,image.cols*image.rows );
  }
  else
  {
    jderobot::Logger::getInstance()->error("Format image not recognized: " + format);
  }
  cb->ice_response(reply);
}



} /* namespace jderobot */
