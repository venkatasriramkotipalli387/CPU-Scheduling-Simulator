#include <stdio.h>
#include "cpu_scheduler.h"

void sjf(Process p[], int n) {
    int completed = 0, t = 0;
    int is_completed[n];

    for (int i = 0; i < n; i++)
        is_completed[i] = 0;

    while (completed != n) {
        int index = -1;
        int min_burst = 1e9; // initialize to a large value

        // Find process with minimum burst time among arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= t && !is_completed[i]) {
                if (p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    index = i;
                } else if (p[i].burst_time == min_burst) {
                    // Tie-breaker: choose process with earlier arrival time
                    if (p[i].arrival_time < p[index].arrival_time)
                        index = i;
                }
            }
        }

        // If no process has arrived yet, increment time
        if (index == -1) {
            t++;
            continue;
        }

        // Schedule the selected process
        p[index].waiting_time = t - p[index].arrival_time;
        t += p[index].burst_time;
        p[index].turnaround_time = p[index].waiting_time + p[index].burst_time;
        is_completed[index] = 1;
        completed++;
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
