#include <stdio.h>
#include "../include/cpu_scheduler.h"

int main() {
    int n, choice, quantum;

    // Get number of processes from the user
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Declare process arrays
    Process p[n], original[n];

    // User input for each process
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;

        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &p[i].arrival_time);

        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &p[i].burst_time);

        printf("Enter priority for P%d (lower number = higher priority): ", i + 1);
        scanf("%d", &p[i].priority);

        p[i].remaining_time = p[i].burst_time;

        // Keep a copy for resetting before each algorithm
        original[i] = p[i];
    }

    // Menu-driven loop
    do {
        printf("\nSelect Scheduling Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. SJF (Non-preemptive)\n");
        printf("3. SRTF (Preemptive SJF)\n");
        printf("4. Round Robin\n");
        printf("5. Priority Scheduling (non-preemptive)\n");
        printf("6. Priority Scheduling (preemptive)\n");
        printf("7. HRRN\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Reset processes to original state for each run
        for (int i = 0; i < n; i++)
            p[i] = original[i];

        switch(choice) {
            case 1:
                fcfs(p, n);
                break;
            case 2:
                sjf(p, n);
                break;
            case 3:
                srtf(p, n);
                break;
            case 4:
                printf("Enter time quantum: ");
                scanf("%d", &quantum);
                round_robin(p, n, quantum);
                break;
            case 5:
                priority_scheduling(p, n);
                break;
            case 6:
                priority_preemptive(p, n);
                break;
            case 7:
                hrrn(p, n);
                break;
            case 8:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice! Please select 1-8.\n");
        }

    } while (choice != 8);

    return 0;
}
