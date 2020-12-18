#pragma once
/**
 * @file Thread.hpp 
 * 
 * This file contains the implementation of the 
 * Thread::Thread class
 */


#define SNJ_MAKE_NONCOPYABLE(c)\
private:                       \
  c(const c&) noexcept = delete;\
  c& operator=(const c&) noexcept = delete

#define SNJ_MAKE_NONMOVABLE(c)\
private:                      \
  c(c&&) noexcept = delete;   \
  c& operator=(c&&) noexcept = delete


#include <pthread.h>
#include <signal.h>

namespace Utils {
  class Thread {
    public:
      //Life cycle management
      ~Thread() {
        if (pthread_id_ != -1){
          pthread_kill(pthread_id_, SIGUSR1);
        }
      }
      SNJ_MAKE_NONCOPYABLE(Thread);
      SNJ_MAKE_NONMOVABLE(Thread);
    public:
      /**
       * This is the constructor of the class
       */
      Thread()
        : pthread_id_(-1)
      {}
      
      /**
       * This function used to start a new thread Thread::Thread
       * 
       * @return 
       *       indicating whether thread start is successfull or not
       *       if succcessfull returns the thread id, else returns -1
       */
      int run() {
        if (pthread_id_ != -1) return -1;
        int retval;
        if((retval = 
            pthread_create(&pthread_id_, nullptr, &start_thread, this)) < 0) {
          perror("Failed to create thread:");
        } 
        return retval;
      }
    protected:
      static void* start_thread(void* t_this_ptr) {
        static_cast<Thread*>(t_this_ptr)->thread_main();
      }
      
      /**
       * This is the function where the actual thread
       * code is implemented
       */
      virtual void thread_main() = 0;
    public:
      /**
       * This is the function, which should be implemented in 
       * the child, to stop the 
       */
      virtual void stop_thread() = 0;

      /**
       * This contains the id of thread created
       */
      pthread_t pthread_id_;
  };
}
