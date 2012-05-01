#
# VoiceAnalysis is Audio Analysis Tool.
#
# Copyright (c) 2011-2012, Nickey Woo <thenickey@gmail.com>
# All rights reserved
#

TARGET := ../bin/snddiagram ../bin/sndanalysis ../bin/stereo2mono ../bin/sndwrite ../bin/reducerate
FLAGS := 
LIBS := -lsndfile

all: $(TARGET)

clean:
	rm -f $(TARGET)

../bin/snddiagram: sounddiagram_main.cpp
	g++ $^ -o $@ $(FLAGS) $(LIBS)

../bin/sndanalysis: soundanalysis_main.cpp
	g++ $^ -o $@ $(FLAGS) $(LIBS)
	
../bin/stereo2mono: stereo2mono_main.cpp
	g++ $^ -o $@ $(FLAGS) $(LIBS)
	
../bin/sndwrite: soundwrite_main.cpp
	g++ $^ -o $@ $(FLAGS) $(LIBS)

../bin/reducerate: reducerate_main.cpp
	g++ $^ -o $@ $(FLAGS) $(LIBS)

