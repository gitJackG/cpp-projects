#ifndef LAYER_HPP
#define LAYER_HPP

#include <memory>
#include <vector>

#include "neuron.hpp"
#include "activation.hpp"

class Layer
{
  std::vector<Neuron> neurons;

  public:
    Layer(size_t dim_of_neuron, size_t num_nuerons, const ActivationType& act_type);

    std::vector<std::shared_ptr<Value>> operator()(const std::vector<std::shared_ptr<Value>>& x);

    void zero_grad();

    std::vector<Value*> parameters() const;

    void print();
};

#endif
