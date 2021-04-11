package com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix;

public class Matrix {

    public static enum Sector {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    }
    public static final double sEpsilon = 1E-6;

    private int mWidth;
    private int mHeight;
    private double[][] mElements;

    public Matrix(int width, int height) {
        mWidth = width;
        mHeight = height;

        mElements = new double[height][width];
    }

    public int width(){
        return mWidth;
    }

    public int height(){
        return mHeight;
    }

    public void set(int row, int col, double value) {
        mElements[row][col] = value;
    }

    public double get(int row, int col) {
        return mElements[row][col];
    }

    /**
     * Fits source matrix in, no matter the sizes.
     */
    public void insert(Matrix B, Sector sector) {
        int rowStartA = 0, colStartA = 0;
        int rowStartB = 0, colStartB = 0;

        int rows = Math.min(this.mHeight, B.mHeight);
        int cols = Math.min(this.mWidth, B.mWidth);

        switch (sector) {

            case TOP_LEFT:
                break;

            case TOP_RIGHT:
                colStartA = mWidth - cols;
                colStartB = B.mWidth - cols;
                break;

            case BOTTOM_LEFT:
                rowStartA = mHeight - rows;
                rowStartB = B.mHeight - rows;
                break;

            case BOTTOM_RIGHT:
                rowStartA = mHeight - rows; colStartA = mWidth - cols;
                rowStartB = B.mHeight - rows; colStartB = B.mWidth - cols;
                break;

        }

        for (int i = 0; i < rows; ++i)
            System.arraycopy(B.mElements[rowStartB + i], colStartB, mElements[rowStartA + i], colStartA, cols);
    }

    @Override
    public boolean equals(Object other) {

        if (this == other)
            return true;
        else if (!(other instanceof Matrix))
            return false;
        else {

            Matrix A = (Matrix) other;
            if (this.mWidth != A.mWidth || this.mHeight != A.mHeight) return false;

            for (int i = 0; i < this.mHeight; ++i)
                for (int j = 0; j < this.mWidth; ++j)
                    if (Math.abs(this.mElements[i][j] - A.mElements[i][j]) > sEpsilon)
                        return false;

            return true;

        }
    }
}
