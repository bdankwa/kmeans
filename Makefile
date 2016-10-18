#-----------------------------------------------------------------
# Makefile for building code for CSCI Project 1
# Author: Boakye Dankwa
# Course: CSCI 36200
# Date: November 2013
#------------------------------------------------------------------
SRCDIR = ./src/
OBJECTS_SIMPLE =  $(SRCDIR)kmean-simple.o $(SRCDIR)KMeans.o $(SRCDIR)utils.o
OBJECTS_PROB = $(SRCDIR)kmean-prob.o $(SRCDIR)KMeans.o $(SRCDIR)utils.o
OBJECTS = $(OBJECTS_SIMPLE) $(OBJECTS_PROB)
TARGET_SIMPLE = kmean-simple
TARGET_PROB = kmean-prob
TARGET= $(TARGET_SIMPLE) $(TARGET_PROB)
INCLUDES =/usr/local/include.
LIBRARIES =.
CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
#------------------------------------------------------------------------------



all: $(TARGET)


$(TARGET_SIMPLE): $(OBJECTS_SIMPLE)
	$(CC) $(OBJECTS_SIMPLE) $(LFLAGS) -o$(TARGET_SIMPLE)

$(TARGET_PROB): $(OBJECTS_PROB)
	$(CC) $(OBJECTS_PROB) $(LFLAGS) -o$(TARGET_PROB)

$(SRCDIR)kmean-simple.o : $(SRCDIR)kmean-simple.cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) $(SRCDIR)kmean-simple.cpp -o $(SRCDIR)kmean-simple.o

$(SRCDIR)kmean-prob.o : $(SRCDIR)kmean-prob.cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) $(SRCDIR)kmean-prob.cpp -o $(SRCDIR)kmean-prob.o
	
$(SRCDIR)utils.o : $(SRCDIR)utils.cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) $(SRCDIR)utils.cpp -o $(SRCDIR)utils.o

$(SRCDIR)KMeans.o : $(SRCDIR)KMeans.cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) $(SRCDIR)KMeans.cpp -o $(SRCDIR)KMeans.o

clean:

	rm -f $(OBJECTS) $(TARGET)