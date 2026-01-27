#include "mlp.hpp"
#include "activation.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>
#include <iostream>

MLP::MLP(size_t nin, std::vector<size_t> nout, const float learning_rate) : learning_rate(learning_rate)
{
  sizes.push_back(nin);
  std::copy(nout.begin(), nout.end(), std::back_inserter(sizes));
  for (size_t idx = 0; idx < sizes.size() - 1; ++idx)
  {
    layers.emplace_back(sizes[idx], sizes[idx + 1], ActivationType::SIGMOID);
  }
}

void MLP::zero_grad()
{
  for (auto& layer : this->layers)
  {
    layer.zero_grad();
  }
}

std::vector<Value*> MLP::parameters() const
{
  std::vector<Value*> params;
  for (const auto& layer : this->layers)
  {
    for (const auto& p : layer.parameters())
    {
      params.push_back(p);
    }
  }
  return params;
}

void MLP::print_parameters()
{
  const auto params = this->parameters();
  std::cout << "Num of Parameters: " << (int)params.size() << "\n";
  for (const auto& p : params)
  {
    std::cout << &p << " ";
    std::cout << "[data=" << p->data << ", grad=" << p->grad << "]\n";
  }
}

void MLP::update()
{
  for (auto& p : this->parameters())
  {
    p->data += (float)((float)-this->learning_rate * (float)p->grad);
  }
}

std::vector<std::shared_ptr<Value>> MLP::operator()(const std::vector<std::shared_ptr<Value>>& input)
{
  std::vector<std::shared_ptr<Value>> x = input;
  for (auto& layer : this->layers)
  {
    auto y = layer(x);
    x = y;
  }
  return x;
}
