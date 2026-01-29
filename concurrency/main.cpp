#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

constexpr long ROWS = 1024;
constexpr long COLS = 1024;

void mat_mult(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C)
{
  for (int i = 0; i < ROWS; ++i)
  {
    for (int k = 0; k < COLS; ++k)
    {
      for (int j = 0; j < COLS; ++j) 
      {
        C[i*COLS + j] += A[i*COLS + k] * B[k*COLS + j];
      }
    }
  }
}

void par_mat_mult(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C)
{
  #pragma omp parallel for
  for (int i = 0; i < ROWS; ++i)
  {
    for (int k = 0; k < COLS; ++k)
    {
      for (int j = 0; j < COLS; ++j) 
      {
        C[i*COLS + j] += A[i*COLS + k] * B[k*COLS + j];
      }
    }
  }
}


int main()
{
  std::vector<int> A(ROWS*COLS);
  std::vector<int> B(ROWS*COLS);
  std::vector<int> C(ROWS*COLS);

  for (long i = 0; i < ROWS; ++i)
  {
    for (long j = 0; j < COLS; ++j)
    {
      A[i * COLS + j] = 1;
      B[i * COLS + j] = 2;
      C[i * COLS + j] = 0;
    }
  }

  auto start = std::chrono::steady_clock::now();
  mat_mult(A, B, C);
  auto end = std::chrono::steady_clock::now();
  auto t1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  start = std::chrono::steady_clock::now();
  par_mat_mult(A, B, C);
  end = std::chrono::steady_clock::now();
  auto t2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "(Sequential): " << t1.count() << " ns\n";
  std::cout << "(Threading): " << t2.count() << " ns\n";

  return 0;
}
