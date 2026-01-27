#include "neuron.hpp"
#include "activation.hpp"
#include <memory>
#include <random>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>
#include <iostream>

float get_random_float()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dis(-1, 1);
  return dis(gen);
}

Neuron::Neuron(size_t nin, const ActivationType& activation_t) : activation_t(activation_t)
{
  for (size_t idx = 0; idx < nin; ++idx)
  {
    weights.emplace_back(Value::create(get_random_float()));
  }
  bias = Value::create(get_random_float()); 
}

void Neuron::zero_grad()
{
  for (auto& weight : weights)
  {
    weight->grad = 0;
  }
  bias->grad = 0;
}

std::shared_ptr<Value> Neuron::operator()(std::vector<std::shared_ptr<Value>>& x)
{
  if (x.size() != weights.size())
  {
    throw std::invalid_argument("Vectors must be of the same length");
  }

  std::shared_ptr<Value> sum = Value::create(0.0f);

  for (size_t idx = 0; idx < weights.size(); ++idx)
  {
    std::shared_ptr<Value> intermediate_val = Value::multiply(x[idx], weights[idx]);
    sum = Value::add(sum, intermediate_val);
  }

  sum = Value::add(sum, bias);

  const auto& activationFn = Activation::mActivationFn.at(activation_t);
  return activationFn(sum);
}

std::vector<std::shared_ptr<Value>> Neuron::parameters() const
{
  std::vector<std::shared_ptr<Value>> out;
  out.reserve(weights.size() + 1);

  out.insert(out.end(), weights.begin(), weights.end());
  out.push_back(bias);

  return out;
}

void Neuron::print_parameters() const
{
  std::cout << "Number of Parameters: " << weights.size() + 1 << "\n";
  for (const auto& param : weights)
  {
    std::cout << param->data << ", " << param->grad << "\n";
  }
  std::cout << bias->data << ", " << bias->grad << "\n";
  std::cout << "\n";
}

size_t Neuron::get_parameters_size() const
{
  return weights.size() + 1;
}
