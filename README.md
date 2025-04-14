# 🧠 Tensor Class in C++

This is a modern, flexible, and lightweight **Tensor class** written in C++, supporting generic types and many advanced tensor operations.  
Ideal for symbolic computation, physics simulations, or mathematical explorations.

---

### 🔧 Features

- Generic type support (e.g., `Tensor<double>`, `Tensor<Symbole>`, etc.)
- Metric-aware scalar products
- Slicing, reshaping, and printing
- Tensor contractions and products
- Axis permutation
- Full operator overloading (`+`, `*`, assignment, etc.)
- Header-only, no dependencies

---

### 🧑‍💻 Author & Metadata

**Author:** JP CHAMPEAUX  
**Version:** 1.1.111.584  
**Date:** 14/04/2025  
**Status:** Release Candidate  
**Dependencies:** None  

---

### 🛠 Constructors

```cpp
Tensor(); // Default constructor
Tensor(const vector<size_t>& shape_, T init_val = T());
Tensor(std::initializer_list<size_t> shape_, T init_val = T());
Tensor(const vector<size_t>& shape_, const vector<T>& values);
Tensor(const Tensor& other);               // Copy constructor
Tensor(Tensor<T>&& other) noexcept;        // Move constructor
```

---

### 🧬 Internal Members

- **vector<T> data**: Data storage for the tensor elements.
- **vector<size_t> shape**: Dimensions of the tensor (e.g., `[2, 2]` for a 2x2 matrix).
- **vector<size_t> strides**: The strides for efficient indexing and access.
- **Tensor<T>* metric**: Optional metric tensor used for scalar products and operations.

---

### 🚀 Public Methods

- **Element Access & Metadata**  
  `T& operator()(Args... args);`  
  `T operator()(Args... args) const;`  
  `const vector<size_t>& get_shape() const;`  
  `const vector<T>& get_data() const;`  
  `size_t ndim() const;`  
  (Returns the number of dimensions of the tensor)

- **Arithmetic Operators**  
  `Tensor operator+(const Tensor& other) const;`  
  `Tensor operator*(const Tensor& other) const;`  
  `template<typename U> Tensor operator*(const U& scalar) const;`  
  `template<typename U> friend Tensor operator*(const U& scalar, const Tensor& tensor);`

- **Assignment Operators**  
  `Tensor& operator=(const Tensor& other);`  
  `Tensor& operator=(Tensor<T>&& other) noexcept;`

- **Printing & Visualization**  
  `friend ostream& operator<<(ostream& os, const Tensor<T>& tensor);`  
  `void print(bool detailed = false);`  
  `void printMatrixRepresentation() const;`  

- **Tensor Manipulation**  
  `void fill(T val);` (Set all elements to a given value)  
  `void reshape(const vector<size_t>& new_shape);` (Reshape the tensor)  
  `Tensor<T> slice(const vector<tuple<size_t, size_t, size_t>>& slices) const;` (Slice the tensor into sub-tensors)  
  `Tensor<T> permute(const vector<size_t>& order) const;` (Permute the tensor axes)

- **Tensor Algebra**  
  `T sum() const;` (Sum of all tensor elements)  
  `T pseudo_norm() const;` (Returns the pseudo-norm of the tensor)  
  `Tensor<T> tensor_product(const Tensor<T>& other) const;` (Tensor product)  
  `Tensor<T> contract(size_t axis1, size_t axis2) const;` (Contract the tensor over two axes)  
  `Tensor<T> contract_with(const Tensor<T>& B, size_t axis_A, size_t axis_B) const;` (Tensor contraction with another tensor)  
  `Tensor<T> contract_with_metric(size_t axis1, size_t axis2) const;` (Contract with a metric tensor)

- **Metric Tensor**  
  `void set_metric(const Tensor<T>& metric_tensor);` (Set a metric tensor)  
  `Tensor<T>* get_metric() const;` (Get the metric tensor)

---

### 📄 Example Usage

```cpp
#include "tensor.h"

int main() {
    // Create a 2x2 tensor with all values initialized to 1.0
    Tensor<double> A({2, 2}, 1.0);
    
    // Create a 2x2 tensor with custom values
    Tensor<double> B({2, 2}, {1.0, 2.0, 3.0, 4.0});
    
    // Multiply tensors A and B
    auto C = A * B;
    C.print();  // Output the result

    // Create a tensor of strings
    Tensor<string> labels({2, 2}, "x");
    labels.print();  // Output the string tensor
}
```

---

### 🧪 TODO

- [ ] Add broadcasting support  
- [ ] Add elementwise operations  
- [ ] Support for sparse tensors  
- [ ] File I/O (save/load tensors)  
- [ ] GPU or OpenMP support  

---

### 📃 License

This project is open-source and free to use for educational or personal purposes.  
Attribution appreciated if reused or modified. 🌟

---

Feel free to contribute or suggest improvements via pull requests!
