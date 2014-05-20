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
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

/**
 * Generates the empty board configuration for the first run of the mapreduce in PossibleMoves.java
 *
 */
public class InitFirst {

    public static class Map extends Mapper<LongWritable, Text, IntWritable, MovesWritable> {
        /**
         * Write the empty board configuration to the context.
         *
         */
        @Override
        public void map(LongWritable key, Text val, Context context) throws IOException, InterruptedException {
            context.write(new IntWritable(0), new MovesWritable((byte)0, null));
        }
    }

    public static class Reduce extends Reducer<IntWritable, MovesWritable, IntWritable, MovesWritable> {
        /**
         * Write the empty board configuration to the context.
         *
         */
        @Override
        public void reduce(IntWritable key, Iterable<MovesWritable> values, Context context) throws IOException, InterruptedException {
            context.write(new IntWritable(0), new MovesWritable((byte)0, null));
        }
    }
}
