#include <stdio.h>
#include "cpu_scheduler.h"

void hrrn(Process p[], int n) {
    int completed = 0, t = 0;
    int is_completed[n];

    for (int i = 0; i < n; i++)
        is_completed[i] = 0;

    while (completed != n) {
        float hrr = -1;
        int index = -1;

        // Find process with highest response ratio among arrived processes
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= t && !is_completed[i]) {
                float response_ratio = (float)(t - p[i].arrival_time + p[i].burst_time) / p[i].burst_time;
                if (response_ratio > hrr) {
                    hrr = response_ratio;
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

    // Calculate total WT and TAT
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
