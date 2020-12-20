COMPILE=g++
LIBS=-lGL -lsfml-graphics -lsfml-window -lsfml-system
OBJS=test.o Engine/EventSystem.o Engine/GraphSystem.o WindowManager.o

quiet_run : clean build
	rm -f output
	./test.out &>>output.log

clean_run : clean build
	./test.out

build : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS)  -o test.out -Werror

test.o : test.cpp 
	$(COMPILE) test.cpp -c -o test.o -Werror

Engine/EventSystem.o : Engine/EventSystem.cpp
	$(COMPILE) Engine/EventSystem.cpp -c -o Engine/EventSystem.o -Werror

Engine/GraphSystem.o : Engine/GraphSystem.cpp
	$(COMPILE) Engine/GraphSystem.cpp -c -o Engine/GraphSystem.o -Werror

WindowManager.o : WindowManager.cpp
	$(COMPILE) WindowManager.cpp -c -o WindowManager.o -Werror

clean:
	rm -f Engine/*.o
	rm -f *.o 
	rm -f *.log