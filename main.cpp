#include <iostream>
#include <stdexcept>
#include "fmatrix2024.h"

int main() {


	unsigned rc = 5;
	unsigned cc = 5;
	unsigned brc = rc;
	unsigned bcc = 1;


	float inf = -10.0f;
	float sup = 10.0f;

	fmatrix2024 A(rc, cc, inf, sup);
	fmatrix2024 B(brc, bcc, inf, sup);
	fmatrix2024 C(rc, cc);
	fmatrix2024 solution(brc, bcc);

	std::cout << "Matrix A:" << std::endl;
	A.PrintMatrix();

	std::cout << "Matrix B:" << std::endl;
	B.PrintMatrix();

	solution = A.gaussSLAE(A, B, rc);

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Solution system:" << std::endl;
	for (unsigned i = 0; i < rc; i++) {
		std::cout << "x" << i + 1 << " = " << solution[i][0] << std::endl;
	}

	solution.ExportMatrix("solution.txt");
	A.ExportMatrix("A.txt");
	std::cout << std::endl;
	C.ImportMatrix("A.txt", rc, cc);
	std::cout << "C\n" << std::endl;
	C.PrintMatrix();
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	A = A * C;
	std::cout << "MatrixMultiply:\n" << std::endl;
	A.PrintMatrix();

	return 0;
}