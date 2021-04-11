package com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions;

import com.sun.javaws.exceptions.InvalidArgumentException;

/**
 * Created by Wetslap7 on 2.1.2015 г..
 */
public class MatrixSizeMultiplicationException extends IllegalArgumentException implements MsgException {

    private static final long serialVersionUID = 1L;

    public MatrixSizeMultiplicationException() {super();}

    @Override
    public String getErrorMsg() { return "Can not perform multiplication on matrices of that size."; }

}
