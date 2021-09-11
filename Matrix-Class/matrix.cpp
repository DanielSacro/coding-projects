// ---------------------------------------------------
// Name: Daniel Sacro
// CMPUT 275, Winter 2021
//
// Assignment 4: Matrix Class
// ---------------------------------------------------
#include "matrix.h"
#include <iostream>
#include <cassert>
using namespace std;

Matrix::Matrix(size_t num_rows, size_t num_columns, float init) {
	/* 	Description: The first constructor that builds a matrix of size num_rows
					 x num_columns and fills every entry with the float value init.

		Arguments:
			num_rows (int): Number of rows in the matrix
			num_columns (int): Number of columns in the matrix
			init (float): Initial float value for every element in the matrix

		Returns:
			Nothing
	*/
	totRows = num_rows;
	totCols = num_columns;
	initVal = init;

	// allocates memory for 2D array
	mtrxArray = new float *[totRows];
	for (int i = 0; i < totRows; i++) {
		mtrxArray[i] = new float[totCols];
	}

	// assigns value of init to every entry in 2D array
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			mtrxArray[r][c] = initVal;
		}
	}
}

Matrix::Matrix(size_t num_rows, size_t num_columns, float *arr_ptr) {
	/* 	Description: The second constructor that builds a matrix of size
					 num_rows x num_columns and fills every entry with the
					 contents of arr_ptr. Entries are filled up completely
					 row by row (i.e first row is filled up first left to
					 right, then the second, etc.).

		Arguments:
			num_rows (int): Number of rows in the matrix
			num_columns (int): Number of columns in the matrix
			arr_ptr (pointer to float array): An array of floats

		Returns:
			Nothing
	*/
	totRows = num_rows;
	totCols = num_columns;
	float initVals[totRows * totCols + 1];

	// temp array so original array cannot be accidentally modified
	for (int j = 0; j < totRows * totCols; j++) {
		initVals[j] = *(arr_ptr + j);
	}

	// allocates memory for 2D array
	mtrxArray = new float *[totRows];
	for (int i = 0; i < totRows; i++) {
		mtrxArray[i] = new float[totCols];
	}

	int arrElement = 0;
	// assigns initial values to 2D array
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			mtrxArray[r][c] = initVals[arrElement];
			arrElement += 1;
		}
	}
}

Matrix::~Matrix() {
	/* 	Description: A class destuctor that frees up any dynamically
					 allocated memory space being used by the matrix.

		Arguments:
			None

		Returns:
			Nothing
	*/
	// frees up the memory space and removes stale pointers
	for (int j = 0; j < totRows; j++) {
		delete[] mtrxArray[j];
		mtrxArray[j] = NULL;
	}
}

Matrix::Matrix(const Matrix &copy) {
	/* 	Description: A copy constructor that creates a deep
					 copy of a matrix when it's called.

		Arguments:
			copy (Address of matrix): The address of the matrix being
									  copied.

		Returns:
			Nothing
	*/
	totRows = copy.totRows;
	totCols = copy.totCols;

	// allocates memory for deep copy
	mtrxArray = new float *[totRows];
	for (int i = 0; i < totRows; i++) {
		mtrxArray[i] = new float[totCols];
	}

	// copies contents of original copy
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			mtrxArray[r][c] = copy.mtrxArray[r][c];
		}
	}
}

Matrix Matrix::operator+(Matrix rhs) {
	/* 	Description: An operator overload for the plus operator. Allows
					 for matrix addition between two matrices with the 
					 same dimensions.

		Arguments:
			rhs (Matrix): The matrix that's on the right hand side of the
						  plus operator.

		Returns:
			A new matrix instance containing the resulting matrix sum.
	*/
	float tempArray1[totRows * totCols + 1];
	int indCounter1 = 0;

	// sums up matrices
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			tempArray1[indCounter1] = (mtrxArray[r][c]) + (rhs.mtrxArray)[r][c];
			indCounter1 += 1;
		}
	}

	return Matrix(totRows, totCols, tempArray1);
}

Matrix Matrix::operator-(Matrix rhs) {
	/* 	Description: An operator overload for the minus operator. Allows
					 for matrix subtraction between two matrices with
					 the same dimensions.

		Arguments:
			rhs (Matrix): The matrix that's on the right hand side of the
						  minus operator.

		Returns:
			A new matrix instance containing the resulting matrix difference.
	*/
	float tempArray2[totRows * totCols + 1];
	int indCounter2 = 0;

	// finds difference in matrix elements
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			tempArray2[indCounter2] = (mtrxArray[r][c]) - (rhs.mtrxArray)[r][c];
			indCounter2 += 1;
		}
	}

	return Matrix(totRows, totCols, tempArray2);
}

Matrix Matrix::operator*(Matrix rhs) {
	/* 	Description: An operator overload for the multiplication operator.
					 Allows for matrix multiplication between two matrices with
					 compatible dimensions (i.e A matrix of size l x m and
					 another of size m x n).

		Arguments:
			rhs (Matrix): The matrix that's on the right hand side of the
						  multiplication operator.

		Returns:
			A new matrix instance containing the resulting matrix product.
	*/
	// Matrix dimensions must be compatible
	assert(totCols == rhs.totRows);

	float tempArray3[totRows * rhs.totCols + 1];
	int indCounter3 = 0;

	// finds matrix product
	for (int l = 0; l < totRows; l++) {
		// dot product between rows and columns
		for (int n = 0; n < rhs.totCols; n++) {
			float sum = 0;
			for (int m = 0; m < totCols; m++) {
				sum += mtrxArray[l][m] * (rhs.mtrxArray)[m][n];
			}
			tempArray3[indCounter3] = sum;
			indCounter3 += 1;
		}
	}

	return Matrix(totRows, rhs.totCols, tempArray3);
}

