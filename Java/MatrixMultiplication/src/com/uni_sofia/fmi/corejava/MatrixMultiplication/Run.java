package com.uni_sofia.fmi.corejava.MatrixMultiplication;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions.ArgsParseException;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Benchmark;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.io.MatrixReader;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.io.MatrixWriter;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.MatrixMultiplier;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers.MultiplierFactory;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.Messages.*;
import static com.uni_sofia.fmi.corejava.MatrixMultiplication.RunSettings.parseArgs;
import static com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Benchmark.BenchmarkBuilder;

import java.io.*;

public class Run {

    private static final String MANUAL_PATH = "res/manual.txt";

    private RunSettings mSettings;
    private MatrixMultiplier mMultiplier;
    private Benchmark mBenchmarker = null;
    private Matrix A;
    private Matrix B;

    public static void main(String[] args) {
        try {
            Run run = new Run(args);
            run.execute();
        } catch (ArgsParseException e) {
            System.out.println(e.getMessage()); // Check ArgsParseException documentation
        }
    }

    private Run(String[] args) throws ArgsParseException {
        mSettings = parseArgs(args);
    }

    private void execute() throws ArgsParseException {
        if (mSettings.shouldPrintManual()){
            printManual();
            return;
        }

        try {
            loadSourceMatrices();
        } catch (IOException e) {
            System.out.println(E_LOADING_MATRICES);
            return;
        }

        if(A.width() != B.height())
            throw new ArgsParseException(E_MATRIX_SIZES);

        mMultiplier = MultiplierFactory.createMultiplier(mSettings.shouldUseGeneral(),
                mSettings.shouldUseParallel(), A, B, mSettings.getPoolSize());

        Matrix result;
        if (mSettings.shouldBenchmark()) {
            buildBenchmark();

            System.out.println(M_BENCHMARKING);
            result = mBenchmarker.benchmark();
        } else {
            System.out.println(M_CALCULATING);
            try {
                result = mMultiplier.call();
            } catch (Exception e) {
                result = null;
            }
        }

        if (result == null) {
            System.out.println(E_UNCHECKED);
            return;
        }

        System.out.print(M_SAVING_RESULT);
        try (MatrixWriter resultWriter = new MatrixWriter(mSettings.getDestination())) {
            resultWriter.writeMatrix(result);
        } catch (IOException e) {
            System.out.println(E_SAVING_RESULT);
        }
    }

    private void buildBenchmark () {
        BenchmarkBuilder builder = new BenchmarkBuilder(mMultiplier).
                passes(mSettings.getBenchmarkPasses());

        if (mSettings.shouldWatch()) {
            builder.watch().reportFrequency(mSettings.getReportFrequency());
        }

        mBenchmarker = builder.build();
    }

    private void loadSourceMatrices() throws IOException {
        System.out.println(M_LOADING_MATRICES);
        try (MatrixReader sourceA = new MatrixReader(mSettings.getSourceA());
             MatrixReader sourceB = new MatrixReader(mSettings.getSourceB())) {
            A = sourceA.readMatrix();
            B = sourceB.readMatrix();
        }
    }

    private static void printManual() {
        try (InputStream is = Run.class.getClassLoader().getResourceAsStream(MANUAL_PATH);
            InputStreamReader ir = new InputStreamReader(is);
            BufferedReader br = new BufferedReader(ir)){

            String line;
            while((line = br.readLine()) != null) {
                System.out.println(line);
            }

        } catch (IOException e) {
            System.out.println(E_MANUAL);
        }
    }
}
