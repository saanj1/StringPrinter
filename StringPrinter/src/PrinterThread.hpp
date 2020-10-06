/**
 * @file PrinterThread.hpp
 *
 * This module contains the implementation
 * for the thread to read the specified number 
 * of characters from the char arr
 */ 

#include <memory>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <Utils/Thread.hpp>

namespace StringPrinter {

  class PrinterThread : public Utils::Thread {
  public:
    //Life cycle management
    ~PrinterThread() = default;
    PrinterThread(const PrinterThread&) = delete;
    PrinterThread(PrinterThread&&) = delete;
    PrinterThread& operator=(const PrinterThread&) = delete;
    PrinterThread& operator=(PrinterThread&&) = delete;
  
  public:
    /**
     * This is the constructor
     * 
     * @param[in] t_str_
     *  pointer to the string that should be printed
     * 
     * @param[in]
     *  number of characters this thread should print
     * 
     * #
     */
    PrinterThread(
    const char* t_str, 
    size_t t_size,
    uint64_t t_chars_to_print,
    uint64_t &t_index,
    uint64_t t_thread_id,
    uint32_t &t_turn,
    std::shared_ptr<std::mutex> t_mut,
    std::shared_ptr<std::condition_variable> t_cond_turn,
    std::shared_ptr<std::condition_variable> t_main_thread_turn_
    ) 
    : data_str_(t_str),
      str_size_(t_size),
      no_chars_to_process_(t_chars_to_print),
      current_index_(t_index),
      thread_id_(t_thread_id),
      thread_turn_(t_turn),
      keep_me_running_(true),
      thread_turn_mutex(t_mut),
      cond_thread_turn_(t_cond_turn),
      main_thread_cond_var_(t_main_thread_turn_)
    {}
  protected:

    /**
     * This is the main thread code
     */
    virtual void thread_main() {
    while(keep_me_running_) {
      std::unique_lock<std::mutex> lock(*thread_turn_mutex.get());
      cond_thread_turn_->wait(lock, 
        [&thread_turn_ = thread_turn_, 
         &thread_id_ = thread_id_
        ]() {
        return thread_turn_ == thread_id_;
         }
      );
      std::cout << "Thread" << thread_id_ << ": ";
      for(size_t i = 0; i < no_chars_to_process_; ++i) {
      std::cout << data_str_[(current_index_ + i) % str_size_];
      }
      std::cout << std::endl;
      thread_turn_ = 0;
      main_thread_cond_var_->notify_one();
    }
    }
    public:
    virtual void stop_thread() {
    if (keep_me_running_) {
      keep_me_running_ = false;
    }
    }

  private:
    /**
     * This contains address to the actual string 
     * that needs to be printed on the stdout.
     */
    const char* data_str_;

    /**
     * This holds the size of the data string
     */
    size_t str_size_;
    
    /**
     * This contains the number of character, this thread
     * should print on the stdout
     */
    uint64_t no_chars_to_process_;

    /**
     * This holds the reference to the global index value 
     * used and updated by all the threads to print the 
     * given string
     */
    uint64_t &current_index_;

    /**
     * This holds the id assigned by the main thread
     */
    uint64_t thread_id_;

     /**
     * This contains the id of the thread that read 
     * the string and print the data
     */
    uint32_t &thread_turn_;

    /**
     * 
     */
    bool keep_me_running_;

    /**
     * This holds the mutex that is shared between all
     * printer thread
     */
    std::shared_ptr<std::mutex> thread_turn_mutex;

    /**
     * This is the condition variable on which 
     * thread waits for it's turn to print the 
     * data
     */
    std::shared_ptr<std::condition_variable> cond_thread_turn_;

    /**
     * This is the condition variable on which 
     * main thread waits for the client thread to 
     * terminate
     */
    std::shared_ptr<std::condition_variable> main_thread_cond_var_;
  };
}
