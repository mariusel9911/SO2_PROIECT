CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses -lmenu

TARGET = NetMonitor
SRCS = NetMonitor.c ui.c ipSearch_v1.c
HEADERS = ui.h ipSearch_v1.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
