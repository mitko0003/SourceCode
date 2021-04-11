package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.MatrixOperations.transpose;

/**
 * Using the general definition of the matrix product.
 * Faster for smaller matrices.
 */
public class ClassicalMatrixMultiplierSerial extends MatrixMultiplier {

    public ClassicalMatrixMultiplierSerial(Matrix A, Matrix B) {
        super(A, B);
    }
    private Matrix tB;

    @Override
    public Matrix call() {
        int colsCount = B.width();
        int rowsCount = A.height();

        tB = transpose(B);
        Matrix result = new Matrix(colsCount, rowsCount);

        double value;
        for (int i = 0; i < rowsCount; ++i)
            for (int j = 0; j < colsCount; ++j) {
                value = multiplyRowCol(i, j);
                result.set(i, j, value);
            }

        tB = null;
        return result;
    }

    private double multiplyRowCol(int row, int col) {
        double result = 0d;

        for (int i = 0; i < A.width(); ++i)
            result += A.get(row, i) * tB.get(col, i);

        return result;
    }
}
