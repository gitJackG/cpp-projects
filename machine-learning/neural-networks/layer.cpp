#include "layer.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

Layer::Layer(size_t dim_of_neuron, size_t num_nuerons, const ActivationType& act_type)
{
  for (size_t idx = 0; idx < num_nuerons; ++idx)
  {
    this->neurons.emplace_back(dim_of_neuron, act_type);
  }
}

std::vector<std::shared_ptr<Value>> Layer::operator()(const std::vector<std::shared_ptr<Value>>& x)
{
  std::vector<std::shared_ptr<Value>> out;
  out.reserve(this->neurons.size());
  std::for_each(this->neurons.begin(), this->neurons.end(), [&out, x = x](auto neuron)mutable
      {
      out.emplace_back(neuron(x));
      });
  return out;
}

void Layer::zero_grad()
{
  for (auto& neuron : neurons)
  {
    neuron.zero_grad();
  }
}

std::vector<Value*> Layer::parameters() const
{
  std::vector<Value*> params;
  for (const auto& neuron : neurons)
  {
    for (const auto& p : neuron.parameters())
    {
      params.push_back(p.get());
    }
  }
  return params;
}

void Layer::print()
{
  const auto params = this->parameters();
  std::cout << "Num paramaters: " << (int)params.size() << "\n";
  for (const auto& p : params)
  {
    std::cout << &p << " ";
    std::cout << "[data=" << p->data << ", grad=" << p->grad << "]\n";
  }
  std::cout << "\n";
}
