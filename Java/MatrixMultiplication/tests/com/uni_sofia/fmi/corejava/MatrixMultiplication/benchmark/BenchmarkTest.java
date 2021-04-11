package com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.ClassicalMatrixMultiplierSerial;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.StrassenMatrixMultiplierSerial;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class BenchmarkTest {

    private Benchmark dummyBenchmark;


    @Before
    public void setUp() {
        Matrix A = new Matrix(1000, 1000);
        Matrix B = new Matrix(1000, 1000);

        StrassenMatrixMultiplierSerial multiplier = new StrassenMatrixMultiplierSerial(A, B);

        dummyBenchmark = new Benchmark.BenchmarkBuilder(multiplier).passes(2).build();
    }

    @Test
    public void getLastBenchmarkShouldBeZeroIfNotExecuted() {
        assertEquals(0, dummyBenchmark.getLastBenchmarkRuntime());
    }

    @Test
    public void getLastBenchmarkShouldNotBeZeroIfExecuted() {
        dummyBenchmark.benchmark();
        assertNotEquals(0, dummyBenchmark.getLastBenchmarkRuntime());
    }

}