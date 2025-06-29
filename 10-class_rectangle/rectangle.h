#pragma once

class Rectangle {
  double width;
  double height;
  mutable double _area;
  mutable double _perimeter;

public:
  void set_width(double w);
  void set_height(double h);
  double get_width() const;
  double get_height() const;
  double area() const;
  double perimeter() const;
};
