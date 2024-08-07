#include<iostream>
#include<functional>
#include<vector>
#include "scalar.cpp"
#include "helper.cpp"

using namespace std;

template <class T>
class Tensor{
public:
    vector<Scalar<T>> _store;
    vector<int> _shape;
    int __num_elements;
    function<void()> _backward;

    Tensor(vector<T> values, vector<int> shape){
        this->_shape = shape;
        for(auto dim: shape){
            this->__num_elements *= dim;
        }
        for(int i = 0; i < this->__num_elements; i++){
            this->_store.push_back(Scalar<T>(values[i]));
        }
    }

    Tensor(vector<T> values, vector<int> shape, function<void()> backward){
        this->_shape = shape;
        this->_backward = backward;
        for(auto dim: shape){
            this->__num_elements *= dim;
        }
        for(int i = 0; i < this->__num_elements; i++){
            this->_store.push_back(Scalar<T>(values[i]));
        }
    }
    // we need implement broadcasting
    // eg: (2, 3) + (3,) -> (2, 3) + (1, 3) -> (2, 3), so we kinda need to check the dims of
    // the two tensors from rightmost end of the dims list. The dims should be equal or one of them should be 1.
    // or else the opeation isnt possible.
    // Tensor<T> operator+(Tensor<T>& other){
    //     bool dims_check = check_dims(this->_shape, other._shape);
    //     assert(dims_check && "Dimensions of the two tensors are not compatible for addition");
    //     vector<int> result_shape = broadcast_dims(this->_shape, other._shape);
        
    // }
};

template<class T>
Tensor<T> randn(vector<int> shape){
    vector<Scalar<T>> values;
    int __num_elements = 1;
    for(auto dim: shape){
        __num_elements *= dim;
    }
    for(int i = 0; i < this->__num_elements; i++){
        values.push_back(Scalar<T>(rand_normal(0, 1)));
    }
    return Tensor<T>(values, shape);
}