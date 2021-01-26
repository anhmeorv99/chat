
#include <gtk/gtk.h>
#include "change_password.h"

#include "list_group_chat.h"


GtkWidget       *window_menu;
GtkWidget       *window_login;
int argc_command;
char *argv_command;
int sockfd_menu_chat;

Object *obj_menu_chat;

void dup_obj_menu_chat(Object *obj){
    obj_menu_chat = duplicate_object(obj);
}

int menu_chat(int argc, char **argv, int sockfd)
{
    GtkBuilder      *builder_menu; 
    GtkWidget *btn_admin;
    argc_command = argc;
    argv_command = *argv;
    sockfd_menu_chat = sockfd;
    gtk_init(&argc, &argv);

    builder_menu = gtk_builder_new();
    gtk_builder_add_from_file (builder_menu, "Glade/menu_chat.glade", NULL);

    window_menu = GTK_WIDGET(gtk_builder_get_object(builder_menu, "menu_chat"));
    btn_admin = GTK_WIDGET(gtk_builder_get_object(builder_menu,"btn_admin"));
    if(obj_menu_chat->login.is_admin != 1){
        gtk_widget_set_visible(btn_admin, FALSE);
    }
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
    if(getCheckChatPrivate() == TRUE){
        gtk_window_close(GTK_WINDOW(window_chat));
 
    }
    
    
    if(getCheckListGroup() == TRUE){
        gtk_window_close(GTK_WINDOW(window_group_chat));
      
    }
    if(getCheckChangePass() == TRUE){
        gtk_window_close(GTK_WINDOW(window_change_pass));
       
    }

       
    
    if(getCheckAdmin() == TRUE){
        gtk_window_close(GTK_WINDOW(window_admin));

    }
    if(getCheckCreateGroup() == TRUE){
        gtk_window_close(GTK_WINDOW(window_create_group_chat));
    }
    if(getCheckChatGroup() == TRUE){
        gtk_window_close(GTK_WINDOW(window_chat_group));
    }
    if(getCheckListGroup() == TRUE){
        gtk_window_close(GTK_WINDOW(window_group_chat));

    }
   
    //---
    obj_menu_chat->signal = SIGNAL_LOGUOT;
    if(send(sockfd_menu_chat,obj_menu_chat,sizeof(Object), 0) < 0){
        perror("Can't logout");
        exit(0);
    }
    gtk_main_quit();
    gtk_widget_set_visible(window_login,TRUE);
    
}

void on_btn_chat_private_clicked(){
  
    if(getCheckChatPrivate() == FALSE){
        setCheckChatPrivate();
        chat_private(argc_command, &argv_command,sockfd_menu_chat);
    }
     
}

void on_btn_chat_group_clicked(){
    if(getCheckListGroup() == FALSE){
        setCheckListGroup();
        list_group_chat(argc_command,&argv_command,sockfd_menu_chat);
    }
    
}

void on_btn_list_friend_menu_clicked(){
    if(getCheckMenuFriend() == FALSE){
        setCheckMenuFriend();
        menu_list_friend(argc_command,&argv_command,sockfd_menu_chat);
    }
    
}

void on_btn_change_password_clicked(){
    if(getCheckChangePass() == FALSE){
        setCheckChangePass();
        change_password(argc_command,&argv_command);
    }
    
}

void on_btn_admin_clicked(){

    if(getCheckAdmin() ==FALSE){
        setCheckAdmin();
        admin(argc_command,&argv_command, sockfd_menu_chat);
    }
    

}

void on_btn_logout_clicked(){
    gtk_window_close(GTK_WINDOW(window_menu));
}
