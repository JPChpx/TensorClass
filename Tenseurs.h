///  -------------------------------------------------
///  Tensor Class 14/04/2024 optimisation ...
///  StandAlone class for Tensor operations
///  Coded by JP CHAMPEAUX
///  contact : JPC@irsamc.upse-tlse.fr
///  Free of use for Academic purpose only !
///  (please, just mention author in your works if used or if your own code is inspired from it)
///  CLASS Tensor
///  --------------------------------------------------


#ifndef TENSEURS_H_INCLUDED
#define TENSEURS_H_INCLUDED

#include <vector>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <tuple>
#include <algorithm>

using namespace std;

template<typename T>
class Tensor
{
private:
    vector<T> data;
    vector<size_t> shape;
    vector<size_t> strides;
    Tensor<T>* metric = nullptr;  // 🔥 pointeur vers tenseur métrique

    void check_shape_match(const Tensor& other) const
    {
        if (shape != other.shape) throw runtime_error("Shape mismatch in operation");
    }


    void slice_data(const vector<pair<size_t, size_t>>& slice_ranges, Tensor<T>& result) const
    {
        vector<size_t> idx(shape.size(), 0);
        vector<size_t> idx_result(shape.size(), 0);
        for (size_t i = 0; i < data.size(); ++i)
        {
            size_t tmp = i;
            bool in_slice = true;
            for (int d = 0; d < shape.size(); ++d)
            {
                idx[d] = tmp / strides[d];
                tmp %= strides[d];
                auto [start, end] = slice_ranges[d];
                if (idx[d] < start || idx[d] >= end)
                {
                    in_slice = false;
                    break;
                }
            }

            if (in_slice)
            {
                for (size_t d = 0; d < shape.size(); ++d)
                    idx_result[d] = idx[d] - slice_ranges[d].first;
                result(idx_result) = data[i];
            }
        }
    }
    void compute_strides()
    {
        strides.resize(shape.size());
        size_t stride = 1;
        for (int i = shape.size() - 1; i >= 0; --i)
        {
            strides[i] = stride;
            stride *= shape[i];
        }
    }

    size_t flatten_index(const vector<size_t>& indices) const
    {
        if (indices.size() != shape.size())
            throw runtime_error("Index dimension mismatch");

        size_t idx = 0;
        for (size_t i = 0; i < shape.size(); ++i)
        {
            if (indices[i] >= shape[i])
                throw out_of_range("Index out of bounds");
            idx += indices[i] * strides[i];
        }
        return idx;
    }


public:
    Tensor() = default;

    Tensor(const vector<size_t>& shape_, T init_val = T())
        : shape(shape_)
    {
        size_t total = 1;
        for (auto d : shape) total *= d;
        data.resize(total, init_val);
        compute_strides();
    }

    // Constructeur par liste d'initialisation
    Tensor(std::initializer_list<size_t> shape_, T init_val = T())
        : shape(shape_)
    {
        size_t total = 1;
        for (auto d : shape) total *= d;
        data.resize(total, init_val);
        compute_strides();
    }

    // Constructeur avec form

    Tensor(const vector<size_t>& shape_, const vector<T>& values)
        : shape(shape_), data(values)
    {
        size_t total = 1;
        for (auto d : shape) total *= d;
        if (data.size() != total)
            throw std::runtime_error("Data size does not match shape");

        compute_strides();
    }

    Tensor(const Tensor& other)
        : data(other.data), shape(other.shape), strides(other.strides)
    {
        if (other.metric)
            metric = new Tensor(*other.metric);
        else
            metric = nullptr;
    }

    Tensor(Tensor<T>&& other) noexcept
        : data(std::move(other.data)), shape(std::move(other.shape)), strides(std::move(other.strides)), metric(other.metric)
    {
        other.metric = nullptr;
    }


    ~Tensor()
    {
        if (metric)
            delete metric;
    }

    // Surcharge de l'opérateur << pour afficher le tenseur
    friend ostream& operator<<(ostream& os, const Tensor<T>& tensor)
    {
        os << "Tensor (shape: ";
        for (size_t i = 0; i < tensor.shape.size(); ++i)
        {
            os << tensor.shape[i];
            if (i != tensor.shape.size() - 1) os << " x ";
        }
        os << "):\n";

        size_t total = tensor.size();
        for (size_t i = 0; i < total; ++i)
        {
            vector<size_t> indices = tensor.flatten_to_indices(i, tensor.shape);
            os << "(";
            for (size_t j = 0; j < indices.size(); ++j)
            {
                os << indices[j];
                if (j != indices.size() - 1) os << ", ";
            }
            os << ") = " << tensor.data[i] << "\n";
        }
        return os;
    }

    template<typename... Args>
    T& operator()(Args... args)
    {
        vector<size_t> indices = {static_cast<size_t>(args)...};
        return (*this)(indices);
    }

