
#include <gtk/gtk.h>
#include "../symtab/object_data.h"
#include "../symtab/error_invalid.h"

typedef struct {
    GtkWidget *change_pass_username;
    GtkWidget *new_pass;
    GtkWidget *re_pass;
    GtkWidget *cur_pass;

    GtkWidget *lbl_err_new_pass;
    GtkWidget *lbl_err_re_pass;
    GtkWidget *lbl_err_cur_pass;
    GtkWidget *lbl_invalid_change_pass;
}Change_Pass;

GtkWidget       *window_change_pass;
Object *obj_login;
int sockfd_change_pass;
gboolean check_change_pass = FALSE;

gboolean getCheckChangePass(){
    return check_change_pass;
}

void setCheckChangePass(){
    if(getCheckChangePass() == FALSE)
        check_change_pass = TRUE;
    else
        check_change_pass = FALSE;
}

void dup_obj_login(Object *obj, int sockfd){
    obj_login = duplicate_object(obj);
    sockfd_change_pass = sockfd;
}

int change_password(int argc, char **argv)
{
    GtkBuilder      *builder_change_pass; 
    Change_Pass *change_pass = g_slice_new(Change_Pass);
    gtk_init(&argc, &argv);

    builder_change_pass = gtk_builder_new();
    gtk_builder_add_from_file (builder_change_pass, "Glade/change_password.glade", NULL);

    window_change_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass, "change_password"));
    gtk_builder_connect_signals(builder_change_pass, change_pass);
    change_pass->change_pass_username = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"change_pass_username"));
    change_pass->new_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"change_pass_new_pass"));
    change_pass->re_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"change_pass_re_pass"));
    change_pass->cur_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"change_pass_cur_pass"));
 
    change_pass->lbl_err_new_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"lbl_err_new_pass"));
    change_pass->lbl_err_re_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"lbl_err_re_pass"));
    change_pass->lbl_err_cur_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"lbl_err_cur_pass"));
    change_pass->lbl_invalid_change_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"lbl_invalid_change_pass"));
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_menu),FALSE);
    gtk_entry_set_text(GTK_ENTRY(change_pass->change_pass_username),obj_login->login.username);
    //---
    g_object_unref(builder_change_pass);

    gtk_widget_show(window_change_pass);                
    gtk_main();
    g_slice_free(Change_Pass,change_pass);

    return 0;
}

// called when window is closed
void on_change_password_destroy()
{
    setCheckChangePass();
    gtk_main_quit();
}

void on_btn_submit_clicked(GtkButton *b, Change_Pass *change_pass){
    Object *data_change_pass = new_change_password_object();
    char re_pass[30];
    Error err_new_pass, err_re_pass, err_cur_pass;
    char *markup_message;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    const char *format_invalid = "<span foreground='green' weight='bold' font='13'>%s</span>";
    g_stpcpy(data_change_pass->change_password.username, gtk_entry_get_text(GTK_ENTRY(change_pass->change_pass_username)));
    g_stpcpy(data_change_pass->change_password.new_password, gtk_entry_get_text(GTK_ENTRY(change_pass->new_pass)));
    g_stpcpy(re_pass, gtk_entry_get_text(GTK_ENTRY(change_pass->re_pass)));
    g_stpcpy(data_change_pass->change_password.cur_password, gtk_entry_get_text(GTK_ENTRY(change_pass->cur_pass)));
    err_new_pass = check_signup_password(data_change_pass->change_password.new_password);
    err_re_pass = check_signup_re_password(re_pass,data_change_pass->change_password.new_password);
    err_cur_pass = check_current_password(data_change_pass->change_password.cur_password,obj_login->login.password);

    gtk_widget_set_visible(change_pass->lbl_err_new_pass,FALSE);
    gtk_widget_set_visible(change_pass->lbl_err_re_pass,FALSE);
    gtk_widget_set_visible(change_pass->lbl_err_cur_pass,FALSE);
    gtk_widget_set_visible(change_pass->lbl_invalid_change_pass,FALSE);

    if((err_new_pass == ERR_NONE) && (err_re_pass == ERR_NONE) && (err_cur_pass == ERR_NONE)){ 
        char message[100];  
        Invalid inval = OK_CHANGE_PASSWORD;
        if(send(sockfd_change_pass,data_change_pass,sizeof(Object),0) < 0){
            perror("send - change pass");
            return;
        }
        invalid_to_string(inval,message);
        g_stpcpy(obj_login->login.password,data_change_pass->change_password.new_password);
        markup_message = g_markup_printf_escaped(format_invalid,message);
        gtk_widget_set_visible(change_pass->lbl_invalid_change_pass,TRUE);
        gtk_label_set_markup(GTK_LABEL(change_pass->lbl_invalid_change_pass),markup_message);       
    }else{
        if(err_new_pass != ERR_NONE){
            char message[100];
            error_to_string(err_new_pass, message);
            markup_message = g_markup_printf_escaped(format_error,message);
            gtk_widget_set_visible(change_pass->lbl_err_new_pass,TRUE);
            gtk_label_set_markup(GTK_LABEL(change_pass->lbl_err_new_pass),markup_message);
        }
        if(err_re_pass != ERR_NONE){
            char message[100];
            error_to_string(err_re_pass, message);
            markup_message = g_markup_printf_escaped(format_error,message);
            gtk_widget_set_visible(change_pass->lbl_err_re_pass,TRUE);
            gtk_label_set_markup(GTK_LABEL(change_pass->lbl_err_re_pass),markup_message);
        }
        if(err_cur_pass != ERR_NONE){
            char message[100];
            error_to_string(err_cur_pass, message);
            markup_message = g_markup_printf_escaped(format_error,message);
            gtk_widget_set_visible(change_pass->lbl_err_cur_pass,TRUE);
            gtk_label_set_markup(GTK_LABEL(change_pass->lbl_err_cur_pass),markup_message);
        }
    }
    g_free(markup_message);
    free_object(data_change_pass);
}

void on_btn_cancel_clicked(){
    gtk_window_close(GTK_WINDOW(window_change_pass));
}