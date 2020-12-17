#include <gtk/gtk.h>
int chat_group(int argc, char **argv)
{
    GtkBuilder      *builder_chat_group; 
    GtkWidget       *window_chat_group;

  

    gtk_init(&argc, &argv);

    builder_chat_group = gtk_builder_new();
    gtk_builder_add_from_file (builder_chat_group, "Glade/list_group_chat.glade", NULL);

    window_chat_group = GTK_WIDGET(gtk_builder_get_object(builder_chat_group, "chat_group"));
    
    //////////
    gtk_builder_connect_signals(builder_chat_group, NULL);
   
    g_object_unref(builder_chat_group);
    
    // start 1 second timer
    

    gtk_widget_show(window_chat_group);    

    //gtk_widget_set_visible(widgets->scrol_list_friend,FALSE);   
           
    gtk_main();


    return 0;
}

// called when window is closed
void on_chat_group_destroy()
{
    gtk_main_quit();
}