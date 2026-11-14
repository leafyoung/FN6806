// `inline`: this folder keeps two variants of the same methods (main.cpp uses
// rectangle_inside.h, whose in-class definitions are implicitly inline), so a project that
// compiles every .cpp file in the folder would link the same symbols twice. `inline` makes
// these definitions weak like the in-class ones, and every linker accepts the pair.
#include "rectangle.h"

inline void Rectangle::set_width(double w) { width = w; }
inline void Rectangle::set_height(double h) { height = h; }

inline double Rectangle::get_width() const { return this->width; }
inline double Rectangle::get_height() const { return height; }

inline double Rectangle::area() const {
  // return this->_area = this->width * this->height;
  return /* this-> */ _area = /* this-> */ width * /* this-> */ height;
}

inline double Rectangle::perimeter() const {
  return /* this-> */ _perimeter =
             (/* this-> */ width + /* this-> */ height) * 2;
}