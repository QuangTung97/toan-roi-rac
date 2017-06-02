#include <stdio.h>
#include <matrix.h>
#include <limits>
#include <string>
#include <fstream>


// Matrix Implement
namespace tung {

Matrix::Matrix(): m{0}, n{0}, data{nullptr} {}

Matrix::Matrix(int m, int n): 
    m{m}, n{n}, data{std::make_unique<int[]>(m * n)} 
{ 
	const int max_int = std::numeric_limits<int>::max();
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (i == j)
				(*this)(i, j) = 0;
			else 
				(*this)(i, j) = max_int;
}

Matrix::Matrix(int m, int n, 
               std::initializer_list<int> list):
	m{m}, n{n}, data{std::make_unique<int[]>(m * n)}
{
	const int max_int = std::numeric_limits<int>::max();
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (i == j)
				(*this)(i, j) = 0;
			else 
				(*this)(i, j) = max_int;

	int i = 0;
	for (int e: list) {
		data[i++] = e;
		if (i >= m * n)
			return;
	}
}

Matrix::Matrix(const Matrix& other):
	m{other.m}, n{other.n}, 
    data{std::make_unique<int[]>(m * n)} 
{
	for (int i = 0; i < m * n; i++)
		data[i] = other.data[i];
}

Matrix::Matrix(Matrix&& other) noexcept:
	m{other.m}, n{other.n}, data{std::move(other.data)} {}

Matrix& Matrix::operator = (const Matrix& other) {
	m = other.m;
	n = other.n;
	data = std::make_unique<int[]>(m * n);

	for (int i = 0; i < m * n; i++)
		data[i] = other.data[i];
	return *this;
}

Matrix& Matrix::operator = (Matrix&& other) noexcept {
	m = other.m;
	n = other.n;
	data = std::move(other.data);
	return *this;
}

Matrix::~Matrix() {}

int& Matrix::operator () (int i, int j) noexcept {
	return data[i * n + j];
}

int Matrix::operator () (int i, int j) const noexcept {
	return data[i * n + j];
}

void Matrix::print() const {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf("%4d ", data[i * n + j]);
		}
		printf("\n");
	}
}

} // namespace tung


// Matrix Reader Implement
namespace tung {

using std::string;
using std::ifstream;
using std::ios;

Matrix MatrixReader::read(string filename) {
	int n;
	ifstream file;

	file.open(filename, ios::in);
	if (!file) 
		throw std::ios_base::failure{"File not found"};

	file >> n;
	Matrix mat{n, n};
	
	for (int i = 0; i < n * n; i++) {
		file >> mat.data[i];
	}
	return mat;
}

}
