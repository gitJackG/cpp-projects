#ifndef NEURON_HPP
#define NEURON_HPP

#include <memory>
#include <vector>

#include "activation.hpp"

float get_random_float();

class Neuron
{
  private:
    std::vector<std::shared_ptr<Value>> weights;
    std::shared_ptr<Value> bias = Value::create(0.0);
    const ActivationType activation_t;

  public:
    Neuron(size_t nin, const ActivationType& activation_t);

    void zero_grad();

    std::shared_ptr<Value> operator()(std::vector<std::shared_ptr<Value>>& x);

    std::vector<std::shared_ptr<Value>> parameters() const;
    void print_parameters() const;
    size_t get_parameters_size() const;

};

#endif
