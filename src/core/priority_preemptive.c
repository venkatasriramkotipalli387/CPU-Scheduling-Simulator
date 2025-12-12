#include <stdio.h>
#include <stdbool.h>
#include "cpu_scheduler.h"

void priority_preemptive(Process p[], int n) {
    int remaining_bt[n];
    int completion_time[n];
    bool completed[n];
    int current_time = 0;
    int completed_count = 0;
    
    // Initialize arrays
    for (int i = 0; i < n; i++) {
        remaining_bt[i] = p[i].burst_time;
        completed[i] = false;
        completion_time[i] = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
    }
    
    while (completed_count < n) {
        int highest_priority = 9999; // Lower number = higher priority
        int selected_process = -1;
        
        // Find the process with highest priority that has arrived and not completed
        for (int i = 0; i < n; i++) {
            if (!completed[i] && p[i].arrival_time <= current_time) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    selected_process = i;
                }
                // If priority is same, select the one that arrived first
                else if (p[i].priority == highest_priority && selected_process != -1) {
                    if (p[i].arrival_time < p[selected_process].arrival_time) {
                        selected_process = i;
                    }
                }
            }
        }
        
        if (selected_process == -1) {
            // No process available, move to next time unit
            current_time++;
            continue;
        }
        
        // Execute the selected process for 1 time unit
        remaining_bt[selected_process]--;
        current_time++;
        
        // Check if process is completed
        if (remaining_bt[selected_process] == 0) {
            completed[selected_process] = true;
            completion_time[selected_process] = current_time;
            completed_count++;
            
            // Calculate turnaround time and waiting time
            p[selected_process].turnaround_time = completion_time[selected_process] - p[selected_process].arrival_time;
            p[selected_process].waiting_time = p[selected_process].turnaround_time - p[selected_process].burst_time;
        }
    }
    
    // Print results
    int total_wt = 0, total_tat = 0;
    printf("PID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               completion_time[i], p[i].turnaround_time, p[i].waiting_time);
    }
    
    printf("Average Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
}
