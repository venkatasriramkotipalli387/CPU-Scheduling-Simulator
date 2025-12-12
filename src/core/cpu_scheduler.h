#ifndef CPU_SCHEDULER_H
#define CPU_SCHEDULER_H

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} Process;

// Function prototypes for all algorithms
void fcfs(Process p[], int n);
void sjf(Process p[], int n);
void srtf(Process p[], int n);
void round_robin(Process p[], int n, int quantum);
void priority_scheduling(Process p[], int n);  // Non-preemptive
void priority_preemptive(Process p[], int n);  // Preemptive
void hrrn(Process p[], int n);

// GUI helper function
char* get_scheduling_result_string(Process p[], int n, int algorithm, int quantum);

#endif
