# Build file for CS61C Project 2  [Spring 2014]
# You should not need to edit this file if you're working on the inst machines.
#
# This file requires GNU make and depends on paths on instruction machines.
#
# If you are working on your own machine, you will need to edit the paths.


####

## Variables

# Source files (java code). wildcard selects all files matching a pattern.
SOURCES = $(wildcard *.java)
# Output JAR file
TARGET = proj2.jar
# Extra JARs to have on the classpath when compiling.
CLASSPATH = /home/ff/cs61c/hadoop/hadoop-core.jar:/home/ff/cs61c/hadoop/lib/commons-cli.jar
# Compatibility flags to build for Java 6. Remove these flags if in the future
# the EC2 servers support Java 7 (or later versions)
COMPAT_FLAGS = -source 6 -target 6
# javac command to use
JAVAC = javac -g $(COMPAT_FLAGS) -deprecation -cp $(CLASSPATH)
# jar command to use
JAR = jar

HEIGHT=3
WIDTH=3
CONNECT=3
HDFS_LOC="hdfs:///all_data"

## Make targets

# General form is target: dependencies (targets or files), followed by
# commands to run to build the target from the dependencies.

# Default target.
all: classes $(SOURCES)
	$(JAVAC) -d classes $(SOURCES)
	$(JAR) cf $(TARGET) -C classes .

proj2: $(TARGET)
	rm -rf all_data
	mkdir all_data
	echo "0 0" >> all_data/temp
	time hadoop jar proj2.jar Proj2 $(WIDTH) $(HEIGHT) $(CONNECT)

proj2-hadoop: $(TARGET)
	hc large dfs -mkdir hdfs:///all_data
	echo "0 0" >> temp_data_that_will_be_erased
	hc large dfs -put temp_data_that_will_be_erased hdfs:///all_data
	rm temp_data_that_will_be_erased
	hc large dfs -mv hdfs:///all_data/temp_data_that_will_be_erased hdfs:///all_data/temp
	time hc large jar proj2.jar Proj2 $(WIDTH) $(HEIGHT) $(CONNECT) $(HDFS_LOC)
	hc large dfs -cat $(HDFS_LOC)/final/part-r-00000 > output_ec2.txt

proj2-hadoop-clean:
	hc large dfs -rmr hdfs:///all_data

classes:
	mkdir classes

clean:
	rm -rf classes all_data $(TARGET)

.PHONY: clean all
