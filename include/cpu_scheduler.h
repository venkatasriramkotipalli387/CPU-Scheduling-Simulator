#ifndef CPU_SCHEDULER_H
#define CPU_SCHEDULER_H

#define MAX_GANTT_ENTRIES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int queue_level;  // For multi-level queue
    int nice;         // For CFS scheduling
} Process;

// Gantt chart entry structure
typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttEntry;

// Gantt chart structure
typedef struct {
    GanttEntry entries[MAX_GANTT_ENTRIES];
    int count;
} GanttChart;

// Function prototypes
void fcfs(Process p[], int n, GanttChart *gantt);
void sjf(Process p[], int n, GanttChart *gantt);
void srtf(Process p[], int n, GanttChart *gantt);
void round_robin(Process p[], int n, int quantum, GanttChart *gantt);
void priority_scheduling(Process p[], int n, GanttChart *gantt);
void priority_preemptive(Process p[], int n, GanttChart *gantt);
void hrrn(Process p[], int n, GanttChart *gantt);
void multi_level_queue(Process processes[], int n, GanttChart *gantt);
void schedule_cfs(Process processes[], int n, GanttChart *gantt);
void mlfq(Process processes[], int n, GanttChart *gantt);

// Helper function to get results as string for GUI
char* get_scheduling_result_string(Process p[], int n, int algorithm, int quantum, GanttChart *gantt);

// Gantt chart helper function
char* gantt_chart_to_string(GanttChart *gantt);

#endif
