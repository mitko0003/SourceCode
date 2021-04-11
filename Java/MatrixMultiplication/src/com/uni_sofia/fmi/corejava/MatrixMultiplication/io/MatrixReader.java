package com.uni_sofia.fmi.corejava.MatrixMultiplication.io;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import java.io.*;
import java.nio.file.Path;

public class MatrixReader implements Closeable {

    private final Path mMatrixFile;
    private InputStream mInputStream;
    private DataInputStream mDataStream;

    public MatrixReader(Path path) throws FileNotFoundException {
        mMatrixFile = path;
        openMatrixFile();
    }

    public Matrix readMatrix() throws IOException {
        int m = mDataStream.readInt();
        int n = mDataStream.readInt();

        Matrix C = new Matrix(n, m);

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                C.set(i, j, mDataStream.readDouble());
            }
        }

        return C;
    }

    @Override
    public void close() throws IOException {
        mDataStream.close();
        mInputStream.close();
    }

    private void openMatrixFile() throws FileNotFoundException {
        mInputStream = new FileInputStream(mMatrixFile.toString());
        mDataStream = new DataInputStream(mInputStream);
    }

}
