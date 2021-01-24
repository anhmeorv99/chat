
#include <gtk/gtk.h>


typedef struct{
    GtkWidget *scrol_user;
    GtkWidget *grid_user;  
    GtkWidget *lbl_null_user;
    GtkWidget *lbl_username[100];
    GtkWidget *btn_view[100];
    GtkWidget *btn_update[100];
    GtkWidget *btn_delete[100];
}admin_user;

GtkWidget       *window_admin;
int sock_admin;
int argc_admin;
char *argv_admin;

int admin(int argc, char **argv)
{
    GtkBuilder      *builder_admin; 
    
    admin_user *ad = g_slice_new(admin_user);
    
    gtk_init(&argc, &argv);

    builder_admin = gtk_builder_new();
    gtk_builder_add_from_file (builder_admin, "Glade/admin.glade", NULL);

    window_admin = GTK_WIDGET(gtk_builder_get_object(builder_admin, "window_admin"));
    gtk_builder_connect_signals(builder_admin, NULL);
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_login),FALSE);
    ad->scrol_user = GTK_WIDGET(gtk_builder_get_object(builder_admin,"scrol_user"));
    ad->grid_user = GTK_WIDGET(gtk_builder_get_object(builder_admin,"grid_user"));
    ad->lbl_null_user = GTK_WIDGET(gtk_builder_get_object(builder_admin,
        "lbl_null_user"));
    
    //---
    int i;
    for(i = 0; i< 3; i++){
        int column = 1;
        ad->lbl_username[i] = gtk_label_new("dai.vn1");
        gtk_widget_set_size_request (ad->lbl_username[i],250,40);
        // gtk_label_set_text(GTK_LABEL(ad->lbl_username),"dai.vn1");
        
        ad->btn_view[i] = gtk_button_new_with_label("VIEW");
        gtk_widget_set_size_request (ad->btn_view[i],80,40);
        gtk_widget_set_name(ad->btn_view[i],"xem");

        ad->btn_update[i] = gtk_button_new_with_label("UPDATE");
        gtk_widget_set_size_request (ad->btn_update[i],80,40);
        gtk_widget_set_name(ad->btn_update[i],"cap nhat");

        ad->btn_delete[i] = gtk_button_new_with_label("DELETE");
        gtk_widget_set_size_request (ad->btn_delete[i],80,40);
        gtk_widget_set_name(ad->btn_delete[i],"xoa");

        gtk_grid_insert_row(GTK_GRID(ad->grid_user),i);
        gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_username[i],column,i,1,1);
        column++;
        gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_view[i],column,i,1,1);
        column++;
        gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_update[i],column,i,1,1);
        column++;
        gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_delete[i],column,i,1,1);

    }
    //
   
    //
    g_object_unref(builder_admin);

    gtk_widget_show(window_admin); 
    gtk_widget_show_all(ad->scrol_user);               
    gtk_main();

    g_slice_free(admin_user,ad);

    return 0;
}

// called when window is closed
void on_window_admin_destroy()
{
    gtk_main_quit();
    //gtk_widget_set_visible(window,TRUE);
}



