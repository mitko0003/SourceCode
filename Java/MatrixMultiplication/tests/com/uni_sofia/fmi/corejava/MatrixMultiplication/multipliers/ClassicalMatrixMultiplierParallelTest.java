package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Test;

import static org.junit.Assert.*;

public class ClassicalMatrixMultiplierParallelTest extends BaseMultiplierTest {

    @Test
    public void calculationsShouldBeRight() throws Exception {
        Matrix result;
        for (int i = 1; i <= sTestCaseCount; ++i) {
            load("Ex" + i);
            ClassicalMatrixMultiplierParallel multiplier = new ClassicalMatrixMultiplierParallel(A, B);
            result = multiplier.call();
            assertTrue(C.equals(result));
        }
    }

    @Test
    public void getThreadUsageShouldReturnZeroWhileNotRunning() throws Exception {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        ClassicalMatrixMultiplierParallel multiplier = new ClassicalMatrixMultiplierParallel(dummyA, dummyB);
        assertEquals(multiplier.getThreadUsage(), 0);
    }

    @Test (expected = IllegalArgumentException.class)
    public void setNegativePoolSizeShouldFail() {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        ClassicalMatrixMultiplierParallel multiplier = new ClassicalMatrixMultiplierParallel(dummyA, dummyB);
        multiplier.setPoolSize(-1);
    }

    @Test
    public void setPositivePoolSize() {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        ClassicalMatrixMultiplierParallel multiplier = new ClassicalMatrixMultiplierParallel(dummyA, dummyB);
        multiplier.setPoolSize(1);
    }

    @Test
    public void isRunningShouldReturnFalseWhileNotRunning() throws Exception {
        Matrix dummyA = new Matrix(5, 5);
        Matrix dummyB = new Matrix(5, 5);
        ClassicalMatrixMultiplierParallel multiplier = new ClassicalMatrixMultiplierParallel(dummyA, dummyB);
        assertFalse(multiplier.isRunning());
    }
}