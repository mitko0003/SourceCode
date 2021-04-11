package com.uni_sofia.fmi.corejava.MatrixMultiplication.io;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.Assert.*;

public class MatrixReaderTest {

    private static final Path testMatrixPath = Paths.get("TestData\\testReaderWriterMatrix");

    @Test
    public void readMatrixShouldWorkProperly() throws IOException {

        try (FileOutputStream FS = new FileOutputStream(testMatrixPath.toString());
             DataOutputStream DS = new DataOutputStream(FS)) {
            DS.writeInt(5);
            DS.writeInt(5);
            for (int i = 0; i < 5; ++i)
                for (int j = 0; j < 5; ++j)
                    DS.writeDouble(i+j);
        }

        try (MatrixReader matrixReader = new MatrixReader(testMatrixPath)) {
            Matrix readMatrix = matrixReader.readMatrix();

            Assert.assertEquals(5, readMatrix.width());
            Assert.assertEquals(5, readMatrix.height());

            for (int i = 0; i < readMatrix.height(); ++i)
                for (int j = 0; j < readMatrix.width(); ++j)
                    Assert.assertEquals(i + j, readMatrix.get(i, j), Matrix.sEpsilon);

        }
    }

    @SuppressWarnings("unused")
    @Test(expected = FileNotFoundException.class)
    public void createReaderForNotExistingFileShouldFail() throws FileNotFoundException{
        Path notExistingMatrixFile = Paths.get("notExisting");
        MatrixReader matrixReader = new MatrixReader(notExistingMatrixFile);
    }

    /**
     * This test's purpose is to make sure one would not forget to close the resources used by MatrixReader
     */
    @Test (expected = IOException.class)
    public void readerCloseShouldFailInSecondCall() throws IOException{
        MatrixReader matrixReader = null;

        // For test purposes otherwise one should use try with resources!
        try {
            matrixReader = new MatrixReader(testMatrixPath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }finally {
            try{
                if (matrixReader != null)
                    matrixReader.close();
            } catch (IOException e){
                e.printStackTrace();
            }
        }

        if (matrixReader != null)
            matrixReader.readMatrix();
    }
}