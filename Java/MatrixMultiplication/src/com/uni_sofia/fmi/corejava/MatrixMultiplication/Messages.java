package com.uni_sofia.fmi.corejava.MatrixMultiplication;

/**
 * All messages to user go here (except those in resources).
 * Easier to make ui changes.
 * Do not use enum here it looks terrible in the code...
 */
public class Messages {
    // Error message
    public static final String E_MANUAL              = "Manual might not be shown correctly.";
    public static final String E_HELP_USED_NOT_ALONE = "-h can only be used alone.";
    public static final String E_OPTION_REPETITION   = " used more then once! Repeating options are not allowed.";
    public static final String E_UNKNOWN_OPTION      = " is not a valid option.";
    public static final String E_INVALID_POOL_SIZE   = " is an invalid threads to use argument.";
    public static final String E_NEGATIVE_PASSES     = " is an invalid number of passes to use for benchmarking.";
    public static final String E_INVALID_FREQUENCY   = " is an invalid report frequency.";
    public static final String E_MISSING_PATHS       = "Specify 3 Paths.";
    public static final String E_MATRIX_SIZES        = "Can not multiply the given matrices.";
    public static final String E_BENCHLESS_WATCH     = "-b needed for -w.";
    public static final String E_SERIAL_WATCH        = "Only parallel algorithms can be watched.";
    public static final String E_UNCHECKED           = "Something went terribly wrong...";
    public static final String E_LOADING_MATRICES    = "Failed to load source matrices.";
    public static final String E_SAVING_RESULT       = "Failed to save result.";

    // Normal message
    public static final String M_LOADING_MATRICES = "Loading matrices...";
    public static final String M_SAVING_RESULT    = "Saving result...";
    public static final String M_AFTER_PASS       = "Pass %d finished in %d %s\n";
    public static final String M_BENCHMARKING     = "Starting benchmark.";
    public static final String M_TOTAL_RUNTIME    = "Benchmark with %d passes finished in %d %s\n";
    public static final String M_NOT_RUNNING      = "Task is not running.";
    public static final String M_THREAD_USED      = "The multiplication task is currently using 0 threads.";
    public static final String M_CALCULATING      = "Calculating...";
    public static final String M_MATRIX_INFO      = "%s matrix is with width %d and height %d.\n";

    private Messages() {}
}
