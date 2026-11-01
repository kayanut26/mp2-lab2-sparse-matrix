#pragma once

#include <cstdlib>
#include <iostream>
#include <random>


template <typename ValT = double>
class CSR_matrix {
public:
    size_t n_rows = 0; // число строк
    size_t n_cols = 0; // число столбцов
    size_t nz = 0; // число ненулевых элементов
    size_t capacity = 0; // объем памяти для хранения значений
    size_t* RowIndex = nullptr;
    size_t* Col = nullptr;
    ValT* Val = nullptr;

    CSR_matrix(size_t _m = 1, size_t _n = 1) : n_rows(_m), n_cols(_n), Col(nullptr), Val(nullptr) {
        RowIndex = new size_t[n_rows + 1]();
    }

    CSR_matrix(size_t _m, size_t _n, size_t _nz) : n_rows(_m), n_cols(_n), nz(_nz) {
        RowIndex = new size_t[n_rows + 1]();
        if (nz > 0) {
            Col = new size_t[nz];
            Val = new ValT[nz]();
        }
        else {
            Col = nullptr;
            Val = nullptr;
        }
        capacity = nz;
    }

    CSR_matrix(const CSR_matrix& copy) : n_rows(copy.n_rows), n_cols(copy.n_cols), nz(copy.nz), capacity(copy.capacity) {
        RowIndex = new size_t[n_rows + 1];
        std::copy(copy.RowIndex, copy.RowIndex + n_rows + 1, RowIndex);
        if (copy.nz > 0 && copy.Col != nullptr && copy.Val != nullptr) {
            Val = new ValT[nz];
            std::copy(copy.Val, copy.Val + nz, Val);
            Col = new size_t[nz];
            std::copy(copy.Col, copy.Col + nz, Col);
        }
        else {
            Val = nullptr;
            Col = nullptr;
        }
    }

    CSR_matrix(CSR_matrix&& mov) : n_rows(mov.n_rows), n_cols(mov.n_cols), nz(mov.nz), capacity(mov.capacity) {
        Col = mov.Col;
        RowIndex = mov.RowIndex;
        Val = mov.Val;

        mov.Col = nullptr;
        mov.RowIndex = nullptr;
        mov.Val = nullptr;
    }

    ~CSR_matrix() {
        delete[] Col;
        delete[] RowIndex;
        delete[] Val;
    }

    CSR_matrix& operator=(const CSR_matrix& copy) {
        if (this == &copy)
            return *this;

        if (n_rows != copy.n_rows) {
            delete[] RowIndex;
            RowIndex = new size_t[copy.n_rows + 1];
        }
        std::copy(copy.RowIndex, copy.RowIndex + copy.n_rows + 1, RowIndex);
        if (capacity < copy.nz) {
            delete[] Col;
            delete[] Val;

            Col = new size_t[copy.nz];
            Val = new ValT[copy.nz];
            capacity = copy.nz;
        }
        std::copy(copy.Col, copy.Col + copy.nz, Col);
        std::copy(copy.Val, copy.Val + copy.nz, Val);

        n_rows = copy.n_rows;
        n_cols = copy.n_cols;
        nz = copy.nz;

        return *this;
    }

    CSR_matrix& operator=(CSR_matrix&& mov) {
        if (this == &mov)
            return *this;

        std::swap(Col, mov.Col);
        std::swap(RowIndex, mov.RowIndex);
        std::swap(Val, mov.Val);
        std::swap(n_rows, mov.n_rows);
        std::swap(n_cols, mov.n_cols);
        std::swap(nz, mov.nz);
        std::swap(capacity, mov.capacity);

        return *this;
    }

    // вектора cols, rows, vals содержат корректное описание матрицы в формате CSR
    void set_from_vectors(const std::vector<size_t>& cols, const std::vector<size_t>& rows,
        const std::vector<ValT>& vals) {

        size_t new_nz = cols.size();
        if (nz != new_nz) {
            delete[] Col;
            delete[] Val;
            nz = new_nz;
            Col = new size_t[new_nz];
            Val = new ValT[new_nz];
        }
        if (n_rows != rows.size() - 1) {
            delete[] RowIndex;
            RowIndex = new size_t[n_rows + 1];
            n_rows = rows.size() - 1;
            n_cols = std::max(*std::max_element(cols.begin(), cols.end()) + 1, n_rows);
        }
        std::copy(cols.begin(), cols.end(), Col);
        std::copy(vals.begin(), vals.end(), Val);
        std::copy(rows.begin(), rows.end(), RowIndex);
    }

