#include <gtk/gtk.h>
#include "chat_group.h"
GtkWidget *window_group_chat;
GtkWidget *window_create_group_chat;
int sockfd_group_chat;
int argc_group_chat;
char *argv_group_chat;

int create_group_chat(int argc, char **argv);
//---------------------------------
//main
int list_group_chat(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder_group_chat; 
    sockfd_group_chat = sockfd;
   	argc_group_chat = argc;
   	argv_group_chat = *argv;
    gtk_init(&argc, &argv);

    builder_group_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_group_chat, "Glade/list_group_chat.glade", NULL);

    window_group_chat = GTK_WIDGET(gtk_builder_get_object(builder_group_chat, "list_group_chat"));
    gtk_builder_connect_signals(builder_group_chat, NULL);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_group_chat);

    gtk_widget_show(window_group_chat);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_list_group_chat_destroy()
{
    gtk_main_quit();
}

void on_btn_create_group_clicked(){
	create_group_chat(argc_group_chat,&argv_group_chat);
}

void on_btn_group_test_clicked(){
	chat_group(argc_group_chat,&argv_group_chat);
}

//-----------------------create group chat----------------------------
//main
int create_group_chat(int argc, char **argv)
{
    GtkBuilder      *builder_create_group_chat; 
   	
    gtk_init(&argc, &argv);

    builder_create_group_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_create_group_chat, "Glade/list_group_chat.glade", NULL);

    window_create_group_chat = GTK_WIDGET(gtk_builder_get_object(builder_create_group_chat, "create_group_chat"));
    gtk_builder_connect_signals(builder_create_group_chat, NULL);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_create_group_chat);

    gtk_widget_show(window_create_group_chat);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_create_group_chat_destroy()
{
    gtk_main_quit();
}
