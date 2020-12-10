
#include <gtk/gtk.h>
#include "change_password.h"
#include "chat_private.h"

GtkWidget       *window_menu;
gboolean check_menu = FALSE;
int argc_command;
char *argv_command;

gboolean getCheckMenu(){
    return check_menu;
}

void setCheckMenu(){
    if(getCheckMenu() == FALSE)
        check_menu = TRUE;
    else
        check_menu = FALSE;
}

int menu_chat(int argc, char **argv)
{
    GtkBuilder      *builder_menu; 
    argc_command = argc;
    argv_command = *argv;

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
    setCheckMenu();
    gtk_main_quit();
}

void on_btn_chat_private_clicked(){
    chat_private(argc_command,&argv_command);
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
