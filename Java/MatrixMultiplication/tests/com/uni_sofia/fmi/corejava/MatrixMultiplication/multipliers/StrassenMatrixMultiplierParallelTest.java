package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Test;

import static org.junit.Assert.*;

public class StrassenMatrixMultiplierParallelTest extends BaseMultiplierTest {

    @Test
    public void calculationsShouldBeRight() throws Exception {
        Matrix result;
        for (int i = 1; i <= sTestCaseCount; ++i) {
            load("Ex" + i);
            StrassenMatrixMultiplierParallel multiplier = new StrassenMatrixMultiplierParallel(A, B);
            result = multiplier.call();
            assertTrue(C.equals(result));
        }
    }

    @Test
    public void getThreadUsageShouldReturnZeroWhileNotRunning() throws Exception {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        StrassenMatrixMultiplierParallel multiplier = new StrassenMatrixMultiplierParallel(dummyA, dummyB);
        assertEquals(multiplier.getThreadUsage(), 0);
    }

    @Test (expected = IllegalArgumentException.class)
    public void setNegativePoolSizeShouldFail() {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        StrassenMatrixMultiplierParallel multiplier = new StrassenMatrixMultiplierParallel(dummyA, dummyB);
        multiplier.setPoolSize(-1);
    }

    @Test
    public void setPositivePoolSize() {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        StrassenMatrixMultiplierParallel multiplier = new StrassenMatrixMultiplierParallel(dummyA, dummyB);
        multiplier.setPoolSize(1);
    }

    @Test
    public void isRunningShouldReturnFalseWhileNotRunning() throws Exception {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        StrassenMatrixMultiplierParallel multiplier = new StrassenMatrixMultiplierParallel(dummyA, dummyB);
        assertFalse(multiplier.isRunning());
    }
}