CC = g++
CFLAGS = -g -Wall -std=c++11
TARGET = nopanel
OBJS = main.o font.o
SRCS = $(OBJS:%.o=%.cpp)
X11 = -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
Xft = -I/usr/include/freetype2 -lXft

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@  $(X11) $(Xft)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) main.cpp -c -o main.o 
	$(CC) $(CFLAGS) font.cpp -c -o font.o $(Xft) 

clean:
	rm -f *.o $(TARGET)
