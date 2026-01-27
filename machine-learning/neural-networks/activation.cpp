#include "activation.hpp"
#include <memory>
#include <unordered_map>

std::shared_ptr<Value> Activation::Relu(const std::shared_ptr<Value>& val)
{
  return Value::relu(val);
}

std::shared_ptr<Value> Activation::Sigmoid(const std::shared_ptr<Value>& val)
{
  return Value::sigmoid(val);
}

std::shared_ptr<Value> Activation::Tanh(const std::shared_ptr<Value>& val)
{
  return Value::tanh(val);
}

std::unordered_map<ActivationType, std::function<std::shared_ptr<Value>(std::shared_ptr<Value>&)>> Activation::mActivationFn =
{
  {ActivationType::RELU, Relu},
  {ActivationType::SIGMOID, Sigmoid},
  {ActivationType::TANH, Tanh}
};

