#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "../critical_section.h"

using namespace std;

read_write_mutex global_rwmutex;
int x = 0;

void* thread_writer(void*){
  usleep(500000);
  try{
    cout << "Thread-writer(" << pthread_self() << ") waiting w-mutex\n";
    global_rwmutex.write_lock();
    cout << "Thread-writer(" << pthread_self() << ") w-mutex adquired\n";
    x++;
    global_rwmutex.write_unlock();
    cout << "Thread-writer(" << pthread_self() << ") w-mutex released\n";
  }catch(exception& e){
    cout << "Thread-writer(" << pthread_self() << "): " << e.what() << "\n";
  }catch(...){
    cout << "Thread-writer(" << pthread_self() << "): exception\n";
  }
}

void* thread_reader(void*){
  try{
    cout << "\tThread-reader(" << pthread_self() << ") waiting r-mutex\n";
    global_rwmutex.read_lock();
    cout << "\tThread-reader(" << pthread_self() << ") r-mutex adquired\n";
    cout << "\tThread-reader(" << pthread_self() << "): x=" << x << "\n";
    global_rwmutex.read_unlock();
    cout << "\tThread-reader(" << pthread_self() << ") r-mutex released\n";
  }catch(exception& e){
    cout << "\tThread-reader(" << pthread_self() << "): " << e.what() << "\n";
  }catch(...){
    cout << "Thread-reader(" << pthread_self() << "): exception\n";
  }
}

int main(){
  pthread_t t[10];
  int i = 0;
  int nthreads = 0;

  for (i = 0; i < 2; i++){/*2 escritores*/
    nthreads++;
    pthread_create(&(t[i]),NULL,thread_writer,NULL);
  }
  

  for (i = 2; i < 5; i++){/*3 lectores*/
    nthreads++;
    pthread_create(&(t[i]),NULL,thread_reader,NULL);
    usleep(300000);
  }
//   usleep(2000000);
//   for (i = 5; i < 8; i++)/*3 escritores*/
//     pthread_create(&(t[i]),NULL,thread_writer,NULL);

//   for (i = 8; i < 9; i++)/*2 lectores*/
//     pthread_create(&(t[i]),NULL,thread_reader,NULL);

  /*wait threads*/
  for (i = 0; i < nthreads; i++)
    pthread_join(t[i],NULL);
}
