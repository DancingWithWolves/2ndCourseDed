COMPILE=g++

LIBS=-L/usr/X11R6/lib -lGL -lGLU -lglut -lm
OBJS=main.o

run : build
	./a.out
clean_run : build clean
	./a.out
build : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS)
	
$(OBJS) : main.cpp
	$(COMPILE) main.cpp -c -o $@ 
	
clean :
	rm -f *.o 
	rm -f *.log