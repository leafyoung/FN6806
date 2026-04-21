// csv_writer.h
#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

// CSVWriter is RAII: constructor opens, destructor closes automatically.
class CSVWriter {
  std::ofstream file_;

 public:
  explicit CSVWriter(const std::string& path) {
    const std::filesystem::path output_path(path);
    const std::filesystem::path parent_path = output_path.parent_path();

    if (!parent_path.empty()) {
      std::filesystem::create_directories(parent_path);
    }

    file_.open(output_path);
    if (!file_.is_open()) {
      throw std::runtime_error("CSVWriter: cannot open " + path);
    }
  }

  template <typename T>
  void write(const T& value) {
    file_ << value << '\n';
  }

  CSVWriter(const CSVWriter&) = delete;
  CSVWriter& operator=(const CSVWriter&) = delete;
};
