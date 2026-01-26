#include "value.h"
#include <iostream>

size_t Value::current_id = 0;

size_t Hash::operator()(const std::shared_ptr<Value> v) const
{
  return std::hash<std::string>()(v->oper) ^ std::hash<float>()(v->data);
}

Value::Value(float data, const std::string& oper, size_t id) 
  : data(data), oper(oper), id(id) {}

Value::~Value()
{
  --Value::current_id;
}

std::shared_ptr<Value> Value::create(float data, const std::string& oper)
{
  return std::shared_ptr<Value>(new Value(data, oper, Value::current_id++));
}

std::shared_ptr<Value> Value::add(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs)
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

std::shared_ptr<Value> Value::subtract(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs)
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

std::shared_ptr<Value> Value::multiply(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs)
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

std::shared_ptr<Value> Value::divide(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs)
{
  auto out = Value::create(lhs->data / rhs->data, "/"); 
  out->prev = {lhs, rhs};
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
    (*it)->print();
  }
}

void Value::print()
{
  std::cout << "[data=" << this->data << ", grad=" << this->grad << "]\n";
}
