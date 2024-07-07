#pragma once

class Rectangle {
  double width;
  double height;
  mutable double _area;
  mutable double _perimeter;

public:
  void set_width(double w) { width = w; }
  void set_height(double h) { height = h; }
  double get_width() const { return width; }
  double get_height() const { return height; };
  double area() const { return _area = height * width; };
  double perimeter() const { return _perimeter = 2 * (height + width); }
};
