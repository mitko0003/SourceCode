package com.uni_sofia.fmi.corejava.MatrixMultiplication;

import com.uni_sofia.fmi.corejava.MatrixMultiplication.Exceptions.ArgsParseException;
import com.uni_sofia.fmi.corejava.MatrixMultiplication.benchmark.Watcher;
import org.junit.Test;

import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.Assert.*;

public class RunSettingsTest {

    protected static String sExistingLeft = "TestData\\Ex1\\left";
    protected static String sExistingRight = "TestData\\Ex1\\right";
    protected static String sExistingDestination = "TestData\\Ex1\\result1";

    @Test
    public void testHelpOption() throws ArgsParseException {
        String args[] = {"-h"};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldPrintManual());
    }

    @Test
    public void noArgsShouldBringHelp() throws ArgsParseException {
        String args[] = {};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldPrintManual());
    }

    @SuppressWarnings("unused")
    @Test(expected = ArgsParseException.class)
    public void helpOptionNotAloneShouldFail() throws ArgsParseException {
        String args[] = {"-h", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @Test
    public void testBenchmark() throws ArgsParseException {
        String args[] = {"-b", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldBenchmark());
        assertEquals(1, settings.getBenchmarkPasses());
    }

    @Test
    public void testBenchmarkWithArg() throws ArgsParseException {
        String args[] = {"-b", "4", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldBenchmark());
        assertEquals(4, settings.getBenchmarkPasses());
    }

    @SuppressWarnings("unused")
    @Test(expected = ArgsParseException.class)
    public void benchmarkWithInvalidArgShouldFail() throws ArgsParseException {
        String args[] = {"-b", "-4", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @Test
    public void testParallel() throws ArgsParseException {
        String args[] = {"-p", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldUseParallel());
        assertEquals(Runtime.getRuntime().availableProcessors(), settings.getPoolSize());
    }

    @Test
    public void testParallelWithArg() throws ArgsParseException {
        String args[] = {"-p", "2",sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldUseParallel());
        assertEquals(2, settings.getPoolSize());
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void parallelWithInvalidArg1ShouldFail() throws ArgsParseException {
        String args[] = {"-p", "0",sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void parallelWithInvalidArg2ShouldFail() throws ArgsParseException {
        String args[] = {"-p", "40",sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @Test
    public void testGeneral() throws ArgsParseException {
        String args[] = {"-g", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldUseGeneral());
    }

    @Test
    public void testWatch() throws ArgsParseException {
        String args[] = {"-w", "-b", "-p", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldWatch());
        assertEquals(1000, settings.getReportFrequency());
    }

    @Test
    public void testWatchWithArg() throws ArgsParseException {
        String args[] = {"-w", "500", "-b", "-p", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldWatch());
        assertEquals(500, settings.getReportFrequency());
    }

    @Test
    public void watchWithTooSmallArgShouldSetMin() throws ArgsParseException {
        String args[] = {"-w", "1", "-b", "-p", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertTrue(settings.shouldWatch());
        assertEquals(Watcher.MIN_REPORT_FREQUENCY, settings.getReportFrequency());
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void watchWithInvalidArgShouldFail() throws ArgsParseException {
        String args[] = {"-w", "-1", "-b", "-p", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void watchWithoutBenchmarkShouldFail() throws ArgsParseException {
        String args[] = {"-w", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void watchWithoutParallelShouldFail() throws ArgsParseException {
        String args[] = {"-w", "-b", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void unknownOptionShouldFail() throws ArgsParseException {
        String args[] = {"-fd", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void repeatingOptionShouldFail() throws ArgsParseException {
        String args[] = {"-w", "-b", "-p", "-b", "20", sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @SuppressWarnings("unused")
    @Test (expected = ArgsParseException.class)
    public void notEnoughArgsShouldFail() throws ArgsParseException {
        String args[] = {sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
    }

    @Test
    public void testPaths() throws ArgsParseException {
        String args[] = {sExistingLeft, sExistingRight, sExistingDestination};
        RunSettings settings = RunSettings.parseArgs(args);
        assertEquals(Paths.get(sExistingLeft), settings.getSourceA());
        assertEquals(Paths.get(sExistingRight), settings.getSourceB());
        assertEquals(Paths.get(sExistingDestination), settings.getDestination());
    }

}