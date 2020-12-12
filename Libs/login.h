
#include <gtk/gtk.h>
#include "menu_chat.h"

typedef struct{
    GtkWidget *account;
    GtkWidget *password;   
    int argc;
    char *argv;
    int sockfd;
}login_form;

GtkWidget       *window_login;

gboolean getCheckLogin(){
    return getCheckMenu();
}

void setCheckLogin(){
    setCheckMenu();
    
}


int login(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder_login; 
    
    login_form *login_inputed = g_slice_new(login_form);
    login_inputed->argc = argc;
    login_inputed->argv = *argv;
    login_inputed->sockfd = sockfd;
    
    gtk_init(&argc, &argv);

    builder_login = gtk_builder_new();
    gtk_builder_add_from_file (builder_login, "Glade/login_ui.glade", NULL);

    window_login = GTK_WIDGET(gtk_builder_get_object(builder_login, "login_ui"));
    gtk_builder_connect_signals(builder_login, login_inputed);
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_login),FALSE);
    login_inputed->account = GTK_WIDGET(gtk_builder_get_object(builder_login,"login_entry_account"));
    login_inputed->password = GTK_WIDGET(gtk_builder_get_object(builder_login,"login_entry_password"));
    //---
    g_object_unref(builder_login);

    gtk_widget_show(window_login);                
    gtk_main();

    g_slice_free(login_form,login_inputed);

    return 0;
}

// called when window is closed
void on_login_ui_destroy()
{
    setCheckLogin();
    gtk_main_quit();
}

void on_login_btn_login_clicked(GtkButton *button, login_form *login_in){
    Object *object = new_login_object();
    g_stpcpy(object->login.username,g_strdup(gtk_entry_get_text(GTK_ENTRY(login_in->account))));
    g_stpcpy(object->login.password , g_strdup(gtk_entry_get_text(GTK_ENTRY(login_in->password))));
    dup_obj_login(object);
    //test
    set_obj_chat_private(object->login.username, login_in->sockfd);
    //
    g_print("Account: %s\n",object->login.username);
    g_print("Password: %s\n",object->login.password);

    if(send(login_in->sockfd,object,sizeof(Object),0) < 0){
        perror("send - login");
        return;
    }
    
    free_object(object);
    //cach 1:
    setCheckLogin();
    gtk_window_close(GTK_WINDOW(window_login));
    //cach 2:
    //gtk_widget_set_visible(window_login,FALSE);
    //---------
    menu_chat(login_in->argc,&login_in->argv);
    
}

void on_login_btn_cance_clicked(){
    
    gtk_window_close(GTK_WINDOW(window_login));
}
