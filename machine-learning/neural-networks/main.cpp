#include "value.hpp"
#include "tensor.hpp"
#include "mlp.hpp"
#include <cstddef>
#include <vector>
#include <iostream>
#include <cmath>

int main() {
  std::vector<Tensor> inputs;
  std::vector<Tensor> outputs;

  for (int i = 0; i < 20; ++i) {
    float t = i * 0.314159f;  

    inputs.push_back(Tensor({
          t,
          t * t,
          t * t * t,
          std::sin(t),
          std::cos(t),
          std::exp(-t * 0.1f),
          t * std::sin(t),
          t * std::cos(t),
          std::sin(t * 0.5f),
          std::cos(t * 0.5f)
          }));

    outputs.push_back(Tensor({
          std::sin(t),
          std::cos(t),
          std::sin(2 * t),
          std::cos(2 * t),
          std::sin(t) * std::cos(t)
          }));
  }

  constexpr float learningRate = 0.01;
  constexpr size_t numIterations = 5000;
  size_t input_size = inputs[0][0].size();   
  size_t output_size = outputs[0][0].size();  

  auto mlp = std::make_unique<MLP>(
      input_size, 
      std::initializer_list<size_t>{16, 16, output_size}, 
      learningRate
      );

  std::cout << "Training on " << inputs.size() << " examples\n";
  std::cout << "Input size: " << input_size << ", Output size: " << output_size << "\n\n";

  for (size_t idx = 0; idx < numIterations; ++idx) {
    auto loss = Value::create(0);

    for (size_t example = 0; example < inputs.size(); ++example) {
      inputs[example].zero_grad();

      auto ypred = (*mlp)(inputs[example][0]);

      for (size_t i = 0; i < output_size; ++i) {
        auto diff = Value::subtract(outputs[example].at(0, i), ypred[i]);
        auto squared = Value::multiply(diff, diff);
        loss = Value::add(loss, squared);
      }
    }

    mlp->zero_grad();
    loss->back_prop();
    mlp->update();

    if (idx % 500 == 0) {
      std::cout << "Iteration: " << idx << " | Loss: " << loss->data << "\n";
    }
  }

  std::cout << "\n========== Testing ==========\n";
  for (size_t i = 0; i < std::min(size_t(5), inputs.size()); ++i) {
    auto prediction = (*mlp)(inputs[i][0]);

    std::cout << "\nExample " << i << ":\n";
    std::cout << "Expected: [";
    for (size_t j = 0; j < output_size; ++j) {
      std::cout << outputs[i].at(0, j)->data;
      if (j < output_size - 1) std::cout << ", ";
    }
    std::cout << "]\n";

    std::cout << "Predicted: [";
    for (size_t j = 0; j < output_size; ++j) {
      std::cout << prediction[j]->data;
      if (j < output_size - 1) std::cout << ", ";
    }
    std::cout << "]\n";
  }

  return 0;
}
