#include "rectangle.h"

void Rectangle::set_width(double w) { width = w; }
void Rectangle::set_height(double h) { height = h; }

double Rectangle::get_width() const { return this->width; }
double Rectangle::get_height() const { return height; }

double Rectangle::area() const {
  // return this->_area = this->width * this->height;
  return /* this-> */ _area = /* this-> */ width * /* this-> */ height;
}

double Rectangle::perimeter() const {
  return /* this-> */ _perimeter =
             (/* this-> */ width + /* this-> */ height) * 2;
}