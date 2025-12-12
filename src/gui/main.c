#include <gtk/gtk.h>
#include "../include/gui.h"

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create and initialize application data
    AppData *app_data = g_malloc(sizeof(AppData));
    app_data->processes = NULL;
    app_data->process_count = 0;
    
    // Create the main window
    create_main_window(app_data);
    
    // Show the window
    gtk_widget_show_all(app_data->window);
    
    // Start the GTK main loop
    gtk_main();
    
    // Cleanup
    if (app_data->processes) {
        g_free(app_data->processes);
    }
    g_free(app_data);
    
    return 0;
}
