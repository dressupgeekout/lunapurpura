/*
 * matrix tests
 *
 * This file is part of Luna Purpura.
 */

#include <stdio.h>

#include <matrix.h>

int
main(void)
{
	Matrix matrix;
	Matrix_Init(&matrix);
	matrix.n_rows = 4;
	matrix.n_cols = 4;

	for (int i = 0; i < matrix.n_rows; i++) {
		for (int j = 0; j < matrix.n_cols; j++) {
			Matrix_SetCellValue(&matrix, j, i, MATRIX_CELL_KIND_NUMBER, 42);
		}
	}

	for (int i = 0; i < matrix.n_rows; i++) {
		for (int j = 0; j < matrix.n_cols; j++) {
			MatrixCell cell = Matrix_GetCell(&matrix, j, i);
			switch (cell.kind) {
			case MATRIX_CELL_KIND_NUMBER:
				printf("%d,", cell.value.number);
				break;
			case MATRIX_CELL_KIND_STRING:
				printf("%s,", cell.value.string);
				break;
			case MATRIX_CELL_KIND_NONE:
				printf("%s,", "(nil)");
				break;
			}
		}
		printf("\n");
	}

	return 0;
}
