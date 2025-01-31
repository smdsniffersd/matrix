#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>

class fmatrix2024 {



public:

	fmatrix2024() : A(nullptr), rc(0), cc(0) {};                                                            // конструктор по умолчанию
	fmatrix2024(unsigned rc, unsigned cc);                                                                  // конструктор
	fmatrix2024(unsigned rc, unsigned cc, float inf, float sup);                                            // конструктор полный
	fmatrix2024(const fmatrix2024& other);                                                                  // конструктор копирования

	~fmatrix2024();                                                                                         // деструктор


	void SwapCols(float** A, unsigned c1, unsigned c2, unsigned rc);                                        // поменять строки
	void SwapRows(float** A, unsigned r1, unsigned r2, unsigned rc);                                        // поменять столбцы
	void PrintMatrix() const;                                                                               // вывести матрицу в консоль
	float** CreateMatrix(unsigned rc, unsigned cc);                                                         // создать матрицу
	float** GenerateMatrix(float inf, float sup);                                                           // заполнить матрицу числами в диапазоне от до
	void DeleteMatrix();																					// функция удаления матриц
	void ExportMatrix(const char* filename);																// функция экспорта матрицы
	void ImportMatrix(const char* filename, unsigned rc, unsigned cc);										// импортировать матрицу
	fmatrix2024 gaussSLAE(fmatrix2024& A_matrix, fmatrix2024& B_matrix, unsigned order);                    // посчитать по гауссу
	fmatrix2024 operator*(const fmatrix2024& B);															// оператор умножения                         
	fmatrix2024& operator=(const fmatrix2024& other);                                                       // Копирующее присваивание
	fmatrix2024& operator=(fmatrix2024&& matrix) noexcept;                                                  // Перемещающее присваивание
	float* operator[](unsigned row);                                                                        // оператор квадратные скобки
	const float* operator[](unsigned row) const;                                                            // оператор квадратные скобки для констант




private:

	float** A;        // Указатель на матрицу
	unsigned rc;      // Количество строк
	unsigned cc;      // Количество столбцов

};


////// Constructor /////////////

fmatrix2024::fmatrix2024(const fmatrix2024& other) {
	rc = other.rc;
	cc = other.cc;

	A = new float* [rc];
	for (unsigned i = 0; i < rc; ++i) {
		A[i] = new float[cc];
		std::copy(other.A[i], other.A[i] + cc, A[i]);
	}
}

fmatrix2024::fmatrix2024(unsigned rc, unsigned cc) : rc(rc), cc(cc), A(nullptr) {
	try {

		A = CreateMatrix(rc, cc);
	}
	catch (...) {
		std::cerr << "Error with create matrix in constructor" << std::endl;
		throw;
	}
}

fmatrix2024::fmatrix2024(unsigned rc, unsigned cc, float inf, float sup) : rc(rc), cc(cc), A(nullptr) {
	try {
	A = CreateMatrix(rc, cc);
	GenerateMatrix(inf, sup);
	}
	catch (...) {
	   std::cerr << "Error with create or generate matrixin constructor" << std::endl;
	    throw;
	}
}

//////////// Destructor /////////
fmatrix2024::~fmatrix2024() {

	DeleteMatrix();

};


//////// Operators /////////////

float* fmatrix2024::operator[](unsigned row) {
	if (row >= rc) {
		throw std::out_of_range("Index out of range");
	}
	return A[row];
}

const float* fmatrix2024::operator[](unsigned row) const {
	if (row >= rc) {
		throw std::out_of_range("Index out of range");
	}
	return A[row];
}



fmatrix2024 fmatrix2024::operator*(const fmatrix2024& B) {
	if (this->cc != B.rc) {
		throw std::invalid_argument("Matrix do not match for multiplication");
	}

	fmatrix2024 C(this->rc, B.cc);
	for (unsigned i = 0; i < this->rc; i++) {
		for (unsigned j = 0; j < B.cc; j++) {
			C[i][j] = 0;
			for (unsigned k = 0; k < this->cc; k++) {
				C[i][j] += this->A[i][k] * B[k][j];
			}
		}
	}
	return C;
}





////////////// MAIN FUNC //////////////



float** fmatrix2024::CreateMatrix(unsigned rc, unsigned cc) {
	if (rc == 0 || cc == 0) {
		throw std::invalid_argument("Error: Matrix rows or columns must not be 0.");
	}

	float** A = nullptr;

	try {
		A = new float* [rc];
		for (unsigned i = 0; i < rc; i++) {
			A[i] = new float[cc];
		}
	}
	catch (...) {
		/*if (A != nullptr) {
			for (unsigned i = 0; i < rc; i++) {
				delete[] A[i];
			}
			delete[] A;
		}*/
		DeleteMatrix();
		throw;
	}

	return A;
}

