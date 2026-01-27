#ifndef MLP_HPP
#define MLP_HPP

#include <memory>
#include <vector>
#include "layer.hpp"

class MLP
{
  private:
    std::vector<size_t> sizes;
    std::vector<Layer> layers;
    float learning_rate;

  public:
    MLP(size_t nin, std::vector<size_t> nout, const float learning_rate = 0.0025);

    void zero_grad();

    std::vector<Value*> parameters() const;

    void print_parameters();

    void update();

    std::vector<std::shared_ptr<Value>> operator()(const std::vector<std::shared_ptr<Value>>& input);
};

#endif
