#include <gtk/gtk.h>
#include <tinyfiledialogs.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GtkBuilder* builder; /* apprently this is like a makeshift asset manager for gtk? */

#define FILTER_PATTERNS_COUNT 2
#define DONE_TIMEOUT 8

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

GtkWidget* combo_box;
GtkWidget* button;

/*  /////////////////////////////////////////////////////
 /
  taken from stackoverflow, https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
 /
*/ /////////////////////////////////////////////////////

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static void run_app(GtkApplication* app, gpointer user_data);
void app_find_blocks();
char* app_select_image_file();
void format_and_image(GtkApplication* app, gpointer user_data);
GtkWidget* app_ui_create_widget(app_widget_props props);
void app_ui_combobox_append(GtkWidget* combo_box,const char* text);
void app_ui_load_css(const gchar* file_path);
void* app_multithread_FAM();

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
  
  button = app_ui_create_widget((app_widget_props){BUTTON, 50, 50, "Format And Image!", "button"});
  gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 1, 1);
  g_signal_connect (button, "clicked", G_CALLBACK (format_and_image), NULL);

  combo_box = app_ui_create_widget((app_widget_props){COMBO_BOX, 60, 150, "", "combo"});
  app_ui_combobox_append(combo_box, "Select a device to flash to!");
  app_find_blocks();
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

  
  if (getuid()){
    char* exit_error = "Please Run this as root (sudo)";
    printf("%s", exit_error);
       tinyfd_messageBox(
	"warning!",
	exit_error,
	"exit",
	"warning",
	0
     );
    exit(1);
  }
  
  g_signal_connect (app, "activate", G_CALLBACK (run_app), NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);

  return status;
}

void* app_multithread_FAM(){
  char* filetoflash;
  char* umount_command;
  char* dd_command = "dd ";
  char* selected;

  selected = gtk_combo_box_text_get_active_text((GtkComboBoxText*)combo_box);
  
  if(strcmp(selected, "Select a device to flash to!") == 0){
    printf("select a proper device!");
    exit(1);
  }
    
  filetoflash = app_select_image_file();
  umount_command = concat("umount ", selected);

  system(umount_command);
      
  dd_command = concat(dd_command, "if=");
  dd_command = concat(dd_command, filetoflash);
  dd_command = concat(dd_command, " of=");
  dd_command = concat(dd_command, selected);
  dd_command = concat(dd_command, " bs=8M status=progress");
  

  gtk_button_set_label (GTK_BUTTON(button), "        Wait!!...       ");
  gtk_widget_set_sensitive(button, FALSE);
  
  system(dd_command);

  sleep(1); /* i don't get it at all, i can only assume that gtk needs a little more time
	       but the app literally crashes without this */
  
  gtk_button_set_label (GTK_BUTTON(button), "Format And Image");
  gtk_widget_set_sensitive(button, TRUE);
}

void format_and_image(GtkApplication* app, gpointer user_data){
  pthread_t thread;
  
  pthread_create(&thread, NULL, app_multithread_FAM, NULL);
  pthread_detach(thread); 
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
char* app_select_image_file(){
  char* filename;
  char const* filters[1] = {"*.iso"};
  
  filename = tinyfd_openFileDialog(
    "Select an image!",
    "../",
    1,
    filters,
    NULL,
    0
  );

 if (! filename){
   tinyfd_messageBox(
	"Error",
	"select a proper iso next time!",
	"ok",
	"error",
	0
     );
  }

 return filename;
};
void app_find_blocks(){
  struct dirent *dir_contents;

    DIR *dptr = opendir("/dev/");
    char* fullname = "/dev/";

    if (dptr == NULL)
    {
        printf("Could not open current directory" );
    }

    while ((dir_contents = readdir(dptr)) != NULL){
      if(strncmp("sd", dir_contents->d_name, 2) == 0 && strlen(dir_contents->d_name) < 4)
      {
	printf("%s\n", dir_contents->d_name);
	app_ui_combobox_append(combo_box, concat(fullname ,dir_contents->d_name));	
      }
    }

    closedir(dptr);   
};