void fmatrix2024::DeleteMatrix() {
	if (A != nullptr) {
		for (unsigned i = 0; i < rc; i++) {
			delete[] A[i];
		}
		delete[] A;
		A = nullptr;
	}
}

float** fmatrix2024::GenerateMatrix(float inf, float sup) {

	if (A != nullptr) {
		for (unsigned i = 0; i < rc; i++) {
			for (unsigned j = 0; j < cc; j++) {
				A[i][j] = inf + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (sup - inf)));
			}
			std::cout << std::endl;
		}
	}
	return A;
}





void fmatrix2024::PrintMatrix() const {
	if (A == nullptr) {
		std::cout << "Matrix is not initialized!" << std::endl;
		return;
	}

	for (unsigned i = 0; i < rc; i++) {
		for (unsigned j = 0; j < cc; j++) {
			std::cout << std::setw(10) << A[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void fmatrix2024::SwapCols(float** A, unsigned c1, unsigned c2, unsigned rc) {
	if (A != nullptr) {
		for (unsigned i = 0; i < rc; i++) {
			float tmp = A[i][c1];
			A[i][c1] = A[i][c2];
			A[i][c2] = tmp;
		}
	}
}

void fmatrix2024::SwapRows(float** A, unsigned r1, unsigned r2, unsigned rc) {
	if ((A != nullptr) && (rc > r1) && (rc > r2)) {
		float* tmp = A[r1];
		A[r1] = A[r2];
		A[r2] = tmp;
	}
}

//////////// IMPORT/EXPORT //////////////////////

void fmatrix2024::ExportMatrix(const char* filename) {
	std::ofstream file(filename);
	if (file.is_open()) {
		file.setf(std::ios::fixed);
		file.precision(7);
		file << std::setfill(' ');

		for (unsigned i = 0; i < rc; i++) {
			for (unsigned j = 0; j < cc; j++) {
				file << std::setw(10) << A[i][j] << "\t";
			}
			file << std::endl;
		}

		file.close();
	}
}

void fmatrix2024::ImportMatrix(const char* filename, unsigned rc, unsigned cc) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::invalid_argument("Error: Unable to open file.");
	}

	for (unsigned i = 0; i < rc; i++) {
		for (unsigned j = 0; j < cc; j++) {
			file >> (*this)[i][j];
		}
	}

	file.close();
}



fmatrix2024& fmatrix2024::operator=(const fmatrix2024& other) {
	if (this == &other) return *this; 

	if (A != nullptr) {
		for (unsigned i = 0; i < rc; ++i) {
			delete[] A[i];
		}
		delete[] A;
	}

	rc = other.rc;
	cc = other.cc;

	A = new float* [rc];
	for (unsigned i = 0; i < rc; ++i) {
		A[i] = new float[cc];
		for (unsigned j = 0; j < cc; ++j) {
			A[i][j] = other.A[i][j];
		}
	}

	return *this;
}

fmatrix2024& fmatrix2024::operator=(fmatrix2024&& other) noexcept {
	if (this == &other) return *this;
	if (A != nullptr) {
		for (unsigned i = 0; i < rc; ++i) {
			delete[] A[i];
		}
		delete[] A;
	}

	A = other.A;
	rc = other.rc;
	cc = other.cc;

	other.A = nullptr;
	other.rc = 0;
	other.cc = 0;

	return *this;
}


////////// GAUSS ////////////////


fmatrix2024 fmatrix2024::gaussSLAE(fmatrix2024& A_matrix, fmatrix2024& B_matrix, unsigned order) {

	if (A_matrix.rc != order || B_matrix.rc != order || B_matrix.cc != 1) {
		throw std::invalid_argument("Matrix dimensions are not valid for Gauss method.");
	}

	fmatrix2024 augmented(A_matrix);
	fmatrix2024 augmentedB(B_matrix);

	for (unsigned i = 0; i < order; ++i) {

		unsigned maxRow = i;
		for (unsigned j = i + 1; j < order; ++j) {
			if (std::abs(augmented[i][j]) > std::abs(augmented[i][maxRow])) {
				maxRow = j;
			}
		}

		if (i != maxRow) {
			augmented.SwapRows(augmented.A, i, maxRow, order);
			std::swap(augmentedB[i][0], augmentedB[maxRow][0]);
		}

		for (unsigned j = i + 1; j < order; ++j) {
			float factor = augmented[j][i] / augmented[i][i];
			augmentedB[j][0] -= factor * augmentedB[i][0];
			for (unsigned k = i; k < order; ++k) {
				augmented[j][k] -= factor * augmented[i][k];
			}
		}
	}

	fmatrix2024 solution(order, 1);

	for (int i = order - 1; i >= 0; --i) {
		solution[i][0] = augmentedB[i][0] / augmented[i][i];
		for (int j = i - 1; j >= 0; --j) {
			augmentedB[j][0] -= augmented[j][i] * solution[i][0];
		}
	}

	return solution;
}