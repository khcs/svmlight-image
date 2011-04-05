#
# makefile for svm_light image processing
# Ken HC Shin, 2011
#

# use the following to compile
CXX = g++ -bind_at_load
LD	= g++

# compiler flags
CXXFLAGS	= `pkg-config --cflags opencv`

# linker flags
LDFLAGS		= `pkg-config --libs opencv`

#used libraries
LIBS			= -lcxcore -lcv -lhighgui -lcvaux -lstdc++
# don't need for now on Mac OS X with CXXFLAGS and LDFLAGS above.

all: svmlight_image

clean: rm -f $(all) *.o



svmlight_image: svmlight_image.o svmlight_image_main.o
	$(LD) $(LDFLAGS) svmlight_image.o svmlight_image_main.o -o svmlight_image

svmlight_image.o: svmlight_image.cpp svmlight_image.h
	$(CXX) -c $(CXXFLAGS) svmlight_image.cpp -o svmlight_image.o $(LDFLAGS)

svmlight_image_main.o: svmlight_image_main.cpp svmlight_image.h
	$(CXX) -c $(CXXFLAGS) svmlight_image_main.cpp -o svmlight_image_main.o $(LDFLAGS)