    void generate_matrix(const size_t avg_degree, ValT minVal, ValT maxVal) {
        size_t new_nz = n_rows * avg_degree;
        if (avg_degree > n_cols) {
            throw std::invalid_argument("avg_degree cannot be greater than the number of columns");
        }

        if (nz != new_nz) {
            delete[] Col;
            delete[] Val;

            nz = new_nz;
            capacity = nz;

            Col = new size_t[nz];
            Val = new ValT[nz];
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        using DistT = std::conditional_t<std::is_floating_point_v<ValT>,
            std::uniform_real_distribution<ValT>,
            std::uniform_int_distribution<ValT>>;
        DistT val_dist(minVal, maxVal);

        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        double p = static_cast<double>(avg_degree) / n_cols;

        size_t current_elem = 0;
        RowIndex[0] = 0;

        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols && current_elem < nz; ++j) {
                if (prob_dist(gen) < p) {
                    Col[current_elem] = j;
                    Val[current_elem] = val_dist(gen);
                    current_elem++;
                }
            }
            RowIndex[i + 1] = current_elem;
        }
        nz = RowIndex[n_rows];
        capacity = nz;
    }


    void print_coo() const {
        std::cout << "row  col val\n";
        for (size_t i = 0; i < n_rows; ++i)
            for (size_t j = RowIndex[i]; j < RowIndex[i + 1]; ++j)
                std::cout << i << " " << Col[j] << " " << Val[j] << '\n';
    }


    void print_dense() const {
        for (size_t i = 0; i < n_rows; ++i) {
            size_t k = 0;
            for (size_t j = RowIndex[i]; j < RowIndex[i + 1]; ++j, ++k) {
                while (k < Col[j]) {
                    std::cout << 0 << " ";
                    ++k;
                }
                std::cout << Val[j] << " ";
            }
            while (k < n_cols) {
                std::cout << 0 << " ";
                ++k;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }


    // сложение матриц в формате CSR
    CSR_matrix operator+(const CSR_matrix& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols)
            throw std::invalid_argument("matrix sizes must be equal");

        std::vector<size_t> rows(1, 0), cols;
        std::vector<ValT> vals;

        for (size_t i = 0; i < n_rows; ++i) {
            size_t a = RowIndex[i], a_end = RowIndex[i + 1];
            size_t b = other.RowIndex[i], b_end = other.RowIndex[i + 1];

            while (a < a_end || b < b_end) {
                if (b == b_end || (a < a_end && Col[a] < other.Col[b])) {
                    // столбец есть только в первой матрице
                    cols.push_back(Col[a]);
                    vals.push_back(Val[a]);
                    ++a;
                }
                else if (a == a_end || other.Col[b] < Col[a]) {
                    // столбец есть только во второй матрице
                    cols.push_back(other.Col[b]);
                    vals.push_back(other.Val[b]);
                    ++b;
                }
                else {
                    // столбец есть в обеих - складываем значения
                    cols.push_back(Col[a]);
                    vals.push_back(Val[a] + other.Val[b]);
                    ++a;
                    ++b;
                }
            }
            rows.push_back(cols.size());
        }

        CSR_matrix res(n_rows, n_cols, vals.size());
        std::copy(rows.begin(), rows.end(), res.RowIndex);
        std::copy(cols.begin(), cols.end(), res.Col);
        std::copy(vals.begin(), vals.end(), res.Val);
        return res;
    }


    // умножение матрицы на вектор
    std::vector<ValT> operator*(const std::vector<ValT>& x) const {
        if (x.size() != n_cols)
            throw std::invalid_argument("vector size must be equal to the number of columns");

        std::vector<ValT> result(n_rows, 0);
        for (size_t i = 0; i < n_rows; ++i)
            for (size_t j = RowIndex[i]; j < RowIndex[i + 1]; ++j)
                result[i] += Val[j] * x[Col[j]];
        return result;
    }


    // транспонирование матрицы в формате CSR
    CSR_matrix transpose() const {
        CSR_matrix res(n_cols, n_rows, nz);

        // 1. считаем, сколько элементов в каждом столбце исходной матрицы
        for (size_t j = 0; j < nz; ++j)
            ++res.RowIndex[Col[j] + 1];

        // 2. префиксные суммы: получаем начала строк результата
        for (size_t i = 0; i < n_cols; ++i)
            res.RowIndex[i + 1] += res.RowIndex[i];

        // 3. раскладываем элементы (next[c] - куда класть следующий элемент строки c)
        std::vector<size_t> next(res.RowIndex, res.RowIndex + n_cols);
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = RowIndex[i]; j < RowIndex[i + 1]; ++j) {
                size_t pos = next[Col[j]]++;
                res.Col[pos] = i;
                res.Val[pos] = Val[j];
            }
        }
        return res;
    }


    // сравнение матриц (значения сравниваем с небольшой погрешностью)
    bool operator==(const CSR_matrix& other) const {
        if (n_rows != other.n_rows || n_cols != other.n_cols || nz != other.nz)
            return false;

        for (size_t i = 0; i <= n_rows; ++i)
            if (RowIndex[i] != other.RowIndex[i])
                return false;

        for (size_t j = 0; j < nz; ++j) {
            if (Col[j] != other.Col[j])
                return false;
            if (std::abs(Val[j] - other.Val[j]) > 1e-9)
                return false;
        }
        return true;
    }
};


