package com.uni_sofia.fmi.corejava.MatrixMultiplication.multipliers;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.io.MatrixReader;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Test;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

public class BaseMultiplierTest {

    private static Path sTestDataDir = Paths.get("TestData");
    protected static int sTestCaseCount = 2;
    protected Matrix A;
    protected Matrix B;
    protected Matrix C;

    protected void load(String dir) throws IOException {
        try (MatrixReader readerA = new MatrixReader(sTestDataDir.resolve(dir).resolve("left"));
             MatrixReader readerB = new MatrixReader(sTestDataDir.resolve(dir).resolve("right"));
             MatrixReader readerC = new MatrixReader(sTestDataDir.resolve(dir).resolve("result"))) {
            A = readerA.readMatrix();
            B = readerB.readMatrix();
            C = readerC.readMatrix();
        }
    }
}
