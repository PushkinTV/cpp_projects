#include <stdio.h>
#include <stdlib.h> 
// Проверим
// Function to multiply two matrices
// C = A * B
void multiplyMatrices(int A_rows, int A_cols, double A[A_rows][A_cols],
                      int B_rows, int B_cols, double B[B_rows][B_cols],
                      double C[A_rows][B_cols]) {
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            C[i][j] = 0;
            for (int k = 0; k < A_cols; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to transpose a matrix
// B = A^T
void transposeMatrix(int A_rows, int A_cols, double A[A_rows][A_cols],
                     double B[A_cols][A_rows]) {
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < A_cols; j++) {
            B[j][i] = A[i][j];
        }
    }
}

// Function to calculate the inverse of a 2x2 matrix
// A_inv = A^-1
int inverseMatrix2x2(double A[2][2], double A_inv[2][2]) {
    double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    if (det == 0) {
        fprintf(stderr, "Determinant is zero, inverse does not exist.\n");
        return 0; // Error: inverse does not exist
    }

    A_inv[0][0] = A[1][1] / det;
    A_inv[0][1] = -A[0][1] / det;
    A_inv[1][0] = -A[1][0] / det;
    A_inv[1][1] = A[0][0] / det;

    return 1; // Success
}

// Function to print a matrix
void printMatrix(int rows, int cols, double matrix[rows][cols], const char* name) {
    printf("%s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.4f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    // Example data points for linear regression y = mx + c
    // (x, y) pairs
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {2, 4, 5, 4, 5};
    int n = sizeof(x) / sizeof(x[0]); // Number of data points

    // Construct the design matrix X (n x 2)
    // Each row is [1, x_i]
    double X[n][2];
    for (int i = 0; i < n; i++) {
        X[i][0] = 1;
        X[i][1] = x[i];
    }
    printMatrix(n, 2, X, "Design Matrix X");

    // Construct the transpose of X (2 x n)
    double X_T[2][n];
    transposeMatrix(n, 2, X, X_T);
    printMatrix(2, n, X_T, "Transpose of X (X_T)");

    // Calculate X_T * X (2 x 2)
    double X_T_X[2][2];
    multiplyMatrices(2, n, X_T, n, 2, X, X_T_X);
    printMatrix(2, 2, X_T_X, "X_T * X");

    // Calculate (X_T * X)^-1 (2 x 2)
    double X_T_X_inv[2][2];
    if (!inverseMatrix2x2(X_T_X, X_T_X_inv)) {
        return 1; // Error occurred
    }
    printMatrix(2, 2, X_T_X_inv, "(X_T * X)^-1");

    // Construct the y vector (n x 1)
    double Y_vec[n][1];
    for (int i = 0; i < n; i++) {
        Y_vec[i][0] = y[i];
    }
    printMatrix(n, 1, Y_vec, "Y Vector");

    // Calculate X_T * Y (2 x 1)
    double X_T_Y[2][1];
    multiplyMatrices(2, n, X_T, n, 1, Y_vec, X_T_Y);
    printMatrix(2, 1, X_T_Y, "X_T * Y");

    // Calculate coefficients beta = (X_T * X)^-1 * (X_T * Y) (2 x 1)
    double beta[2][1];
    multiplyMatrices(2, 2, X_T_X_inv, 2, 1, X_T_Y, beta);
    printMatrix(2, 1, beta, "Coefficients (beta)");

    // The coefficients are beta[0][0] = c (intercept) and beta[1][0] = m (slope)
    double c = beta[0][0];
    double m = beta[1][0];

    printf("Linear Regression Equation: y = %.4fx + %.4f\n", m, c);

    // Verification: Calculate predicted y values and sum of squared residuals
    double sum_sq_residuals = 0;
    printf("\nVerification:\n");
    for (int i = 0; i < n; i++) {
        double y_predicted = m * x[i] + c;
        double residual = y[i] - y_predicted;
        sum_sq_residuals += residual * residual;
        printf("x = %.2f, actual y = %.2f, predicted y = %.4f, residual = %.4f\n",
               x[i], y[i], y_predicted, residual);
    }
    printf("Sum of Squared Residuals (SSE): %.4f\n", sum_sq_residuals);

    return 0;
}
