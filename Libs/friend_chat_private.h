#include <gtk/gtk.h>
#include "chat_private.h"

int argc_friend_chat;
char *argv_friend_chat;

int friend_chat_private(int argc, char **argv)
{
    GtkBuilder      *builder_friend_chat; 
 
    argc_friend_chat = argc;
    argv_friend_chat = *argv;
  

    gtk_init(&argc, &argv);

    builder_friend_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_friend_chat, "Glade/friend_chat_private.glade", NULL);

    window_friend_chat = GTK_WIDGET(gtk_builder_get_object(builder_friend_chat, "friend_chat_private"));
    
    //////////
    gtk_builder_connect_signals(builder_friend_chat, NULL);
   
    g_object_unref(builder_friend_chat);
    
    // start 1 second timer
    

    gtk_widget_show(window_friend_chat);    

    //gtk_widget_set_visible(widgets->scrol_list_friend,FALSE);   
           
    gtk_main();

  

    return 0;
}

// called when window is closed
void on_friend_chat_private_destroy()
{
    gtk_main_quit();
}

void on_btn_friend_test_clicked(){
    gtk_widget_set_visible(window_friend_chat,FALSE);
	chat_private(argc_friend_chat,&argv_friend_chat);
}