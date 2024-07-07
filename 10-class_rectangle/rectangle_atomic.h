#include <atomic>

namespace RectangleAtomic {

class Rectangle {
  std::atomic<double> _width;
  std::atomic<double> _height;
  /*
  mutable double _area;
  mutable double _perimeter;
  mutable bool b_area{false};
  mutable bool b_perimeter{false};
  */
  mutable std::atomic<double> _area;
  mutable std::atomic<double> _perimeter;
  mutable std::atomic_bool b_area{false};
  mutable std::atomic_bool b_perimeter{false};

public:
  Rectangle(double w, double h)
      : _width(w), _height(h), _area(0), _perimeter(0) {}
  inline void width(double w) {
    if (_width != w) {
      _width = w;
      invalid_calc();
    }
  }
  inline void height(double h) {
    if (_height != h) {
      _height = h;
      invalid_calc();
    }
  }

  inline double width() const { return _width; }
  inline double height() const { return _height; }
  inline double area() const {
    if (b_area)
      return _area;
    b_area = true;
    return _area = _height * _width;
  }
  inline double perimeter() const {
    if (b_perimeter)
      return _perimeter;
    b_perimeter = true;
    return _perimeter = 2 * (_height + _width);
  }

private:
  inline void invalid_calc() { b_area = b_perimeter = false; }
};

} // namespace RectangleAtomic