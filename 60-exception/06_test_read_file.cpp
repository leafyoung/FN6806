#include "06_test_read_file.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

void test_read_file() {
  std::string filename = "60-exception/example.txt";
  std::ifstream file(filename.c_str());

  if (!file.is_open()) {
    std::cout << "File does not exist after 1st attempt!\n";

    std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for 3 second

    file.open(filename);
    if (file.is_open()) {
      std::cout << "File successfully read!\n";
    } else {
      std::cout << "File does not exist after 2nd attempt!\n";
    }
  } else {
    std::cout << "File exists!\n";
  }
}

void test_read_file(int max_retries) {
  std::string filename = "60-exception/example.txt";
  std::ifstream file(filename.c_str());
  int tries = 0;
  while (tries < max_retries) {
    file.open(filename);
    tries++;
    if (file.is_open()) {
      std::cout << "File successfully read!\n";
      break;
    } else {
      std::cout << "File does not exist after " << tries << " attempt!\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for 3 second
  }
  if (tries < max_retries) {
    std::cout << "processing data\n";
  }
}