SRC = src
BUILD = build
OUT = $(BUILD)/cm8080-as
ARGS = example/1.asm

COBJ = $(patsubst %.c,$(BUILD)/%.o,$(wildcard $(SRC)/*.c))

CC = cc
CFLAGS = -Iinclude -Wall

all: clean $(OUT) run

clean:
	mkdir -p $(BUILD)
	rm -rf $(BUILD)/*

run: $(OUT)
	./$(OUT) $(ARGS)

$(OUT): $(COBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(COBJ) $(LIBS)

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
