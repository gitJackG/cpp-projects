#ifndef VALUE_H
#define VALUE_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Value;

struct Hash
{
  size_t operator()(const std::shared_ptr<Value> v) const;
};

class Value : public std::enable_shared_from_this<Value>
{
  public:
    static size_t current_id;
    float data;
    float grad;
    std::string oper;
    size_t id;
    std::vector<std::shared_ptr<Value>> prev;
    std::function<void()> backward;

  private:
    Value(float data, const std::string& oper, size_t id);

  public:
    static std::shared_ptr<Value> create(float data, const std::string& oper = "");
    ~Value();

    static std::shared_ptr<Value> add(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs);
    static std::shared_ptr<Value> subtract(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs);
    static std::shared_ptr<Value> multiply(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs);
    static std::shared_ptr<Value> divide(std::shared_ptr<Value>& lhs, std::shared_ptr<Value>& rhs);

    void build_topo(
        std::shared_ptr<Value> v, 
        std::unordered_set<std::shared_ptr<Value>, Hash>& visited,
        std::vector<std::shared_ptr<Value>>& topo
        );

    void back_prop();
    void print();
};

#endif // VALUE_H
