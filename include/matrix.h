#ifndef MATRIX_H
#define MATRIX_H


#include <vector>
#include <initializer_list>
#include <string>
#include <memory>


// Matrix
namespace tung {

using std::unique_ptr;

class MatrixReader;

class Matrix {
private:
	friend MatrixReader;
	int m, n;
	unique_ptr<int[]> data;

	/*            n                                        
	  +------------------------+ j                         
        m | (0, 0)  (0, 1)  (0, 2) |         
	  | (1, 0)  (1, 1)  (1, 2) |         
	  +------------------------+        
	  i                                                    
	*/

public:
	Matrix();
	Matrix(int m, int n);
	Matrix(int m, int n, std::initializer_list<int> list);
	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;

	Matrix& operator = (const Matrix& other);
	Matrix& operator = (Matrix&& other) noexcept;

	int& operator () (int i, int j) noexcept;
	int operator () (int i, int j) const noexcept;
	
	int get_m() const { return m;}
	int get_n() const { return n;}

	void print() const;

	~Matrix();
};

} // namespace tung


// Matrix Reader
namespace tung {

class MatrixReader {
public:
	Matrix read(std::string filename);
};

} // namespace tung

#endif
