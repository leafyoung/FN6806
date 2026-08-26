#include <iostream>

int def() {
  int a, b, c; // 3 automatic (stack) ints, no relation to def()'s call depth
               // from main yet
  std::cout << "def!\n"
            << &a << " " << &b << " " << &c
            << "\n"
            // ptrdiff: (&b - &a) is (byte offset)/sizeof(int), NOT raw bytes
            << (&b - &a) << " " << (&c - &b) << "\n";
  return 1;
}

int abc(bool call_def = false) {
  int a, b, c; // separate stack frame from def()'s a,b,c
  std::cout << "abc!\n"
            << &a << " " << &b << " " << &c << "\n"
            << (&b - &a) << " " << (&c - &b) << "\n";
  if (call_def) {
    def(); // nested call -> def()'s frame sits BELOW abc()'s frame
  }
  return 1;
}

int main() {
  int a, b, c;
  std::cout << &a << " " << &b << " " << &c << "\n"
            << (&b - &a) << " " << (&c - &b) << "\n";
  abc();     // call depth 1 from main
  def();     // call depth 1 from main (same depth as the abc() call above)
  abc(true); // call depth 1, then calls def() internally -> depth 2

  int *p =
      new int[3]{1, -1, 4}; // heap array, unrelated to the stack story above
  std::cout << p[0] << "," << p[1] << "," << p[2] << "\n";
  delete[] p;
}
