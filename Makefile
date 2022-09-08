CC = gcc
CFLAGS = -Wall
OBJS = main.o stitch.o
OUT = fakeStitch

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

clean:
	rm -f $(OBJS) $(OUT) ./*~
