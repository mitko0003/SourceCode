package com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix.Sector.*;

/**
* A collection of all functions needed for the Strassen algorithm.
*/
public class MatrixOperations {

    public static Matrix pad(Matrix A, int padding) {
        Matrix C = new Matrix(padding, padding);

        C.insert(A, TOP_LEFT);

        return C;
    }

    public static int calculatePadding(Matrix A, Matrix B) {
        int max = Math.max(Math.max(A.width(), A.height()),
                           Math.max(B.width(), B.height()));
        return nextPowerOfTwo(max);
    }

    private static int nextPowerOfTwo(int n) {
        int power = 1;
        while (power < n)
            power <<= 1;
        return power;
    }

    public static Matrix transpose(Matrix A) {
        Matrix C = new Matrix(A.height(), A.width());

        for (int i = 0; i < A.height(); i++)
            for (int j = 0; j < A.width(); j++)
                C.set(j, i, A.get(i, j));

        return C;
    }

    public static Matrix subtract(Matrix A, Matrix B) {
        Matrix C = new Matrix(A.width(), A.height());

        for (int i = 0; i < A.height(); i++)
            for (int j = 0; j < A.width(); j++)
                C.set(i, j, A.get(i, j) - B.get(i, j));

        return C;
    }

    public static Matrix add(Matrix A, Matrix B) {
        Matrix C = new Matrix(A.width(), A.height());

        for (int i = 0; i < A.height(); i++)
            for (int j = 0; j < A.width(); j++)
                C.set(i, j, A.get(i, j) + B.get(i, j));

        return C;
    }

    public static Matrix getQuarter(Matrix A, Matrix.Sector sector) {
        int half = A.height() / 2;

        Matrix C = new Matrix(half, half);
        C.insert(A, sector);
        return C;
    }

    private MatrixOperations() {};
}
