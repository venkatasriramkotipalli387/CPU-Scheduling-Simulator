# CPU Scheduling Simulator

This project is a CPU Scheduling Simulator written in C to demonstrate how different CPU scheduling algorithms work in an operating system.
It allows users to enter process details and observe execution order, timing values, and Gantt chart output using both terminal and GUI modes.

A detailed explanation of design, algorithms, and results is provided in the Documentation Report included in this repository.

## The Simulator Shows:
* Execution order of processes
* Gantt chart visualization
* Completion Time, Waiting Time, Turnaround Time, Response Time
* Difference between preemptive and non-preemptive scheduling

## Algorithms Implemented:
* FCFS
* SJF
* SRTF
* Priority (Preemptive & Non-Preemptive)
* Round Robin
* Multilevel Queue (MLQ)
* Multilevel Feedback Queue (MLFQ)
* Completely Fair Scheduler (CFS)

## Tech Stack:
- Language: C
- GUI: GTK
- Build: Makefile
- Platform: Linux / macOS

## Requirements:
* GCC compiler
* GTK+ 3 (pkg-config must be available)

## HOW TO RUN:
### Compile and Run the GUI Version

```bash
cd CPU_Scheduling_Project

gcc -Wall -Wextra -std=c99 $(pkg-config --cflags gtk+-3.0) -Iinclude \
src/gui/main.c src/gui/main_window.c src/gui/process_input.c \
src/gui/results_display.c src/gui/gtk_helpers.c src/core/cpu_scheduler.c \
-o cpu_scheduler_gui $(pkg-config --libs gtk+-3.0) -lm && ./cpu_scheduler_gui



