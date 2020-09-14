COMPILE=clang++

LIBS=-L/usr/X11R6/lib -lGL -lGLU -lglut -lXmu -lXi -lm
OBJS=main.o

all : ui_test

ui_test : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS)
	
$(OBJS) : windowManager.cpp
	$(COMPILE) windowManager.cpp -c -o $@ 
	
clean :
	rm -f *.o 
	rm -f ui_test