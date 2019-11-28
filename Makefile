.PHONY: all clean mrproper rebuild install uninstall

CC = gcc
CFLAGS = -Wall -Wextra

OBJECTS = brightnessctl.o
NAME = brightnessctl
INSTALL_PATH = /usr/local/bin/$(NAME)
INSTALL_MODE = 4555
INSTALL_OWNER = root

BASE_PATH = $(shell echo /sys/class/backlight/* | head -n1)
DEFINES = -DBASE_PATH="\"$(BASE_PATH)\""

all: brightnessctl

clean:
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(NAME)

rebuild: mrproper all

install: all
	install -s -m $(INSTALL_MODE) -o $(INSTALL_OWNER) -T $(NAME) $(INSTALL_PATH)

uninstall:
	rm -f $(INSTALL_PATH)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEFINES) $(CFLAGS) -o $@ $^ -c

