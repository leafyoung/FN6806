#include <iostream>
#include <thread>
using std::cout;
using std::thread;

class ForThread {
  int input, output;

public:
  ForThread(int input) : input(input), output(0) {}
  int add(int add) { return output = input + add; }
  int get_output() { return output; }
};

void test_launch() {
  cout << "test_launch:\n";
  cout << "ForThread\n";
  ForThread for_thread(3);
  thread t2(&ForThread::add, &for_thread, 4);
  t2.join();
  cout << for_thread.get_output() << "\n\n";
  /*
  {
  cout << "ForThread" << "\n";
  ForThread for_thread(3);
  // It will run a copy of for_thread, can not update for_thread
  thread t(&for_thread, 4);
  t.join();
  cout << "g: " << for_thread.get_output() << "\n";
  }
  */
}