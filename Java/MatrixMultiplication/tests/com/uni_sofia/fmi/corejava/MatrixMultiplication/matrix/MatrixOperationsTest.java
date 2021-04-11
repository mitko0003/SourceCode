package com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.MatrixOperations;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class MatrixOperationsTest {

    private Matrix A;
    private Matrix B;

    private static double left[][] =
            {{1,2,3},
             {4,5,6},
             {7,8,9}};

    private static double right[][] =
            {{-2,0,5},
             {9,5,1},
             {2,4,5}};

    private static double transposed[][] =
            {{1,4,7},
            {2,5,8},
            {3,6,9}};

    private static double subtracted[][] =
            {{3,2,-2},
             {-5,0,5},
             {5,4,4}};

    private static double added[][] =
            {{-1,2,8},
             {13,10,7},
             {9,12,14}};

    private static double paddedLeft[][] =
            {{1,2,3,0},
             {4,5,6,0},
             {7,8,9,0},
             {0,0,0,0}};

    private static double topLeft[][] =
            {{1,2},
             {4,5}};

    private static double topRight[][] =
            {{3,0},
             {6,0}};

    private static double bottomLeft[][] =
            {{7,8},
             {0,0}};

    private static double bottomRight[][] =
            {{9,0},
             {0,0}};

    @Before
    public void setUp() {
        A = new Matrix(3, 3);
        B = new Matrix(3, 3);

        for (int i = 0; i < A.height(); ++i)
            for (int j = 0; j < A.width(); ++j) {
                A.set(i, j, left[i][j]);
                B.set(i, j, right[i][j]);
        }
    }

    @Test
    public void testTranspose() {
        Matrix C = new Matrix(3, 3);

        for (int i = 0; i < C.height(); ++i)
            for (int j = 0; j < C.width(); ++j)
                C.set(i, j, transposed[i][j]);

        assertTrue(C.equals(MatrixOperations.transpose(A)));
    }

    @Test
    public void testPad() {
        Matrix C = new Matrix(4, 4);

        for (int i = 0; i < C.height(); ++i)
            for (int j = 0; j < C.width(); ++j)
                C.set(i, j, paddedLeft[i][j]);

        assertTrue(C.equals(MatrixOperations.pad(A, 4)));
    }

    @Test
    public void testCalculatePadding() {
        Matrix A = new Matrix(1000, 102);
        Matrix B = new Matrix(1025, 102);
        assertEquals(2048, MatrixOperations.calculatePadding(A, B));
    }

    @Test
    public void testSubtract() {
        Matrix C = new Matrix(3, 3);

        for (int i = 0; i < C.height(); ++i)
            for (int j = 0; j < C.width(); ++j)
                C.set(i, j, subtracted[i][j]);

        assertTrue(C.equals(MatrixOperations.subtract(A, B)));
    }

    @Test
    public void testAdd() throws Exception {
        Matrix C = new Matrix(3, 3);

        for (int i = 0; i < C.height(); ++i)
            for (int j = 0; j < C.width(); ++j)
                C.set(i, j, added[i][j]);

        assertTrue(C.equals(MatrixOperations.add(A, B)));
    }

    @Test
    public void testGetQuarter() throws Exception {
        Matrix C = new Matrix(4, 4);
        Matrix tL = new Matrix(2, 2);
        Matrix tR = new Matrix(2, 2);
        Matrix bL = new Matrix(2, 2);
        Matrix bR = new Matrix(2, 2);

        for (int i = 0; i < C.height(); ++i)
            for (int j = 0; j < C.width(); ++j)
                C.set(i, j, paddedLeft[i][j]);

        for (int i = 0; i < C.height() / 2; ++i)
            for (int j = 0; j < C.width() / 2; ++j) {
                tL.set(i, j, topLeft[i][j]);
                tR.set(i, j, topRight[i][j]);
                bL.set(i, j, bottomLeft[i][j]);
                bR.set(i, j, bottomRight[i][j]);
            }

        assertTrue(tL.equals(MatrixOperations.getQuarter(C, Matrix.Sector.TOP_LEFT)));
        assertTrue(tR.equals(MatrixOperations.getQuarter(C, Matrix.Sector.TOP_RIGHT)));
        assertTrue(bL.equals(MatrixOperations.getQuarter(C, Matrix.Sector.BOTTOM_LEFT)));
        assertTrue(bR.equals(MatrixOperations.getQuarter(C, Matrix.Sector.BOTTOM_RIGHT)));
    }
}