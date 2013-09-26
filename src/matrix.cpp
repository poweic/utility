#include <matrix.h>
using namespace std;

template <typename T>
Matrix2D<T>::Matrix2D(): _element(NULL) {
  resize(1, 1);
}

template <typename T>
Matrix2D<T>::Matrix2D(int rows, int cols): _element(NULL) {
  resize(rows, cols);
}

template <typename T>
Matrix2D<T>::Matrix2D(const Matrix2D<T>& mat): _element(NULL) {
  *this = mat;
}

template <typename T>
Matrix2D<T>::Matrix2D(string filename, int max_rows, int max_cols): _element(NULL) {
  int rows = getFileLineNumber(filename);
  int cols = getFileColumnNumber(filename);
  if(rows <= 0 || cols <= 0 || max_rows < 0 || max_cols < 0)
    throw util::MyException("Failed to construct Matrix2D, invalid col/row number");

  if(max_rows != 0)
    rows = MIN(max_rows, rows);
  else
    max_rows = rows;

  if(max_cols != 0)
    cols = MIN(max_cols, cols);
  else
    max_cols = cols;

  this->resize(max_rows, max_cols);

  fstream iFile;
  iFile.open(filename.c_str(), fstream::in);

  size_t line_no = 0;
  while(!iFile.eof()) {
    T* row_head = _element[line_no]; // + line_no*_cols;
    for(size_t c=0; c<_cols; ++c)
      iFile >> row_head[c];
    if(++line_no >= _rows)
      break;
  }

  iFile.close();
}

template <typename T>
Matrix2D<T>::~Matrix2D() {
  if(_element != NULL) {
    for(size_t i=0; i<_rows; ++i)
      delete [] _element[i];
    delete [] _element;
  }
}

template <typename T>
T* & Matrix2D<T>::operator[](int row) { 
  return _element[row]; 
}

template <typename T>
T* const& Matrix2D<T>::operator[](int row) const {
  return _element[row]; 
}

template <typename T>
T& Matrix2D<T>::get(size_t x, size_t y) const {
  assert(x >= 0 && x < _rows && y >= 0 && y < _cols);
  return _element[x][y];
}

template <typename T>
T* Matrix2D<T>::getRows(size_t r) const {
  assert(r >= 0 && r < _rows);
  return _element[r];
}

// Cross Some Columns
template <typename T>
void DataSheet<T>::cross_some_cols(bool* cross_matrix, int dc) {
  DataSheet<T> temp(Matrix2D<T>::_rows, Matrix2D<T>::_cols - dc);

  int start = 0, c_counter = 0;
  for(size_t i=0; i<Matrix2D<T>::_cols; i++) {
    if(cross_matrix[i]) {
      for(size_t r=0; r<Matrix2D<T>::_rows; r++)
	memcpy(temp.getRows(r) + c_counter, this->getRows(r) + start, (i-start)*sizeof(T));
      c_counter += (i-start);
      start = i+1;
    }
  }

  if(start <= Matrix2D<T>::_cols - 1) {
    for(size_t r=0; r<Matrix2D<T>::_rows; r++)
      memcpy(temp.getRows(r) + c_counter, this->getRows(r) + start, (Matrix2D<T>::_cols-start)*sizeof(T));
  }

  *this = temp;
}

// Cross Some Rows
template <typename T>
void DataSheet<T>::cross_some_rows(bool* cross_matrix, int dr) {
  DataSheet<T> temp(Matrix2D<T>::_rows - dr, Matrix2D<T>::_cols);

  int r_counter = 0;
  for(size_t i=0; i<Matrix2D<T>::_rows; i++) {
    if(!cross_matrix[i])
      memcpy(temp.getRows(r_counter++), this->getRows(i), Matrix2D<T>::_cols*sizeof(T));
  }
  *this = temp;
}

// Assignment operator
template <typename T>
Matrix2D<T>& Matrix2D<T>::operator = (const Matrix2D<T>& rhs) {
  if(this == &rhs) return *this;

  resize(rhs._rows, rhs._cols);
  for(size_t i=0; i<_rows; ++i)
    memcpy(_element[i], rhs._element[i], _cols*sizeof(T));
  return *this;
}

