#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "cpu_scheduler.h"

// Add MLFQ to the algorithm enum
typedef enum {
    ALGORITHM_FCFS,
    ALGORITHM_SJF,
    ALGORITHM_SRTF,
    ALGORITHM_ROUND_ROBIN,
    ALGORITHM_PRIORITY_NON_PREEMPTIVE,
    ALGORITHM_PRIORITY_PREEMPTIVE,
    ALGORITHM_HRRN,
    ALGORITHM_MLQ,
    ALGORITHM_CFS,
    ALGORITHM_MLFQ  // Add MLFQ
} SchedulingAlgorithm;

// GUI structure to hold all widgets
typedef struct {
    GtkWidget *window;
    GtkWidget *main_box;
    
    // Input section
    GtkWidget *process_count_spin;
    GtkWidget *process_table;
    GtkWidget *quantum_spin;
    
    // Algorithm selection
    GtkWidget *algorithm_combo;
    
    // Results section
    GtkWidget *results_text;
    GtkWidget *execute_btn;
    
    // Data
    Process *processes;
    int process_count;
} AppData;

// Main GUI functions
void initialize_gui(int argc, char *argv[]);
void create_main_window(AppData *app_data);

// Callback functions
void on_execute_clicked(GtkWidget *widget, gpointer data);
void on_process_count_changed(GtkWidget *widget, gpointer data);
void on_process_field_changed(GtkWidget *widget, gpointer data);
void on_algorithm_changed(GtkWidget *widget, gpointer data);

// Process table management
void update_process_table(AppData *app_data);

// Algorithm helper function
const char* get_algorithm_name(SchedulingAlgorithm algorithm);

// GTK Helper Functions
GtkWidget* create_labeled_spin_button(const gchar *label_text, 
                                     double min, double max, double step, 
                                     double initial_value);
GtkWidget* create_vbox(gint spacing, gint border);
GtkWidget* create_hbox(gint spacing, gint border);
GtkWidget* create_labeled_frame(const gchar *label_text, GtkWidget *child);
GtkWidget* create_grid(gint row_spacing, gint column_spacing);
void set_widget_margin(GtkWidget *widget, gint margin);
GtkWidget* create_button(const gchar *label, GCallback callback, gpointer user_data);
GtkWidget* create_scrolled_text_view(gboolean editable, gboolean monospace);
gchar* get_text_from_text_view(GtkTextView *text_view);
void set_text_in_text_view(GtkTextView *text_view, const gchar *text);
GtkWidget* create_header_label(const gchar *text);
GtkWidget* create_subheader_label(const gchar *text);
void show_error_dialog(GtkWindow *parent, const gchar *title, const gchar *message);
void show_info_dialog(GtkWindow *parent, const gchar *title, const gchar *message);
GtkWidget* create_separator(GtkOrientation orientation);
void add_widgets_to_container(GtkContainer *container, GtkWidget *first_widget, ...);
GtkWidget* create_combo_box_text(const gchar **entries, gint count, gint active);
gchar* get_combo_box_active_text(GtkComboBoxText *combo);
void set_widgets_sensitivity(gboolean sensitive, GtkWidget *first_widget, ...);
GtkWidget* create_progress_bar(void);
void update_progress_bar(GtkProgressBar *progress, gdouble fraction, const gchar *text);

#endif
