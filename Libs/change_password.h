
#include <gtk/gtk.h>
#include "../symtab/object_data.h"
#include "../symtab/error_invalid.h"

GtkWidget       *window_change_pass;
Object *obj_login;
GtkWidget *change_pass_account;

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

void dup_obj_login(Object *obj){
    obj_login = duplicate_object(obj);
}

int change_password(int argc, char **argv)
{
    GtkBuilder      *builder_change_pass; 
    
    gtk_init(&argc, &argv);

    builder_change_pass = gtk_builder_new();
    gtk_builder_add_from_file (builder_change_pass, "Glade/menu_chat.glade", NULL);

    window_change_pass = GTK_WIDGET(gtk_builder_get_object(builder_change_pass, "change_password"));
    gtk_builder_connect_signals(builder_change_pass, NULL);
    change_pass_account = GTK_WIDGET(gtk_builder_get_object(builder_change_pass,"change_pass_account"));
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_menu),FALSE);
    gtk_entry_set_text(GTK_ENTRY(change_pass_account),obj_login->login.username);
    //---
    g_object_unref(builder_change_pass);

    gtk_widget_show(window_change_pass);                
    gtk_main();

    return 0;
}

// called when window is closed
void on_change_password_destroy()
{
    setCheckChangePass();
    gtk_main_quit();
}

