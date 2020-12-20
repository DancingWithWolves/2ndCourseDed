COMPILE=g++
LIBS=-lGL -lsfml-graphics -lsfml-window -lsfml-system
OBJS=Builds/test.o Builds/EventSystem.o Builds/GraphSystem.o Builds/WindowManager.o

quiet_run : clean build
	rm -f output
	./test.out &>>output.log

clean_run : clean build
	./test.out

build : $(OBJS)
	$(COMPILE) $(OBJS) $(LIBS) -o test.out -Werror

Builds/test.o : test.cpp 
	$(COMPILE) test.cpp -c -o Builds/test.o -Werror

Builds/EventSystem.o : Engine/EventSystem.cpp
	$(COMPILE) Engine/EventSystem.cpp -c -o Builds/EventSystem.o -Werror

Builds/GraphSystem.o : Engine/GraphSystem.cpp
	$(COMPILE) Engine/GraphSystem.cpp -c -o Builds/GraphSystem.o -Werror

Builds/WindowManager.o : WindowManager.cpp
	$(COMPILE) WindowManager.cpp -c -o Builds/WindowManager.o -Werror

clean:
	rm -f Engine/*.o
	rm -f *.o 
	rm -f *.log