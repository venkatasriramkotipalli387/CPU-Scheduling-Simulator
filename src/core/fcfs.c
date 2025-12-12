#include <stdio.h>
#include "cpu_scheduler.h"

void fcfs(Process p[], int n) {
    // Sort processes by arrival time (FCFS should consider arrival order)
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                // Swap processes
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int total_wt = 0, total_tat = 0;
    int completion_time = 0;
    
    for (int i = 0; i < n; i++) {
        // If process arrives after previous completion, start at arrival time
        if (completion_time < p[i].arrival_time) {
            completion_time = p[i].arrival_time;
        }
        
        p[i].waiting_time = completion_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        completion_time += p[i].burst_time;
        
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    printf("PID\tBT\tAT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].burst_time, p[i].arrival_time, p[i].waiting_time, p[i].turnaround_time);
    }

    printf("Average Waiting Time = %.2f\n", (float)total_wt/n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat/n);
}