template <typename T> template <typename S>
Matrix2D<T>& Matrix2D<T>::operator = (const Matrix2D<S>& rhs) {
  resize(rhs.getRows(), rhs.getCols());
  for(size_t i=0; i<_rows; ++i)
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] = static_cast<T>(rhs[i][j]);
  return *this;
}

// Operator +, +=
template <typename T>
Matrix2D<T> Matrix2D<T>::operator+(const Matrix2D<T>& rhs) const {
  Matrix2D<T> temp(*this);
  return temp += rhs;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator+=(const Matrix2D<T>& rhs) {
  if(!isMatched(rhs))
    throw util::MyException("In operator +=, matrix dimensions mismatched.");

  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] += rhs._element[i][j];
  }
  return *this;
}

// Operator -, -=
template <typename T>
Matrix2D<T> Matrix2D<T>::operator-(const Matrix2D<T>& rhs) const {
  Matrix2D<T> temp(*this);
  return temp -= rhs;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator-=(const Matrix2D<T>& rhs) {
  if(!isMatched(rhs))
    throw util::MyException("In operator -=, matrix dimensions mismatched.");

  for(size_t i=0; i<_rows; i++) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] -= rhs._element[i][j];
  }
  return *this;
}

// Operator *, *=

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator*=(const Matrix2D<T>& rhs) {
  *this = *this * rhs;
  return *this;
}

template <typename T>
Matrix2D<T> Matrix2D<T>::operator*(const Matrix2D<T>& rhs) const {
  assert(_cols == rhs._rows);
  Matrix2D<T> temp(_rows, rhs._cols);

  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<rhs._cols; ++j) {

      T sum = 0;
      for(size_t k=0; k<_cols; ++k)
	sum += _element[i][k] * rhs._element[k][j];
      temp._element[i][j] = sum;
    }
  }

  return temp;
}

// Math operation with scalar c
template <typename T>
Matrix2D<T> Matrix2D<T>::operator+(T c) const {
  Matrix2D<T> temp(*this);
  return temp += c;
}

template <typename T>
Matrix2D<T> Matrix2D<T>::operator-(T c) const {
  Matrix2D<T> temp(*this);
  return temp -= c;
}

template <typename T>
Matrix2D<T> Matrix2D<T>::operator*(T c) const {
  Matrix2D<T> temp(*this);
  return temp *= c;
}

template <typename T>
Matrix2D<T> Matrix2D<T>::operator/(T c) const {
  Matrix2D<T> temp(*this);
  return temp /= c;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator+=(T c) {
  for(size_t i=0; i<_rows; i++) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] += c;
  }
  return *this;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator-=(T c) {
  for(size_t i=0; i<_rows; i++) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] -= c;
  }
  return *this;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator*=(T c) {
  for(size_t i=0; i<_rows; i++) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] *= c;
  }
  return *this;
}

template <typename T>
Matrix2D<T>& Matrix2D<T>::operator/=(T c) {
  for(size_t i=0; i<_rows; i++) {
    for(size_t j=0; j<_cols; ++j)
      _element[i][j] /= c;
  }
  return *this;
}

// ====================================
// matrix inversioon
// the result is put in Y
  template <typename T>
Matrix2D<T> Matrix2D<T>::inv(const Matrix2D<T>& A)
{
  assert(A._rows == A._cols);
  size_t order = A._rows;

  Matrix2D<T> Y(order, order);

  // memory allocation
  Matrix2D<T> minor_matrix(order-1, order-1);

  for(size_t j=0; j<order; ++j) {
    for(size_t i=0; i<order; ++i) {
      // get the co-factor (matrix) of A(j,i)
      GetMinor(A, minor_matrix, j, i);
      Y[i][j] = Matrix2D<T>::det(minor_matrix);
      if( (i+j) % 2 == 1)
	Y[i][j] = -Y[i][j];
    }
  }
  T test = Matrix2D<T>::det(A);
  cout << "test = " << test << endl;
  Y *= 1.0/Matrix2D<T>::det(A);
  return Y;
}

// calculate the cofactor of element (row,col)
  template <typename T>
