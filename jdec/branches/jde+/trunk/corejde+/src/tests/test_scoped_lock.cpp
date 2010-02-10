/*
  FIXME find best test
*/
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "../critical_section.h"

using namespace std;

mutex global_mutex;
int x = 0;

void* thread_f(void*){
  int i = 0;

  while(1){
    cout << "\tWaiting for mutex on thread_f(" << i <<")\n";
    {
      scoped_lock l(global_mutex);
      cout << "\tMutex adquired on thread_f(" << i <<")\n";
      for (; x < 10; x++)
	cout << "\tx=" << x << "\n";
    }
    cout << "\tMutex released on thread_f(" << i++ <<")\n";
    usleep(5000000);
  }
}

int main(){
  pthread_t t;
  int i = 0;

  pthread_create(&t,NULL,thread_f,NULL);

  while(1){
    cout << "Waiting for mutex on main(" << i <<")\n";
    {
      scoped_lock l(global_mutex);
      cout << "Mutex adquired on main(" << i <<")\n";
      for (; x > 0; x--)
	cout << "x=" << x << "\n";
    }
    cout << "Mutex released on main(" << i++ <<")\n";
    usleep(1000000);
  }
}
