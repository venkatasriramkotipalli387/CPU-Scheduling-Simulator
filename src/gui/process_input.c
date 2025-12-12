#include <gtk/gtk.h>
#include "../../include/gui.h"

void on_process_count_changed(GtkWidget *widget, gpointer data) {
    (void)widget; // Mark as unused to suppress warning
    AppData *app_data = (AppData *)data;
    
    // Get new process count
    app_data->process_count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->process_count_spin));
    
    // Free old processes if any
    if (app_data->processes) {
        g_free(app_data->processes);
    }
    
    // Allocate new processes
    app_data->processes = g_malloc0_n(app_data->process_count, sizeof(Process));
    
    // Update process table
    update_process_table(app_data);
}

void update_process_table(AppData *app_data) {
    // Clear existing table
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(app_data->process_table));
    for(iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Create table headers
    GtkWidget *pid_header = gtk_label_new("PID");
    GtkWidget *at_header = gtk_label_new("Arrival Time");
    GtkWidget *bt_header = gtk_label_new("Burst Time");
    GtkWidget *priority_header = gtk_label_new("Priority");
    GtkWidget *nice_header = gtk_label_new("Nice Value"); // Add nice header
    GtkWidget *queue_header = gtk_label_new("Queue Level");
    
    gtk_widget_set_halign(pid_header, GTK_ALIGN_START);
    gtk_widget_set_halign(at_header, GTK_ALIGN_START);
    gtk_widget_set_halign(bt_header, GTK_ALIGN_START);
    gtk_widget_set_halign(priority_header, GTK_ALIGN_START);
    gtk_widget_set_halign(nice_header, GTK_ALIGN_START);
    gtk_widget_set_halign(queue_header, GTK_ALIGN_START);
    
    gtk_grid_attach(GTK_GRID(app_data->process_table), pid_header, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(app_data->process_table), at_header, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(app_data->process_table), bt_header, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(app_data->process_table), priority_header, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(app_data->process_table), nice_header, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(app_data->process_table), queue_header, 5, 0, 1, 1);
    
    // Create input fields for each process
    for (int i = 0; i < app_data->process_count; i++) {
        app_data->processes[i].pid = i + 1;
        
        // PID label
        GtkWidget *pid_label = gtk_label_new(g_strdup_printf("P%d", i + 1));
        gtk_widget_set_halign(pid_label, GTK_ALIGN_START);
        gtk_grid_attach(GTK_GRID(app_data->process_table), pid_label, 0, i + 1, 1, 1);
        
        // Arrival time spin button
        GtkWidget *at_spin = gtk_spin_button_new_with_range(0, 100, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(at_spin), 0);
        gtk_grid_attach(GTK_GRID(app_data->process_table), at_spin, 1, i + 1, 1, 1);
        g_object_set_data(G_OBJECT(at_spin), "process_index", GINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(at_spin), "field_type", GINT_TO_POINTER(0)); // 0 for arrival time
        g_signal_connect(at_spin, "value-changed", G_CALLBACK(on_process_field_changed), app_data);
        
        // Burst time spin button
        GtkWidget *bt_spin = gtk_spin_button_new_with_range(1, 100, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(bt_spin), 5);
        gtk_grid_attach(GTK_GRID(app_data->process_table), bt_spin, 2, i + 1, 1, 1);
        g_object_set_data(G_OBJECT(bt_spin), "process_index", GINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(bt_spin), "field_type", GINT_TO_POINTER(1)); // 1 for burst time
        g_signal_connect(bt_spin, "value-changed", G_CALLBACK(on_process_field_changed), app_data);
        
        // Priority spin button
        GtkWidget *priority_spin = gtk_spin_button_new_with_range(1, 10, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(priority_spin), 1);
        gtk_grid_attach(GTK_GRID(app_data->process_table), priority_spin, 3, i + 1, 1, 1);
        g_object_set_data(G_OBJECT(priority_spin), "process_index", GINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(priority_spin), "field_type", GINT_TO_POINTER(2)); // 2 for priority
        g_signal_connect(priority_spin, "value-changed", G_CALLBACK(on_process_field_changed), app_data);
        
        // Nice value spin button (for CFS)
        GtkWidget *nice_spin = gtk_spin_button_new_with_range(-20, 19, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(nice_spin), 0);
        gtk_grid_attach(GTK_GRID(app_data->process_table), nice_spin, 4, i + 1, 1, 1);
        g_object_set_data(G_OBJECT(nice_spin), "process_index", GINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(nice_spin), "field_type", GINT_TO_POINTER(4)); // 4 for nice value
        g_signal_connect(nice_spin, "value-changed", G_CALLBACK(on_process_field_changed), app_data);
        
        // Queue level spin button
        GtkWidget *queue_spin = gtk_spin_button_new_with_range(0, 1, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(queue_spin), 0);
        gtk_grid_attach(GTK_GRID(app_data->process_table), queue_spin, 5, i + 1, 1, 1);
        g_object_set_data(G_OBJECT(queue_spin), "process_index", GINT_TO_POINTER(i));
        g_object_set_data(G_OBJECT(queue_spin), "field_type", GINT_TO_POINTER(3)); // 3 for queue level
        g_signal_connect(queue_spin, "value-changed", G_CALLBACK(on_process_field_changed), app_data);
        
        // Initialize process data
        app_data->processes[i].arrival_time = 0;
        app_data->processes[i].burst_time = 5;
        app_data->processes[i].priority = 1;
        app_data->processes[i].nice = 0;
        app_data->processes[i].queue_level = 0;
    }
    
    // Show all widgets
    gtk_widget_show_all(app_data->process_table);
}

void on_process_field_changed(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    
    int process_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "process_index"));
    int field_type = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "field_type"));
    int value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
    
    switch (field_type) {
        case 0: // Arrival time
            app_data->processes[process_index].arrival_time = value;
            break;
        case 1: // Burst time
            app_data->processes[process_index].burst_time = value;
            break;
        case 2: // Priority
            app_data->processes[process_index].priority = value;
            break;
        case 3: // Queue Level
            app_data->processes[process_index].queue_level = value;
            break;
        case 4: // Nice value
            app_data->processes[process_index].nice = value;
            break;
    }
}

void on_algorithm_changed(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    
    int algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    
    // Show quantum input only for Round Robin
    GList *children = gtk_container_get_children(GTK_CONTAINER(gtk_widget_get_parent(widget)));
    GList *iter = children;
    
    // Find quantum label and spin in the parent container
    while (iter != NULL) {
        GtkWidget *child = GTK_WIDGET(iter->data);
        if (GTK_IS_LABEL(child)) {
            const gchar *text = gtk_label_get_text(GTK_LABEL(child));
            if (g_strcmp0(text, "Time Quantum:") == 0) {
                if (algorithm == 3) { // Round Robin
                    gtk_widget_show(child);
                } else {
                    gtk_widget_hide(child);
                }
            }
        } else if (GTK_IS_SPIN_BUTTON(child) && child != app_data->process_count_spin) {
            if (algorithm == 3) { // Round Robin
                gtk_widget_show(child);
            } else {
                gtk_widget_hide(child);
            }
        }
        iter = g_list_next(iter);
    }
    g_list_free(children);
}
