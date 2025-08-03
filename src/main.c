#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

GtkBuilder* builder; /* apprently this is like a makeshift asset manager for gtk? */

typedef enum {
  SPACE,
  BUTTON,
  LABEL,
 COMBO_BOX
 
} widget_type;

typedef struct {
  widget_type type;
  gint height, width;
  const char* label_text;
  const char* css_class_name;
} app_widget_props;

gint global_list_item_counter = 0;

void list_add_item (GtkWidget *listbox, char *list_label);
void format_and_image(GtkApplication* app, gpointer user_data);
GtkWidget* app_ui_create_widget(app_widget_props props);
void app_ui_combobox_append(GtkWidget* combo_box,const char* text);
void app_ui_load_css(const gchar* file_path);

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
  button = app_ui_create_widget((app_widget_props){BUTTON, 50, 50, "Format And Image!", "button"});
  gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 1, 1);
  g_signal_connect (button, "clicked", G_CALLBACK (format_and_image), NULL);

  GtkWidget* combo_box;
  combo_box = app_ui_create_widget((app_widget_props){COMBO_BOX, 60, 150, "", "combo"});
  app_ui_combobox_append(combo_box, "UWU ;3");
  app_ui_combobox_append(combo_box,"OWO ;3");
  app_ui_combobox_append(combo_box,"VWV ;3");
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);
  gtk_grid_attach(GTK_GRID(grid), combo_box, 0, 1, 3, 1);

  GtkWidget *space;
  space = app_ui_create_widget((app_widget_props){SPACE, 30, 100, "", "space"});
  gtk_grid_attach(GTK_GRID(grid), space, 0, 0, 1, 1 );

  space = app_ui_create_widget((app_widget_props){SPACE, 30, 100, "", "space"});
  gtk_grid_attach(GTK_GRID(grid), space, 2, 0, 1, 1 );

  space = app_ui_create_widget((app_widget_props){SPACE, 30, 100, "", "space"});
  gtk_grid_attach(GTK_GRID(grid), space, 0, 1, 1, 1 );
  
  gtk_window_set_title (GTK_WINDOW (window), "Simple Image Writer (GNU-Linux Only)");
  gtk_window_set_default_size (GTK_WINDOW (window), 250, 200);
  
  app_ui_load_css("../resources/style.css");
 
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

void app_ui_load_css(const gchar* file_path){
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

GtkWidget* app_ui_create_widget(app_widget_props props){
  GtkWidget* temp_widget;
  switch(props.type){
    case SPACE:
        temp_widget = gtk_label_new("");
      break;
    case BUTTON:
        temp_widget = gtk_button_new_with_label(props.label_text);
      break;
    case LABEL:
        temp_widget = gtk_label_new(props.label_text);
      break;
    case COMBO_BOX:
        temp_widget = gtk_combo_box_text_new();
      break;
    default:
      printf("none selected!\n");
  }
  
  gtk_widget_set_name(temp_widget, props.css_class_name); 
  gtk_widget_set_size_request (temp_widget, props.width, props.height);
  return temp_widget;
};

void app_ui_combobox_append(GtkWidget* combo_box,const char* text){
  if (strcmp(text, "") == 0){
    printf("\e[31mWARNING:\e[0m empty string has been entered");
  };
  
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box), NULL, text);
}
