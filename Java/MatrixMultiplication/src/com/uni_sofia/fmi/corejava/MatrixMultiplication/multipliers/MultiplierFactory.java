package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

/**
 * Created by dimit_000 on 1/27/2015.
 */
public class MultiplierFactory {

    public static MatrixMultiplier createMultiplier(boolean general, boolean parallel, Matrix A, Matrix B,
                                                    int poolSize) {
        if (general && parallel)
        {
            ClassicalMatrixMultiplierParallel result = new ClassicalMatrixMultiplierParallel(A, B);
            result.setPoolSize(poolSize);
            return result;
        }
        else if (general)
        {
            return new ClassicalMatrixMultiplierSerial(A, B);
        }
        else if (parallel)
        {
            StrassenMatrixMultiplierParallel result = new StrassenMatrixMultiplierParallel(A, B);
            result.setPoolSize(poolSize);
            return result;
        }
        else
        {
            return new StrassenMatrixMultiplierSerial(A, B);
        }
    }

    private MultiplierFactory() {}
}
