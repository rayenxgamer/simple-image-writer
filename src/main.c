#include <gtk/gtk.h>
#include <stdio.h>

GtkBuilder* builder; /* apprently this is like a makeshift asset manager for gtk? */
GtkWidget* device_list;
gint global_list_item_counter = 0;

void AddListItem (GtkWidget *listbox, char *sText){
      gtk_list_box_insert(GTK_LIST_BOX(device_list), g_object_new(
      GTK_TYPE_LABEL,
      "visible", TRUE,
      "label", sText,
      NULL
    ), global_list_item_counter);
  global_list_item_counter++;
};

void hello(GtkWidget *widget, gpointer *data){
  g_print("hello world!\n");
};

static void activate (GtkApplication* app, gpointer        user_data) {
  GtkWidget* button_press_me_pls;
   
  GtkWidget *window = g_object_new(
    GTK_TYPE_APPLICATION_WINDOW,
    "application", GTK_APPLICATION(app),
    "default-width", 400,
    "default-height", 400,
    "border-width", 10,
    NULL
  );
  
  GtkWidget *scroll = g_object_new(
    GTK_TYPE_SCROLLED_WINDOW,
    "visible", TRUE,
    "shadow-type", GTK_SHADOW_NONE,
    "propagate-natural-width", TRUE,
    "propagate-natural-height", TRUE,
    NULL
  );
  
  device_list = g_object_new( GTK_TYPE_LIST_BOX, "visible", TRUE,  NULL  );
  gtk_container_add(GTK_CONTAINER(scroll), device_list);

  gtk_window_set_title (GTK_WINDOW (window), "Simple Image Writer (GNU-Linux Only)");
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

  gtk_container_add (GTK_CONTAINER (window), scroll);
 
  AddListItem (device_list, "This is a list of cats!");
  AddListItem (device_list, "Cats are very cute!!!");
  AddListItem (device_list, "They literally just lay down and :3");
  AddListItem (device_list, "My cat bites me tho! all day everyday");
  AddListItem (device_list, "Yum Yum!!!");
  
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_present((GtkWindow*)window);
}

int main (int  argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
