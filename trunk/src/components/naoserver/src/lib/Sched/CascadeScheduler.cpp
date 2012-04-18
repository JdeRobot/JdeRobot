#include "CascadeScheduler.h"

pthread_t CascadeScheduler::t1;
pthread_mutex_t CascadeScheduler::mutex;
bool CascadeScheduler::running;

void*
CascadeScheduler::runThread(void *data) {
	while (running) {
		pthread_mutex_lock(&mutex);

		Body::getInstance()->step();
		Head::getInstance()->step();
		Kinematics::getInstance()->step();
		Perception::getInstance()->step();
		SensorRecorderNao::getInstance()->step();

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit( NULL);
}

CascadeScheduler::CascadeScheduler() {
	running = false;

	pthread_mutex_init(&mutex, NULL);
}

CascadeScheduler::~CascadeScheduler() {
	running = false;
}

void CascadeScheduler::init() {
}

void CascadeScheduler::run() {
	if(!running) {
		cout << "Starting scheduler" << endl;
		running = true;
		pthread_create(&t1, NULL, CascadeScheduler::runThread, NULL);
	}
}

