#include "tensor.hpp"
#include "value.hpp"
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

Tensor::Tensor(const std::initializer_list<float>& input)
{
  for (const auto& value : input)
  {
    std::vector<std::shared_ptr<Value>> sub_tensor;
    sub_tensor.emplace_back(Value::create(value));
    tensor.emplace_back(sub_tensor);
  }
}

std::vector<std::vector<std::shared_ptr<Value>>>::iterator Tensor::begin()
{
  return tensor.begin();
}

std::vector<std::vector<std::shared_ptr<Value>>>::iterator Tensor::end()
{
  return tensor.end();
}

std::vector<std::vector<std::shared_ptr<Value>>>::const_iterator Tensor::begin() const
{
  return tensor.begin();
}

std::vector<std::vector<std::shared_ptr<Value>>>::const_iterator Tensor::end() const
{
  return tensor.end();
}

void Tensor::zero_grad()
{
  for (auto& sub_tensor : tensor)
  {
    for (auto& value : sub_tensor)
    {
      value->grad = 0.0f;
    }
  }
}

void Tensor::clear()
{
  tensor.clear();
}

std::vector<std::shared_ptr<Value>> Tensor::operator[](const size_t idx) const
{
  if (tensor.size() <= idx)
  {
    throw std::invalid_argument("Accessing a Tensor out of bounds");
  }
  return tensor[idx];
}

std::shared_ptr<Value> Tensor::at(const size_t idx, const size_t jdx) const
{
  if (tensor.size() <= idx || tensor[idx].size() <= jdx)
  {
    throw std::invalid_argument("Accessing a Tensor out of bounds");
  }
  return tensor[idx][jdx];
}

void Tensor::push_back(const std::vector<std::shared_ptr<Value>>& value)
{
  std::vector<std::shared_ptr<Value>> sub_tensor;
  std::copy(value.begin(), value.end(), std::back_inserter(sub_tensor));
  this->tensor.emplace_back(sub_tensor);
}

size_t Tensor::size() const
{
  return tensor.size();
}

