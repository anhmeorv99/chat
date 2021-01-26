#include "../symtab/jsonapi.h"
#include <gtk/gtk.h>


typedef struct{
    GtkWidget *scrol_user;
    GtkWidget *grid_user;  
    GtkWidget *lbl_null_user;
    GtkWidget *lbl_name_stt;
    GtkWidget *lbl_stt[50];
    GtkWidget *lbl_name_name;
    GtkWidget *lbl_name[50];
    GtkWidget *lbl_name_username;
    GtkWidget *lbl_username[50];
    GtkWidget *lbl_name_status;
    GtkWidget *lbl_status[50];
    GtkWidget *lbl_name_tinh_nang;
    GtkWidget *lbl_name_created_at;
    GtkWidget *lbl_created_at[50];
    GtkWidget *btn_view[50];
    GtkWidget *btn_update[50];
    GtkWidget *btn_delete[50];
}admin_user;

typedef struct {
    GtkWidget *entry_name;
   GtkWidget *entry_username;
   GtkWidget *entry_password;
   GtkWidget *lbl_err_name;
   GtkWidget *lbl_err_password;
}update_admin_ok;

GtkWidget *window_update_admin;
GtkWidget       *window_admin;
int sock_admin;
int argc_admin;
char *argv_admin;
int row_list_user;
Object *obj_admin;
int id_update_user;
admin_user *ad;
Data_base_user *user;


gboolean check_admin = FALSE;
gboolean check_update_admin = FALSE;

gboolean getCheckAdmin(){
    return check_admin;
}

gboolean getCheckUpdateAdmin(){
    return check_update_admin;
}

void setCheckAdmin(){
    if(getCheckAdmin() == FALSE)
        check_admin = TRUE;
    else
        check_admin = FALSE;
}

void setCheckUpdateAdmin(){
    if(getCheckUpdateAdmin() == FALSE)
        check_update_admin = TRUE;
    else
        check_update_admin = FALSE;
}


void dup_obj_admin(Object *obj){
    obj_admin = duplicate_object(obj);
}
int update_admin_user(int argc, char **argv,int id);
char * convert_timestamp_to_date_admin(char* timestamp){
    time_t rawtime = (time_t)atoi(timestamp);
    struct tm  ts;
    char  *buf = (char*)malloc(100*sizeof(char));
                        // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, 100*sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    return buf;
}


void on_btn_update_admin(GtkButton *b, admin_user *ad){
    // gtk_widget_set_sensitive(window_admin,FALSE);
    if(getCheckUpdateAdmin()== FALSE){
        setCheckUpdateAdmin();
        update_admin_user(argc_admin,&argv_admin,atoi(gtk_widget_get_name(GTK_WIDGET(b))));
    }
    
}


