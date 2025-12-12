#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/cpu_scheduler.h"

// Helper function to add entry to Gantt chart
void add_gantt_entry(GanttChart *gantt, int pid, int start_time, int end_time) {
    if (gantt->count < MAX_GANTT_ENTRIES) {
        gantt->entries[gantt->count].pid = pid;
        gantt->entries[gantt->count].start_time = start_time;
        gantt->entries[gantt->count].end_time = end_time;
        gantt->count++;
    }
}

// First-Come First-Served Scheduling with Gantt chart
void fcfs(Process p[], int n, GanttChart *gantt) {
    // Initialize Gantt chart
    gantt->count = 0;
    
    // Sort processes by arrival time
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int total_wt = 0, total_tat = 0;
    int completion_time = 0;
    
    for (int i = 0; i < n; i++) {
        if (completion_time < p[i].arrival_time) {
            // Add idle time to Gantt chart
            add_gantt_entry(gantt, -1, completion_time, p[i].arrival_time);
            completion_time = p[i].arrival_time;
        }
        
        // Add to Gantt chart
        add_gantt_entry(gantt, p[i].pid, completion_time, completion_time + p[i].burst_time);
        
        p[i].waiting_time = completion_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        p[i].completion_time = completion_time + p[i].burst_time;
        completion_time = p[i].completion_time;
        
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
}

// Shortest Job First (Non-preemptive) with Gantt chart
void sjf(Process p[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // Sort processes by arrival time
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int current_time = 0;
    int completed = 0;
    bool is_completed[n];
    for (int i = 0; i < n; i++) {
        is_completed[i] = false;
    }

    while (completed != n) {
        int idx = -1;
        int min_bt = 9999;
        
        // Find process with shortest burst time that has arrived and not completed
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !is_completed[i]) {
                if (p[i].burst_time < min_bt) {
                    min_bt = p[i].burst_time;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Add to Gantt chart
            add_gantt_entry(gantt, p[idx].pid, current_time, current_time + p[idx].burst_time);
            
            // Process found
            p[idx].waiting_time = current_time - p[idx].arrival_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            p[idx].completion_time = current_time + p[idx].burst_time;
            current_time = p[idx].completion_time;
            is_completed[idx] = true;
            completed++;
        } else {
            // No process available, advance time
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
        }
    }
}

// Shortest Remaining Time First (Preemptive SJF) with Gantt chart
void srtf(Process p[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    int remaining_bt[n];
    bool completed[n];
    int current_time = 0;
    int completed_count = 0;
    int last_pid = -1;
    int last_start_time = 0;
    
    // Initialize arrays
    for (int i = 0; i < n; i++) {
        remaining_bt[i] = p[i].burst_time;
        completed[i] = false;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
    }
    
    while (completed_count < n) {
        int shortest_job = -1;
        int min_remaining = 9999;
        
        // Find process with shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (!completed[i] && p[i].arrival_time <= current_time) {
                if (remaining_bt[i] < min_remaining) {
                    min_remaining = remaining_bt[i];
                    shortest_job = i;
                }
                // If same remaining time, choose the one that arrived first
                else if (remaining_bt[i] == min_remaining && shortest_job != -1) {
                    if (p[i].arrival_time < p[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }
        
        if (shortest_job == -1) {
            // No process available - add idle time to Gantt chart
            if (last_pid != -1) {
                // Close the previous process entry
                add_gantt_entry(gantt, last_pid, last_start_time, current_time);
                last_pid = -1;
            }
            // Add idle entry
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
            continue;
        }
        
        // Check if process changed
        if (last_pid != p[shortest_job].pid) {
            if (last_pid != -1) {
                // Close the previous process entry
                add_gantt_entry(gantt, last_pid, last_start_time, current_time);
            }
            // Start new entry for current process
            last_pid = p[shortest_job].pid;
            last_start_time = current_time;
        }
        
        // Execute for 1 time unit
        remaining_bt[shortest_job]--;
        current_time++;
        
        // Update waiting time for other ready processes
        for (int i = 0; i < n; i++) {
            if (i != shortest_job && !completed[i] && p[i].arrival_time < current_time && remaining_bt[i] > 0) {
                p[i].waiting_time++;
            }
        }
        
        // Check if process completed
        if (remaining_bt[shortest_job] == 0) {
            // Close the current process entry
            add_gantt_entry(gantt, last_pid, last_start_time, current_time);
            
            completed[shortest_job] = true;
            completed_count++;
            p[shortest_job].completion_time = current_time;
            p[shortest_job].turnaround_time = p[shortest_job].completion_time - p[shortest_job].arrival_time;
            
            last_pid = -1;
        }
    }
    
    // Handle case where last process might still be running
    if (last_pid != -1) {
        add_gantt_entry(gantt, last_pid, last_start_time, current_time);
    }
}

// Round Robin Scheduling with Gantt chart
void round_robin(Process p[], int n, int quantum, GanttChart *gantt) {
    gantt->count = 0;
    
    int remaining_bt[n];
    int waiting_time[n];
    int current_time = 0;
    int completed = 0;
    
    // Initialize arrays
    for (int i = 0; i < n; i++) {
        remaining_bt[i] = p[i].burst_time;
        waiting_time[i] = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
    }
    
    while (completed < n) {
        int executed = 0;
        
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && remaining_bt[i] > 0) {
                executed = 1;
                int exec_time = (remaining_bt[i] > quantum) ? quantum : remaining_bt[i];
                int start_time = current_time;
                
                // Add to Gantt chart
                add_gantt_entry(gantt, p[i].pid, start_time, start_time + exec_time);
                
                // Update waiting time for other ready processes
                for (int j = 0; j < n; j++) {
                    if (j != i && p[j].arrival_time <= current_time && remaining_bt[j] > 0) {
                        waiting_time[j] += exec_time;
                    }
                }
                
                current_time += exec_time;
                remaining_bt[i] -= exec_time;
                
                if (remaining_bt[i] == 0) {
                    p[i].completion_time = current_time;
                    p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time = waiting_time[i];
                    completed++;
                }
            }
        }
        
        // If no process was executed, advance time
        if (!executed) {
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
        }
    }
}

// Priority Scheduling (Non-preemptive) with Gantt chart
void priority_scheduling(Process p[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // Sort by arrival time first
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int current_time = 0;
    int completed = 0;
    bool is_completed[n];
    for (int i = 0; i < n; i++) {
        is_completed[i] = false;
    }

    while (completed != n) {
        int idx = -1;
        int highest_priority = 9999; // Lower number = higher priority
        
        // Find process with highest priority that has arrived and not completed
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !is_completed[i]) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Add to Gantt chart
            add_gantt_entry(gantt, p[idx].pid, current_time, current_time + p[idx].burst_time);
            
            // Process found
            p[idx].waiting_time = current_time - p[idx].arrival_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            p[idx].completion_time = current_time + p[idx].burst_time;
            current_time = p[idx].completion_time;
            is_completed[idx] = true;
            completed++;
        } else {
            // No process available, advance time
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
        }
    }
}

// Priority Scheduling (Preemptive) with Gantt chart
void priority_preemptive(Process p[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    int remaining_bt[n];
    bool completed[n];
    int current_time = 0;
    int completed_count = 0;
    int last_pid = -1;
    int last_start_time = 0;
    
    // Initialize arrays
    for (int i = 0; i < n; i++) {
        remaining_bt[i] = p[i].burst_time;
        completed[i] = false;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
    }
    
    while (completed_count < n) {
        int highest_priority = 9999; // Lower number = higher priority
        int selected_process = -1;
        
        // Find highest priority process that has arrived and not completed
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
            // No process available - add idle time to Gantt chart
            if (last_pid != -1) {
                // Close the previous process entry
                add_gantt_entry(gantt, last_pid, last_start_time, current_time);
                last_pid = -1;
            }
            // Add idle entry
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
            continue;
        }
        
        // Check if process changed
        if (last_pid != p[selected_process].pid) {
            if (last_pid != -1) {
                // Close the previous process entry
                add_gantt_entry(gantt, last_pid, last_start_time, current_time);
            }
            // Start new entry for current process
            last_pid = p[selected_process].pid;
            last_start_time = current_time;
        }
        
        // Execute the process for 1 time unit
        remaining_bt[selected_process]--;
        current_time++;
        
        // Update waiting time for all other ready processes
        for (int i = 0; i < n; i++) {
            if (i != selected_process && !completed[i] && p[i].arrival_time < current_time && remaining_bt[i] > 0) {
                p[i].waiting_time++;
            }
        }
        
        // Check if process completed
        if (remaining_bt[selected_process] == 0) {
            // Close the current process entry
            add_gantt_entry(gantt, last_pid, last_start_time, current_time);
            
            completed[selected_process] = true;
            completed_count++;
            p[selected_process].completion_time = current_time;
            p[selected_process].turnaround_time = p[selected_process].completion_time - p[selected_process].arrival_time;
            
            last_pid = -1;
        }
    }
    
    // Handle case where last process might still be running
    if (last_pid != -1) {
        add_gantt_entry(gantt, last_pid, last_start_time, current_time);
    }
}

// Highest Response Ratio Next with Gantt chart
void hrrn(Process p[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // Sort by arrival time
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }

    int current_time = 0;
    int completed = 0;
    bool is_completed[n];
    for (int i = 0; i < n; i++) {
        is_completed[i] = false;
    }

    while (completed != n) {
        int idx = -1;
        float highest_response_ratio = -1;
        
        // Calculate response ratio for all available processes
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !is_completed[i]) {
                float waiting_time = current_time - p[i].arrival_time;
                float response_ratio = (waiting_time + p[i].burst_time) / p[i].burst_time;
                
                if (response_ratio > highest_response_ratio) {
                    highest_response_ratio = response_ratio;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            // Add to Gantt chart
            add_gantt_entry(gantt, p[idx].pid, current_time, current_time + p[idx].burst_time);
            
            // Process found
            p[idx].waiting_time = current_time - p[idx].arrival_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            p[idx].completion_time = current_time + p[idx].burst_time;
            current_time = p[idx].completion_time;
            is_completed[idx] = true;
            completed++;
        } else {
            // No process available, advance time
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
        }
    }
}

// Multi-level Queue Scheduling with Gantt chart
void multi_level_queue(Process processes[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // Separate processes into different queues based on queue_level
    Process queue1[n], queue2[n];
    int q1_count = 0, q2_count = 0;
    
    for (int i = 0; i < n; i++) {
        if (processes[i].queue_level == 0) {
            queue1[q1_count++] = processes[i];
        } else {
            queue2[q2_count++] = processes[i];
        }
    }
    
    // Apply Round Robin to queue1 (system processes)
    if (q1_count > 0) {
        GanttChart gantt1;
        gantt1.count = 0;
        round_robin(queue1, q1_count, 4, &gantt1); // Quantum of 4 for system processes
        // Copy Gantt chart entries
        for (int i = 0; i < gantt1.count; i++) {
            add_gantt_entry(gantt, gantt1.entries[i].pid, gantt1.entries[i].start_time, gantt1.entries[i].end_time);
        }
    }
    
    // Apply FCFS to queue2 (user processes)
    if (q2_count > 0) {
        GanttChart gantt2;
        gantt2.count = 0;
        fcfs(queue2, q2_count, &gantt2);
        // Copy Gantt chart entries with offset time
        int time_offset = 0;
        if (gantt->count > 0) {
            time_offset = gantt->entries[gantt->count - 1].end_time;
        }
        for (int i = 0; i < gantt2.count; i++) {
            add_gantt_entry(gantt, gantt2.entries[i].pid, 
                           gantt2.entries[i].start_time + time_offset, 
                           gantt2.entries[i].end_time + time_offset);
        }
    }
    
    // Merge results back into original array
    int idx = 0;
    for (int i = 0; i < q1_count; i++) {
        processes[idx++] = queue1[i];
    }
    for (int i = 0; i < q2_count; i++) {
        processes[idx++] = queue2[i];
    }
}

// Completely Fair Scheduler (CFS) with Gantt chart
void schedule_cfs(Process processes[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // CFS-specific process structure
    typedef struct {
        int pid;
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

    #define TIME_SLICE 10.0
    CFSProcess cfs_procs[20]; // MAX_PROCESSES
    float total_weight = 0;
    int completed = 0;
    int time = 0;
    
    // Initialize CFS processes
    for (int i = 0; i < n; i++) {
        cfs_procs[i].pid = processes[i].pid;
        cfs_procs[i].nice = processes[i].nice; // Use nice value from Process struct
        strcpy(cfs_procs[i].type, "CPU"); // Default to CPU type
        cfs_procs[i].exec_time = (float)processes[i].burst_time;
        cfs_procs[i].remaining = (float)processes[i].burst_time;
        
        // Linux-like weight calculation
        cfs_procs[i].weight = 1024.0 * pow(1.25, -cfs_procs[i].nice);
        
        cfs_procs[i].vruntime = 0;
        cfs_procs[i].sleeping = 0;
        cfs_procs[i].arrival_time = processes[i].arrival_time;
        cfs_procs[i].waiting_time = 0;
        cfs_procs[i].turnaround_time = 0;
        cfs_procs[i].completion_time = 0;
        total_weight += cfs_procs[i].weight;
    }

    // Find process with minimum vruntime that's not finished or sleeping
    int get_next_process() {
        int idx = -1;
        float min_vr = 999999.0;
        for (int i = 0; i < n; i++) {
            if (cfs_procs[i].remaining > 0 && !cfs_procs[i].sleeping && cfs_procs[i].vruntime < min_vr) {
                min_vr = cfs_procs[i].vruntime;
                idx = i;
            }
        }
        return idx;
    }

    printf("\n=== CFS Scheduling Simulation ===\n");
    
    while (completed < n) {
        // Wake up sleeping I/O processes randomly
        for (int i = 0; i < n; i++) {
            if (cfs_procs[i].sleeping && rand() % 3 == 0) {
                cfs_procs[i].sleeping = 0; // wake up
            }
        }

        int idx = get_next_process();
        if (idx == -1) { // No runnable process
            add_gantt_entry(gantt, -1, time, time + (int)TIME_SLICE);
            time += (int)TIME_SLICE;
            continue;
        }

        // Add to Gantt chart
        int start_time = time;
        float actual = (cfs_procs[idx].remaining < TIME_SLICE) ? 
                      cfs_procs[idx].remaining : TIME_SLICE;
        add_gantt_entry(gantt, cfs_procs[idx].pid, start_time, start_time + (int)actual);

        // Run the selected process
        printf("Time %3d-%3d ms -> Running P%d (nice=%d, weight=%.2f)\n",
               time, time + (int)actual, cfs_procs[idx].pid, 
               cfs_procs[idx].nice, cfs_procs[idx].weight);
        
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
    printf("PID\tNice\tWeight\tVRuntime\tWT\tTAT\tCT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%.2f\t%.2f\t\t%d\t%d\t%d\n", 
               cfs_procs[i].pid, cfs_procs[i].nice, cfs_procs[i].weight, 
               cfs_procs[i].vruntime, cfs_procs[i].waiting_time, 
               cfs_procs[i].turnaround_time, cfs_procs[i].completion_time);
    }
}

// Multilevel Feedback Queue Scheduling with Gantt chart
void mlfq(Process processes[], int n, GanttChart *gantt) {
    gantt->count = 0;
    
    // Define queue structure
    typedef struct {
        Process *processes;
        int count;
        int quantum;
    } Queue;
    
    // Initialize three queues
    Queue queues[3];
    for (int i = 0; i < 3; i++) {
        queues[i].processes = malloc(n * sizeof(Process));
        queues[i].count = 0;
    }
    
    // Set time quantums for each queue
    queues[0].quantum = 2;  // Q0: RR with quantum 2
    queues[1].quantum = 5;  // Q1: RR with quantum 5
    queues[2].quantum = -1; // Q2: FCFS (no quantum, run to completion)
    
    // Initialize process data
    Process *process_copy = malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        process_copy[i] = processes[i];
        process_copy[i].remaining_time = process_copy[i].burst_time;
        process_copy[i].waiting_time = 0;
        process_copy[i].turnaround_time = 0;
        process_copy[i].completion_time = 0;
        process_copy[i].queue_level = 0; // All processes start in Q0
    }
    
    int current_time = 0;
    int completed = 0;
    
    printf("\n=== Multilevel Feedback Queue (MLFQ) Scheduling ===\n");
    printf("Q0: Round Robin (Quantum=2)\n");
    printf("Q1: Round Robin (Quantum=5)\n");
    printf("Q2: First-Come First-Served\n\n");
    
    while (completed < n) {
        // Add arrived processes to Q0
        for (int i = 0; i < n; i++) {
            if (process_copy[i].arrival_time <= current_time && 
                process_copy[i].remaining_time > 0 && 
                process_copy[i].queue_level == -1) {
                process_copy[i].queue_level = 0;
                queues[0].processes[queues[0].count++] = process_copy[i];
            }
        }
        
        int process_executed = 0;
        
        // Check queues in priority order (Q0 -> Q1 -> Q2)
        for (int q = 0; q < 3; q++) {
            if (queues[q].count > 0) {
                // Get the first process in the current queue
                Process *current_process = &queues[q].processes[0];
                int process_index = -1;
                
                // Find the original index of the current process
                for (int i = 0; i < n; i++) {
                    if (process_copy[i].pid == current_process->pid) {
                        process_index = i;
                        break;
                    }
                }
                
                if (process_index == -1 || process_copy[process_index].remaining_time <= 0) {
                    // Remove from queue if not found or completed
                    for (int j = 0; j < queues[q].count - 1; j++) {
                        queues[q].processes[j] = queues[q].processes[j + 1];
                    }
                    queues[q].count--;
                    continue;
                }
                
                Process *proc = &process_copy[process_index];
                process_executed = 1;
                
                int exec_time;
                if (q == 2) {
                    // Q2: FCFS - run to completion
                    exec_time = proc->remaining_time;
                } else {
                    // Q0 and Q1: RR with respective quantums
                    exec_time = (proc->remaining_time < queues[q].quantum) ? 
                               proc->remaining_time : queues[q].quantum;
                }
                
                // Add to Gantt chart
                add_gantt_entry(gantt, proc->pid, current_time, current_time + exec_time);
                
                printf("Time %3d-%3d: Running P%d from Q%d (Remaining: %d)\n",
                       current_time, current_time + exec_time, 
                       proc->pid, q, proc->remaining_time - exec_time);
                
                // Update waiting time for other processes
                for (int i = 0; i < n; i++) {
                    if (i != process_index && 
                        process_copy[i].arrival_time <= current_time && 
                        process_copy[i].remaining_time > 0) {
                        process_copy[i].waiting_time += exec_time;
                    }
                }
                
                current_time += exec_time;
                proc->remaining_time -= exec_time;
                
                // Remove from current queue
                for (int j = 0; j < queues[q].count - 1; j++) {
                    queues[q].processes[j] = queues[q].processes[j + 1];
                }
                queues[q].count--;
                
                if (proc->remaining_time > 0) {
                    // Process not completed
                    if (q < 2) {
                        // Demote to next lower queue
                        proc->queue_level = q + 1;
                        queues[q + 1].processes[queues[q + 1].count++] = *proc;
                        printf("  -> P%d demoted to Q%d\n", proc->pid, q + 1);
                    } else {
                        // In Q2, keep in same queue if not completed
                        queues[q].processes[queues[q].count++] = *proc;
                    }
                } else {
                    // Process completed
                    proc->completion_time = current_time;
                    proc->turnaround_time = proc->completion_time - proc->arrival_time;
                    completed++;
                    printf("  -> P%d completed at time %d\n", proc->pid, current_time);
                }
                
                break; // Break after executing one process
            }
        }
        
        if (!process_executed) {
            // No process ready, advance time
            add_gantt_entry(gantt, -1, current_time, current_time + 1);
            current_time++;
        }
        
        // Re-add processes to appropriate queues (handles queue management)
        for (int i = 0; i < n; i++) {
            if (process_copy[i].arrival_time <= current_time && 
                process_copy[i].remaining_time > 0 && 
                process_copy[i].queue_level != -1) {
                
                int in_queue = 0;
                for (int q = 0; q < 3; q++) {
                    for (int j = 0; j < queues[q].count; j++) {
                        if (queues[q].processes[j].pid == process_copy[i].pid) {
                            in_queue = 1;
                            break;
                        }
                    }
                    if (in_queue) break;
                }
                
                if (!in_queue) {
                    queues[process_copy[i].queue_level].processes[
                        queues[process_copy[i].queue_level].count++] = process_copy[i];
                }
            }
        }
    }
    
    // Copy results back to original array
    for (int i = 0; i < n; i++) {
        processes[i] = process_copy[i];
    }
    
    // Free allocated memory
    free(process_copy);
    for (int i = 0; i < 3; i++) {
        free(queues[i].processes);
    }
    
    printf("\n=== MLFQ Scheduling Completed ===\n");
}

// Gantt chart to string function
char* gantt_chart_to_string(GanttChart *gantt) {
    if (gantt->count == 0) {
        char *result = malloc(100);
        strcpy(result, "Gantt Chart: No data available\n");
        return result;
    }
    
    char *result = malloc(2048);
    char buffer[256];
    
    strcpy(result, "Gantt Chart:\n");
    strcat(result, "Time: ");
    
    // Create timeline
    for (int i = 0; i < gantt->count; i++) {
        sprintf(buffer, "%2d---%2d   ", gantt->entries[i].start_time, gantt->entries[i].end_time);
        strcat(result, buffer);
    }
    strcat(result, "\n");
    
    // Create process bars
    strcat(result, "Proc: ");
    for (int i = 0; i < gantt->count; i++) {
        if (gantt->entries[i].pid == -1) {
            sprintf(buffer, "  IDLE    ");
        } else {
            sprintf(buffer, "   P%d    ", gantt->entries[i].pid);
        }
        strcat(result, buffer);
    }
    strcat(result, "\n");
    
    return result;
}

// Helper function to get results as string for GUI
char* get_scheduling_result_string(Process p[], int n, int algorithm, int quantum, GanttChart *gantt) {
    // Calculate totals
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
    
    // Create result string
    char *result = malloc(8192);
    char buffer[1024];
    
    strcpy(result, "CPU Scheduling Results\n");
    strcat(result, "=====================\n\n");
    
    // Algorithm name
    const char *algo_names[] = {
        "First-Come First-Served (FCFS)",
        "Shortest Job First (Non-preemptive)",
        "Shortest Remaining Time First (Preemptive SJF)",
        "Round Robin",
        "Priority Scheduling (Non-preemptive)",
        "Priority Scheduling (Preemptive)",
        "Highest Response Ratio Next (HRRN)",
        "Multi-level Queue",
        "Completely Fair Scheduler (CFS)",
        "Multilevel Feedback Queue (MLFQ)"
    };
    
    sprintf(buffer, "Algorithm: %s\n", algo_names[algorithm]);
    strcat(result, buffer);
    
    if (algorithm == 3) { // Round Robin
        sprintf(buffer, "Time Quantum: %d\n", quantum);
        strcat(result, buffer);
    }
    
    if (algorithm == 9) { // MLFQ
        strcat(result, "Queue Structure:\n");
        strcat(result, "- Q0: Round Robin (Quantum=2)\n");
        strcat(result, "- Q1: Round Robin (Quantum=5)\n");
        strcat(result, "- Q2: First-Come First-Served\n");
        strcat(result, "Note: Processes that use full quantum in Q0/Q1 are demoted to next queue.\n");
    }
    
    strcat(result, "\n");
    
    // Add Gantt chart
    char *gantt_str = gantt_chart_to_string(gantt);
    strcat(result, gantt_str);
    free(gantt_str);
    
    strcat(result, "\nProcess Details:\n");
    
    // Table header
    if (algorithm == 8) { // CFS
        strcat(result, "PID\tAT\tBT\tNice\tWT\tTAT\tCT\n");
        strcat(result, "---\t--\t--\t----\t--\t---\t--\n");
        
        for (int i = 0; i < n; i++) {
            sprintf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
                    p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].nice,
                    p[i].waiting_time, p[i].turnaround_time, p[i].completion_time);
            strcat(result, buffer);
        }
    } else if (algorithm == 9) { // MLFQ - show final queue level
        strcat(result, "PID\tAT\tBT\tFinalQ\tWT\tTAT\tCT\n");
        strcat(result, "---\t--\t--\t------\t--\t---\t--\n");
        
        for (int i = 0; i < n; i++) {
            sprintf(buffer, "%d\t%d\t%d\tQ%d\t%d\t%d\t%d\n", 
                    p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].queue_level,
                    p[i].waiting_time, p[i].turnaround_time, p[i].completion_time);
            strcat(result, buffer);
        }
    } else {
        // Standard table for other algorithms
        strcat(result, "PID\tAT\tBT\tPri\tNice\tQL\tWT\tTAT\tCT\n");
        strcat(result, "---\t--\t--\t---\t----\t--\t--\t---\t--\n");
        
        for (int i = 0; i < n; i++) {
            sprintf(buffer, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
                    p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
                    p[i].nice, p[i].queue_level,
                    p[i].waiting_time, p[i].turnaround_time, p[i].completion_time);
            strcat(result, buffer);
        }
    }
    
    strcat(result, "\n");
    sprintf(buffer, "Average Waiting Time: %.2f\n", total_wt / n);
    strcat(result, buffer);
    sprintf(buffer, "Average Turnaround Time: %.2f\n", total_tat / n);
    strcat(result, buffer);
    
    return result;
}
