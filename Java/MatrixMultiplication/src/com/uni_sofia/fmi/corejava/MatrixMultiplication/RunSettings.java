package com.uni_sofia.fmi.corejava.MatrixMultiplication;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions.ArgsParseException;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Watcher;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.Messages.*;

/**
* Input parsing logic goes here.
*/
public class RunSettings {
    // Tried Enum it does not do the job.
    private static final String GENERAL       = "-g";
    private static final String PARALLEL      = "-p";
    private static final String BENCHMARK     = "-b";
    private static final String WATCH         = "-w";
    private static final String HELP          = "-h";

    public static final int CORE_COUNT = Runtime.getRuntime().availableProcessors();

    private boolean mPrintManual = false;
    private boolean mWatch = false;
    private boolean mBenchmark = false;
    private boolean mUseParallel = false;
    private boolean mUseGeneral = false;
    private int mBenchmarkPasses = 1;
    private int mPoolSize = CORE_COUNT;
    private int mReportFrequency = Watcher.DEFAULT_REPORT_FREQUENCY;

    private Path mSourceA;
    private Path mSourceB;
    private Path mDestination;

    public static RunSettings parseArgs(String[] args) throws ArgsParseException {
        RunSettings settings = new RunSettings();

        if (args.length == 0 || (args.length == 1 && args[0].equals(HELP))) {
            settings.mPrintManual = true;
            return settings;
        }

        Set<String> used = new HashSet<>();

        for(int i = 0; i < args.length - 3; ++i) {
            if (used.contains(args[i])) { // allowing only single option use for clarity.
                throw new ArgsParseException(args[i] + E_OPTION_REPETITION);
            }
            used.add(args[i]);

            switch (args[i]) {
                case GENERAL   :
                    settings.mUseGeneral = true;
                    break;

                case PARALLEL  :
                    settings.mUseParallel = true;

                    if (isInteger(args[i+1]))
                        settings.setPoolSize(Integer.parseInt(args[++i]));

                    break;

                case BENCHMARK :
                    settings.mBenchmark = true;

                    if (isInteger(args[i+1]))
                        settings.setBenchmarkPasses(Integer.parseInt(args[++i]));

                    break;

                case WATCH     :
                    settings.mWatch = true;

                    if (isInteger(args[i+1]))
                        settings.setReportFrequency(Integer.parseInt(args[++i]));

                    break;

                case HELP      :
                    throw new ArgsParseException(E_HELP_USED_NOT_ALONE);

                default:
                    throw new ArgsParseException(args[i] + E_UNKNOWN_OPTION);
            }
        }
        if (args.length < 3)
            throw new ArgsParseException(E_MISSING_PATHS);

        settings.setSources(Arrays.copyOfRange(args, args.length - 3, args.length));
        settings.validate();
        return settings;
    }

    private static boolean isInteger(String arg) {
        return arg.matches("-?\\d+");
    }

    /**
     * Change this method if you want to catch wrong paths earlier.
    **/
    private void setSources(String[] sources) throws ArgsParseException {
        mSourceA = Paths.get(sources[0]);
        mSourceB = Paths.get(sources[1]);

        mDestination = Paths.get(sources[2]);
    }

    private void setPoolSize(int size) throws ArgsParseException {
        if (size <= 0 || size > CORE_COUNT * 2) {
            throw new ArgsParseException(size + E_INVALID_POOL_SIZE);
        }
        mPoolSize = size;
    }

    private void setBenchmarkPasses(int passes) throws ArgsParseException {
        if (passes <= 0) {
            throw new ArgsParseException(passes + E_NEGATIVE_PASSES);
        }
        mBenchmarkPasses = passes;
    }

    private void setReportFrequency(int frequency) throws ArgsParseException {
        if (frequency <= 0) {
            throw new ArgsParseException(frequency + E_INVALID_FREQUENCY);
        }
        if (frequency < Watcher.MIN_REPORT_FREQUENCY) {
            mReportFrequency = Watcher.MIN_REPORT_FREQUENCY;
            return;
        }
        mReportFrequency = frequency;
    }

    private void validate() throws ArgsParseException {
        if (mWatch && !mBenchmark) {
            throw new ArgsParseException(E_BENCHLESS_WATCH);
        }
        if (mWatch && !mUseParallel) {
            throw new ArgsParseException(E_SERIAL_WATCH);
        }
    }

    public boolean shouldPrintManual() {
        return mPrintManual;
    }

    public boolean shouldBenchmark() {
        return mBenchmark;
    }

    public boolean shouldWatch() {
        return mWatch;
    }

    public boolean shouldUseParallel() {
        return mUseParallel;
    }

    public boolean shouldUseGeneral() {
        return mUseGeneral;
    }

    public int getBenchmarkPasses() {
        return mBenchmarkPasses;
    }

    public int getPoolSize() {
        return mPoolSize;
    }

    public int getReportFrequency() {
        return mReportFrequency;
    }

    public Path getSourceA() {
        return mSourceA;
    }

    public Path getSourceB() {
        return mSourceB;
    }

    public Path getDestination() {
        return mDestination;
    }

    private RunSettings() {}
}
