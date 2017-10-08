CC = gcc
TARGET = nowm
OBJS = main.o util.o hkey.o
CFLAGS = -g -Wall

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
clean:
	rm -f *.o $(TARGET)
