
#include <gtk/gtk.h>
#include "admin.h"

typedef struct{
    GtkWidget *info_email;
    GtkWidget *info_name;  
    GtkWidget *info_status;
 
}friend_info;

typedef struct {
    GtkWidget *entry_add_friend;
    GtkWidget *lbl_err_add_friend;
    GtkWidget *grid_list_friend;
    GtkWidget *scrol_list_friend;
    GtkWidget *lbl_null_list_friend;
    GtkWidget *btn_list_friend[100];
    //GtkWidget *btn_add_friend;
}w_list_friend;

GtkWidget *window_list_friend;

Data_base *db_list_friend;
Object *obj_login_list_friend;
GtkWidget       *window_info_friend;
int sock_info_friend;
int argc_info_friend;
char *argv_info_friend;
int id_info_friend;
int row_list_friend;
void dup_obj_list_friend(Object *obj){
    obj_login_list_friend = duplicate_object(obj);
}

int info_friend(int argc, char **argv,int sock, int id_friend , w_list_friend *list_friend)
{
    GtkBuilder      *builder_info_friend; 
    
    friend_info *info = g_slice_new(friend_info);
    id_info_friend = id_friend;
    argc_info_friend = argc;
    argv_info_friend = *argv;
    sock_info_friend = sock;
    gtk_init(&argc, &argv);

    builder_info_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_info_friend, "Glade/info_friend.glade", NULL);

    window_info_friend = GTK_WIDGET(gtk_builder_get_object(builder_info_friend, "window_info_friend"));
    gtk_builder_connect_signals(builder_info_friend, list_friend);
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_login),FALSE);
    info->info_email = GTK_WIDGET(gtk_builder_get_object(builder_info_friend,"info_email"));
    info->info_name = GTK_WIDGET(gtk_builder_get_object(builder_info_friend,"info_name"));
    info->info_status = GTK_WIDGET(gtk_builder_get_object(builder_info_friend,
        "info_status"));
    
    //---

    //
    int i;
    for(i =0 ; i< db_list_friend->list_friend.length_list_friend; i++){
        if(id_friend == db_list_friend->list_friend.list_friend[i].ID){
            gtk_label_set_text(GTK_LABEL(info->info_email),db_list_friend->list_friend.list_friend[i].username);
            gtk_label_set_text(GTK_LABEL(info->info_name),db_list_friend->list_friend.list_friend[i].name);
            if(db_list_friend->list_friend.list_friend[i].loginStatus == 1){
                gtk_label_set_text(GTK_LABEL(info->info_status),"Online");
            }else{
                gtk_label_set_text(GTK_LABEL(info->info_status),"Offline");
            }
            break;
        }
    }
    //
    g_object_unref(builder_info_friend);

    gtk_widget_show(window_info_friend);                
    gtk_main();

    g_slice_free(friend_info,info);

    return 0;
}

// called when window is closed
void on_window_info_friend_destroy()
{
    gtk_widget_set_sensitive (window_list_friend, TRUE);
    gtk_main_quit();
    
}

void delete_btn_friend(int id){
    int i;
    for (i = 0; i < db_list_friend->list_friend.length_list_friend; i++){
        if(db_list_friend->list_friend.list_friend[i].ID == id){
            int j;
            for(j = i + 1; j < db_list_friend->list_friend.length_list_friend; j++){
                db_list_friend->list_friend.list_friend[j-1] = db_list_friend->list_friend.list_friend[j];
            }
            db_list_friend->list_friend.length_list_friend--;
            break;
        }
    }
}

void on_btn_delete_friend_clicked(GtkButton *b, w_list_friend *list_friend){
    int i;
    delete_btn_friend(id_info_friend);
    for(i = 0; i < row_list_friend ; i++){
        if(id_info_friend == atoi(gtk_widget_get_name(list_friend->btn_list_friend[i]))){
            gtk_widget_set_visible(list_friend->btn_list_friend[i],FALSE);
            obj_login_list_friend->add_member.ID = id_info_friend;
            obj_login_list_friend->signal = SIGNAL_NO_CONFIRM_FRIEND;
            if(send(sock_info_friend, obj_login_list_friend, sizeof(Object), 0) < 0){
                perror("send - confirm friend");
                exit(0);
            }
            break;
        }
    }
    gtk_window_close(GTK_WINDOW(window_info_friend));
}

void on_btn_cancel_info_clicked(){
    gtk_window_close(GTK_WINDOW(window_info_friend));
}

