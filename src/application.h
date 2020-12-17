#include <gtk/gtk.h>
#include "../Libs/signup.h"

int app(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder; 
    argc_param = argc;
    argv_param = *argv;
    sock_app = sockfd;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Glade/app_chat.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "app_chat"));

    gtk_builder_connect_signals(builder, NULL);

    //gtk_window_set_decorated(GTK_WINDOW(window),FALSE);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_app_chat_destroy()
{
    gtk_main_quit();
}

void on_btn_login_clicked(){
    gtk_widget_set_visible(window,FALSE);
    login(argc_param,&argv_param);
}

void on_btn_signup_clicked(){
    gtk_widget_set_visible(window,FALSE);
    signup(argc_param,&argv_param); 
}