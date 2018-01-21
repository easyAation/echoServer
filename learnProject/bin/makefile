objects = echo.o config.o epoll.o handle_echo.o heart.o callbackfunc.o

echo : $(objects)
	gcc -o echo $(objects) -lpthread

heart.o : heart.h heart.c epoll.o config.o
	gcc -c heart.c -lphread 

callbackfunc.o : callbackfunc.h callbackfunc.c heart.o epoll.o
	gcc -c callbackfunc.c -lphread

config.o : config.c config.h
	gcc -c config.c	-lpthread

echo.o : echo.c echo.h  config.o callbackfunc.o
	gcc -c echo.c config.c -lphread

epoll.o : epoll.c epoll.h
	gcc -c epoll.c -lphread

handle_echo.o : handle_echo.c handle_echo.h epoll.o heart.o callbackfunc.o
	gcc -c handle_echo.c -lphread

.PHONY : clean 

clean : 
	-rm $(objects)
