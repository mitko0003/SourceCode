package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions.MatrixSizeMultiplicationException;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import java.util.concurrent.Callable;

public abstract class MatrixMultiplier implements Callable<Matrix> {

    // Using single letter names for readability
    protected final Matrix A; // left matrix
    protected final Matrix B; // right matrix

    protected MatrixMultiplier(Matrix A, Matrix B) {
        if(A.width() != B.height())
            throw new MatrixSizeMultiplicationException();

        this.A = A;
        this.B = B;
    }

    public final Matrix getLeft() {
        return A;
    }

    public final Matrix getRight() {
        return B;
    }
}
