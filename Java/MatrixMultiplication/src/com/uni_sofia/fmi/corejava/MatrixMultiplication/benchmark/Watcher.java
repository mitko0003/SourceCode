package com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark;

import static com.uni_sofia.fmi.corejava.MatrixMultiplication.Messages.*;

/**
 * Runnable class which periodically prints to console and logs
 * info about a watchable task.
 */
public class Watcher extends Thread {

    public static final int MIN_REPORT_FREQUENCY = 50;
    public static final int DEFAULT_REPORT_FREQUENCY = 1000;

    private Watchable mTask;
    private long mReportFrequency = 1000;

    public Watcher(Watchable task, int reportFrequency) {
        mTask = task;
        if (reportFrequency > 0) {
            mReportFrequency = reportFrequency;
        }
    }

    @Override
    public void run() {
        while (true) {
            String message = getMessage();

            System.out.println(message);

            try {
                Thread.sleep(mReportFrequency); // until the next report
            } catch (InterruptedException e) {
                return; // Interrupted by the benchmarker when finished.
            }
        }
    }

    private String getMessage() {
        if (mTask.isRunning()) {
            int threadsInUse = mTask.getThreadUsage();
            return M_THREAD_USED.replace("0", String.valueOf(threadsInUse));
        } else {
            return M_NOT_RUNNING;
        }
    }
}
