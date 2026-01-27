#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <initializer_list>
#include <memory>
#include <vector>

#include "value.hpp"

class Tensor
{
  private:
    std::vector<std::vector<std::shared_ptr<Value>>> tensor;

  public:
    Tensor(const std::initializer_list<float>& input);

    std::vector<std::vector<std::shared_ptr<Value>>>::iterator begin();
    std::vector<std::vector<std::shared_ptr<Value>>>::iterator end();
    std::vector<std::vector<std::shared_ptr<Value>>>::const_iterator begin() const;
    std::vector<std::vector<std::shared_ptr<Value>>>::const_iterator end() const;

    void zero_grad();
    
    void clear();

    std::vector<std::shared_ptr<Value>> operator[](const size_t idx) const;

    std::shared_ptr<Value> at(const size_t idx, const size_t jdx = 0) const;

    void push_back(const std::vector<std::shared_ptr<Value>>& value);

    size_t size() const;
};

#endif
