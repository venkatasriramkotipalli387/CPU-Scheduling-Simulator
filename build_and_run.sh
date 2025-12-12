#!/bin/bash
echo "🔧 Compiling CPU Scheduling GUI..."

# Clean previous build
rm -f cpu_scheduler_gui

# Compile with all necessary flags and libraries
gcc -Wall -Wextra -std=c99 $(pkg-config --cflags gtk+-3.0) -Iinclude \
    src/gui/main.c \
    src/gui/main_window.c \
    src/gui/process_input.c \
    src/gui/results_display.c \
    src/gui/gtk_helpers.c \
    src/core/cpu_scheduler.c \
    -o cpu_scheduler_gui $(pkg-config --libs gtk+-3.0) -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful! Running GUI..."
    ./cpu_scheduler_gui
else
    echo "❌ Compilation failed!"
    exit 1
fi
