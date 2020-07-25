/*
 * matrix.h
 *
 * This file is part of Luna Purpura.
 */

#ifndef LUNAPURPURA_MATRIX_H
#define LUNAPURPURA_MATRIX_H

#include <stdarg.h>
#include <stdint.h>

#define LP_SUBSYSTEM_MATRIX "matrix"

#define MATRIX_MAX_COLS 16
#define MATRIX_MAX_ROWS 16

typedef char MatrixString[32];
typedef uint32_t MatrixNumber;

enum MatrixCellKind {
	MATRIX_CELL_KIND_NONE,
	MATRIX_CELL_KIND_STRING,
	MATRIX_CELL_KIND_NUMBER,
};

union MatrixCellValue {
	MatrixString string;
	MatrixNumber number;
};

struct MatrixCell {
	enum MatrixCellKind kind;
	union MatrixCellValue value;
};
typedef struct MatrixCell MatrixCell;

struct Matrix {
	uint8_t n_rows;
	uint8_t n_cols;
	MatrixCell cells[MATRIX_MAX_COLS][MATRIX_MAX_ROWS];
};
typedef struct Matrix Matrix;

void Matrix_Init(Matrix *matrix);
void Matrix_SetCellValue(Matrix *matrix, int col, int row, enum MatrixCellKind kind, ...);
struct MatrixCell Matrix_GetCell(Matrix *matrix, int col, int row);

#endif /* LUNAPURPURA_MATRIX_H */
