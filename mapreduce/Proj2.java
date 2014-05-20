/*
 * CS61C Spring 2014 Project2
 * Reminders:
 *
 * DO NOT SHARE CODE IN ANY WAY SHAPE OR FORM, NEITHER IN PUBLIC REPOS OR FOR DEBUGGING.
 *
 * DO NOT MODIFY THIS FILE. ANY CHANGES HERE WILL NOT BE RUN IN THE GRADING OF YOUR PROJECT.
 */
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.lang.Math;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
/**
 * Contains the main method for the CS61C Spring 2014 Project 2 that sets up the different
 * runs of each of the mapreduce jobs involved in solving the game of Connect N.
 *
 */
public class Proj2 {
    /**
     * Sets up and runs each of the mapreduce jobs used to solve Connect N.
     *
     */
    public static void main(String[] rawArgs) throws Exception {
        GenericOptionsParser parser = new GenericOptionsParser(rawArgs);
        Configuration conf = parser.getConfiguration();
        String[] args = parser.getRemainingArgs();

        // This is to indicate who's turn the game is.
        boolean OTurn = true;

        int boardWidth = 3;
        int boardHeight = 3;
        int connectWin = 3;
        String pathName = "all_data";

        try {
            boardWidth = Integer.parseInt(args[0]);
            boardHeight = Integer.parseInt(args[1]);
            connectWin = Integer.parseInt(args[2]);
        } catch (Exception e) {
            System.out.printf("You need to specify the width, height, and connection needed to win (in that order) as arguments.\n");
            System.out.printf("You should probably be using the Makefile by calling make; for example use make proj2 WIDTH=4 HEIGHT=3 CONNECT=3 (or some other numbers.)\n");
            System.exit(1);
        }

        try {
            pathName = args[3];
        } catch (Exception e) {

        }

        int area = boardWidth * boardHeight;

        // Setting up all of the directories that will be used.
        Path[] dataFirst = new Path[area+1];
        Path[] dataSecond = new Path[area];
        Path initPath = new Path(pathName + "/temp");
        Path finalPath = new Path(pathName + "/final");

        for (int i = 0; i <= area; i++) {
            dataFirst[i] = new Path(pathName + "/data_first" + i);
        }

        for (int i = 1; i < area; i++) {
            dataSecond[i] = new Path(pathName + "/data_second" + i);
        }

        // Setting up the variables that the mapreduce needs to solve games.
        conf.setInt("boardWidth", boardWidth);
        conf.setInt("boardHeight", boardHeight);
        conf.setInt("connectWin", connectWin);

        // Setting up the initial run. You don't even write code for this mapreduce.
        {
            Job initJob = new Job(conf, "initJob");

            initJob.setJarByClass(InitFirst.class);
            initJob.setMapOutputKeyClass(IntWritable.class);
            initJob.setMapOutputValueClass(MovesWritable.class);
            initJob.setOutputKeyClass(IntWritable.class);
            initJob.setOutputValueClass(MovesWritable.class);

            initJob.setMapperClass(InitFirst.Map.class);
            initJob.setReducerClass(InitFirst.Reduce.class);

            initJob.setInputFormatClass(TextInputFormat.class);
            initJob.setOutputFormatClass(SequenceFileOutputFormat.class);

            FileInputFormat.addInputPath(initJob, initPath);
            FileOutputFormat.setOutputPath(initJob, dataFirst[0]);

            initJob.waitForCompletion(true);
        }

        // Setting up the PossibleMoves run. This is the first of the two map reduces you write.
        for (int i = 1; i <= area; i++) {
            Job firstJob = new Job(conf, "job1_iter" + i);

            firstJob.setJarByClass(PossibleMoves.class);

            // Determining who's turn this is.
            OTurn = (i % 2) == 0;
            firstJob.getConfiguration().setBoolean("OTurn", OTurn);
            firstJob.getConfiguration().setBoolean("lastRound", i == area);

            firstJob.setMapOutputKeyClass(IntWritable.class);
            firstJob.setMapOutputValueClass(IntWritable.class);
            firstJob.setOutputKeyClass(IntWritable.class);
            firstJob.setOutputValueClass(MovesWritable.class);

            firstJob.setMapperClass(PossibleMoves.Map.class);
            firstJob.setReducerClass(PossibleMoves.Reduce.class);

            firstJob.setInputFormatClass(SequenceFileInputFormat.class);
            firstJob.setOutputFormatClass(SequenceFileOutputFormat.class);

            FileInputFormat.addInputPath(firstJob, dataFirst[i-1]);
            FileOutputFormat.setOutputPath(firstJob, dataFirst[i]);

            firstJob.waitForCompletion(true);
        }

        // Setting up the SolveMoves run. This is the second of the two map reduces you write.
        for (int i = area; i > 1; i--) {
            Job secondJob = new Job(conf, "job2_iter" + i);

            OTurn = (i % 2) == 0;
            secondJob.getConfiguration().setBoolean("OTurn", OTurn);

            secondJob.setJarByClass(SolveMoves.class);

            secondJob.setMapOutputKeyClass(IntWritable.class);
            secondJob.setMapOutputValueClass(ByteWritable.class);
            secondJob.setOutputKeyClass(IntWritable.class);
            secondJob.setOutputValueClass(MovesWritable.class);

            secondJob.setMapperClass(SolveMoves.Map.class);
            secondJob.setReducerClass(SolveMoves.Reduce.class);

            secondJob.setInputFormatClass(SequenceFileInputFormat.class);
            secondJob.setOutputFormatClass(SequenceFileOutputFormat.class);

            FileInputFormat.addInputPath(secondJob, dataFirst[i]);
            if (i != area) {
                FileInputFormat.addInputPath(secondJob, dataSecond[i]);
            }
            FileOutputFormat.setOutputPath(secondJob, dataSecond[i-1]);

            secondJob.waitForCompletion(true);
        }

        // Setting up the final run. This takes all of the outputs of solve moves
        // and possible moves, and outputs everything that doesn't have a value of 0.
        {
            Job thirdJob = new Job(conf, "job3");

            thirdJob.setJarByClass(FinalMoves.class);

            thirdJob.setMapOutputKeyClass(IntWritable.class);
            thirdJob.setMapOutputValueClass(ByteWritable.class);
            thirdJob.setOutputKeyClass(Text.class);
            thirdJob.setOutputValueClass(ByteWritable.class);

            thirdJob.setMapperClass(FinalMoves.Map.class);
            thirdJob.setReducerClass(FinalMoves.Reduce.class);

            thirdJob.setInputFormatClass(SequenceFileInputFormat.class);
            thirdJob.setOutputFormatClass(TextOutputFormat.class);

            for (int i = 1; i <= area; i++) {
                FileInputFormat.addInputPath(thirdJob, dataFirst[i]);
            }
            for (int i = 1; i < area; i++) {
                FileInputFormat.addInputPath(thirdJob, dataSecond[i]);
            }

            FileOutputFormat.setOutputPath(thirdJob, finalPath);

            thirdJob.waitForCompletion(true);
        }
    }
}
