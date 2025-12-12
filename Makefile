CC = gcc
CFLAGS_GUI = -Wall -Wextra -std=c99 $(shell pkg-config --cflags gtk+-3.0)
CFLAGS_TERMINAL = -Wall -Wextra -std=c99
LIBS = $(shell pkg-config --libs gtk+-3.0)
TARGET = cpu_scheduler_gui
TERMINAL_TARGET = cpu_scheduler_terminal

# Source files for GUI
GUI_SOURCES = src/gui/main.c \
              src/gui/main_window.c \
              src/gui/process_input.c \
              src/gui/results_display.c \
              src/gui/gtk_helpers.c

# Core algorithm files - ONLY cpu_scheduler.c since all algorithms are integrated there
CORE_SOURCES = src/core/cpu_scheduler.c

# Source files for terminal version
TERMINAL_SOURCES = src/main_terminal.c

all: $(TARGET) $(TERMINAL_TARGET)

$(TARGET): $(GUI_SOURCES) $(CORE_SOURCES)
	$(CC) $(CFLAGS_GUI) -o $(TARGET) $(GUI_SOURCES) $(CORE_SOURCES) $(LIBS) -lm

$(TERMINAL_TARGET): $(TERMINAL_SOURCES) $(CORE_SOURCES)
	$(CC) $(CFLAGS_TERMINAL) -o $(TERMINAL_TARGET) $(TERMINAL_SOURCES) $(CORE_SOURCES) -lm

clean:
	rm -f $(TARGET) $(TERMINAL_TARGET)

install-dependencies:
	sudo apt-get update
	sudo apt-get install libgtk-3-dev

.PHONY: all clean install-dependencies
