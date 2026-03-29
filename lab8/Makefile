PROGRAMS = main
IMAGE_PROCESS_OBJS = convolve.o main.o lodepng/lodepng.o utils.o
OUTPUT = ./output/*.png

INCLUDES = -I.
CFLAGS = -g -Wall -O3 $(INCLUDES)

all: $(PROGRAMS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

main: $(IMAGE_PROCESS_OBJS)
	$(CC) -o main $(CFLAGS) $(IMAGE_PROCESS_OBJS)

clean:
	rm -rf $(PROGRAMS) $(IMAGE_PROCESS_OBJS) $(OUTPUT)
