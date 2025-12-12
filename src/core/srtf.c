#include <stdio.h>
#include "cpu_scheduler.h"

void srtf(Process p[], int n) {
    int completed = 0, t = 0, min_index;
    int remaining[n];
    int is_completed[n];

    for (int i = 0; i < n; i++) {
        remaining[i] = p[i].burst_time;
        is_completed[i] = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
    }

    while (completed != n) {
        min_index = -1;
        int min_remaining = 1e9;

        // Find process with shortest remaining time among arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= t && !is_completed[i] && remaining[i] < min_remaining) {
                min_remaining = remaining[i];
                min_index = i;
            }
        }

        // If no process has arrived yet, increment time
        if (min_index == -1) {
            t++;
            continue;
        }

        // Execute process for 1 unit of time
        remaining[min_index]--;
        t++;

        // If process finished execution
        if (remaining[min_index] == 0) {
            p[min_index].completion_time = t;
            p[min_index].turnaround_time = p[min_index].completion_time - p[min_index].arrival_time;
            p[min_index].waiting_time = p[min_index].turnaround_time - p[min_index].burst_time;
            is_completed[min_index] = 1;
            completed++;
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