    template<typename... Args>
    T operator()(Args... args) const
    {
        vector<size_t> indices = {static_cast<size_t>(args)...};
        return (*this)(indices);
    }

    // Opérateur d'affectation par déplacement
    Tensor& operator=(Tensor<T>&& other) noexcept
    {
        if (this != &other)
        {
            data = std::move(other.data);
            shape = std::move(other.shape);
            strides = std::move(other.strides);
            metric = other.metric;
            other.metric = nullptr; // On "déplace" le pointeur de metric
        }
        return *this;
    }


    Tensor& operator=(const Tensor& other)
    {
        if (this != &other)
        {
            data = other.data;
            shape = other.shape;
            strides = other.strides;

            if (metric)
            {
                delete metric;
                metric = nullptr;
            }

            if (other.metric)
                metric = new Tensor(*other.metric);
        }
        return *this;
    }

    size_t ndim() const
    {
        return shape.size();
    }

    const vector<size_t>& get_strides() const
    {
        return strides;
    }

    size_t size() const
    {
        return data.size();
    }

    T& operator()(const vector<size_t>& indices)
    {
        return data[flatten_index(indices)];
    }

    T operator()(const vector<size_t>& indices) const
    {
        return data[flatten_index(indices)];
    }

    Tensor operator+(const Tensor& other) const
    {
        check_shape_match(other);
        Tensor result(shape);
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::plus<T>());
        return result;
    }


    Tensor operator*(const Tensor& other) const
    {
        check_shape_match(other);
        Tensor result(shape);
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::multiplies<T>());
        return result;
    }
/*
    Tensor operator*(T scalar) const
    {
        Tensor result(shape);
        std::transform(data.begin(), data.end(), result.data.begin(), [scalar](T val) { return val * scalar; });
        return result;
    }
    */
    template<typename U>
    Tensor operator*(const U& scalar) const {
    Tensor result(shape);
    std::transform(data.begin(), data.end(), result.data.begin(),
                   [scalar](const T& val) { return val * static_cast<T>(scalar); });
    return result;
    }

    template<typename U>
    friend Tensor operator*(const U& scalar, const Tensor& tensor) {
    return tensor * scalar; // réutilise la logique de l'opérateur déjà défini
    }

    T sum() const
    {
        return accumulate(data.begin(), data.end(), T());
    }

    T pseudo_norm() const
    {
        if (!metric)
        {
            return std::accumulate(data.begin(), data.end(), T(0), [](T sum, T val) { return sum + val * val; });
        }
        else
        {
            const Tensor<T>& G = *metric;
            T norm_squared = 0.0;
            for (size_t i = 0; i < shape[0]; ++i)
            {
                for (size_t j = 0; j < shape[0]; ++j)
                {
                    norm_squared += data[i] * G({i, j}) * data[j];
                }
            }
            return norm_squared;
        }
    }


    void fill(T val)
    {
        std::fill(data.begin(), data.end(), val);
    }


    void print(bool detailed = false) const
    {
        if (detailed)
        {
            std::cout << "Shape: ";
            if(shape.size()!=0)
            {
            for (auto s : shape) std::cout << s << " ";
            std::cout << std::endl;
            }
            else cout<<"1d"<<endl;
        }

        if (shape.size() == 0)
        {
            // Tenseur scalaire
            std::cout << data[0] << std::endl;
        }
        else if (shape.size() == 1)
        {
            // Vecteur 1D
            for (size_t i = 0; i < shape[0]; ++i)
                std::cout << (*this)({i}) << " ";
            std::cout << std::endl;
        }
        else if (shape.size() == 2)
        {
            // Matrice 2D
            for (size_t i = 0; i < shape[0]; ++i)
            {
                for (size_t j = 0; j < shape[1]; ++j)
                {
                    std::cout << (*this)({i, j}) << " ";
                }
                std::cout << std::endl;
            }
        }
        else
        {
            // Tenseur >2D
            printMatrixRepresentation(); // ou ta méthode personnalisée
        }
    }

