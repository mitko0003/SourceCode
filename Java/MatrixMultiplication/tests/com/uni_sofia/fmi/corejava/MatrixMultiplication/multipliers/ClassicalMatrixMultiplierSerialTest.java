package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions.MatrixSizeMultiplicationException;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.io.MatrixReader;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.Assert.*;

public class ClassicalMatrixMultiplierSerialTest extends BaseMultiplierTest {

    @Test
    public void calculationsShouldBeRight() throws Exception {
        Matrix result;
        for (int i = 1; i <= sTestCaseCount; ++i) {
            load("Ex" + i);
            ClassicalMatrixMultiplierSerial multiplier = new ClassicalMatrixMultiplierSerial(A, B);
            result = multiplier.call();
            assertTrue(C.equals(result));
        }
    }

    @SuppressWarnings("unused")
    @Test(expected = MatrixSizeMultiplicationException.class)
    public void passingMatricesWithBadSizesShouldFail() throws Exception {
        Matrix A = new Matrix(5,1);
        Matrix B = new Matrix(4,6);

        ClassicalMatrixMultiplierSerial multiplier = new ClassicalMatrixMultiplierSerial(A, B);
    }

    @Test
    public void testGetters() throws Exception {
        Matrix A = new Matrix(5,5);
        Matrix B = new Matrix(5,5);

        ClassicalMatrixMultiplierSerial multiplier = new ClassicalMatrixMultiplierSerial(A, B);
        assertSame(A, multiplier.getLeft());
        assertSame(B, multiplier.getRight());
    }
}