int Matrix2D<T>::GetMinor(const Matrix2D<T>& src, Matrix2D<T>& dest, size_t row, size_t col)
{
  assert(src._rows == src._cols);
  assert(dest._rows == dest._cols);
  assert(src._rows == dest._rows + 1);
  size_t order = src._rows;

  // indicate which col and row is being copied to dest
  int colCount=0, rowCount=0;

  for(size_t i=0; i<order; ++i) {
    if( i != row ) {
      colCount = 0;
      for(size_t j=0; j<order; ++j)
	if( j != col )	// when j is not the element
	  dest[rowCount][colCount++] = src[i][j];
      ++rowCount;
    }
  }

  return 1;
}

// Calculate the determinant recursively.
  template <typename T>
T Matrix2D<T>::det(const Matrix2D<T>& mat)
{
  assert(mat._rows == mat._cols);
  size_t order = mat._rows;

  // order must be >= 0
  // stop the recursion when matrix is a single element
  if( order == 2 ) {
    T** e = mat._element;
    return e[0][0]*e[1][1] - e[1][2]*e[2][1];
    //return mat(0, 0);
  }

  // the determinant value
  float det = 0;

  // Allocate the cofactor matrix
  Matrix2D<T> minor_matrix(order-1, order-1);

  for(size_t i=0; i<order; ++i) {
    GetMinor(mat, minor_matrix, 0, i);

    det += (i%2==1?-1.0:1.0) * mat[0][i] * Matrix2D<T>::det(minor_matrix);
  }

  return det;
}

// ====================================

// Operator ~ (transpose)
template <typename T>
Matrix2D<T> Matrix2D<T>::operator~() const {
  Matrix2D<T> temp(_cols, _rows);

  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<_cols; ++j)
      temp._element[j][i] = _element[i][j];
  }
  return temp;
}

// Type Conversion (Matrix => Array)
template <typename T>
Matrix2D<T>::operator Array<T>() {
  Array<T> arr(_size);
  int idx=0;
  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<_cols; ++j)
      arr[idx++] = _element[i][j];
  }
  return arr;
}

// Size Compatible
template <typename T>
bool Matrix2D<T>::isMatched(const Matrix2D<T>& rhs) const {
  return (_rows != rhs.getRows() || _cols != rhs.getCols())?(false):(true);
}

template <typename T>
bool Matrix2D<T>::isMatched(const Matrix2D<T>& lhs, const Matrix2D<T>& rhs) {
  return (lhs.getRows() != rhs.getRows() || lhs.getCols() != rhs.getCols())?(false):(true);
}

template <typename T>
bool Matrix2D<T>::isSymmetric() const {
  for(size_t i=0; i<_rows; ++i)
    for(size_t j=0; j<i; ++j)
      if(_element[i][j] != _element[j][i])
	return false;
  return true;
}
// Resize
template <typename T>
void Matrix2D<T>::resize(int rows, int cols) {
  if(_element != NULL) {
    for(size_t i=0; i<_rows; ++i)
      delete [] _element[i];
    delete [] _element;
  }

  _rows = MAX(rows, 1);
  _cols = MAX(cols, 1);
  _size = _rows*_cols;
  _element = new T*[_rows];
  for(size_t i=0; i<_rows; ++i) {
    _element[i] = new T[_cols];
    memset(_element[i], 0, _cols*sizeof(T));
  }
}

template <typename T>
void Matrix2D<T>::saveas(string filename) const {
  fstream oFile;
  oFile.open(filename.c_str(), fstream::out);

  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<_cols; ++j)
      oFile << _element[i][j] << " ";
    oFile << endl;
  }
  oFile.close();
}

template <typename T>
void Matrix2D<T>::saveAsBinary(string filename) const {
  FILE* filePtr = fopen(filename.c_str(), "w");
  fseek (filePtr, 0L, SEEK_SET);

  fwrite( &_rows, sizeof(_rows), 1, filePtr);
  fwrite( &_cols, sizeof(_cols), 1, filePtr);

  for(size_t i=0; i<_rows; ++i)
    fwrite( _element[i], sizeof(T), _cols, filePtr);

  fclose(filePtr);
}

