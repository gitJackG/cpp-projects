#include <iostream>

#include "matrix.hpp"

int main()
{
  Matrix A(2, 2);
  Matrix B(2, 2);

  for (std::size_t i = 0; i < 2; ++i)
  {
    std::cout << "[ ";
    for (std::size_t j = 0; j < 2; ++j)
    {
      std::cout << A(i,j) << " ";
    }
    std::cout << "]\n";
  }

  return 0;
}
