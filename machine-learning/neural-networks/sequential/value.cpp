#include "value.hpp"
#include <cmath>
#include <iostream>
#include <memory>

size_t Value::current_id = 0;

size_t Hash::operator()(const std::shared_ptr<Value> v) const
{
  return std::hash<std::string>()(v->oper) ^ std::hash<float>()(v->data);
}

Value::Value(float data, const std::string& oper, size_t id) 
  : data(data), grad(0.0f), oper(oper), id(id) {}

Value::~Value() = default;

std::shared_ptr<Value> Value::create(float data, const std::string& oper)
{
  return std::shared_ptr<Value>(new Value(data, oper, Value::current_id++));
}

std::shared_ptr<Value> Value::add(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs)
{
  auto out = Value::create(lhs->data + rhs->data, "+");
  out->prev = {lhs, rhs};
  out->backward = 
    [
    lhs_weak = std::weak_ptr<Value>(lhs), 
    rhs_weak = std::weak_ptr<Value>(rhs), 
    out_weak = std::weak_ptr<Value>(out)
    ]()
    {
      lhs_weak.lock()->grad += out_weak.lock()->grad;
      rhs_weak.lock()->grad += out_weak.lock()->grad;
    };
  return out;
}

std::shared_ptr<Value> Value::subtract(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs)
{
  auto out = Value::create(lhs->data - rhs->data, "-");
  out->prev = {lhs, rhs};
  out->backward = 
    [
    lhs_weak = std::weak_ptr<Value>(lhs), 
    rhs_weak = std::weak_ptr<Value>(rhs), 
    out_weak = std::weak_ptr<Value>(out)
    ]()
    {
      lhs_weak.lock()->grad += out_weak.lock()->grad;
      rhs_weak.lock()->grad -= out_weak.lock()->grad;
    };

  return out;
}

std::shared_ptr<Value> Value::multiply(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs)
{
  auto out = Value::create(lhs->data * rhs->data, "*");
  out->prev = {lhs, rhs};
  out->backward = 
    [
    lhs_weak = std::weak_ptr<Value>(lhs), 
    rhs_weak = std::weak_ptr<Value>(rhs), 
    out_weak = std::weak_ptr<Value>(out)
    ]()
    {
      lhs_weak.lock()->grad += rhs_weak.lock()->data * out_weak.lock()->grad;
      rhs_weak.lock()->grad += lhs_weak.lock()->data * out_weak.lock()->grad;
    };
  return out;
}


std::shared_ptr<Value> Value::power(const std::shared_ptr<Value>& base, float exp)
{
  float new_value = std::pow(base->data, exp);
  auto out = Value::create(new_value, "^");
  out->prev = {base};
  out->backward =
    [
    base_weak = std::weak_ptr<Value>(base),
    out_weak = std::weak_ptr<Value>(out),
    exp
    ]()
    {
      if (auto base = base_weak.lock())
      {
        base->grad += exp * std::pow(base->data, exp - 1) * out_weak.lock()->grad;
      }
    };
  return out;
}

std::shared_ptr<Value> Value::divide(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs)
{
  auto rec = power(rhs, -1);
  return multiply(lhs, rec);
}

std::shared_ptr<Value> Value::relu(const std::shared_ptr<Value>& input)
{
  float val = std::max(0.0f, input->data);
  auto out = Value::create(val, "ReLU");
  out->prev = {input};
  out->backward =
    [
    input_weak = std::weak_ptr<Value>(input),
    out_weak = std::weak_ptr<Value>(out)
    ]()
    {
      auto input_ptr = input_weak.lock();
      auto out_ptr = out_weak.lock();
      if (input_ptr && out_ptr) {
        input_ptr->grad += (out_ptr->data > 0) * out_ptr->grad;
      }
    };
  return out;
}

std::shared_ptr<Value> Value::sigmoid(const std::shared_ptr<Value>& input)
{
  float x = input->data;
  float t = std::exp(x) / (1 + std::exp(x));
  auto out = Value::create(t, "Sigmoid");
  out->prev = {input};
  out->backward =
    [
    input_weak = std::weak_ptr<Value>(input),
    out_weak = std::weak_ptr<Value>(out),
    t
    ]()
    {
      auto input_ptr = input_weak.lock();
      auto out_ptr = out_weak.lock();
      if (input_ptr && out_ptr) {
        input_ptr->grad += t * (1 - t) * out_ptr->grad;
      }
    };
  return out;
}

std::shared_ptr<Value> Value::tanh(const std::shared_ptr<Value>& input)
{
  float x = input->data;
  float t = (std::exp(2 * x) - 1) / (std::exp(2 * x) + 1);
  auto out = Value::create(t, "tanh");
  out->prev = {input};
  out->backward =
    [
    input_weak = std::weak_ptr<Value>(input),
    out_weak = std::weak_ptr<Value>(out),
    t
    ]()
    {
      auto input_ptr = input_weak.lock();
      auto out_ptr = out_weak.lock();
      if (input_ptr && out_ptr) {
        input_ptr->grad += (1 - t * t) * out_ptr->grad;
      }
    };
  return out;
}

void Value::build_topo(
    std::shared_ptr<Value> v, 
    std::unordered_set<std::shared_ptr<Value>, Hash>& visited,
    std::vector<std::shared_ptr<Value>>& topo
    )
{
  if (visited.find(v) == visited.end())
  {
    visited.insert(v);
    for (const auto& child : v->prev)
    {
      build_topo(child, visited, topo);
    }
    topo.push_back(v);
  }
}

void Value::back_prop()
{
  std::vector<std::shared_ptr<Value>> topo;
  std::unordered_set<std::shared_ptr<Value>, Hash> visited; 
  build_topo(shared_from_this(), visited, topo);
  this->grad = 1.0f;
  for (auto it = topo.rbegin(); it != topo.rend(); ++it)
  {
    if ((*it)->backward)
    {
      (*it)->backward();
    }
    //(*it)->print();
  }
}

void Value::print()
{
  std::cout << "[data=" << this->data << ", grad=" << this->grad << "]\n";
}
