#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <memory>
#include <cassert>

class Matrix
{
  private:
    std::size_t rows_;
    std::size_t cols_;
    std::unique_ptr<int[]> data_;
  
  public:
    Matrix(std::size_t rows, std::size_t cols);

    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    int& operator()(std::size_t r, std::size_t c) noexcept;
    const int& operator()(std::size_t r, std::size_t c) const noexcept;

    std::size_t rows() const noexcept { return rows_; }
    std::size_t cols() const noexcept { return cols_; }

    int* data() noexcept { return data_.get(); }
    const int* data() const noexcept { return data_.get(); }
};

#endif
