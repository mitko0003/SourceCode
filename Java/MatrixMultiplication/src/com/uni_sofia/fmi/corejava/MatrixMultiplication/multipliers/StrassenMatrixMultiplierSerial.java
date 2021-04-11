package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix.Sector.*;
import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.MatrixOperations.*;

/**
 * Using the following algorithm:
 * http://en.wikipedia.org/wiki/Strassen_algorithm
 */
public class StrassenMatrixMultiplierSerial extends MatrixMultiplier {

    private static final int LEAF_SIZE = 256;

    public StrassenMatrixMultiplierSerial(Matrix A, Matrix B) {
        super(A, B);
    }

    @Override
    public Matrix call() {
        int padding = calculatePadding(A, B);

        // Making matrices square and with size power of two.
        Matrix paddedA = pad(A, padding);
        Matrix paddedB = pad(B, padding);

        Matrix paddedC = multiply(paddedA, paddedB);

        Matrix result = new Matrix(B.width(), A.height());
        result.insert(paddedC, TOP_LEFT);
        return result;
    }

    private Matrix multiply(Matrix A, Matrix B) {
        if (A.width() < LEAF_SIZE) {
            ClassicalMatrixMultiplierSerial multiplication = new ClassicalMatrixMultiplierSerial(A, B);
            return multiplication.call();
        }

        /**
         * 00 - bottom left  = 21
         * 01 - bottom right = 22
         * 10 - top left     = 11
         * 11 - top right    = 12
         * Like in a coordinate system
         **/
        Matrix A00 = getQuarter(A, BOTTOM_LEFT);
        Matrix A01 = getQuarter(A, BOTTOM_RIGHT);
        Matrix A10 = getQuarter(A, TOP_LEFT);
        Matrix A11 = getQuarter(A, TOP_RIGHT);

        Matrix B00 = getQuarter(B, BOTTOM_LEFT);
        Matrix B01 = getQuarter(B, BOTTOM_RIGHT);
        Matrix B10 = getQuarter(B, TOP_LEFT);
        Matrix B11 = getQuarter(B, TOP_RIGHT);

        Matrix M1 = multiply(add(A10, A01), add(B10, B01));
        Matrix M2 = multiply(add(A00, A01), B10);
        Matrix M3 = multiply(A10, subtract(B11, B01));
        Matrix M4 = multiply(A01, subtract(B00, B10));
        Matrix M5 = multiply(add(A10, A11), B01);
        Matrix M6 = multiply(subtract(A00, A10), add(B10, B11));
        Matrix M7 = multiply(subtract(A11, A01), add(B00, B01));

        Matrix C10 = add(subtract(add(M1, M4), M5), M7);
        Matrix C11 = add(M3, M5);
        Matrix C00 = add(M2, M4);
        Matrix C01 = add(subtract(add(M1, M3), M2), M6);

        Matrix result = new Matrix(A.width(), A.width());
        result.insert(C10, TOP_LEFT);
        result.insert(C11, TOP_RIGHT);
        result.insert(C00, BOTTOM_LEFT);
        result.insert(C01, BOTTOM_RIGHT);

        return result;
    }
}
