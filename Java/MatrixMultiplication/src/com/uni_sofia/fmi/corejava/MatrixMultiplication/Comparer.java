//package com.uni_sofia.fmi.corejava.MatrixMultiplication;
//
//import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Benchmark;
//import com.uni_sofia.fmi.corejava.MatrixMultiplication.io.MatrixReader;
//import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
//import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.StrassenMatrixMultiplierParallel;
//import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.StrassenMatrixMultiplierSerial;
//
//import java.io.File;
//import java.io.IOException;
//import java.io.PrintStream;
//import java.io.PrintWriter;
//import java.nio.file.Paths;
//
//import static com.uni_sofia.fmi.corejava.MatrixMultiplication.Messages.M_LOADING_MATRICES;
//
///**
//* Class created to satisfy task requirements.
//*/
//public class Comparer {
//
//    private static final String LEFT_PATH  = "TestData\\Ex1\\left";
//    private static final String RIGHT_PATH = "TestData\\Ex1\\right";
//    private static final String BENCHMARK_PATH = "TestData\\Ex1\\benchmark";
//
//    private static Matrix A;
//    private static Matrix B;
//
//    private static final int PASSES = 10;
//
//    public static void main(String[] args) throws IOException {
//        loadSourceMatrices();
//        StrassenMatrixMultiplierSerial multiplierSerial = new StrassenMatrixMultiplierSerial(A, B);
//        StrassenMatrixMultiplierParallel multiplierParallel = new StrassenMatrixMultiplierParallel(A, B);
//
//        Benchmark benchmarkSerial = new Benchmark.BenchmarkBuilder(multiplierSerial).passes(PASSES).build();
//        benchmarkSerial.benchmark();
//        long serialTime = benchmarkSerial.getLastBenchmarkRuntime();
//
//        Benchmark benchmarkParallel = new Benchmark.BenchmarkBuilder(multiplierParallel).passes(PASSES).build();
//        try (PrintWriter pw = new PrintWriter(new File(BENCHMARK_PATH))) {
//            pw.println("Average serial time is: " + serialTime / (double) PASSES);
//
//            for (int i = 1; i <= RunSettings.CORE_COUNT*2; ++i) {
//                multiplierParallel.setPoolSize(i);
//                benchmarkParallel.benchmark();
//                long parallelTime = benchmarkParallel.getLastBenchmarkRuntime();
//
//                pw.printf("Average parallel with %d pool size is: %.1f", i, parallelTime / (double) PASSES);
//                pw.println();
//                pw.printf("a(%d) = T / t(%d) = %.4f", i, i, serialTime / (double) parallelTime);
//                pw.println();
//            }
//        }
//    }
//
//    private static void loadSourceMatrices() throws IOException {
//        System.out.println(M_LOADING_MATRICES);
//        try (MatrixReader sourceA = new MatrixReader(Paths.get(LEFT_PATH));
//             MatrixReader sourceB = new MatrixReader(Paths.get(RIGHT_PATH))) {
//            A = sourceA.readMatrix();
//            B = sourceB.readMatrix();
//        }
//    }
//
//}
