// ---------------------------------------------------
// Name: Daniel Sacro
// CMPUT 275, Winter 2021
//
// Assignment 4: Matrix Class
// ---------------------------------------------------
#include <iostream>
using namespace std;

class Matrix {
public:
	float **mtrxArray;

	// Constructors
	Matrix(size_t num_rows, size_t num_columns, float init);

	Matrix(size_t num_rows, size_t num_columns, float *arr_ptr);

	// Copy constructor
	Matrix(const Matrix &copy);

	// Matrix arithmetic
	Matrix operator+(Matrix rhs);
	Matrix operator-(Matrix rhs);
	Matrix operator*(Matrix rhs);

	// Unary negation
	Matrix operator-();

	// Matrix transposition
	Matrix transpose();

	// Submatrix
	Matrix submatrix(size_t row_start, size_t row_end, size_t column_start, size_t column_end);

	// Row indexing
	float *operator[](int index) const;

	// Destructor
	~Matrix();

	// iostream support for matrices
	friend istream &operator>>(istream &in, Matrix &mtrx);
	friend ostream &operator<<(ostream &out, Matrix mtrx);

private:
	size_t totRows, totCols;
	float initVal;
};