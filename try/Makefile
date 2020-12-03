COMPILE=g++
LIBS=-lGL -lsfml-graphics -lsfml-window -lsfml-system
OBJS=test.o


clean_run : clean build
	./test.out

build : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS) -o test.out

$(OBJS) : test.cpp
	$(COMPILE) test.cpp -c -o $@ -Werror

clean: 
	rm -f *.o 
	rm -f *.log