#include <gtk/gtk.h>
#include "../../include/gui.h"
#include "../../include/cpu_scheduler.h"

const char* get_algorithm_name(SchedulingAlgorithm algorithm) {
    switch (algorithm) {
        case ALGORITHM_FCFS: return "FCFS";
        case ALGORITHM_SJF: return "SJF";
        case ALGORITHM_SRTF: return "SRTF";
        case ALGORITHM_ROUND_ROBIN: return "ROUND_ROBIN";
        case ALGORITHM_PRIORITY_NON_PREEMPTIVE: return "PRIORITY_NP";
        case ALGORITHM_PRIORITY_PREEMPTIVE: return "PRIORITY_P";
        case ALGORITHM_HRRN: return "HRRN";
        case ALGORITHM_MLQ: return "MLQ";
        case ALGORITHM_CFS: return "CFS";
        case ALGORITHM_MLFQ: return "MLFQ";
        default: return "FCFS";
    }
}

void on_execute_clicked(GtkWidget *widget, gpointer data) {
    (void)widget; // Mark as unused to suppress warning
    
    AppData *app_data = (AppData *)data;
    
    if (app_data->process_count == 0) {
        return;
    }
    
    // Get selected algorithm
    int algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(app_data->algorithm_combo));
    
    // Get quantum if Round Robin
    int quantum = 2; // default
    if (algorithm == 3) { // Round Robin
        quantum = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->quantum_spin));
    }
    
    // Create a copy of processes for calculation
    Process *process_copy = g_malloc0_n(app_data->process_count, sizeof(Process));
    for (int i = 0; i < app_data->process_count; i++) {
        process_copy[i] = app_data->processes[i];
        process_copy[i].remaining_time = process_copy[i].burst_time;
        process_copy[i].queue_level = -1; // Initialize for MLFQ
    }
    
    // Create Gantt chart
    GanttChart gantt;
    gantt.count = 0;
    
    // Execute the selected algorithm
    switch (algorithm) {
        case 0: // FCFS
            fcfs(process_copy, app_data->process_count, &gantt);
            break;
        case 1: // SJF
            sjf(process_copy, app_data->process_count, &gantt);
            break;
        case 2: // SRTF
            srtf(process_copy, app_data->process_count, &gantt);
            break;
        case 3: // Round Robin
            round_robin(process_copy, app_data->process_count, quantum, &gantt);
            break;
        case 4: // Priority Non-preemptive
            priority_scheduling(process_copy, app_data->process_count, &gantt);
            break;
        case 5: // Priority Preemptive
            priority_preemptive(process_copy, app_data->process_count, &gantt);
            break;
        case 6: // HRRN
            hrrn(process_copy, app_data->process_count, &gantt);
            break;
        case 7: // Multi-level Queue
            multi_level_queue(process_copy, app_data->process_count, &gantt);
            break;
        case 8: // CFS
            schedule_cfs(process_copy, app_data->process_count, &gantt);
            break;
        case 9: // MLFQ
            mlfq(process_copy, app_data->process_count, &gantt);
            break;
    }
    
    // Get results as string (includes Gantt chart)
    char *result_string = get_scheduling_result_string(process_copy, app_data->process_count, algorithm, quantum, &gantt);
    
    // Display results
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->results_text));
    gtk_text_buffer_set_text(buffer, result_string, -1);
    
    // Cleanup
    g_free(result_string);
    g_free(process_copy);
}
