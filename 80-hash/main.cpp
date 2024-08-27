#include <functional>
#include <iostream>
#include <string>
using namespace std;

int main() {
  char nts1[] = "Test";
  char nts2[] = "Test";
  string str1(nts1);
  string str2(nts2);

  hash<char *> ptr_hash;
  hash<string> str_hash;

  cout << "same hashes:\n" << boolalpha;
  cout << ptr_hash(nts1) << endl;
  cout << ptr_hash(nts2) << endl;
  cout << str_hash(str1) << endl;
  cout << str_hash(str2) << endl;

  cout << "nts1 and nts2: " << (ptr_hash(nts1) == ptr_hash(nts2)) << '\n';
  cout << "str1 and str2: " << (str_hash(str1) == str_hash(str2)) << '\n';
  str2 = "Test2";
  cout << str_hash(str1) << endl;
  cout << str_hash(str2) << endl;
  cout << "str1 and str2: " << (str_hash(str1) == str_hash(str2)) << '\n';

  return 0;
}