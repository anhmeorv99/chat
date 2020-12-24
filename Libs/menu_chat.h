
#include <gtk/gtk.h>
#include "change_password.h"
#include "friend_chat_private.h"
#include "list_group_chat.h"
#include "menu_list_friend.h"

GtkWidget       *window_menu;
GtkWidget       *window_login;
int argc_command;
char *argv_command;
int sockfd_menu_chat;

int menu_chat(int argc, char **argv, int sockfd)
{
    GtkBuilder      *builder_menu; 
    argc_command = argc;
    argv_command = *argv;
    sockfd_menu_chat = sockfd;
    gtk_init(&argc, &argv);

    builder_menu = gtk_builder_new();
    gtk_builder_add_from_file (builder_menu, "Glade/menu_chat.glade", NULL);

    window_menu = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_chat"));
    gtk_builder_connect_signals(builder_menu, NULL);
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_menu),FALSE);
    //---
    g_object_unref(builder_menu);

    gtk_widget_show(window_menu);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_menu_chat_destroy()
{
    gtk_main_quit();
    gtk_widget_set_visible(window_login,TRUE);
}

void on_btn_chat_private_clicked(){
    friend_chat_private(argc_command,&argv_command);
}

void on_btn_chat_group_clicked(){
    list_group_chat(argc_command,&argv_command,sockfd_menu_chat);
}

void on_btn_list_friend_menu_clicked(){
    menu_list_friend(argc_command,&argv_command,sockfd_menu_chat);
}

void on_btn_change_password_clicked(){
    if(getCheckChangePass() == FALSE){
        setCheckChangePass();
        change_password(argc_command,&argv_command);
    }
    
}

void on_btn_logout_clicked(){
    gtk_window_close(GTK_WINDOW(window_menu));
}
