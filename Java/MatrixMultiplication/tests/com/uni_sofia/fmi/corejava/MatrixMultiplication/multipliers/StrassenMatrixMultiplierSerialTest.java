package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Test;

import static org.junit.Assert.*;

public class StrassenMatrixMultiplierSerialTest extends BaseMultiplierTest {

    @Test
    public void calculationsShouldBeRight() throws Exception {
        Matrix result;
        for (int i = 1; i <= sTestCaseCount; ++i) {
            load("Ex" + i);
            StrassenMatrixMultiplierSerial multiplier = new StrassenMatrixMultiplierSerial(A, B);
            result = multiplier.call();
            assertTrue(C.equals(result));
        }
    }
}