package com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions;

import java.io.IOException;

/**
 * Every throw of this exception should result in printing/logging its message to the user
 * and termination of the program.
 */
public class ArgsParseException extends IOException {

    private static final long serialVersionUID = 1L;

    public ArgsParseException(String message) {
        super(message);
    }

}
