package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.RunSettings;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Watchable;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix.Sector.*;
import static com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.MatrixOperations.*;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

/**
* Using the following algorithm:
* http://en.wikipedia.org/wiki/Strassen_algorithm
*/
public class StrassenMatrixMultiplierParallel extends MatrixMultiplier
        implements Watchable {

    private static final int LEAF_SIZE = 256;

    private int mParallelism;
    private ForkJoinPool mForkJoinPool;

    public StrassenMatrixMultiplierParallel(Matrix A, Matrix B) {
        super(A, B);
        mParallelism = RunSettings.CORE_COUNT;
    }

    @Override
    public Matrix call() {
        mForkJoinPool = new ForkJoinPool(mParallelism);
        int padding = calculatePadding(A, B);

        // Making matrices square and with size power of two.
        Matrix paddedA = pad(A, padding);
        Matrix paddedB = pad(B, padding);
        try
        {
            Matrix paddedC = mForkJoinPool.invoke(new MultiplierTask(paddedA, paddedB));

            Matrix result = new Matrix(B.width(), A.height());
            result.insert(paddedC, TOP_LEFT);
            return result;
        }
        finally
        {
            mForkJoinPool.shutdown();
            mForkJoinPool = null;
        }
    }

    public void setPoolSize(int size) {
        if (size <= 0) // throw because if one gets this wrong it will take a lot of time to find
            throw new IllegalArgumentException();
        mParallelism = size;
    }

    @Override
    public int getThreadUsage() {
        if (mForkJoinPool == null)
            return 0;
        return mForkJoinPool.getActiveThreadCount();
    }

    @Override
    public boolean isRunning() {
        return mForkJoinPool != null && !mForkJoinPool.isTerminated();
    }

    private class MultiplierTask extends RecursiveTask<Matrix> {

        private Matrix A;
        private Matrix B;

        public MultiplierTask(Matrix A, Matrix B) {
            this.A = A;
            this.B = B;
        }

        @Override
        protected Matrix compute() {
            if (A.width() < LEAF_SIZE) {
                ClassicalMatrixMultiplierSerial multiplication = new ClassicalMatrixMultiplierSerial(A, B);
                return multiplication.call();
            }

            /**
             * 00 - bottom left  = 21
             * 01 - bottom right = 22
             * 10 - top left     = 11
             * 11 - top right    = 12
             * Like in a coordinate system
             **/
            Matrix A00 = getQuarter(A, BOTTOM_LEFT);
            Matrix A01 = getQuarter(A, BOTTOM_RIGHT);
            Matrix A10 = getQuarter(A, TOP_LEFT);
            Matrix A11 = getQuarter(A, TOP_RIGHT);

            Matrix B00 = getQuarter(B, BOTTOM_LEFT);
            Matrix B01 = getQuarter(B, BOTTOM_RIGHT);
            Matrix B10 = getQuarter(B, TOP_LEFT);
            Matrix B11 = getQuarter(B, TOP_RIGHT);

            List<MultiplierTask> tasks = new ArrayList<>(7);
            tasks.add(new MultiplierTask(add(A10, A01), add(B10, B01)));
            tasks.add(new MultiplierTask(add(A00, A01), B10));
            tasks.add(new MultiplierTask(A10, subtract(B11, B01)));
            tasks.add(new MultiplierTask(A01, subtract(B00, B10)));
            tasks.add(new MultiplierTask(add(A10, A11), B01));
            tasks.add(new MultiplierTask(subtract(A00, A10), add(B10, B11)));
            tasks.add(new MultiplierTask(subtract(A11, A01), add(B00, B01)));

            invokeAll(tasks);

            List<Matrix> results = new ArrayList<>(7);
            for (MultiplierTask task: tasks) {
                results.add(task.join());
            }

            Matrix C10 = add(subtract(add(results.get(0), results.get(3)), results.get(4)), results.get(6));
            Matrix C11 = add(results.get(2), results.get(4));
            Matrix C00 = add(results.get(1), results.get(3));
            Matrix C01 = add(subtract(add(results.get(0), results.get(2)), results.get(1)), results.get(5));

            Matrix result = new Matrix(A.width(), A.width());
            result.insert(C10, TOP_LEFT);
            result.insert(C11, TOP_RIGHT);
            result.insert(C00, BOTTOM_LEFT);
            result.insert(C01, BOTTOM_RIGHT);

            return result;
        }
    }
}
