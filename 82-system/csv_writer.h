#include <fstream>
#include <stdexcept>
#include <string>

// CSVWriter is RAII: constructor opens, destructor closes - from Lec06 Q05
class CSVWriter {
  std::ofstream file_;

public:
  explicit CSVWriter(const std::string &path) : file_(path) {
    if (!file_.is_open())
      throw std::runtime_error("CSVWriter: cannot open " + path);
  }
  // No destructor needed - std::ofstream closes automatically (RAII)

  template <typename T> void write(const T &value) { file_ << value << '\n'; }

  CSVWriter(const CSVWriter &) = delete;
  CSVWriter &operator=(const CSVWriter &) = delete;
};
