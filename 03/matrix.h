[[noreturn]] void error() { throw std::out_of_range(""); }

class Matrix {
private:
    const size_t rows;
    const size_t cols;
    int* arr;
    class Row {
    private:
        const size_t size;
        int* row;
    public:
        Row(size_t s, int* r)
            : size(s), row(r) {}
        int operator[](size_t j) const {
            if (j >= size) error();
            return row[j];
        }
        int& operator[](size_t j) {
            if (j >= size) error();
            return row[j];
        }
    };
public:
    Matrix(size_t r, size_t c)
        : rows(r), cols(c), arr(new int[r * c]) {}
    size_t getRows() const {
        return rows;
    }
    size_t getColumns() const {
        return cols;
    }
    Matrix& operator*=(int a) {
        for (size_t i = 0; i < rows * cols; ++i)
            arr[i] *= a;
        return *this;
    }
    Row operator[](size_t i) {
        if (i >= rows) error();
        return Row(cols, arr + cols * i);
    }
    const Row operator[](size_t i) const {
        if (i >= rows) error();
        return Row(cols, arr + cols * i);
    }
    bool operator==(const Matrix& A) const {
        if (rows != A.getRows() || cols != A.getColumns())
            return false;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (arr[i * cols + j] != A[i][j])
                    return false;
            }
        }
        return true;
    }
    bool operator!=(const Matrix& A) const {
        return !(*this == A);
    }
    ~Matrix() { delete[] arr; }
};
