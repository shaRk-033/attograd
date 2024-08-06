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

int main() {
    Scalar<int> a(2);
    Scalar<int> b(3);
    Scalar<int> c = a + b;
    Scalar<int> d = c - a;
    Scalar<int> e = c * d;
    e._grad = 1;
    e.backward();
    cout << "a: " << a._grad << endl;
    cout << "b: " << b._grad << endl;
    cout << "c: " << c._grad << endl;
    cout << "d: " << d._grad << endl;
    cout << "e: " << e._grad << endl;
    return 0;
}
