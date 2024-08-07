#include <functional>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

template <class T>
class Scalar {
public:
    T _value;
    vector<Scalar<T>*> _children;
    function<void()> _backward;
    T _grad;
    Scalar(T value){
        _value = value;
        _grad = 0;
    }
    Scalar(T value, vector<Scalar<T>*> children){
        _value = value;
        _children = children;
        _grad = 0;
    }
    
    Scalar<T> operator+(Scalar<T>& other) {
        Scalar<T> result(this->_value + other._value, {this, &other});
        result._backward = [this, other_ptr = &other, res_ptr = &result]() {
            this->_grad += res_ptr->_grad;
            other_ptr->_grad += res_ptr->_grad;
        };
        return result;
    }

    Scalar<T> operator-(Scalar<T>& other) {
        Scalar<T> result(this->_value - other._value, {this, &other});
        result._backward = [this, other_ptr = &other, res_ptr = &result]() {
            this->_grad += res_ptr->_grad;
            other_ptr->_grad -= res_ptr->_grad;
        };
        return result;
    }

    Scalar<T> operator*(Scalar<T>& other) {
        Scalar<T> result(this->_value * other._value, {this, &other});
        result._backward = [this, other_ptr = &other, res_ptr = &result]() {
            this->_grad += res_ptr->_grad * other_ptr->_value;
            other_ptr->_grad += res_ptr->_grad * this->_value;
        };
        return result;
    }

    bool operator==(Scalar<T>& other) {
        return this->_value == other._value;
    }

    Scalar<T> exp(){
        Scalar<T> result(exp(this->_value), {this});
        result._backward = [this, res_ptr = &result]() {
            this->_grad += res_ptr->_grad * res_ptr->_value;
        };
        return result;
    }

    Scalar<T> log(){
        Scalar<T> result(log(this->_value), {this});
        result._backward = [this, res_ptr = &result]() {
            this->_grad += res_ptr->_grad / this->_value;
        };
        return result;
    }

    void dfs(Scalar<T>* node, set<Scalar<T>*>& children){
        children.insert(node);
        if(node->_children.size() > 0){
            for(auto& child : node->_children){
                dfs(child, children);
            }
        }
        return;
    }
    void backward(){
        set<Scalar<T>*> children;
        dfs(this, children);
        for(auto& child : children){
            if(child->_backward){
                child->_backward();
            }
        }
    }
};

