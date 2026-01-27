#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "value.hpp"
#include <memory>
#include <unordered_map>

enum ActivationType
{
  RELU,
  SIGMOID,
  TANH
};

class Activation
{
  static std::shared_ptr<Value> Relu(const std::shared_ptr<Value>& val);
  static std::shared_ptr<Value> Sigmoid(const std::shared_ptr<Value>& val);
  static std::shared_ptr<Value> Tanh(const std::shared_ptr<Value>& val);

public:
  static std::unordered_map<ActivationType, std::function<std::shared_ptr<Value>(std::shared_ptr<Value>&)>> mActivationFn;
};

#endif
