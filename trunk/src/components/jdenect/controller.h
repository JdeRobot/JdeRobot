#include "libfreenect.hpp"
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Mutex {
public:
	Mutex();
	void lock();
	void unlock();
private:
	pthread_mutex_t m_mutex;
};



/* thanks to Yoda---- from IRC */
class MyFreenectDevice : public Freenect::FreenectDevice {
public:
	MyFreenectDevice(freenect_context *_ctx, int _index);
	//~MyFreenectDevice(){}
	// Do not call directly even in child
	void VideoCallback(void* _rgb, uint32_t timestamp);
	// Do not call directly even in child
	void DepthCallback(void* _depth, uint32_t timestamp);
	bool getRGB(std::vector<unsigned char> &buffer);
	bool getIR(std::vector<unsigned char> &buffer);
	bool getDepth(std::vector<uint8_t> &buffer);
private:
	std::vector<unsigned char> m_buffer_depth;
	std::vector<unsigned char> m_buffer_video;
	std::vector<int> m_buffer_depth_values;
	std::vector<uint16_t> m_gamma;
	Mutex m_rgb_mutex;
	Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;
};