void reshape(const vector<size_t>& new_shape)
    {
        size_t new_total = std::accumulate(new_shape.begin(), new_shape.end(), size_t(1), std::multiplies<size_t>());
        if (new_total != data.size()) throw runtime_error("Reshape size mismatch");
        shape = new_shape;
        compute_strides();
    }

    const vector<T>& get_data() const
    {
        return data;
    }
 void set_metric(const Tensor<T>& metric_tensor)
    {
        delete metric;
        metric = new Tensor(metric_tensor);
    }

    Tensor<T>* get_metric() const
    {
        return metric;
    }


 Tensor<T> slice(const vector<tuple<size_t, size_t, size_t>>& slices) const
    {
        vector<size_t> new_shape = shape;
        vector<pair<size_t, size_t>> slice_ranges(shape.size(), {0, 0});
        for (const auto& s : slices)
        {
            size_t dim, start, end;
            std::tie(dim, start, end) = s;
            if (dim >= shape.size() || start >= end || end > shape[dim])
                throw out_of_range("Invalid slice range");

            slice_ranges[dim] = {start, end};
            new_shape[dim] = end - start;
        }

        Tensor<T> result(new_shape);
        slice_data(slice_ranges, result);
        return result;
    }



    void printMatrixRepresentation() const
    {
        for (size_t i = 0; i < shape[0]; ++i)
        {
            std::cout << "Slice " << i << " :\n";
            for (size_t j = 0; j < shape[1]; ++j)
            {
                for (size_t k = 0; k < shape[2]; ++k)
                {
                    std::cout << (*this)({i, j, k}) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    vector<size_t> flatten_to_indices(size_t index, const vector<size_t>& other_shape) const
    {
        vector<size_t> indices(other_shape.size(), 0);
        size_t remaining_index = index;

        for (int i = other_shape.size() - 1; i >= 0; --i)
        {
            indices[i] = remaining_index % other_shape[i];
            remaining_index /= other_shape[i];
        }
        return indices;
    }


    Tensor<T> tensor_product(const Tensor<T>& other) const
    {
        // Nouvelle forme du tenseur résultant (Kronecker)
        vector<size_t> new_shape;

        // On suppose ici que les deux tenseurs ont la même taille pour chaque dimension
        size_t ndim_a = shape.size();
        size_t ndim_b = other.shape.size();
        size_t max_ndim = max(ndim_a, ndim_b);

        // Calcul des nouvelles dimensions pour le produit tensoriel
        for (size_t i = 0; i < max_ndim; ++i)
        {
            size_t dim_a = (i < ndim_a) ? shape[i] : 1;  // Si la dimension n'existe pas, on prend 1
            size_t dim_b = (i < ndim_b) ? other.shape[i] : 1; // Idem pour l'autre tenseur
            new_shape.push_back(dim_a * dim_b);
            // cout<<"--->"<<dim_a * dim_b<<endl;
        }


        // Créer le tenseur résultant avec la forme calculée
        Tensor<T> result(new_shape);
      //  result.print();

        for (size_t i = 0; i < data.size(); ++i)
        {
            vector<size_t> indices_a = flatten_to_indices(i, shape);

            for (size_t j = 0; j < other.data.size(); ++j)
            {
                vector<size_t> indices_b = flatten_to_indices(j, other.shape);

                vector<size_t> new_indices;
                size_t ndim = result.shape.size(); // Taille du tenseur résultat

                // On "pad" les indices manquants avec 0
                while (indices_a.size() < ndim)
                    indices_a.push_back(0);
                while (indices_b.size() < ndim)
                    indices_b.push_back(0);

                for (size_t k = 0; k < ndim; ++k)
                {
                    size_t dim_a = (k < shape.size()) ? shape[k] : 1;
                    size_t dim_b = (k < other.shape.size()) ? other.shape[k] : 1;

                    size_t index = (indices_a[k] * dim_b) + indices_b[k];
                    new_indices.push_back(index);
                }

                T res = data[i] * other.data[j];
                result(new_indices) = res;
            }
        }

        return result;
    }

    Tensor<T> permute(const vector<size_t>& order) const
    {
        if (order.size() != shape.size())
            throw runtime_error("Order size must match the number of dimensions");

        vector<size_t> new_shape(shape.size());
        for (size_t i = 0; i < shape.size(); ++i)
            new_shape[i] = shape[order[i]];

        Tensor<T> result(new_shape);
        vector<size_t> old_indices(shape.size());
        vector<size_t> new_indices(shape.size());

        for (size_t i = 0; i < data.size(); ++i)
        {
            old_indices = flatten_to_indices(i, shape);
            for (size_t j = 0; j < shape.size(); ++j)
                new_indices[j] = old_indices[order[j]];

            result(new_indices) = data[i];
        }

        return result;
    }

    const vector<size_t>& get_shape()
    {
        return shape;
    }


    Tensor<T> contract(size_t axis1, size_t axis2) const
    {
        if (axis1 >= shape.size() || axis2 >= shape.size())
            throw std::runtime_error("Invalid axis");

        if (axis1 == axis2)
            throw std::runtime_error("Cannot contract the same axis");

        if (shape[axis1] != shape[axis2])
            throw std::runtime_error("Cannot contract axes with different sizes");

        // Déterminer la nouvelle forme après contraction
        vector<size_t> new_shape;
        for (size_t i = 0; i < shape.size(); ++i)
        {
            if (i != axis1 && i != axis2)
                new_shape.push_back(shape[i]);
        }

        // Résultat initialisé à zéro (même si new_shape est vide)
        Tensor<T> result(new_shape, T(0));

        vector<size_t> indices(shape.size());

        for (size_t i = 0; i < data.size(); ++i)
        {
            indices = flatten_to_indices(i, shape);

            // Conserver seulement les cas où les deux axes ont la même valeur (contraction diagonale)
            if (indices[axis1] == indices[axis2])
            {
                vector<size_t> reduced_indices;
                for (size_t j = 0; j < indices.size(); ++j)
                {
                    if (j != axis1 && j != axis2)
                        reduced_indices.push_back(indices[j]);
                }
                result(reduced_indices) += data[i];
            }
        }

        return result;
    }

    Tensor<T> flatten() const
    {
        return Tensor<T>({data.size()}, data);
    }


    Tensor<T> contract_with(const Tensor<T>& B, size_t axis_A, size_t axis_B) const
    {
        if (axis_A >= shape.size() || axis_B >= B.shape.size())
            throw std::runtime_error("Invalid contraction axes");

        size_t dim = shape[axis_A];
        if (dim != B.shape[axis_B])
            throw std::runtime_error("Mismatched dimensions for contraction");

        bool use_metric = (metric != nullptr);
        if (use_metric)
        {
            if (metric->shape.size() != 2 || metric->shape[0] != dim || metric->shape[1] != dim)
                throw std::runtime_error("Metric must be a square matrix matching contraction dimension");
        }

        // Compute shape of result
        std::vector<size_t> new_shape;
        for (size_t i = 0; i < shape.size(); ++i)
            if (i != axis_A)
                new_shape.push_back(shape[i]);

        for (size_t i = 0; i < B.shape.size(); ++i)
            if (i != axis_B)
                new_shape.push_back(B.shape[i]);

        Tensor<T> result(new_shape, T{});

        // Temporary indices
        std::vector<size_t> idx_A(shape.size());
        std::vector<size_t> idx_B(B.shape.size());
        std::vector<size_t> idx_res(result.shape.size());

        size_t total = result.data.size();
        for (size_t r = 0; r < total; ++r)
        {
            idx_res = result.flatten_to_indices(r, result.shape);

            size_t ai = 0;
            for (size_t i = 0; i < shape.size(); ++i)
                if (i != axis_A)
                    idx_A[i] = idx_res[ai++];

            size_t bi = 0;
            for (size_t i = 0; i < B.shape.size(); ++i)
                if (i != axis_B)
                    idx_B[i] = idx_res[ai + bi++];

            T sum = T{};
            for (size_t k = 0; k < dim; ++k)
            {
                idx_A[axis_A] = k;
                for (size_t l = 0; l < dim; ++l)
                {
                    idx_B[axis_B] = l;

                    T m = (use_metric ? (*metric)({k, l}) : (k == l ? T{1} : T{0}));
                    sum  = sum + (*this)(idx_A) * m * B(idx_B);
                }
            }

            result.data[r] = sum;
        }

        return result;
    }

    Tensor<T> contract_with_metric(size_t axis1, size_t axis2) const
    {
        if (axis1 >= shape.size() || axis2 >= shape.size())
            throw std::runtime_error("Invalid axis indices");

        if (shape[axis1] != shape[axis2])
            throw std::runtime_error("Axes must have the same dimension");

        // Si aucun tenseur métrique défini → matrice identité
        Tensor<T> identity_metric({shape[axis1], shape[axis2]}, T(0));
        if (!metric)
        {
            for (size_t i = 0; i < shape[axis1]; ++i)
                identity_metric({i, i}) = T(1);
        }

        const Tensor<T>& g = metric ? *metric : identity_metric;

        // Nouvelle forme sans les axes contractés
        std::vector<size_t> new_shape;
        for (size_t i = 0; i < shape.size(); ++i)
        {
            if (i != axis1 && i != axis2)
                new_shape.push_back(shape[i]);
        }

        Tensor<T> result(new_shape, T(0));
        std::vector<size_t> idx(shape.size());

        for (size_t i = 0; i < data.size(); ++i)
        {
            idx = flatten_to_indices(i, shape);
            std::vector<size_t> reduced_idx;
            for (size_t j = 0; j < idx.size(); ++j)
            {
                if (j != axis1 && j != axis2)
                    reduced_idx.push_back(idx[j]);
            }

            T sum = T(0);
            for (size_t k = 0; k < shape[axis1]; ++k)
            {
                for (size_t l = 0; l < shape[axis2]; ++l)
                {
                    idx[axis1] = k;
                    idx[axis2] = l;
                    sum += (*this)(idx) * g({k, l});
                }
            }
            result(reduced_idx) += sum;
        }

        return result;
    }


};

#endif // TENSEURS_H_INCLUDED
