# 🧠 Tensor Class in C++

> A standalone, header-only C++ class for tensor manipulation and algebra, including support for tensor contractions, slicing, metric tensors, and more.

---

### 📦 Overview

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
Tensor(); // Default
Tensor(const vector<size_t>& shape_, T init_val = T());
Tensor(std::initializer_list<size_t> shape_, T init_val = T());
Tensor(const vector<size_t>& shape_, const vector<T>& values);
Tensor(const Tensor& other); // Copy
Tensor(Tensor<T>&& other) noexcept; // Move

 

 
