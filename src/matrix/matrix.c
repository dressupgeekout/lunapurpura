/*
 * matrix.c
 *
 * This file is part of Luna Purpura.
 */

#include <stdarg.h>
#include <stdio.h>

#include "matrix.h"

void
Matrix_Init(Matrix *matrix)
{
	matrix->n_rows = 0;
	matrix->n_cols = 0;

	for (int i = 0; i < MATRIX_MAX_COLS; i++) {
		for (int j = 0; j < MATRIX_MAX_ROWS; j++) {
			matrix->cells[i][j].kind = MATRIX_CELL_KIND_NONE;
		}
	}
}


void
Matrix_SetCellValue(Matrix *matrix, int col, int row, enum MatrixCellKind kind, ...)
{
	if (col > matrix->n_cols) { 
		/* XXX */
		return;
	}

	if (row > matrix->n_rows) {
		/* XXX */
		return;
	}

	va_list ap;
	va_start(ap, kind);

	MatrixCell cell = matrix->cells[col][row];

	switch (kind) {
	case MATRIX_CELL_KIND_NONE:
		cell.kind = MATRIX_CELL_KIND_NONE;
		break;
	case MATRIX_CELL_KIND_STRING:
		cell.kind = MATRIX_CELL_KIND_STRING;
		snprintf(cell.value.string, sizeof(cell.value.string), "%s", va_arg(ap, MatrixString));
		break;
	case MATRIX_CELL_KIND_NUMBER:
		cell.kind = MATRIX_CELL_KIND_NUMBER;
		cell.value.number = va_arg(ap, MatrixNumber);
		break;
	}

	va_end(ap);
}


struct MatrixCell
Matrix_GetCell(Matrix *matrix, int col, int row)
{
	//if (col > matrix.n_cols) { 
	//	/* XXX */
	//	return;
	//}

	//if (row > matrix.n_rows) {
	//	/* XXX */
	//	return;
	//}

	return matrix->cells[col][row];
}
