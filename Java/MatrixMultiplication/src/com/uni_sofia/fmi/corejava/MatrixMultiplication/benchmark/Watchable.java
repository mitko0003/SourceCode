package com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark;

/**
 * Currently implemented only by parallel multipliers, but
 * one can extend the functionality
 */
public interface Watchable {
    public int getThreadUsage();
    public boolean isRunning();
}
