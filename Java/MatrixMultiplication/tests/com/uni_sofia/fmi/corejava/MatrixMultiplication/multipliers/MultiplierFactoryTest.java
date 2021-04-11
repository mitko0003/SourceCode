package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Test;

import static org.junit.Assert.*;

public class MultiplierFactoryTest {

    @SuppressWarnings("unused")
    @Test
    public void creatingMultiplierShouldNotFail() throws Exception {
        Matrix A = new Matrix(5, 5);
        Matrix B = new Matrix(5, 5);

        try {
            ClassicalMatrixMultiplierSerial createValidator0 = (ClassicalMatrixMultiplierSerial) MultiplierFactory
                    .createMultiplier(true, false, A, B, 1);
            ClassicalMatrixMultiplierParallel createValidator1 = (ClassicalMatrixMultiplierParallel) MultiplierFactory
                    .createMultiplier(true, true, A, B, 1);
            StrassenMatrixMultiplierSerial createValidator2 = (StrassenMatrixMultiplierSerial) MultiplierFactory
                    .createMultiplier(false, false, A, B, 1);
            StrassenMatrixMultiplierParallel createValidator3 = (StrassenMatrixMultiplierParallel) MultiplierFactory
                    .createMultiplier(false, true, A, B, 1);
        } catch (Exception e) {
            fail(e.getMessage());
        }
    }
}