Matrix Matrix::operator-() {
	/* 	Description: An operator overload for the minus operator.
					 Allows for unary negation on a matrix (switching the sign
					 of every float inside).

		Arguments:
			None

		Returns:
			A new matrix instance containing the same values as the old matrix,
			but with each entry having its sign switched.
	*/
	float tempArray4[totRows * totCols + 1];
	int indCounter4 = 0;

	// switches sign of each entry
	for (int r = 0; r < totRows; r++) {
		for (int c = 0; c < totCols; c++) {
			tempArray4[indCounter4] = -(mtrxArray[r][c]);
			indCounter4 += 1;
		}
	}

	return Matrix(totRows, totCols, tempArray4);
}

Matrix Matrix::transpose()
{
	/* 	Description: Performs transposition on a matrix causing rows
					 to become columns and columns to become rows.

		Arguments:
			None

		Returns:
			A new matrix instance containing the resulting transposed
			matrix.
	*/
	float tempArray5[totRows * totCols + 1];
	int indCounter5 = 0;

	// temporarily saves matrix contents in a different order so that
	// a new transposed matrix can directly be made using the temp array.
	for (int c = 0; c < totCols; c++) {
		for (int r = 0; r < totRows; r++) {
			tempArray5[indCounter5] = (mtrxArray[r][c]);
			indCounter5 += 1;
		}
	}

	return Matrix(totCols, totRows, tempArray5);
}

Matrix Matrix::submatrix(size_t row_start, size_t row_end, size_t column_start, size_t column_end) {
	/* 	Description: Finds the desired submatrix of a matrix that
					 contains rows row_start to row_end - 1 and
					 columns column_start to column_start - 1.

		Arguments:
			row_start (int): The starting row to include in the
							 submatrix.
			row_end (int): The last row to stop including rows at.
			column_start (int): The starting column to include in
								the submatrix.
			column_end (int): The last column to stop including
							  columns at.

		Returns:
			A new matrix instance containing the resuling submatrix.
	*/
	int totalRows = row_end - row_start;
	int totalCols = column_end - column_start;
	float tempArray6[totalRows * totalCols + 1];
	int indCounter6 = 0;

	// temporarily saves specific contents of a matrix in such an order
	// that a new submatrix can directly be made using the temp array.
	for (int r = row_start; r < row_end; r++) {
		for (int c = column_start; c < column_end; c++) {
			tempArray6[indCounter6] = (mtrxArray[r][c]);
			indCounter6 += 1;
		}
	}

	return Matrix(totalRows, totalCols, tempArray6);
}

float *Matrix::operator[](int index) const {
	/* 	Description: An operator overload for the bracket operator.
					 Allows access to any row in the matrix as
					 specified by the index argument.

		Arguments:
			index (int): The row of the matrix to be accessed.

		Returns:
			A pointer to a float array. The float array is a row of
			the matrix.
	*/
	return mtrxArray[index];
}

istream &operator>>(istream &in, Matrix &mtrx) {
	/* 	Description: An operator overload for the extraction operator.
					 Allows for floats to be inputted into a matrix.

		Arguments:
			in (address of istream): Address of in
			mtrx (address of matrix): Address of mtrx

		Returns:
			in (address of istream): Address of in
	*/
	float inputFloat;

	// Float inputs are directly placed into the matrix
	for (int a = 0; a < mtrx.totRows; a++) {
		for (int b = 0; b < mtrx.totCols; b++) {
			cin >> inputFloat;
			(mtrx.mtrxArray)[a][b] = inputFloat;
		}
	}

	// in is unused, but must be returned
	return in;
}

ostream &operator<<(ostream &out, Matrix mtrx) {
	/* 	Description: An operator overload for the extraction operator.
					 Allows for matrices to be displayed in the terminal
					 exactly how it would be visualized physically.

		Arguments:
			out (address of ostream): Address of out
			mtrx (Matrix): The matrix on the right hand side of the
						   extraction operator to be outputted.

		Returns:
			out (address of ostream): Address of out
	*/
	for (int d = 0; d < mtrx.totRows - 1; d++) {
		for (int e = 0; e < mtrx.totCols - 1; e++) {
			cout << (mtrx.mtrxArray)[d][e] << " ";
		}
		// last float in a row is printed without a trailing space
		cout << (mtrx.mtrxArray)[d][mtrx.totCols - 1];
		cout << endl;
	}

	// last row in a matrix is printed without a trailing new line
	// character
	for (int f = 0; f < mtrx.totCols - 1; f++) {
		cout << (mtrx.mtrxArray)[mtrx.totRows - 1][f] << " ";
	}
	// last row in a float is again printed without a trailing space
	cout << (mtrx.mtrxArray)[mtrx.totRows - 1][mtrx.totCols - 1];

	// out is unused, but must be returned
	return out;
}
