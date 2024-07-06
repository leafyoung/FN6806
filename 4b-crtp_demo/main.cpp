// https://replit.com/@YeKunlun/4b-crtpdemo

#include <iostream>
#include <memory>
#include <string>
#include <vector>

void func(int *ptr) {
  if (ptr != nullptr)
    std::cout << *ptr << '\n';
  else
    std::cout << "null" << '\n';
}

class controlbase {
public:
  virtual void draw() = 0;
  virtual ~controlbase() {}
};

// Inheritance is to make control derived classes in container
template <class T> class control : public controlbase {
  T *derived() { return static_cast<T *>(this); }

protected:
  std::string _name;

public:
  void draw() {
    derived()->erase_background();
    derived()->paint();
  }
  // constructor is not inherited, so we will create them in the derived classes
  control(std::string name) : _name(name) {}
};

class button : public control<button> {
public:
  void erase_background() {
    std::cout << "erasing " << _name << " background... " << '\n';
  }
  void paint() { std::cout << "painting " << _name << " ... " << '\n'; }
  button(std::string name = "button") : control<button>(name) {}
};

class checkbox : public control<checkbox> {
public:
  void erase_background() {
    std::cout << "erasing " << _name << " background... " << '\n';
  }
  void paint() { std::cout << "painting " << _name << " ... " << '\n'; }
  checkbox(std::string name = "checkbox") : control<checkbox>(name) {}
};

template <class T> void draw_control(control<T> &c) { c.draw(); }

void draw_controls(std::vector<std::unique_ptr<controlbase>> &v) {
  for (auto &c : v) {
    c->draw();
  }
}

int main() {

  std::cout << "Hello World!\n";

  std::unique_ptr<int> pi;
  func(pi.get()); // prints null

  pi = std::make_unique<int>(42);
  func(pi.get()); // prints 42

  button b;
  draw_control(b);
  checkbox c;
  draw_control(c);

  std::vector<std::unique_ptr<controlbase>> v;
  v.emplace_back(std::make_unique<button>("button1"));
  v.emplace_back(std::make_unique<checkbox>("checkbox1"));
  v.emplace_back(std::make_unique<button>("button2"));

  draw_controls(v);
}