template <typename T>
void Matrix2D<T>::readAsBinary(string filename) {
  FILE* filePtr = fopen(filename.c_str(), "r");
  if(filePtr == NULL)
    throw util::MyException("Failed to read binary file. File: " + filename + " may not exist");

  fseek (filePtr, 0L, SEEK_SET);

  size_t rows, cols;
  fread( &rows, sizeof(_rows), 1, filePtr);
  fread( &cols, sizeof(_cols), 1, filePtr);
  this->resize(rows, cols);

  for(size_t i=0; i<_rows; ++i)
    fread( _element[i], sizeof(T), _cols, filePtr);

  fclose(filePtr);
}

template <typename T>
void Matrix2D<T>::row_sorting(int pivot_column) {
  Array<util::kvPair<int, T> > kvpairs(_rows);
  for(size_t i=0; i<_rows; i++) {
    kvpairs[i]._key = i;
    kvpairs[i]._value = this->get(i, pivot_column);
  }

  util::sorting::sort(kvpairs, util::sorting::descend);

  Matrix2D<T> temp(_rows, _cols);
  for(size_t i=0; i<_rows; i++)
    memcpy(temp.getRows(i), this->getRows(kvpairs[i]._key), _cols*sizeof(T));

  *this = temp;
}


// Print to terminal
template <typename T>
void Matrix2D<T>::printDiag(int precision) const {
  Array<T> temp(_rows);
  for(size_t i=0; i<_rows; ++i)
    temp[i] = _element[i][i];
  temp.print();
}

template <typename T>
void Matrix2D<T>::print(int precision) const {

  int maxLength = 0;
  for(size_t i=0; i<_rows; ++i) {
    for(size_t j=0; j<_cols; ++j) {
      double temp = abs((double) _element[i][j]);
      double l = (temp > 1) ? (ceil(log10(temp))) : (precision);

      if(_element[i][j] < 0)
	++l;
      if(l > maxLength)
	maxLength = l;
    }
  }

  char pFormat[20];
  sprintf(pFormat, "%s%df", "%.", precision);
  maxLength += (maxLength < precision) ? (precision) : (3);

  cout << endl;
  for(size_t i=0; i<_rows; i++) {
    if(i == 0)
      cout << " / ";
    else if(i == _rows-1)
      cout << " \\ ";
    else
      cout << "|  ";

    for(size_t j=0; j<_cols; j++) {
      char buf[20];
      sprintf(buf, pFormat, (double) _element[i][j]);
      cout << setw(maxLength) << buf << " ";
    }

    if(i == 0)
      cout << " \\";
    else if(i == _rows-1)
      cout << " /";
    else
      cout << "  |";
    cout << endl;
  }
}

// Load mat file
int getFileLineNumber(string filename) {

  FILE* file = fopen(filename.c_str(), "r");
  if(file == NULL)
    throw util::MyException("Failed to open file: " + filename);

  int ch, nLines = 0;

  do {
    ch = fgetc(file);
    if(ch == '\n')
      ++nLines;
  } while (ch != EOF);

  fclose(file);

  // last line doesn't end with a new line!
  // but there has to be a line at least before the last line
  if(ch != '\n' && ch != EOF && nLines != 0) 
    ++nLines;

  return nLines;
}

int getFileColumnNumber(string filename) {
  fstream iFile;
  iFile.open(filename.c_str(), fstream::in);

  if(!iFile.is_open())
    throw util::MyException("Failed to open file: " + filename);

  int counter =0;
  if(!iFile.eof()) {

    char buffer[65536];
    iFile.getline(buffer, 65536);
    string text = string(buffer);
    if(text.compare(text.size()-1, 1, " ") != 0)
      text.append(" ");

    size_t pos = 0;
    size_t pos2 = 0;

    while((pos2 = text.find(" ", pos)) != string::npos) {
      counter++;
      pos = pos2+1;
    }
  }
  iFile.close();
  return counter;
}

double diagMatDet(const mat& m) {
  double det = 1;
  for(size_t i=0; i<m.getRows(); ++i)
    det *= m[i][i];
  return det;
}

// To keep implementation seperated, explicitly instantiate all the template instances you'll need 
template class Matrix2D<int>;
template class Matrix2D<double>;
template Matrix2D<double>& Matrix2D<double>::operator = (const Matrix2D<long double>& rhs);
template Matrix2D<double>::Matrix2D(const Matrix2D<long double>& mat);
template class Matrix2D<long double>;
