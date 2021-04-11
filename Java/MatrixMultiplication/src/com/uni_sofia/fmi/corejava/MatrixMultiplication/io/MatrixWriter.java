package com.uni_sofia.fmi.corejava.MatrixMultiplication.io;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.matrix.Matrix;

import java.io.*;
import java.nio.file.Path;

/**
* Created by Wetslap7 on 2.1.2015 г..
*/
public class MatrixWriter implements Closeable {

    private final Path mMatrixFile;
    private OutputStream mOutputStream;
    private DataOutputStream mDataStream;

    public MatrixWriter(Path path) throws FileNotFoundException {
        mMatrixFile = path;
        openMatrixFile();
    }

    public void writeMatrix(Matrix C) throws IOException {
        // Writing out sizes
        mDataStream.writeInt(C.height());
        mDataStream.writeInt(C.width());

        // Writing out data
        for (int i = 0; i < C.height(); ++i) {
            for (int j = 0; j < C.width(); ++j) {
                mDataStream.writeDouble(C.get(i, j));
            }
        }

        mDataStream.flush();
    }

    @Override
    public void close() throws IOException {
        mDataStream.close();
        mOutputStream.close();
    }

    private void openMatrixFile() throws FileNotFoundException {
        mOutputStream = new FileOutputStream(mMatrixFile.toString());
        mDataStream = new DataOutputStream(mOutputStream);
    }

}
