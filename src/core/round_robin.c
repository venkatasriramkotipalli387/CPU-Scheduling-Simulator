#include <stdio.h>
#include "cpu_scheduler.h"

void round_robin(Process p[], int n, int quantum) {
    int t = 0, completed = 0;
    int remaining[n];
    int waiting_time[n];
    
    // Initialize arrays
    for (int i = 0; i < n; i++) {
        remaining[i] = p[i].burst_time;
        waiting_time[i] = 0;
    }

    while (completed != n) {
        int executed = 0; // Track if any process was executed in this cycle

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= t && remaining[i] > 0) {
                executed = 1;
                
                if (remaining[i] > quantum) {
                    // Add waiting time for other processes
                    for (int j = 0; j < n; j++) {
                        if (j != i && p[j].arrival_time <= t && remaining[j] > 0) {
                            waiting_time[j] += quantum;
                        }
                    }
                    t += quantum;
                    remaining[i] -= quantum;
                } else {
                    // Add waiting time for other processes
                    for (int j = 0; j < n; j++) {
                        if (j != i && p[j].arrival_time <= t && remaining[j] > 0) {
                            waiting_time[j] += remaining[i];
                        }
                    }
                    t += remaining[i];
                    p[i].turnaround_time = t - p[i].arrival_time;
                    p[i].waiting_time = waiting_time[i];
                    remaining[i] = 0;
                    completed++;
                }
            }
        }

        // If no process was executed but not all are completed
        if (!executed && completed < n) {
            t++; // Find next arriving process
        }
    }

    // Print results
    int total_wt = 0, total_tat = 0;
    printf("PID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
    }

    printf("Average Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
}
