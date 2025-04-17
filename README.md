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
int main() {
    // Colored header display
    cout << blue;
    std::cout << "-----------------------------------------------------------------------\n";
    cout << white;
    cout << blue << "Tensor Class C++ \n";
    cout << green << "Coded by JP CHAMPEAUX                 \n";
    cout << white;
    cout << red << AutoVersion::FULLVERSION_STRING << endl;
    cout << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << endl;
    cout << AutoVersion::STATUS << " version" << endl;
    cout << blue;
    cout << "Dependencies :        none " << endl;
    std::cout << "-----------------------------------------------------------------------\n";
    cout << white;

    // Start timing
    clock_t t1, t2;
    t1 = clock();

    /// --------------------------
    /// BASIC OPERATIONS
    /// --------------------------

    cout << yellow << "Creating a 2D tensor (3x3) initialized with 0s:" << white << endl;
    Tensor<double> T2D_3x3({3, 3});
    cout << T2D_3x3 << endl;

    // Modifying tensor elements
    T2D_3x3(0, 0) = 1.0;
    T2D_3x3(0, 1) = 2.0;
    T2D_3x3(0, 2) = 3.0;
    T2D_3x3(1, 0) = 4.0;
    T2D_3x3(1, 1) = 5.0;
    T2D_3x3(1, 2) = 6.0;
    T2D_3x3(2, 0) = 7.0;
    T2D_3x3(2, 1) = 8.0;
    T2D_3x3(2, 2) = 9.0;

    cout << yellow << "Tensor after modification Nom(id,id)=val:" << white << endl;
    T2D_3x3.print(true);

    // Sum of tensor elements
    double sum = T2D_3x3.sum();
    cout << yellow << "Sum of values Nom.sum(): " << white << sum << endl;

    // Create another 2D tensor with all elements initialized to 1
    cout << yellow << "\nCreating another 2D tensor (3x3) initialized with 1s:" << white << endl;
    Tensor<double> t2D_2({3, 3}, 1.0);
    t2D_2.print(true);

    // Tensor addition
    cout << yellow << "\nAddition of two tensors (T2D_3x3 + t2D_2):" << white << endl;
    Tensor<double> sum_tensors = T2D_3x3 + t2D_2;
    sum_tensors.print(true);

    // Element-wise multiplication (Hadamard product)
    cout << yellow << "\nElement-wise multiplication (T2D_3x3 * t2D_2):" << white << endl;
    Tensor<double> prod_tensors = T2D_3x3 * t2D_2;
    prod_tensors.print(true);

    // Scalar multiplication
    cout << yellow << "\nMultiplying tensor by scalar (2.5):" << white << endl;
    Tensor<double> scalar_prod = T2D_3x3 * 2.5;
    scalar_prod.print(true);

    // Norm without metric
    cout << yellow << "\nPseudo-norm of 2D tensor (without metric):" << white << endl;
    double norm = T2D_3x3.pseudo_norm();
    cout << yellow << "Norm: " << norm << white << endl;

    // Reshape tensor to (1, 9)
    cout << yellow << "\nReshaping the tensor to shape (1, 9):" << white << endl;
    T2D_3x3.reshape({1, 9});
    T2D_3x3.print(true);

    // Tensor slicing
    cout << yellow << "\nSlicing tensor (0, 0, 3) to (0, 0, 6):" << white << endl;
    Tensor<double> sliced_tensor = T2D_3x3.slice({{1, 3, 6}});
    sliced_tensor.print(true);

    // Print sliced tensor
    cout << yellow << "\nDisplaying the sliced tensor:" << white << endl;
    sliced_tensor.print(true);

    // Create a (3x1) tensor and modify values
    cout << yellow << "Creating a (3x1) tensor initialized to 0:" << white << endl;
    Tensor<double> t2Dp({3, 1});
    t2Dp.print(true);

    t2Dp(0, 0) = 1.0;
    t2Dp(1, 0) = 2.0;
    t2Dp(2, 0) = 3.0;

    cout << yellow << "\nDisplaying the modified tensor:" << white << endl;
    t2Dp.print(true);

    // Metric tensor (3x3 identity-like with custom values)
    cout << yellow << "\nCreating a metric tensor (3x3):" << white << endl;
    Tensor<double> metric({3, 3});
    metric(0, 0) = 1.0;
    metric(1, 1) = -1.0;
    metric(2, 2) = -1.0;
    metric.print(true);

    // Set metric to vector
    t2Dp.set_metric(metric);

    // Compute pseudo-norm with metric
    cout << yellow << "\nPseudo-norm with metric (3x3 identity variant):" << white << endl;
    double norm_with_metric = t2Dp.pseudo_norm();
    cout << yellow << "\nNorm with metric:\n" << norm_with_metric << white << endl;

    // Create and display a 3x3x3 tensor
    Tensor<double> t({3, 3, 3});
    for (int i = 0; i < 27; ++i) t.data[i] = i + 1.0;

    cout << yellow << "\n3x3x3 Tensor (as 2D matrix representation):" << white << endl;
    t.print(true);

    /// ---------------------------
    /// Tensor Product
    /// ---------------------------
    Tensor<int> A({2, 2}), B({2, 3});
    A(0, 0) = 1; A(0, 1) = 2; A(1, 0) = 3; A(1, 1) = 4;
    B(0, 0) = 5; B(0, 1) = 6; B(0, 2) = 3;
    B(1, 0) = 7; B(1, 1) = 8; B(1, 2) = 9;
    A.print(true); B.print(true);

    Tensor<int> C = A.tensor_product(B);
    C.print(true);

    /// Example with symbolic tensors
    cout << yellow << "Example: Tensor of custom class (Symbole):" << white << "\nT1 =" << endl;
    Tensor<Symbole> Aa({2, 2});
    Aa({0, 0}) = Symbole("a"); Aa({0, 1}) = Symbole("b");
    Aa({1, 0}) = Symbole("c"); Aa({1, 1}) = Symbole("d");
    Aa.print();

    cout << "\nT2 =" << endl;
    Tensor<Symbole> Bb({2, 2});
    Bb({0, 0}) = Symbole("x"); Bb({0, 1}) = Symbole("y");
    Bb({1, 0}) = Symbole("z"); Bb({1, 1}) = Symbole("w");
    Bb.print();

    cout << "\nT1 (x) T2 =" << endl;
    Tensor<Symbole> Cc = Aa.tensor_product(Bb);
    Cc.print(true);

    Tensor<Symbole> D;
    string pi = "3.14";
    D = Bb * 3.14 + 2 * Aa;
    D.print();

    /// ----------------
    /// PERMUTATIONS
    /// ----------------
    cout << yellow << "PERMUTATION of INDICES" << white << endl;
    Tensor<double> Tens({2, 2, 2}, {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0});
    cout << "Initial tensor:" << endl;
    Tens.print();
    Tensor<double> Permutation = Tens.permute({0, 2, 1});
    cout << "After permutation (0,1,2 -> 0,2,1):" << endl;
    Permutation.print();

    /// ----------------
    /// CONTRACTIONS
    /// ----------------
    cout << "Initial tensor:" << endl;
    Tens.print();
    Tensor<double> Contr = Tens.contract(0, 1);
    cout << "\nTensor after contraction (axes 0 and 1):" << endl;
    Contr.print();

    Tensor<double> Aaa({2, 3}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    cout << "Tensor A (2x3):" << endl;
    Aaa.print(true);

    Tensor<double> Bbb({3, 2}, {7.0, 8.0, 9.0, 10.0, 11.0, 12.0});
    cout << "Tensor B (3x2):" << endl;
    Bbb.print(true);

    auto contracted = Aaa.contract_with(Bbb, 1, 0);
    cout << "\nContracted Tensor (A[1] x B[0]):" << endl;
    contracted.print(true);

    /// ----------------------------
    /// Scalar contraction example
    /// ----------------------------
    cout << "\nScalar contraction of vectors:" << endl;
    Tensor<double> Tv1({3}, {1.0, 2.0, 3.0});
    Tv1.print();

    Tensor<double> Tv2({3}, {1.0, 2.0, 3.0});
    Tv2.print();

    contracted = Tv1.contract_with(Tv2, 0, 0);
    contracted.print(true);

    // End timing and report
    t2 = clock();
    float temp = (float)(t2 - t1) / CLOCKS_PER_SEC;
    cout << green << "\nAll tasks completed in " << temp << " sec" << endl;
    cout << white << "Have Fun! World is a Playground\n\n";
    cout << " (°)~(°) \n";
    cout << " (=0.0=) \n";
    cout << "   (w)  \n";
    cout << " (:) (:)___°\n";

    return 0;
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

🤝 Contributions

Pull requests, bug reports, or feature suggestions are welcome!
Let's improve this library together 🚀