int admin(int argc, char **argv,int sock)
{
    GtkBuilder      *builder_admin; 
    
    ad = g_slice_new(admin_user);
    sock_admin = sock;
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
    obj_admin->signal = SIGNAL_ADMIN;
    if(send(sock,obj_admin,sizeof(Object),0) < 0){
        perror("send admin");
        exit(0);
    }
    user = (Data_base_user*)malloc(sizeof(Data_base_user));
    if(recv(sock,user,sizeof(Data_base_user), 0) < 0){
        perror("recv admin");
        exit(0);
    }
    if(user->signal == DB_ADMIN){
        if(user->Users.length_user > 0){
            int i;
            int column = 1;
            row_list_user = 0;
            ad->lbl_name_stt = gtk_label_new("STT");
            gtk_widget_set_size_request (ad->lbl_name_stt,40,40);

            ad->lbl_name_name = gtk_label_new("Ten");
            gtk_widget_set_size_request (ad->lbl_name_name,100,40);

            ad->lbl_name_username = gtk_label_new("Tai khoan");
            gtk_widget_set_size_request (ad->lbl_name_username,100,40);

            ad->lbl_name_status = gtk_label_new("Trang thai");
            gtk_widget_set_size_request (ad->lbl_name_status,100,40);

            ad->lbl_name_created_at = gtk_label_new("Ngay tao");
            gtk_widget_set_size_request (ad->lbl_name_created_at,100,40);

            ad->lbl_name_tinh_nang = gtk_label_new("Chuc nang");
            gtk_widget_set_size_request (ad->lbl_name_status,100,40);
            //them element 
                    gtk_grid_insert_row(GTK_GRID(ad->grid_user),row_list_user);
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_stt,column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_name,column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_username,column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_status,column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_created_at,column,row_list_user,1,1);
                    column++;
               
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name_tinh_nang,column,row_list_user,1,1);
                    
            row_list_user++;
            for(i = 0; i < user->Users.length_user;i++){
                if(user->Users.user[i].is_admin != 1){
                    column = 1;
                    char *stt = (char*)malloc(5*sizeof(char));
                    char *id_user = (char*)malloc(5*sizeof(char));
                    sprintf(stt,"%d",row_list_user);
                    sprintf(id_user,"%d",user->Users.user[i].ID_user);
                    ad->lbl_stt[row_list_user] = gtk_label_new(stt);
                    gtk_widget_set_size_request (ad->lbl_stt[row_list_user],40,40);

                    ad->lbl_name[row_list_user] = gtk_label_new(user->Users.user[i].name);
                    gtk_widget_set_name(ad->lbl_name[row_list_user],id_user);
                    gtk_widget_set_size_request (ad->lbl_name[row_list_user],100,40);

                    ad->lbl_username[row_list_user] = gtk_label_new(user->Users.user[i].username);
                    gtk_widget_set_size_request (ad->lbl_username[row_list_user],100,40);

                    if(user->Users.user[i].login_status == 1){
                        ad->lbl_status[row_list_user] = gtk_label_new("Online");   
                    }else{
                        ad->lbl_status[row_list_user] = gtk_label_new("Offline");
                    }
                    gtk_widget_set_size_request (ad->lbl_username[row_list_user],100,40);


                    ad->lbl_created_at[row_list_user] = gtk_label_new(convert_timestamp_to_date_admin(user->Users.user[i].created_at));
                    gtk_widget_set_size_request (ad->lbl_created_at[row_list_user],100,40);
                    //btn

                    
                    // ad->btn_view[row_list_user] = gtk_button_new_with_label("VIEW");
                    // gtk_widget_set_size_request (ad->btn_view[row_list_user],80,40);
                    // gtk_widget_set_name(ad->btn_view[row_list_user],id_user);

                    ad->btn_update[row_list_user] = gtk_button_new_with_label("UPDATE");
                    gtk_widget_set_size_request (ad->btn_update[row_list_user],80,40);
                    gtk_widget_set_name(ad->btn_update[row_list_user],id_user);
                    g_signal_connect(ad->btn_update[row_list_user],"clicked",G_CALLBACK(on_btn_update_admin),ad);
                    // ad->btn_delete[row_list_user] = gtk_button_new_with_label("DELETE");
                    // gtk_widget_set_size_request (ad->btn_delete[row_list_user],80,40);
                    // gtk_widget_set_name(ad->btn_delete[row_list_user],id_user);

                    //them element 
                    gtk_grid_insert_row(GTK_GRID(ad->grid_user),row_list_user);
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_stt[row_list_user],column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_name[row_list_user],column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_username[row_list_user],column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_status[row_list_user],column,row_list_user,1,1);
                    column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->lbl_created_at[row_list_user],column,row_list_user,1,1);
                    column++;
               
                    // gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_view[row_list_user],column,row_list_user,1,1);
                    // column++;
                    gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_update[row_list_user],column,row_list_user,1,1);
                    // column++;
                    // gtk_grid_attach(GTK_GRID(ad->grid_user),ad->btn_delete[row_list_user],column,row_list_user,1,1);
                    row_list_user++;
                }
                
            }
            gtk_widget_show_all(ad->scrol_user);
            gtk_widget_set_visible(ad->lbl_null_user,FALSE);
        }
        else{
            gtk_widget_set_visible(ad->scrol_user, FALSE);
        }

    }
    
   
    //
    g_object_unref(builder_admin);

    gtk_widget_show(window_admin); 
                   
    gtk_main();

    g_slice_free(admin_user,ad);

    return 0;
}

// called when window is closed
void on_window_admin_destroy()
{
    if(getCheckUpdateAdmin() == TRUE){
        gtk_window_close(GTK_WINDOW(window_update_admin));
        
    }
    setCheckAdmin();
    gtk_main_quit();
    //gtk_widget_set_visible(window,TRUE);
}




///-------------------update

