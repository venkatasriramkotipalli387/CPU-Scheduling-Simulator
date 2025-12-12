#include <gtk/gtk.h>
#include "../../include/gui.h"

void create_main_window(AppData *app_data) {
    // Create the main window
    app_data->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app_data->window), "CPU Scheduling Simulator");
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 900, 700);
    gtk_container_set_border_width(GTK_CONTAINER(app_data->window), 10);
    
    // Connect the destroy signal
    g_signal_connect(app_data->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main vertical box
    app_data->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(app_data->window), app_data->main_box);
    
    // Add title
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span size='xx-large' weight='bold'>CPU Scheduling Simulator</span>");
    gtk_box_pack_start(GTK_BOX(app_data->main_box), title_label, FALSE, FALSE, 0);
    
    // Add separator
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(app_data->main_box), separator, FALSE, FALSE, 0);
    
    // Process input section
    GtkWidget *input_frame = gtk_frame_new("Process Input");
    gtk_box_pack_start(GTK_BOX(app_data->main_box), input_frame, FALSE, FALSE, 0);
    
    GtkWidget *input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(input_box), 10);
    gtk_container_add(GTK_CONTAINER(input_frame), input_box);
    
    // Process count selection
    GtkWidget *count_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(input_box), count_box, FALSE, FALSE, 0);
    
    GtkWidget *count_label = gtk_label_new("Number of Processes:");
    gtk_box_pack_start(GTK_BOX(count_box), count_label, FALSE, FALSE, 0);
    
    app_data->process_count_spin = gtk_spin_button_new_with_range(1, 20, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->process_count_spin), 4);
    gtk_box_pack_start(GTK_BOX(count_box), app_data->process_count_spin, FALSE, FALSE, 0);
    
    g_signal_connect(app_data->process_count_spin, "value-changed", G_CALLBACK(on_process_count_changed), app_data);
    
    // Process table
    app_data->process_table = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(app_data->process_table), 5);
    gtk_grid_set_column_spacing(GTK_GRID(app_data->process_table), 10);
    gtk_box_pack_start(GTK_BOX(input_box), app_data->process_table, FALSE, FALSE, 0);
    
    // Algorithm selection section
    GtkWidget *algo_frame = gtk_frame_new("Algorithm Selection");
    gtk_box_pack_start(GTK_BOX(app_data->main_box), algo_frame, FALSE, FALSE, 0);
    
    GtkWidget *algo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(algo_box), 10);
    gtk_container_add(GTK_CONTAINER(algo_frame), algo_box);
    
    GtkWidget *algo_label = gtk_label_new("Scheduling Algorithm:");
    gtk_box_pack_start(GTK_BOX(algo_box), algo_label, FALSE, FALSE, 0);
    
    app_data->algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "First Come First Serve (FCFS)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Shortest Job First (SJF)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Shortest Remaining Time First (SRTF)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Round Robin");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Priority (Non-Preemptive)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Priority (Preemptive)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Highest Response Ratio Next (HRRN)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Multi-Level Queue (MLQ)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Completely Fair Scheduler (CFS)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data->algorithm_combo), "Multilevel Feedback Queue (MLFQ)"); // Add MLFQ
    gtk_combo_box_set_active(GTK_COMBO_BOX(app_data->algorithm_combo), 0);
    gtk_box_pack_start(GTK_BOX(algo_box), app_data->algorithm_combo, FALSE, FALSE, 0);
    
    // Quantum input (initially hidden)
    GtkWidget *quantum_label = gtk_label_new("Time Quantum:");
    gtk_box_pack_start(GTK_BOX(algo_box), quantum_label, FALSE, FALSE, 0);
    
    app_data->quantum_spin = gtk_spin_button_new_with_range(1, 100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->quantum_spin), 2);
    gtk_box_pack_start(GTK_BOX(algo_box), app_data->quantum_spin, FALSE, FALSE, 0);
    gtk_widget_hide(quantum_label);
    gtk_widget_hide(app_data->quantum_spin);
    
    // Show quantum only for Round Robin
    g_signal_connect(app_data->algorithm_combo, "changed", G_CALLBACK(on_algorithm_changed), app_data);
    
    // Execute button
    app_data->execute_btn = gtk_button_new_with_label("Execute Scheduling");
    g_signal_connect(app_data->execute_btn, "clicked", G_CALLBACK(on_execute_clicked), app_data);
    gtk_box_pack_start(GTK_BOX(app_data->main_box), app_data->execute_btn, FALSE, FALSE, 0);
    
    // Results section
    GtkWidget *results_frame = gtk_frame_new("Results");
    gtk_box_pack_start(GTK_BOX(app_data->main_box), results_frame, TRUE, TRUE, 0);
    
    GtkWidget *results_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(results_scrolled), 10);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(results_scrolled),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(results_frame), results_scrolled);
    
    app_data->results_text = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(app_data->results_text), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(app_data->results_text), TRUE);
    gtk_container_add(GTK_CONTAINER(results_scrolled), app_data->results_text);
    
    // Initialize process table
    on_process_count_changed(NULL, app_data);
}
