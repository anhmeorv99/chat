
#include <gtk/gtk.h>
#include <string.h>
#include "login.h"
typedef struct{
    GtkWidget *name;
    GtkWidget *account;
    GtkWidget *password;
    GtkWidget *re_password;
    GtkWidget *lbl_err_name;
    GtkWidget *lbl_err_username;
    GtkWidget *lbl_err_password;
    GtkWidget *lbl_err_re_password;
    GtkWidget *lbl_invalid_signup;
    int sockfd;
}signup_form;

GtkWidget       *window_signup;

//main
int signup(int argc, char **argv)
{
    GtkBuilder      *builder_signup; 
    
    signup_form *signup_inputed = g_slice_new(signup_form);
    
    signup_inputed->sockfd = sock_app;
    gtk_init(&argc, &argv);

    builder_signup = gtk_builder_new();
    gtk_builder_add_from_file (builder_signup, "Glade/signup_ui.glade", NULL);

    window_signup = GTK_WIDGET(gtk_builder_get_object(builder_signup, "signup_ui"));
    gtk_builder_connect_signals(builder_signup, signup_inputed);
    //--
    gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    signup_inputed->name = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_name"));
    signup_inputed->account = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_account"));
    signup_inputed->password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_password"));
    signup_inputed->re_password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_re_password"));

    signup_inputed->lbl_err_username = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_err_username"));
    signup_inputed->lbl_err_name = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_err_name"));
    signup_inputed->lbl_err_password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_err_password"));
    signup_inputed->lbl_err_re_password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_err_re_password"));
    signup_inputed->lbl_invalid_signup = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_invalid_signup"));
    //--
    g_object_unref(builder_signup);

    gtk_widget_show(window_signup);                
    gtk_main();

    g_slice_free(signup_form,signup_inputed);
    return 0;
}

// called when window is closed
void on_signup_ui_destroy()
{
    gtk_main_quit();
    gtk_widget_set_visible(window,TRUE);
}

void on_signup_btn_signup_clicked(GtkButton *button, signup_form *signup_in){
    Object *obj = new_signup_object();
    Error err_name, err_username, err_password, err_re_password;
    char *markup_message;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    const char *format_invalid = "<span foreground='green' weight='bold' font='13'>%s</span>";
    g_stpcpy(obj->signup.name , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->name))));
    g_stpcpy(obj->signup.username , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->account))));
    g_stpcpy(obj->signup.password , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->password))));
    g_stpcpy(obj->signup.re_password , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->re_password))));

    //check loi
    err_name = check_signup_name(obj->signup.name);
    err_username = check_signup_username(obj->signup.username);
    err_password = check_signup_password(obj->signup.password);
    err_re_password = check_signup_re_password(obj->signup.re_password,obj->signup.password);
    //hien thi loi neu co
    if((err_name == ERR_NONE) && (err_username == ERR_NONE) && 
        (err_password == ERR_NONE) && (err_re_password == ERR_NONE)){
        Error err_has_username_; char msg_err_username[100];
        int recvBytes;
        char str[100];
        //dong cac tab loi va invalid
        gtk_widget_set_visible(signup_in->lbl_err_name,FALSE);
        gtk_widget_set_visible(signup_in->lbl_err_username,FALSE);
        gtk_widget_set_visible(signup_in->lbl_err_password,FALSE);
        gtk_widget_set_visible(signup_in->lbl_err_re_password,FALSE);
        gtk_widget_set_visible(signup_in->lbl_invalid_signup,FALSE);
        //gui obj len server neu dau vao dc kiem duyet
        if(send(signup_in->sockfd,obj,sizeof(Object),0) < 0){
            perror("send- signup");
            exit(0);
        }
        //nhan tin hieu tu server 
        if((recvBytes = recv(signup_in->sockfd,msg_err_username,sizeof(msg_err_username),0)) < 0){
            perror("recv - signup");
            exit(0);
        }
        msg_err_username[recvBytes] = '\0';
        err_has_username_ = error_to_enum(msg_err_username);
        error_to_string(err_has_username_,str);
        if(err_has_username_ == ERR_NONE){ //dang ky thanh cong
            char message[100];
            Invalid inv = OK_SIGNUP;
            invalid_to_string(inv, message);
            markup_message = g_markup_printf_escaped(format_invalid,message);
            if(gtk_widget_is_visible(signup_in->lbl_invalid_signup) == FALSE)
                gtk_widget_set_visible(signup_in->lbl_invalid_signup,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_invalid_signup),markup_message);
            
        }else{ //username da ton tai
            char message[101];
            error_to_string(err_has_username_,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(signup_in->lbl_err_username) == FALSE)
                gtk_widget_set_visible(signup_in->lbl_err_username,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_username),markup_message);
        }

    }else{
        //hien thi loi name
        if(err_name != ERR_NONE){
            char message[100];
            error_to_string(err_name,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(signup_in->lbl_err_name) == FALSE){
                gtk_widget_set_visible(signup_in->lbl_err_name,TRUE);
            }             
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_name),markup_message);
        }else{
            gtk_widget_set_visible(signup_in->lbl_err_name,FALSE);
        }
        //hien thi loi username
        if(err_username != ERR_NONE){
            char message[100];
            error_to_string(err_username,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(signup_in->lbl_err_username) == FALSE)
                gtk_widget_set_visible(signup_in->lbl_err_username,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_username),markup_message);
        }else{
            gtk_widget_set_visible(signup_in->lbl_err_username,FALSE);
        }
        //hien thi loi password
        if(err_password != ERR_NONE){
            char message[100];
            error_to_string(err_password,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(signup_in->lbl_err_password) == FALSE)
                gtk_widget_set_visible(signup_in->lbl_err_password,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_password),markup_message);
        }else{
            gtk_widget_set_visible(signup_in->lbl_err_password,FALSE);
        }
        //hien thi loi confirm password
        
        if(err_re_password != ERR_NONE){
            char message[100];
            error_to_string(err_re_password,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(signup_in->lbl_err_re_password) == FALSE)
                gtk_widget_set_visible(signup_in->lbl_err_re_password,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_re_password),markup_message);
        }else{
            gtk_widget_set_visible(signup_in->lbl_err_re_password,FALSE);
        }
    }
    //giai phong bo nho
    g_free(markup_message);
    free_object(obj);

}

void on_signup_btn_cance_clicked(){
    gtk_window_close(GTK_WINDOW(window_signup));
}
