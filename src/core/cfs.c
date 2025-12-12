#include "../include/cpu_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define MAX_PROCESSES 20
#define TIME_SLICE 10.0

// CFS-specific process structure
typedef struct {
    char pid[5];
    int nice;
    char type[10];     // "CPU" or "IO"
    float exec_time;   // total required execution time (ms)
    float remaining;   // remaining time
    float weight;
    float vruntime;
    int sleeping;      // 1 if I/O sleeping
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} CFSProcess;

// Approximate Linux-like weight calculation
static float get_weight(int nice) {
    // Simplified Linux formula - lower nice => higher weight
    return 1024.0 * pow(1.25, -nice);
}

// Find process with minimum vruntime that's not finished or sleeping
static int get_next_process(CFSProcess p[], int n) {
    int idx = -1;
    float min_vr = FLT_MAX;
    for (int i = 0; i < n; i++) {
        if (p[i].remaining > 0 && !p[i].sleeping && p[i].vruntime < min_vr) {
            min_vr = p[i].vruntime;
            idx = i;
        }
    }
    return idx;
}

// CFS scheduler implementation
void schedule_cfs(Process processes[], int n) {
    CFSProcess cfs_procs[MAX_PROCESSES];
    float total_weight = 0;
    int completed = 0;
    int time = 0;
    
    // Initialize CFS processes
    for (int i = 0; i < n; i++) {
        sprintf(cfs_procs[i].pid, "P%d", processes[i].pid);
        cfs_procs[i].nice = processes[i].nice; // Use nice value from Process struct
        strcpy(cfs_procs[i].type, "CPU"); // Default to CPU type
        cfs_procs[i].exec_time = (float)processes[i].burst_time;
        cfs_procs[i].remaining = (float)processes[i].burst_time;
        cfs_procs[i].weight = get_weight(cfs_procs[i].nice);
        cfs_procs[i].vruntime = 0;
        cfs_procs[i].sleeping = 0;
        cfs_procs[i].arrival_time = processes[i].arrival_time;
        cfs_procs[i].waiting_time = 0;
        cfs_procs[i].turnaround_time = 0;
        cfs_procs[i].completion_time = 0;
        total_weight += cfs_procs[i].weight;
    }

    printf("\n=== CFS Process Table ===\n");
    printf("PID\tNice\tType\tWeight\t\tCPU Share (%%)\n");
    for (int i = 0; i < n; i++) {
        float share = (cfs_procs[i].weight / total_weight) * 100.0;
        printf("%s\t%d\t%s\t%.2f\t\t%.2f%%\n", 
               cfs_procs[i].pid, cfs_procs[i].nice, cfs_procs[i].type, 
               cfs_procs[i].weight, share);
    }

    printf("\n=== CFS Scheduling Simulation ===\n");
    
    while (completed < n) {
        // Wake up sleeping I/O processes randomly
        for (int i = 0; i < n; i++) {
            if (cfs_procs[i].sleeping && rand() % 3 == 0) {
                cfs_procs[i].sleeping = 0; // wake up
            }
        }

        int idx = get_next_process(cfs_procs, n);
        if (idx == -1) { // No runnable process
            time += (int)TIME_SLICE;
            continue;
        }

        // Run the selected process
        printf("Time %3d-%3d ms -> Running %-2s (nice=%d, %s)\n",
               time, time + (int)TIME_SLICE, cfs_procs[idx].pid, 
               cfs_procs[idx].nice, cfs_procs[idx].type);
        
        float actual = (cfs_procs[idx].remaining < TIME_SLICE) ? 
                      cfs_procs[idx].remaining : TIME_SLICE;
        
        // Update waiting time for other processes
        for (int i = 0; i < n; i++) {
            if (i != idx && !cfs_procs[i].sleeping && cfs_procs[i].remaining > 0) {
                cfs_procs[i].waiting_time += (int)actual;
            }
        }
        
        cfs_procs[idx].remaining -= actual;
        cfs_procs[idx].vruntime += actual * (1024.0 / cfs_procs[idx].weight);
        time += (int)actual;

        // Simulate I/O process sleeping behavior
        if (strcmp(cfs_procs[idx].type, "IO") == 0 && rand() % 2 == 0) {
            cfs_procs[idx].sleeping = 1; // simulate I/O wait
        }

        // Mark completed
        if (cfs_procs[idx].remaining <= 0) {
            completed++;
            cfs_procs[idx].remaining = 0;
            cfs_procs[idx].completion_time = time;
            cfs_procs[idx].turnaround_time = cfs_procs[idx].completion_time - cfs_procs[idx].arrival_time;
        }
    }

    // Copy results back to original processes
    for (int i = 0; i < n; i++) {
        processes[i].waiting_time = cfs_procs[i].waiting_time;
        processes[i].turnaround_time = cfs_procs[i].turnaround_time;
        processes[i].completion_time = cfs_procs[i].completion_time;
    }

    // Display final stats
    printf("\n=== CFS Final Statistics ===\n");
    printf("PID\tNice\tType\tRuntime(ms)\tVRuntime\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%s\t%.2f\t\t%.2f\n", 
               cfs_procs[i].pid, cfs_procs[i].nice, cfs_procs[i].type, 
               cfs_procs[i].exec_time, cfs_procs[i].vruntime);
    }
}
