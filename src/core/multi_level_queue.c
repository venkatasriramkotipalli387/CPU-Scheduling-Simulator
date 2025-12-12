#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/cpu_scheduler.h"

void multi_level_queue(Process p[], int n) {
    // Define queues: 
    // Queue 0: System processes (RR with quantum=4)
    // Queue 1: Interactive processes (RR with quantum=8)  
    // Queue 2: Batch processes (FCFS)
    
    Process system_queue[n];
    Process interactive_queue[n];
    Process batch_queue[n];
    int sys_count = 0, int_count = 0, batch_count = 0;
    
    // Separate processes into queues based on queue_level
    for (int i = 0; i < n; i++) {
        switch(p[i].queue_level) {
            case 0: system_queue[sys_count++] = p[i]; break;
            case 1: interactive_queue[int_count++] = p[i]; break;
            case 2: batch_queue[batch_count++] = p[i]; break;
        }
    }
    
    int current_time = 0;
    
    // Execute System Queue (Round Robin with quantum=4)
    if (sys_count > 0) {
        int remaining_sys[sys_count];
        for (int i = 0; i < sys_count; i++) {
            remaining_sys[i] = system_queue[i].burst_time;
            system_queue[i].waiting_time = 0;
        }
        
        int completed = 0;
        while (completed < sys_count) {
            int executed = 0;
            for (int i = 0; i < sys_count; i++) {
                if (remaining_sys[i] > 0 && system_queue[i].arrival_time <= current_time) {
                    executed = 1;
                    int exec_time = (remaining_sys[i] > 4) ? 4 : remaining_sys[i];
                    
                    // Update waiting time for other system processes
                    for (int j = 0; j < sys_count; j++) {
                        if (j != i && remaining_sys[j] > 0 && system_queue[j].arrival_time <= current_time) {
                            system_queue[j].waiting_time += exec_time;
                        }
                    }
                    
                    current_time += exec_time;
                    remaining_sys[i] -= exec_time;
                    
                    if (remaining_sys[i] == 0) {
                        system_queue[i].completion_time = current_time;
                        system_queue[i].turnaround_time = system_queue[i].completion_time - system_queue[i].arrival_time;
                        completed++;
                    }
                }
            }
            if (!executed) current_time++;
        }
    }
    
    // Execute Interactive Queue (Round Robin with quantum=8)
    if (int_count > 0) {
        int remaining_int[int_count];
        for (int i = 0; i < int_count; i++) {
            remaining_int[i] = interactive_queue[i].burst_time;
            interactive_queue[i].waiting_time = 0;
        }
        
        int completed = 0;
        while (completed < int_count) {
            int executed = 0;
            for (int i = 0; i < int_count; i++) {
                if (remaining_int[i] > 0 && interactive_queue[i].arrival_time <= current_time) {
                    executed = 1;
                    int exec_time = (remaining_int[i] > 8) ? 8 : remaining_int[i];
                    
                    // Update waiting time for other interactive processes
                    for (int j = 0; j < int_count; j++) {
                        if (j != i && remaining_int[j] > 0 && interactive_queue[j].arrival_time <= current_time) {
                            interactive_queue[j].waiting_time += exec_time;
                        }
                    }
                    
                    current_time += exec_time;
                    remaining_int[i] -= exec_time;
                    
                    if (remaining_int[i] == 0) {
                        interactive_queue[i].completion_time = current_time;
                        interactive_queue[i].turnaround_time = interactive_queue[i].completion_time - interactive_queue[i].arrival_time;
                        completed++;
                    }
                }
            }
            if (!executed) current_time++;
        }
    }
    
    // Execute Batch Queue (FCFS)
    if (batch_count > 0) {
        // Sort batch processes by arrival time
        for (int i = 0; i < batch_count-1; i++) {
            for (int j = 0; j < batch_count-i-1; j++) {
                if (batch_queue[j].arrival_time > batch_queue[j+1].arrival_time) {
                    Process temp = batch_queue[j];
                    batch_queue[j] = batch_queue[j+1];
                    batch_queue[j+1] = temp;
                }
            }
        }
        
        for (int i = 0; i < batch_count; i++) {
            if (current_time < batch_queue[i].arrival_time) {
                current_time = batch_queue[i].arrival_time;
            }
            batch_queue[i].waiting_time = current_time - batch_queue[i].arrival_time;
            batch_queue[i].completion_time = current_time + batch_queue[i].burst_time;
            batch_queue[i].turnaround_time = batch_queue[i].waiting_time + batch_queue[i].burst_time;
            current_time = batch_queue[i].completion_time;
        }
    }
    
    // Merge results back to original array
    int idx = 0;
    for (int i = 0; i < sys_count; i++) p[idx++] = system_queue[i];
    for (int i = 0; i < int_count; i++) p[idx++] = interactive_queue[i];
    for (int i = 0; i < batch_count; i++) p[idx++] = batch_queue[i];
}
