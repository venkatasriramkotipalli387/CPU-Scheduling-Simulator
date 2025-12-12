#include <gtk/gtk.h>
#include "../../include/gui.h"

// Create a labeled spin button with specified range and value
GtkWidget* create_labeled_spin_button(const gchar *label_text, 
                                     double min, double max, double step, 
                                     double initial_value) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    
    GtkWidget *label = gtk_label_new(label_text);
    GtkWidget *spin = gtk_spin_button_new_with_range(min, max, step);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), initial_value);
    
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);
    
    return hbox;
}

// Create a vertical box with specified spacing and border
GtkWidget* create_vbox(gint spacing, gint border) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    if (border > 0) {
        gtk_container_set_border_width(GTK_CONTAINER(vbox), border);
    }
    return vbox;
}

// Create a horizontal box with specified spacing and border
GtkWidget* create_hbox(gint spacing, gint border) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
    if (border > 0) {
        gtk_container_set_border_width(GTK_CONTAINER(hbox), border);
    }
    return hbox;
}

// Create a frame with label and contained widget
GtkWidget* create_labeled_frame(const gchar *label_text, GtkWidget *child) {
    GtkWidget *frame = gtk_frame_new(label_text);
    if (child) {
        gtk_container_add(GTK_CONTAINER(frame), child);
    }
    return frame;
}

// Create a grid with specified spacing
GtkWidget* create_grid(gint row_spacing, gint column_spacing) {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), row_spacing);
    gtk_grid_set_column_spacing(GTK_GRID(grid), column_spacing);
    return grid;
}

// Set widget margin on all sides
void set_widget_margin(GtkWidget *widget, gint margin) {
    gtk_widget_set_margin_start(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
}

// Create a button with label and connect signal
GtkWidget* create_button(const gchar *label, GCallback callback, gpointer user_data) {
    GtkWidget *button = gtk_button_new_with_label(label);
    if (callback) {
        g_signal_connect(button, "clicked", callback, user_data);
    }
    return button;
}

// Create a scrolled text view with monospace font
GtkWidget* create_scrolled_text_view(gboolean editable, gboolean monospace) {
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), editable);
    
    if (monospace) {
        // Use CSS for modern GTK
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, 
            "textview { font-family: Monospace; font-size: 10pt; }", -1, NULL);
        GtkStyleContext *context = gtk_widget_get_style_context(text_view);
        gtk_style_context_add_provider(context, 
            GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(provider);
    }
    
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    return scrolled_window;
}
// Get text from text view
gchar* get_text_from_text_view(GtkTextView *text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

// Set text in text view
void set_text_in_text_view(GtkTextView *text_view, const gchar *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, text ? text : "", -1);
}

// Create a header label with large bold text
GtkWidget* create_header_label(const gchar *text) {
    GtkWidget *label = gtk_label_new(NULL);
    gchar *markup = g_markup_printf_escaped("<span size='x-large' weight='bold'>%s</span>", text);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    g_free(markup);
    return label;
}

// Create a subheader label with bold text
GtkWidget* create_subheader_label(const gchar *text) {
    GtkWidget *label = gtk_label_new(NULL);
    gchar *markup = g_markup_printf_escaped("<span size='large' weight='bold'>%s</span>", text);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    g_free(markup);
    return label;
}

// Show error dialog
void show_error_dialog(GtkWindow *parent, const gchar *title, const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_ERROR,
                                              GTK_BUTTONS_OK,
                                              "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Show info dialog
void show_info_dialog(GtkWindow *parent, const gchar *title, const gchar *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                              GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO,
                                              GTK_BUTTONS_OK,
                                              "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Create a separator
GtkWidget* create_separator(GtkOrientation orientation) {
    return gtk_separator_new(orientation);
}

// Add multiple widgets to a container
void add_widgets_to_container(GtkContainer *container, GtkWidget *first_widget, ...) {
    va_list args;
    GtkWidget *widget;
    
    if (first_widget) {
        gtk_container_add(container, first_widget);
        
        va_start(args, first_widget);
        while ((widget = va_arg(args, GtkWidget*)) != NULL) {
            gtk_container_add(container, widget);
        }
        va_end(args);
    }
}

// Create a combo box with string entries
GtkWidget* create_combo_box_text(const gchar **entries, gint count, gint active) {
    GtkWidget *combo = gtk_combo_box_text_new();
    
    for (gint i = 0; i < count; i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), entries[i]);
    }
    
    if (active >= 0 && active < count) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), active);
    }
    
    return combo;
}

// Get active text from combo box
gchar* get_combo_box_active_text(GtkComboBoxText *combo) {
    return gtk_combo_box_text_get_active_text(combo);
}

// Set sensitivity of multiple widgets
void set_widgets_sensitivity(gboolean sensitive, GtkWidget *first_widget, ...) {
    va_list args;
    GtkWidget *widget;
    
    if (first_widget) {
        gtk_widget_set_sensitive(first_widget, sensitive);
        
        va_start(args, first_widget);
        while ((widget = va_arg(args, GtkWidget*)) != NULL) {
            gtk_widget_set_sensitive(widget, sensitive);
        }
        va_end(args);
    }
}

// Create a progress bar
GtkWidget* create_progress_bar(void) {
    GtkWidget *progress = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress), TRUE);
    return progress;
}

// Update progress bar
void update_progress_bar(GtkProgressBar *progress, gdouble fraction, const gchar *text) {
    gtk_progress_bar_set_fraction(progress, fraction);
    if (text) {
        gtk_progress_bar_set_text(progress, text);
    }
}
