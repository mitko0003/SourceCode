package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.RunSettings;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Watchable;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.MatrixOperations.transpose;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

/**
 * Using the general definition of the matrix product.
 * Faster for smaller matrices.
 */
public class ClassicalMatrixMultiplierParallel extends MatrixMultiplier
    implements Watchable {

    private int mPoolSize;
    private ThreadPoolExecutor mExecutor;
    private Matrix tB;

    public ClassicalMatrixMultiplierParallel(Matrix A, Matrix B) {
        super(A, B);
        mPoolSize = RunSettings.CORE_COUNT;
    }

    @Override
    public Matrix call() {
        int colsCount = B.width();
        int rowsCount = A.height();

        tB = transpose(B);
        mExecutor = (ThreadPoolExecutor) Executors.newFixedThreadPool(mPoolSize);
        List<Future<Double>> results = new ArrayList<>();

        for (int i = 0; i < rowsCount; ++i)
            for (int j = 0; j < colsCount; ++j) {
                RowColMultiplier callable = new RowColMultiplier(i, j);
                Future<Double> future = mExecutor.submit(callable);
                results.add(future);
            }


        Matrix result = new Matrix(colsCount, rowsCount);
        try
        {
            double value;
            for (int i = 0; i < results.size(); ++i) {
                value = results.get(i).get();
                result.set(i / colsCount, i % colsCount, value);
            }
            return result;
        }
        catch (ExecutionException | InterruptedException e)
        {
            return null;
        }
        finally
        {
            mExecutor.shutdown();
            mExecutor = null;
            tB = null;
        }
    }

    public void setPoolSize(int size) {
        if (size <= 0) // throw because if one gets this wrong it will take a lot of time to find
            throw new IllegalArgumentException();
        mPoolSize = size;
    }

    @Override
    public int getThreadUsage() {
        if (mExecutor == null)
            return 0;
        return mExecutor.getActiveCount();
    }

    @Override
    public boolean isRunning() {
        return mExecutor != null && !mExecutor.isTerminated();
    }

    private class RowColMultiplier implements Callable<Double> {

        private final int mRow;
        private final int mCol;

        public RowColMultiplier(int row, int col) {
            mRow = row;
            mCol = col;
        }

        @Override
        public Double call() {
            double result = 0d;

            for (int i = 0; i < A.width(); ++i)
                result += A.get(mRow, i) * tB.get(mCol, i);

            return result;
        }
    }
}
