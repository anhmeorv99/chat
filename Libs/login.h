
#include <gtk/gtk.h>
#include "menu_chat.h"

typedef struct{
    GtkWidget *account;
    GtkWidget *password;  
    GtkWidget *lbl_err_username;
    GtkWidget *lbl_err_password;
    GtkWidget *lbl_invalid_login; 
}login_form;

GtkWidget       *window;
int sock_app;
int argc_param;
char *argv_param;

int login(int argc, char **argv)
{
    GtkBuilder      *builder_login; 
    
    login_form *login_inputed = g_slice_new(login_form);
    
    gtk_init(&argc, &argv);

    builder_login = gtk_builder_new();
    gtk_builder_add_from_file (builder_login, "Glade/login_ui.glade", NULL);

    window_login = GTK_WIDGET(gtk_builder_get_object(builder_login, "login_ui"));
    gtk_builder_connect_signals(builder_login, login_inputed);
    //----
    gtk_window_set_decorated(GTK_WINDOW(window_login),FALSE);
    login_inputed->account = GTK_WIDGET(gtk_builder_get_object(builder_login,"login_entry_account"));
    login_inputed->password = GTK_WIDGET(gtk_builder_get_object(builder_login,"login_entry_password"));
    login_inputed->lbl_err_username = GTK_WIDGET(gtk_builder_get_object(builder_login,
        "login_err_username"));
    login_inputed->lbl_err_password = GTK_WIDGET(gtk_builder_get_object(builder_login,
        "login_err_password"));
    login_inputed->lbl_invalid_login = GTK_WIDGET(gtk_builder_get_object(builder_login,
        "login_invalid_login"));
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
    gtk_main_quit();
    gtk_widget_set_visible(window,TRUE);
}

void on_login_btn_login_clicked(GtkButton *button, login_form *login_in){
    Object *object = new_login_object();
    Error err_username = ERR_NONE, err_password = ERR_NONE;
    char *markup_message;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    //const char *format_invalid = "<span foreground='green' weight='bold' font='13'>%s</span>";
    g_stpcpy(object->login.username,g_strdup(gtk_entry_get_text(GTK_ENTRY(login_in->account))));
    g_stpcpy(object->login.password , g_strdup(gtk_entry_get_text(GTK_ENTRY(login_in->password))));
    
    //test
    
    //
    g_print("Account: %s\n",object->login.username);
    g_print("Password: %s\n",object->login.password);
    //check loi
    gtk_widget_set_visible(login_in->lbl_err_username,FALSE);
    gtk_widget_set_visible(login_in->lbl_err_password,FALSE);
    gtk_widget_set_visible(login_in->lbl_invalid_login,FALSE);
    if(strlen(object->login.username) == 0)
        err_username = ERR_NULL_USERNAME;
    if(strlen(object->login.password) == 0)
        err_password = ERR_NULL_PASSWORD;
    if((err_username == ERR_NONE) && (err_password == ERR_NONE)){
        char msg_err_login[100];
        int recvBytes;
        if(send(sock_app,object,sizeof(Object),0) < 0){
            perror("send - login");
            exit(0);
        }
        if((recvBytes = recv(sock_app,object,sizeof(Object),0)) < 0){
            perror("recv-login");
            exit(0);
        }
        // msg_err_login[recvBytes] = '\0';
        error_to_string(object->login.err,msg_err_login);
        if(object->login.err != ERR_NONE){ // username hoac password sai
            markup_message = g_markup_printf_escaped(format_error,msg_err_login);
            if(object->login.err == ERR_NOT_USERNAME){
                gtk_widget_set_visible(login_in->lbl_err_username,TRUE);
                gtk_label_set_markup(GTK_LABEL(login_in->lbl_err_username),markup_message);
            }
            else if(object->login.err == ERR_USERNAME_LOGIN){
                gtk_widget_set_visible(login_in->lbl_err_username,TRUE);
                gtk_label_set_markup(GTK_LABEL(login_in->lbl_err_username),markup_message);
            }
            else{
                gtk_widget_set_visible(login_in->lbl_err_password,TRUE);
                gtk_label_set_markup(GTK_LABEL(login_in->lbl_err_password),markup_message);
            }
            g_free(markup_message);
            free_object(object);    
        }else{  // dang nhap thanh cong 
            dup_obj_login(object, sock_app);
            dup_obj_list_friend(object);
            dup_obj_menu_chat(object);
            dup_obj_chat_private(object); 
            dup_login_list_group(object);   
            dup_obj_admin(object);      
            free_object(object); 
            //gtk_window_close(GTK_WINDOW(window_login));
            gtk_widget_set_visible(window_login,FALSE);
            menu_chat(argc_param,&argv_param,sock_app);         
        }

    }else{
        if(err_username != ERR_NONE){
            char message[100];
            error_to_string(err_username,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            gtk_widget_set_visible(login_in->lbl_err_username,TRUE);
            gtk_label_set_markup(GTK_LABEL(login_in->lbl_err_username),markup_message);
        }
        if(err_password != ERR_NONE){
            char message[100];
            error_to_string(err_password,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            gtk_widget_set_visible(login_in->lbl_err_password,TRUE);
            gtk_label_set_markup(GTK_LABEL(login_in->lbl_err_password),markup_message);
        }
        g_free(markup_message);
        free_object(object);
    }
    //giai phong
    
}

void on_login_btn_cance_clicked(){
    gtk_window_close(GTK_WINDOW(window_login));
}
