#ifndef _MATRIX_2D_H
#define _MATRIX_2D_H

//#pragma GCC diagnostic ignored "-Wunused-result"

#ifndef MAX_BUFFER_LENGTH
#define MAX_BUFFER_LENGTH 262144
#endif

#include <cassert>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

//#include <vector.h>
#include <array.h>
#include <sorting.h>
#include <stdexcept>

using namespace std;

template <typename T> class Matrix2D;

// template <typename T> Matrix2D<T> operator*(const MyVector<T>& vector, const Matrix2D<T>& mat);
// template <typename T> Matrix2D<T> operator*(const Matrix2D<T>& mat, const MyVector<T>& vector);

template <typename T>
class Matrix2D {
public:
    Matrix2D();
    Matrix2D(int rows, int cols);
    Matrix2D(const Matrix2D<T>& mat);
    Matrix2D(string filename, int max_rows = 0, int max_cols = 0);
    ~Matrix2D();

    // Conversion 
    //operator Array<T>();

    // Assignment
    Matrix2D<T>& operator = (const Matrix2D<T>& rhs);

    template <typename S> Matrix2D(const Matrix2D<S>& mat): _element(NULL) { *this = mat; }
    template <typename S> Matrix2D<T>& operator = (const Matrix2D<S>& rhs);

    // Get Function
    size_t size() const { return _rows * _cols; }
    size_t getRows() const { return _rows; }
    size_t getCols() const { return _cols; }
   
    T& get(size_t x, size_t y) const;
    T* getRows(size_t r) const;

    T* & operator[](int row);
    T* const& operator[](int row) const; 

    // Friend (NOTE!! This function must be placed before operator*)
    // friend Matrix2D<T> operator*<T>(const MyVector<T>&, const Matrix2D<T>&);
    // friend Matrix2D<T> operator*<T>(const Matrix2D<T>&, const MyVector<T>&);
    //friend ofstream& operator << <> (ofstream& ofs, const Matrix2D<T>& matrix);

    // Math Operation
    Matrix2D<T>	operator+(const Matrix2D<T>& rhs) const;
    Matrix2D<T> operator-(const Matrix2D<T>& rhs) const;
    Matrix2D<T> operator*(const Matrix2D<T>& rhs) const;
    Matrix2D<T>& operator+=(const Matrix2D<T>& rhs);
    Matrix2D<T>& operator-=(const Matrix2D<T>& rhs);
    Matrix2D<T>& operator*=(const Matrix2D<T>& rhs);
    Matrix2D<T> operator~() const;

    Matrix2D<T>	operator+(T c) const;
    Matrix2D<T> operator-(T c) const;
    Matrix2D<T> operator*(T c) const;
    Matrix2D<T> operator/(T c) const;
    Matrix2D<T>& operator+=(T c);
    Matrix2D<T>& operator-=(T c);
    Matrix2D<T>& operator*=(T c);
    Matrix2D<T>& operator/=(T c);

    static Matrix2D<T> inv(const Matrix2D<T>& mat);
    static Matrix2D<T> pinv(const Matrix2D<T>& mat);
    static T det(const Matrix2D<T>& mat);
    static T trace(const Matrix2D<T>& mat);
    static int GetMinor(const Matrix2D<T>& src, Matrix2D<T>& dest, size_t row, size_t col);

    // Utility Functions
    void resize(int rows, int cols);
    void saveas(string filename) const;
    void saveAsBinary(string filename) const;
    void readAsBinary(string filename);

    void row_sorting(int pivot_column);
    void print(int precision = 1) const;
    void printDiag(int precision = 1) const;

    bool isMatched(const Matrix2D<T>& rhs) const;
    static bool isMatched(const Matrix2D<T>& lhs, const Matrix2D<T>& rhs);
    bool isScalar() const { return (_rows == 1) && (_cols == 1);}
    bool isSymmetric() const;

protected:
    size_t _rows;
    size_t _cols;
    size_t _size;
   
    T** _element;
};

template <typename T>
class DataSheet: public Matrix2D<T> {
public:
    // Constructors
    DataSheet(): Matrix2D<T>() {}
    DataSheet(int rows, int cols): Matrix2D<T>(rows, cols) {}
    DataSheet(const DataSheet<T>& source): Matrix2D<T>(source) {}
    DataSheet(string filename, int max_rows = 0, int max_cols = 0): Matrix2D<T>(filename, max_rows, max_cols) {}
    
    void cross_some_cols(bool* cross_matrix, int dc);
    void cross_some_rows(bool* cross_matrix, int dr);
};

int getFileLineNumber(string filename);
int getFileColumnNumber(string filename);

template <typename T>
Matrix2D<T> operator*(T c, const Matrix2D<T>& rhs) {
    return rhs*c;
}

/*template <typename T>
Matrix2D<T> operator*(const MyVector<T>& vector, const Matrix2D<T>& mat) {
    Matrix2D<T> temp(1, vector.size());
    memcpy(temp._element[0], &vector[0], vector.size()*sizeof(T));
    return temp*mat;
}*/

/*template <typename T>
Matrix2D<T> operator*(const Matrix2D<T>& mat, const MyVector<T>& vector) {
    Matrix2D<T> temp(vector.size(), 1);
    for(int i=0; i<vector.size(); ++i)
	temp._element[i][0] = vector[i];
    return mat*temp;
}*/

double diagMatDet(const Matrix2D<double>& m);

#endif
