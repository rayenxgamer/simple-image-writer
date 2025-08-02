#include <gtk/gtk.h>
#include <stdio.h>

GtkBuilder* builder; /* apprently this is like a makeshift asset manager for gtk? */

gint global_list_item_counter = 0;

void list_add_item (GtkWidget *listbox, char *list_label);
void format_and_image(GtkApplication* app, gpointer user_data);
void app_load_css(const gchar* file_path);

static void init_app(GtkApplication* app, gpointer user_data);
static void run_app(GtkApplication* app, gpointer user_data);

static void init_app(GtkApplication* app, gpointer user_data){
  return;
}

static void run_app (GtkApplication* app, gpointer        user_data) {   
  GtkWidget *window = g_object_new(
    GTK_TYPE_APPLICATION_WINDOW,
    "application", GTK_APPLICATION(app),
    "default-width", 400,
    "default-height", 400,
    "border-width", 10,
    NULL
  );
  

  GtkWidget* grid;
  grid = gtk_grid_new();
  gtk_container_add (GTK_CONTAINER (window), grid);
  gtk_widget_set_name(grid, "main_grid");

  GtkWidget* button;
  button = gtk_button_new_with_label("Format and Image!");
  g_signal_connect (button, "clicked", G_CALLBACK (format_and_image), NULL);

  GtkWidget *space;
  space = gtk_label_new("");

  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
  
  button = gtk_button_new_with_label ("Button 2");
  g_signal_connect (button, "clicked", G_CALLBACK (format_and_image), NULL);

  gtk_grid_attach(GTK_GRID(grid), space, 0, 1, 1, 1 );
  gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);
  
  space = gtk_label_new("");
  gtk_grid_attach(GTK_GRID(grid), space, 2, 1, 1, 1 );

    button = gtk_button_new_with_label ("Button 3");
  g_signal_connect (button, "clicked", G_CALLBACK (format_and_image), NULL);

  gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);
  
  gtk_window_set_title (GTK_WINDOW (window), "Simple Image Writer (GNU-Linux Only)");
  gtk_window_set_default_size (GTK_WINDOW (window), 250, 200);
  
  app_load_css("../resources/style.css");
 
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_widget_show_all(window);
}


int main (int  argc, char **argv) {
  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  int status;
  
  g_signal_connect (app, "startup", G_CALLBACK (init_app), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (run_app), NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

void list_add_item (GtkWidget *listbox, char *list_label){
      gtk_list_box_insert(GTK_LIST_BOX(listbox), g_object_new(
      GTK_TYPE_LABEL,
      "visible", TRUE,
      "label", list_label,
      NULL
    ), global_list_item_counter);
  global_list_item_counter++;
}
void format_and_image(GtkApplication* app, gpointer user_data){
  printf("hello world!");
  return;
}

void app_load_css(const gchar* file_path){
  GtkCssProvider* provider;
  GdkDisplay* display;
  GdkScreen* screen;
  
  GError* error_handler = 0;
  gboolean success = 1;

  GFile* css_fp = g_file_new_for_path(file_path);
  
  provider = gtk_css_provider_new();
  display = gdk_display_get_default();
  screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_css_provider_load_from_file(provider, css_fp, &error_handler);
  
  if(!success)
  {
    printf("failed to load css file from path: %s\n", file_path);
  }
  g_object_unref(provider);
}
