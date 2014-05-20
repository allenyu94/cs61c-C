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
 * Filters out any unnecessary lines from the final output file.
 *
 */
public class FinalMoves {
    public static class Map extends Mapper<IntWritable, MovesWritable, IntWritable, ByteWritable> {
        /**
         * Emits provided key-value pair as long as the value field of the MovesWritable is
         * non-zero.
         *
         */
        @Override
        public void map(IntWritable key, MovesWritable val, Context context) throws IOException, InterruptedException {
            byte value = val.getValue();
            if (value == 0) {
                return;
            }
            context.write(key, new ByteWritable(value));
        }
    }

    public static class Reduce extends Reducer<IntWritable, ByteWritable, Text, ByteWritable> {

        int boardWidth;
        int boardHeight;

        /**
         * Configuration and setup that occurs before map gets called for the first time.
         *
         **/
        @Override
        public void setup(Context context) {
            // load up the config vars specified in Proj2.java#main()
            boardWidth = context.getConfiguration().getInt("boardWidth", 0);
            boardHeight = context.getConfiguration().getInt("boardHeight", 0);
        }

        /**
         * Emit all the values for the final solved values table. We want to print this out with
         * the expanded string representation of the game board (to make the output file easily
         * human readable and thus easier to debug).
         *
         */
        @Override
        public void reduce(IntWritable key, Iterable<ByteWritable> values, Context context) throws IOException, InterruptedException {
            int keyInt = key.get();
            if (keyInt == 0)
                return;
            String hash = "'" + Proj2Util.gameUnhasher(keyInt, boardWidth, boardHeight) + "'";
            for (ByteWritable val: values)
                context.write(new Text(hash), val);
        }
    }
}
