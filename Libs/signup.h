
#include <gtk/gtk.h>
#include <string.h>
typedef struct{
    GtkWidget *name;
    GtkWidget *account;
    GtkWidget *password;
    GtkWidget *re_password;
    GtkWidget *lbl_err_account;
    int sockfd;
}signup_form;
gboolean check_signup = FALSE;
GtkWidget       *window_signup;

gboolean getCheckSignup(){
    return check_signup;
}

void setCheckSignup(){
	if(getCheckSignup() == FALSE)
    	check_signup = TRUE;
    else
    	check_signup = FALSE;
}

//check error or invalid

Error check_account(char *account){
    Error err;
    if(strlen(account) < 6){
        err = ERR_ACCOUNT;
    }
    else{
        err = ERR_NONE;
    }
    return err;
}

Error check_password(char *password){
    Error err;
    if(strlen(password) < 6){
        err = ERR_MIN_PASSWORD;
    }
    else{
        err = ERR_NONE;
    }
    return err;
}

//main
int signup(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder_signup; 
    
    signup_form *signup_inputed = g_slice_new(signup_form);
    signup_inputed->sockfd = sockfd;
    gtk_init(&argc, &argv);

    builder_signup = gtk_builder_new();
    gtk_builder_add_from_file (builder_signup, "Glade/signup_ui.glade", NULL);

    window_signup = GTK_WIDGET(gtk_builder_get_object(builder_signup, "signup_ui"));
    gtk_builder_connect_signals(builder_signup, signup_inputed);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    signup_inputed->name = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_name"));
    signup_inputed->account = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_account"));
    signup_inputed->password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_password"));
    signup_inputed->re_password = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_entry_re_password"));

    signup_inputed->lbl_err_account = GTK_WIDGET(gtk_builder_get_object(builder_signup,"signup_lbl_err_account"));
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
    setCheckSignup();
    gtk_main_quit();
}

void on_signup_btn_signup_clicked(GtkButton *button, signup_form *signup_in){
    
   
    Object *obj = new_signup_object();
    Error error;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    //const char *format_invalid = "<span foreground='green' weight='bold' font='13'>%s</span>";
    char *markup_message;
    g_stpcpy(obj->signup.name , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->name))));
    g_stpcpy(obj->signup.username , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->account))));
    g_stpcpy(obj->signup.password , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->password))));
    g_stpcpy(obj->signup.re_password , g_strdup(gtk_entry_get_text(GTK_ENTRY(signup_in->re_password))));
    g_print("Name: %s\n",obj->signup.name);
    g_print("Account: %s\n",obj->signup.username);
    g_print("Password: %s\n",obj->signup.password);
    g_print("Confirm Password: %s\n",obj->signup.re_password);
    error = check_account(obj->signup.username);
    if(error == ERR_ACCOUNT){
        char message[100];
        error_to_string(error,message);
        markup_message = g_markup_printf_escaped(format_error,message);
        if(gtk_widget_is_visible(signup_in->lbl_err_account)){
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_account),markup_message);
            //gtk_label_set_text(GTK_LABEL(signup_in->lbl_err_account),message);
        }else{
            gtk_widget_set_visible(signup_in->lbl_err_account,TRUE);
            gtk_label_set_markup(GTK_LABEL(signup_in->lbl_err_account),markup_message);
            //gtk_label_set_text(GTK_LABEL(signup_in->lbl_err_account),message);
        }
        g_free(markup_message);
        
    }else{
 
        if(send(signup_in->sockfd,obj,sizeof(Object),0) < 0){
            perror("send- login");
            return;
        }
    }
    free_object(obj);

}

void on_signup_btn_cance_clicked(){
    
    gtk_window_close(GTK_WINDOW(window_signup));
}