int update_admin_user(int argc, char **argv,int id){
    GtkBuilder      *builder_update_admin; 
    
    id_update_user = id;
   
    gtk_init(&argc, &argv);
    update_admin_ok *update_ad = g_slice_new(update_admin_ok);
    builder_update_admin = gtk_builder_new();
    gtk_builder_add_from_file (builder_update_admin, "Glade/admin.glade", NULL);

    window_update_admin = GTK_WIDGET(gtk_builder_get_object(builder_update_admin, "window_update_admin"));
    gtk_builder_connect_signals(builder_update_admin, update_ad);
    //----
    //gtk_window_set_decorated(GTK_WINDOW(window_login),FALSE);
    update_ad->entry_name = GTK_WIDGET(gtk_builder_get_object(builder_update_admin,"entry_name"));
    update_ad->entry_username = GTK_WIDGET(gtk_builder_get_object(builder_update_admin,"entry_username"));
    update_ad->entry_password = GTK_WIDGET(gtk_builder_get_object(builder_update_admin,
        "entry_password"));
    update_ad->lbl_err_name = GTK_WIDGET(gtk_builder_get_object(builder_update_admin,"lbl_err_name"));
    update_ad->lbl_err_password = GTK_WIDGET(gtk_builder_get_object(builder_update_admin,"lbl_err_password"));
    
    //---
    
    int i;
    for(i = 0 ; i  < user->Users.length_user; i++){
        if(id == user->Users.user[i].ID_user){
            gtk_entry_set_text(GTK_ENTRY(update_ad->entry_name), user->Users.user[i].name);
            gtk_entry_set_text(GTK_ENTRY(update_ad->entry_username), user->Users.user[i].username);
            gtk_entry_set_text(GTK_ENTRY(update_ad->entry_password), user->Users.user[i].password);
            gtk_widget_set_visible(update_ad->lbl_err_name, FALSE);
            gtk_widget_set_visible(update_ad->lbl_err_password, FALSE);
            break;
        }
    }
   
    //
    g_object_unref(builder_update_admin);

    gtk_widget_show(window_update_admin); 
                   
    gtk_main();
    g_slice_free(update_admin_ok,update_ad);
   

    return 0;
}

void on_window_update_admin_destroy(){

    setCheckUpdateAdmin();

    gtk_main_quit();
    gtk_widget_set_sensitive(window_admin,TRUE);
}

void on_btn_ok_clicked(GtkButton *b, update_admin_ok *update_ad){
    char *name = (char*)malloc(30*sizeof(char));
    char *password = (char*)malloc(30*sizeof(char));
    char *markup_message;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    const char *format_invalid = "<span foreground='green' weight='bold' font='13'>%s</span>";
    Error err_name, err_password;
    strcpy(name,gtk_entry_get_text(GTK_ENTRY(update_ad->entry_name)));
    strcpy(password , gtk_entry_get_text(GTK_ENTRY(update_ad->entry_password)));
    err_name = check_signup_name(name);
    err_password = check_signup_password(password);

    if(err_name == ERR_NONE && err_password == ERR_NONE){
        char message[] = "Update thanh cong";
        int i;  
            markup_message = g_markup_printf_escaped(format_invalid,message);
            if(gtk_widget_is_visible(update_ad->lbl_err_name) == FALSE){
                gtk_widget_set_visible(update_ad->lbl_err_name,TRUE);
            }             
            gtk_label_set_markup(GTK_LABEL(update_ad->lbl_err_name),markup_message);
            Object *obj_update_user = (Object*)malloc(sizeof(Object));
            strcpy(obj_update_user->login.name, name);
            strcpy(obj_update_user->login.password, password);
            obj_update_user->login.id = id_update_user;
            obj_update_user->signal = SIGNAL_UPDATE_ADMIN;
            if(send(sock_admin, obj_update_user, sizeof(Object), 0) < 0){
                perror("send update admin");
                exit(0);
            }
        for(i = 1; i < row_list_user ; i++){
            if(id_update_user == atoi(gtk_widget_get_name(ad->lbl_name[i]))){
                gtk_label_set_text(GTK_LABEL(ad->lbl_name[i]), name);
                break;
            }
        }
        for(i = 0; i < user->Users.length_user; i++){
            if(id_update_user == user->Users.user[i].ID_user){
                strcpy(user->Users.user[i].password, password);
                strcpy(user->Users.user[i].name, name);
                break;
            }
        }

    }else{
        if(err_name != ERR_NONE){
            char message[100];
            error_to_string(err_name,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(update_ad->lbl_err_name) == FALSE){
                gtk_widget_set_visible(update_ad->lbl_err_name,TRUE);
            }             
            gtk_label_set_markup(GTK_LABEL(update_ad->lbl_err_name),markup_message);
        }
        if(err_password != ERR_NONE){
            char message[100];
            error_to_string(err_password,message);
            markup_message = g_markup_printf_escaped(format_error,message);
            if(gtk_widget_is_visible(update_ad->lbl_err_password) == FALSE){
                gtk_widget_set_visible(update_ad->lbl_err_password,TRUE);
            }             
            gtk_label_set_markup(GTK_LABEL(update_ad->lbl_err_password),markup_message);
        }
    }
}

void on_btn_cancel_update_clicked(){
    gtk_window_close(GTK_WINDOW(window_update_admin));
}