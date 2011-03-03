/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include "controller.h"

	Mutex::Mutex() {
		pthread_mutex_init( &m_mutex, NULL );
	}
	void Mutex::lock() {
		pthread_mutex_lock( &m_mutex );
	}
	void Mutex::unlock() {
		pthread_mutex_unlock( &m_mutex );
	}


	MyFreenectDevice::MyFreenectDevice(freenect_context *_ctx, int _index)
		: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_VIDEO_RGB_SIZE),m_buffer_video(FREENECT_VIDEO_RGB_SIZE), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false)
	{
		for( unsigned int i = 0 ; i < 2048 ; i++) {
			float v = i/2048.0;
			v = std::pow(v, 3)* 6;
			m_gamma[i] = v*6*256;
		}
	}
	//~MyFreenectDevice(){}
	// Do not call directly even in child
	void MyFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
		m_rgb_mutex.lock();
		uint8_t* rgb = static_cast<uint8_t*>(_rgb);
		std::copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
		m_new_rgb_frame = true;
		m_rgb_mutex.unlock();
	};
	// Do not call directly even in child
	void MyFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
		m_depth_mutex.lock();
		uint16_t* depth = static_cast<uint16_t*>(_depth);
		int distance;
		m_buffer_depth_values.resize(0);
		for( unsigned int i = 0 ; i < FREENECT_FRAME_PIX ; i++) {
			int pval = m_gamma[depth[i]];
			m_buffer_depth_values.push_back(depth[0]);
			int lb = pval & 0xff;
			switch (pval>>8) {
			case 0:
				m_buffer_depth[3*i+0] = 255;
				m_buffer_depth[3*i+1] = 255-lb;
				m_buffer_depth[3*i+2] = 255-lb;
				break;
			case 1:
				m_buffer_depth[3*i+0] = 255;
				m_buffer_depth[3*i+1] = lb;
				m_buffer_depth[3*i+2] = 0;
				break;
			case 2:
				m_buffer_depth[3*i+0] = 255-lb;
				m_buffer_depth[3*i+1] = 255;
				m_buffer_depth[3*i+2] = 0;
				break;
			case 3:
				m_buffer_depth[3*i+0] = 0;
				m_buffer_depth[3*i+1] = 255;
				m_buffer_depth[3*i+2] = lb;
				break;
			case 4:
				m_buffer_depth[3*i+0] = 0;
				m_buffer_depth[3*i+1] = 255-lb;
				m_buffer_depth[3*i+2] = 255;
				break;
			case 5:
				m_buffer_depth[3*i+0] = 0;
				m_buffer_depth[3*i+1] = 0;
				m_buffer_depth[3*i+2] = 255-lb;
				break;
			default:
				m_buffer_depth[3*i+0] = 0;
				m_buffer_depth[3*i+1] = 0;
				m_buffer_depth[3*i+2] = 0;
				break;
			}
			/*std::cout << "(" << (int)m_buffer_depth[3*i+0] << ","  << (int)m_buffer_depth[3*i+1] << "," << (int)m_buffer_depth[3*i+2] << std::endl;
			if (((int)m_buffer_depth[3*i+0]==255) && ( ((int)m_buffer_depth[3*i+1] >0 ) && ((int)m_buffer_depth[3*i+1] < 255)) && (((int)m_buffer_depth[3*i+2] > 0 ) && ((int)m_buffer_depth[3*i+2] < 255))){
				std::cout << "0" << std::endl;
				distance = -((int)m_buffer_depth[3*i+1] - 255);
			}
			else if (((int)m_buffer_depth[3*i+0]==255) && (((int)m_buffer_depth[3*i+1] >= 0) && ((int)m_buffer_depth[3*i+1]<255)) && ((int)m_buffer_depth[3*i+2] == 0)){
				std::cout << "1" << std::endl;
				distance = 255+ (int)m_buffer_depth[3*i+1];
			}
			else if ((((int)m_buffer_depth[3*i+0] >= 0 ) && ((int)m_buffer_depth[3*i+0] < 255))  && ((int)m_buffer_depth[3*i+1]==255) && ((int)m_buffer_depth[3*i+2] == 0)){
				std::cout << "2" << std::endl;
				distance = 2*255 - ((int)m_buffer_depth[3*i+0] - 255);
			}
			else if (((int)m_buffer_depth[3*i+0] == 0) && ((int)m_buffer_depth[3*i+1]==255) && (((int)m_buffer_depth[3*i+2] >= 0 ) && ((int)m_buffer_depth[3*i+2] < 255))){
				std::cout << "3" << std::endl;
				distance = 3*255 + (int)m_buffer_depth[3*i+2];
			}
			else if (((int)m_buffer_depth[3*i+0] == 0) && ((int)m_buffer_depth[3*i+1] >= 0 ) && ((int)m_buffer_depth[3*i+1] < 255) && ((int)m_buffer_depth[3*i+2]==255)){
				std::cout << "4" << std::endl;
				distance = 4*255 - ((int)m_buffer_depth[3*i+1] - 255);
			}
			else if (((int)m_buffer_depth[3*i+0] == 0) && ((int)m_buffer_depth[3*i+1] == 0) && ((int)m_buffer_depth[3*i+2] > 0 ) && ((int)m_buffer_depth[3*i+2] < 255)){
				std::cout << "5" << std::endl;
				distance = 5*255 - ((int)m_buffer_depth[3*i+2] - 255);
			}
			else{
				std::cout << "-1" << std::endl;
			}
			std::cout <<"distancia:" << distance << std::endl;
			}*/
			
		}
		m_new_depth_frame = true;
		m_depth_mutex.unlock();
	}
	bool MyFreenectDevice::getRGB(std::vector<unsigned char> &buffer) {
		m_rgb_mutex.lock();
		this->setVideoFormat(FREENECT_VIDEO_RGB);
		if(m_new_rgb_frame) {
			buffer.swap(m_buffer_video);
			m_new_rgb_frame = false;
			m_rgb_mutex.unlock();
			return true;
		} else {
			m_rgb_mutex.unlock();
			return false;
		}
	}

	bool MyFreenectDevice::getIR(std::vector<unsigned char> &buffer) {
		m_rgb_mutex.lock();
		this->setVideoFormat(FREENECT_VIDEO_IR_8BIT);
		if(m_new_rgb_frame) {
			buffer.swap(m_buffer_video);
			m_new_rgb_frame = false;
			m_rgb_mutex.unlock();
			return true;
		} else {
			m_rgb_mutex.unlock();
			return false;
		}
	}

	bool MyFreenectDevice::getDepth(std::vector<uint8_t> &buffer) {
		m_depth_mutex.lock();
		if(m_new_depth_frame) {
			buffer.swap(m_buffer_depth);
			m_new_depth_frame = false;
			m_depth_mutex.unlock();
			return true;
		} else {
			m_depth_mutex.unlock();
			return false;
		}
	}
