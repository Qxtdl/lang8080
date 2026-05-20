SRC = src
BUILD = build
NAME = 8080-as
OUT = $(BUILD)/$(NAME)
ARGS = example/1.asm $(BUILD)/out.bin

COBJ = $(patsubst %.c,$(BUILD)/%.o,$(wildcard $(SRC)/*.c))

CC = cc
CFLAGS = -g -Iinclude -Wall

all: clean $(OUT) run

clean:
	mkdir -p $(BUILD)
	rm -rf $(BUILD)/*

run: $(OUT)
	./$(OUT) $(ARGS)

$(OUT): $(COBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(COBJ) $(LIBS)
	sudo cp $(OUT) /usr/bin/$(NAME)

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
