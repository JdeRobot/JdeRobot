#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "../critical_section.h"

using namespace std;

mutex global_mutex;
condition global_cond;
int x = 0;

void* thread_f(void*){
  int i = 0;

  while(1){
    cout << "\tWaiting for mutex on thread_f(" << i <<")\n";
    {
      scoped_lock l(global_mutex);
      cout << "\tMutex adquired on thread_f(" << i <<")\n";
      cout << "\tWaiting...(" << i <<")\n";
      global_cond.wait(l);
      cout << "\tNotified(" << i <<")\n";
    }
    cout << "\tMutex released on thread_f(" << i++ <<")\n";
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
      usleep(1000000);
      cout << "Notifying(" << i <<")\n";
      global_cond.notify_one();
    }
    cout << "Mutex released on main(" << i++ <<")\n";
    usleep(1000000);
  }
}
