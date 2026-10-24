CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
CPPFLAGS = -Iheaders

TARGET = bin/labirinto
SOURCES = main.c $(wildcard codes/*.c)
OBJECTS = $(patsubst %.c,obj/%.o,$(notdir $(SOURCES)))
HEADERS = $(wildcard headers/*.h)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

obj/main.o: main.c $(HEADERS) | obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

obj/%.o: codes/%.c $(HEADERS) | obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

obj bin:
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) obj/*.o