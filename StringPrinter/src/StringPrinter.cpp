#include <vector>
#include <signal.h>
#include <condition_variable>
#include <memory>
#include <thread>
#include "PrinterThread.hpp"

namespace {
  // This flag is set by our SIGINT signal handler in order to cause the main
  // program's polling loop to exit and let the program clean up and
  // terminate.
  bool terminate = false;

  void onSigInt(int) {
    terminate = true;
    std::cout << "Received signInt" << std::endl;
  }

  //This is the function called from the main program in order 
  //to read the passed string, while the SIGINT is set to control
  //when the program should terminate
  int InterruptibleMain(int argc, char *argv[]) {
    if (argc < 4) {
      printf("Invalid number of arguements, Ex. <Exec> <string> <number of characters> <number of threads>\n"); 
      return EXIT_FAILURE;
    }
    std::string str = argv[1];
    uint64_t no_of_chars_to_print = atoll(argv[2]);
    uint32_t no_of_threads = atoi(argv[3]);
    uint64_t str_size = str.size();
    uint64_t current_index = 0;
    uint32_t turn = 0, current_turn = 0;
    std::shared_ptr<std::mutex> turn_mutex
            = std::make_shared<std::mutex>();
    std::shared_ptr<std::condition_variable> cond_thread_turn 
            = std::make_shared<std::condition_variable>();
    std::shared_ptr<std::condition_variable> main_thread_turn 
            = std::make_shared<std::condition_variable>();
    std::vector<std::unique_ptr<StringPrinter::PrinterThread>> thread_ptr_vec;
    for(uint32_t t_i = 0; t_i < no_of_threads; t_i++) {
      thread_ptr_vec.push_back(
        std::make_unique<StringPrinter::PrinterThread>(
          str.c_str(),
          str_size,
          no_of_chars_to_print,
          current_index,
          t_i + 1,
          current_turn,
          turn_mutex,
          cond_thread_turn,
          main_thread_turn
        )
      );
      thread_ptr_vec[t_i]->run();
    }
    while(!terminate) {
      current_turn = (turn % no_of_threads)  + 1;
      turn = current_turn;
      cond_thread_turn->notify_all();
      //wait until the thread prints the characters
      // and the turn to 0.
      std::unique_lock<std::mutex> lock(*turn_mutex.get());
      main_thread_turn->wait(lock, [&current_turn]() {
        return current_turn == 0;
      });
      current_index += no_of_chars_to_print;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    int test_count = 0;
    for(const auto &thread_ptr : thread_ptr_vec) {
      thread_ptr->stop_thread();
    }
    printf("Program exiting.\n");
    return EXIT_SUCCESS;
  }
}

int main(int argc, char *argv[]) {
   // Catch SIGINT (interrupt signal, typically sent when the user presses
  // <Ctrl>+<C> on the terminal) during program execution.
  const auto previousInterruptHandler = signal(SIGINT, onSigInt);
  const auto returnValue =  InterruptibleMain(argc, argv);
  (void)signal(SIGINT, previousInterruptHandler);
  return returnValue;
} 
