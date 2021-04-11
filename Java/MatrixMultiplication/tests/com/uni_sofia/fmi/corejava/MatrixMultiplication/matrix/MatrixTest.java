package com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class MatrixTest {

    private Matrix A;

    @Before
    public void setUp() {
        A = new Matrix(20, 10);

        for (int i = 0; i < A.height(); ++i)
            for (int j = 0; j < A.width(); ++j)
                A.set(i, j, i + j);

    }

    @Test(expected = NegativeArraySizeException.class)
    public void invalidConstructionShouldFail() {
        A = new Matrix(-1, -2);
    }

    @Test
    public void testGetWidth() {
        Assert.assertEquals(20, A.width());
    }

    @Test
    public void testGetHeight() {
        Assert.assertEquals(10, A.height());
    }

    @Test
    public void testSet() {
        A.set(1, 1, 2);
        Assert.assertEquals(2, A.get(1, 1), Matrix.sEpsilon);
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void setOutOfBoundsShouldFail() {
        A.set(20, 10, 2);
    }

    @Test
    public void defaultValueShouldBeZero() {
        A = new Matrix(20, 10);
        Assert.assertEquals(0, A.get(1, 1), Matrix.sEpsilon);
    }

    @Test
    public void testEquals() {
        Matrix B = new Matrix(20, 10);

        Assert.assertFalse(A.equals(1));
        Assert.assertTrue(A.equals(A));
        Assert.assertFalse(A.equals(B));


        for (int i = 0; i < B.height(); ++i)
            for (int j = 0; j < B.width(); ++j)
                B.set(i, j, i + j);

        Assert.assertTrue(A.equals(B));
    }

    @Test(expected = IndexOutOfBoundsException.class)
    public void getOutOfBoundsShouldFail() {
        A.get(20, 10);
    }

    @Test
    public void insertTopLeft() {
        Matrix smallDefault = new Matrix(A.width()/2, A.height()/2);
        smallDefault.insert(A, Matrix.Sector.TOP_LEFT);

        for (int i = 0; i < smallDefault.height(); ++i)
            for (int j = 0; j < smallDefault.width(); ++j)
                Assert.assertEquals(A.get(i, j), smallDefault.get(i, j), Matrix.sEpsilon);
    }

    @Test
    public void insertTopRight() {
        Matrix smallDefault = new Matrix(A.width()/2, A.height()/2);
        smallDefault.insert(A, Matrix.Sector.TOP_RIGHT);

        for (int i = 0; i < smallDefault.height(); ++i)
            for (int j = 0; j < smallDefault.width(); ++j)
                Assert.assertEquals(A.get(i, j + A.width()/2), smallDefault.get(i, j), Matrix.sEpsilon);
    }

    @Test
    public void insertBottomLeft() {
        Matrix smallDefault = new Matrix(A.width()/2, A.height()/2);
        smallDefault.insert(A, Matrix.Sector.BOTTOM_LEFT);

        for (int i = 0; i < smallDefault.height(); ++i)
            for (int j = 0; j < smallDefault.width(); ++j)
                Assert.assertEquals(A.get(i + A.height()/2, j), smallDefault.get(i, j), Matrix.sEpsilon);
    }

    @Test
    public void insertBottomRight() {
        Matrix smallDefault = new Matrix(A.width()/2, A.height()/2);
        smallDefault.insert(A, Matrix.Sector.BOTTOM_RIGHT);

        for (int i = 0; i < smallDefault.height(); ++i)
            for (int j = 0; j < smallDefault.width(); ++j)
                Assert.assertEquals(A.get(i + A.height()/2, j + A.width()/2), smallDefault.get(i, j), Matrix.sEpsilon);
    }
}