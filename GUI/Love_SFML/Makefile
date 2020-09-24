COMPILE=g++

LIBS= -lsfml-graphics -lsfml-window -lsfml-system
OBJS=test.o

run : build
	./a.out
clean_run : build clean
	./a.out
build : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS)
	
$(OBJS) : test.cpp
	$(COMPILE) test.cpp -c -o $@ -Werror
	
clean :
	rm -f *.o 
	rm -f *.log