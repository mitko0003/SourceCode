package com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.MatrixMultiplier;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.Messages.*;

import java.sql.Time;
import java.util.concurrent.TimeUnit;

public class Benchmark {

    private final MatrixMultiplier mMultiplier;
    private Watcher mWatcher = null;
    private int mPasses;
    private long mRuntime = 0;
    private TimeUnit mTimeUnit;

    private Benchmark(BenchmarkBuilder builder) {
        mMultiplier = builder.mMultiplier;
        mPasses = builder.mPasses;
        mTimeUnit = builder.mTimeUnit;

        if (builder.mWatch) {
            mWatcher = new Watcher((Watchable) mMultiplier, builder.mReportFrequency);
        }
    }

    public Matrix benchmark() {
        mRuntime = 0;
        Matrix result = null;
        if (mWatcher != null)
            mWatcher.start();

        printPreComputationInfo();
        try {
            for(int i = 0; i < mPasses; ++i) {
                long then = System.nanoTime();

                result = mMultiplier.call();

                long runtime = System.nanoTime() - then;
                mRuntime += runtime;

                System.out.printf(M_AFTER_PASS, i + 1,
                        mTimeUnit.convert(runtime, TimeUnit.NANOSECONDS),
                        mTimeUnit.toString().toLowerCase());
            }
        } catch (Exception e) {
            return null;
        }

        if (mWatcher != null)
            mWatcher.interrupt();

        printGatheredInfo();
        return result;
    }

    public long getLastBenchmarkRuntime() {
        return mTimeUnit.convert(mRuntime, TimeUnit.NANOSECONDS);
    }

    private void printGatheredInfo() {
        System.out.printf(M_TOTAL_RUNTIME, mPasses,
                mTimeUnit.convert(mRuntime, TimeUnit.NANOSECONDS),
                mTimeUnit.toString().toLowerCase());
    }

    private void printPreComputationInfo() {
        System.out.printf(M_MATRIX_INFO, "Left",
                mMultiplier.getLeft().width(),
                mMultiplier.getLeft().height());

        System.out.printf(M_MATRIX_INFO, "Right",
                mMultiplier.getRight().width(),
                mMultiplier.getRight().height());
    }

    public static class BenchmarkBuilder {
        private final MatrixMultiplier mMultiplier;
        private boolean mWatch = false;
        private int mPasses = 1;
        private int mReportFrequency = Watcher.DEFAULT_REPORT_FREQUENCY;
        private TimeUnit mTimeUnit = TimeUnit.MILLISECONDS;

        public BenchmarkBuilder(MatrixMultiplier multiplier) {
            mMultiplier = multiplier;
        }

        public BenchmarkBuilder watch() {
            mWatch = true;
            return this;
        }

        public BenchmarkBuilder reportFrequency(int frequency) {
            mReportFrequency = frequency;
            return this;
        }

        public BenchmarkBuilder passes(int passesCount) {
            mPasses = passesCount;
            return this;
        }

        public Benchmark build() {
            return new Benchmark(this);
        }
    }
}
