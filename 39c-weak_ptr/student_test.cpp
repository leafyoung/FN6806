#include <iostream>
#include <memory>  // for std::shared_ptr and std::weak_ptr
#include <string>

/*

This file demonstrates how to use `std::weak_ptr` to represent a non-owning relationship between two
`Person` objects.

- `Person` stores a name and a weak reference to a partner.
- `partner_up()` links two people in both directions.
- The partner references are `weak_ptr`, so they do not increase the objects' reference counts or
create a memory leak through a circular ownership relationship.
- `get_partner()` checks whether the partner still exists, then uses `lock()` to temporarily obtain
a `shared_ptr`.
- `student_test()` creates Lucy and Ricky, partners them, and prints Ricky's partner.

Expected output is approximately:

```text
Lucy created
Ricky created
Lucy is now partnered with Ricky
Ricky's partner is: Lucy
Ricky destroyed
Lucy destroyed
```

The important distinction is that `shared_ptr` owns an object, while `weak_ptr` only observes it.
`student_test()` is a helper function, not `main()`, so another program or test runner must call it.

*/

class Person {
  std::string m_name;
  std::weak_ptr<Person> m_partner;  // note: This is now a std::weak_ptr

 public:
  Person(const std::string& name) : m_name(name) { std::cout << m_name << " created\n"; }
  ~Person() { std::cout << m_name << " destroyed\n"; }

  friend bool partner_up(std::shared_ptr<Person>& p1, std::shared_ptr<Person>& p2) {
    if (!p1 || !p2)
      return false;

    p1->m_partner = p2;
    p2->m_partner = p1;

    std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

    return true;
  }

  const std::shared_ptr<Person> get_partner() const {
    if (m_partner.expired())  // use lock() to convert weak_ptr to shared_ptr
      return nullptr;
    return m_partner.lock();
  }
  const std::string& get_name() const { return m_name; }
};

int student_test() {
  auto lucy{std::make_shared<Person>("Lucy")};
  auto ricky{std::make_shared<Person>("Ricky")};

  partner_up(lucy, ricky);

  auto partner = ricky->get_partner();  // get shared_ptr to Ricky's partner
  std::cout << ricky->get_name() << "'s partner is: " << partner->get_name() << '\n';

  return 0;
}