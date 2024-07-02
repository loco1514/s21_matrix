#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int er_s = OK;
  if (rows < 1 || columns < 1)
    er_s = INCORRECT_MATRIX;
  else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
  }

  if (result->matrix != NULL) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      if (!result->matrix[i]) {
        for (int j = 0; j < i; j++) free(result->matrix[j]);
        free(result->matrix);
        er_s = INCORRECT_MATRIX;
        break;
      }
    }
  } else {
    er_s = INCORRECT_MATRIX;
  }
  return er_s;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
  }
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int er_s = OK;
  if (s21_enty(A) == 0) {
    if (s21_create_matrix(A->columns, A->rows, result) == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++)
          result->matrix[j][i] = A->matrix[i][j];
      }
    } else
      er_s = INCORRECT_MATRIX;
  } else
    er_s = INCORRECT_MATRIX;
  return er_s;
}

int s21_determinant(matrix_t *A, double *result) {
  int er_s = OK;
  if (er_s == OK) {
    if (A->rows == A->columns) {
      *result = s21_get_determinant(A);
    } else {
      er_s = CALCULATION_ERROR;
    }
  }
  return er_s;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int er_s = OK;
  if (er_s == OK) {
    if (A->rows == A->columns) {
      s21_create_matrix(A->columns, A->rows, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          matrix_t calc;
          double determinant = 0;
          s21_create_matrix(A->columns - 1, A->rows - 1, &calc);
          s21_get_calc(i, j, A, &calc);
          s21_determinant(&calc, &determinant);
          result->matrix[i][j] = pow(-1, (i + j)) * determinant;
          s21_remove_matrix(&calc);
        }
      }
    } else {
      er_s = INCORRECT_MATRIX;
    }
  }
  return er_s;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int er_s = OK;
  if (!A || (A->rows < 1) || (A->columns < 1) || !result)
    er_s = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    er_s = CALCULATION_ERROR;
  else {
    double det = 0.0;
    s21_determinant(A, &det);
    if (fabs(det) < ACCURACY)
      er_s = CALCULATION_ERROR;
    else if (A->rows == 1) {
      s21_create_matrix(1, 1, result);
      result->matrix[0][0] = (1 / det);
    } else {
      matrix_t B;
      s21_calc_complements(A, result);
      s21_transpose(result, &B);
      s21_remove_matrix(result);
      s21_mult_number(&B, (1.0 / det), result);
      s21_remove_matrix(&B);
    }
  }
  return er_s;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int er_s = OK;
  if (s21_enty(A) == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) == 0) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++)
          result->matrix[i][j] = A->matrix[i][j] * number;
      }
    } else
      er_s = CALCULATION_ERROR;
  } else
    er_s = INCORRECT_MATRIX;
  return er_s;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;
  if (!A || !B || (A->rows != B->rows) || (A->columns != B->columns)) {
    status = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-07) {
          status = FAILURE;
          break;
        }
      }
    }
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int er_s = OK;
  if (s21_enty(A) == 0 && s21_enty(B) == 0) {
    if (A->columns == B->columns && A->rows == B->rows) {
      if (s21_create_matrix(A->rows, A->columns, result) == 0) {
        for (int i = 0; i < result->rows; i++) {
          for (int j = 0; j < result->columns; j++)
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      } else
        er_s = CALCULATION_ERROR;
    } else
      er_s = INCORRECT_MATRIX;
  } else
    er_s = INCORRECT_MATRIX;
  return er_s;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int er_s = OK;
  if (s21_enty(A) == 0 && s21_enty(B) == 0) {
    if (A->columns == B->columns && A->rows == B->rows) {
      if (s21_create_matrix(A->rows, A->columns, result) == 0) {
        for (int i = 0; i < result->rows; i++) {
          for (int j = 0; j < result->columns; j++)
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      } else
        er_s = CALCULATION_ERROR;
    } else
      er_s = INCORRECT_MATRIX;
  } else
    er_s = INCORRECT_MATRIX;
  return er_s;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int er_s = OK;
  if (s21_enty(A) == 0 && s21_enty(B) == 0) {
    if (A->columns == B->rows) {
      if (s21_create_matrix(A->rows, B->columns, result) == 0) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < B->columns; j++) {
            for (int k = 0; k < A->columns; k++)
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      } else
        er_s = CALCULATION_ERROR;
    } else
      er_s = INCORRECT_MATRIX;
  } else
    er_s = INCORRECT_MATRIX;
  return er_s;
}

int s21_enty(matrix_t *A) {
  int er_s = OK;
  if (A == NULL || A->matrix == NULL || A->columns == 0 || A->rows == 0)
    er_s = INCORRECT_MATRIX;
  return er_s;
}

double s21_get_determinant(matrix_t *A) {
  double flag = 0;
  if (A->rows == 1) {
    flag = A->matrix[0][0];
  } else {
    matrix_t tmp;
    s21_create_matrix(A->rows - 1, A->columns - 1, &tmp);
    for (int i = 0; i < A->columns; i++) {
      s21_get_calc(0, i, A, &tmp);
      if (i % 2) {
        flag -= A->matrix[0][i] * s21_get_determinant(&tmp);
      } else {
        flag += A->matrix[0][i] * s21_get_determinant(&tmp);
      }
    }
    s21_remove_matrix(&tmp);
  }

  return flag;
}

void s21_get_calc(int row, int col, matrix_t *A, matrix_t *result) {
  int a_row = 0;
  int a_col;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    a_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        continue;
      }
      result->matrix[a_row][a_col] = A->matrix[i][j];
      a_col++;
    }
    a_row++;
  }
}
