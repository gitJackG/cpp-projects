#include "value.h"
#include <iostream>

int main()
{
  auto a = Value::create(1.0f, "");
  auto b = Value::create(2.0f, "");

  auto c = Value::add(a, b);
  auto d = Value::multiply(c, c);

  auto loss = Value::add(d, d);

  loss->back_prop();

  return 0;
}
