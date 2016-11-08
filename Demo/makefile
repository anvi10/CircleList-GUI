CFLAGS = -g
COMPILE = g++ $(CFLAGS) -c
LINK = g++ $(CFLAGS)
LIBS = -lGL -lGLU -lglut

demo	: plot.o
	$(LINK) plot.o $(LIBS) -o demo

plot.o: plot.cpp plot.h
	$(COMPILE) plot.cpp

clean:
	rm *.o demo
