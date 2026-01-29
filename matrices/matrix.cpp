#include "matrix.hpp"

Matrix::Matrix(std::size_t rows, std::size_t cols)
    : rows_(rows),
      cols_(cols),
      data_(std::make_unique<int[]>(rows * cols))
{}

int& Matrix::operator()(std::size_t r, std::size_t c) noexcept {
    assert(r < rows_ && c < cols_);
    return data_[r * cols_ + c];
}

const int& Matrix::operator()(std::size_t r, std::size_t c) const noexcept {
    assert(r < rows_ && c < cols_);
    return data_[r * cols_ + c];
}
