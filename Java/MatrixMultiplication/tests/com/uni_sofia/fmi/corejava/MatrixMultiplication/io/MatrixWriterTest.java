package com.uni_sofia.fmi.corejava.MatrixMultiplication.io;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.Assert.*;

public class MatrixWriterTest {

    private static final Path testMatrixPath = Paths.get("TestData\\testReaderWriterMatrix");

    @Test
    public void writeMatrixShouldWorkProperly() throws IOException {
        Matrix writeMatrix = new Matrix(5, 5);

        for (int i = 0; i < writeMatrix.height(); ++i)
            for (int j = 0; j < writeMatrix.width(); ++j)
                writeMatrix.set(i, j, i + j);

        try (MatrixWriter matrixWriter = new MatrixWriter(testMatrixPath)) {
            matrixWriter.writeMatrix(writeMatrix);
        }

        // Check if written correctly
        try (MatrixReader matrixReader = new MatrixReader(testMatrixPath)) {
            Matrix readMatrix = matrixReader.readMatrix();
            Assert.assertTrue(readMatrix.equals(writeMatrix));
        }
    }

    /**
     * This test's purpose is to make sure one would not forget to close the resources used by MatrixWriter
     */
    @Test (expected = IOException.class)
    public void writerCloseShouldFailOnSecondCall() throws IOException{
        MatrixWriter matrixWriter = null;

        // For test purposes otherwise one should use try with resources!
        try {
            matrixWriter = new MatrixWriter(testMatrixPath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }finally {
            try{
                if (matrixWriter != null)
                    matrixWriter.close();
            } catch (IOException e){
                e.printStackTrace();
            }
        }

        if (matrixWriter != null)
            matrixWriter.writeMatrix(new Matrix(5, 5));
    }
}