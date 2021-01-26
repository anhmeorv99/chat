#include <gtk/gtk.h>

#include "info_friend.h"

typedef struct {
    GtkWidget *scrol_thong_bao;
    GtkWidget *grid_thong_bao;
    GtkWidget *btn_thong_bao[50];
    GtkWidget *lbl_null_thong_bao;
}w_thong_bao;
GtkWidget *window_menu_friend;
GtkWidget *window_thong_bao;

GtkWidget *window_confirm_friend;
int sockfd_friend;
int argc_friend;
char *argv_friend;
int id_confirm_friend;
int row_thong_bao;



gboolean check_menu_list_friend = FALSE;

gboolean getCheckMenuFriend(){
    return check_menu_list_friend;
}

void setCheckMenuFriend(){
    if(getCheckMenuFriend() == FALSE)
        check_menu_list_friend = TRUE;
    else
        check_menu_list_friend = FALSE;
}


int thong_bao(int argc, char **argv);
int list_friend(int argc, char **argv);
//---------------Menu list friend------------------
//main
int menu_list_friend(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder_menu_friend; 
    sockfd_friend = sockfd;
   	argc_friend = argc;
   	argv_friend = *argv;
    gtk_init(&argc, &argv);

    builder_menu_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_menu_friend, "Glade/menu_list_friend.glade", NULL);

    window_menu_friend = GTK_WIDGET(gtk_builder_get_object(builder_menu_friend, "menu_list_friend"));
    gtk_builder_connect_signals(builder_menu_friend, NULL);
    //--
    obj_login_list_friend->signal = SIGNAL_RECV_LIST_FRIEND;
    if(send(sockfd_friend,obj_login_list_friend,sizeof(Object), 0) < 0){
        perror("send - list friend");
        exit(0);
    }
    db_list_friend = (Data_base*)malloc(sizeof(Data_base));

  
        if(recv(sockfd_friend,db_list_friend,sizeof(Data_base), MSG_WAITALL) < 0){
            perror("recv - list friend");
            exit(0);
        }
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_menu_friend);

    gtk_widget_show(window_menu_friend);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_menu_list_friend_destroy()
{
    setCheckMenuFriend();
    gtk_main_quit();
}

void on_btn_thong_bao_clicked(){
    //gtk_window_close(GTK_WINDOW(window_menu_friend));
    gtk_widget_set_visible(window_menu_friend,FALSE);
    thong_bao(argc_friend,&argv_friend);
}

void on_btn_list_friend_clicked(){
    //gtk_window_close(GTK_WINDOW(window_menu_friend));
    gtk_widget_set_visible(window_menu_friend,FALSE);
   
    list_friend(argc_friend,&argv_friend);
  
    
}

//------------thong bao------------------
int confirm_friend(int argc, char **argv, w_thong_bao *thong_bao,int id_confirm_friend_ok);
void on_btn_thong_bao(GtkButton *b, w_thong_bao *thong_bao){
    gtk_widget_set_sensitive (window_thong_bao, FALSE);
    confirm_friend(argc_friend,&argv_friend,thong_bao,atoi(gtk_widget_get_name(GTK_WIDGET(b))));
}
//main
int thong_bao(int argc, char **argv)
{
    int i;
    
    GtkBuilder      *builder_thong_bao; 
    gtk_init(&argc, &argv);
    w_thong_bao *thong_bao = g_slice_new(w_thong_bao);
    builder_thong_bao = gtk_builder_new();
    gtk_builder_add_from_file (builder_thong_bao, "Glade/menu_list_friend.glade", NULL);

    window_thong_bao = GTK_WIDGET(gtk_builder_get_object(builder_thong_bao, "thong_bao"));
    thong_bao->grid_thong_bao = GTK_WIDGET(gtk_builder_get_object(builder_thong_bao, "grid_thong_bao"));
    thong_bao->scrol_thong_bao = GTK_WIDGET(gtk_builder_get_object(builder_thong_bao, "scrol_thong_bao"));
    thong_bao->lbl_null_thong_bao = GTK_WIDGET(gtk_builder_get_object(builder_thong_bao, "lbl_null_thong_bao"));
    gtk_builder_connect_signals(builder_thong_bao, NULL);
    //--
    
    if (db_list_friend->signal == SIGNAL_DB_LIST_FRIEND){
            if(db_list_friend->list_friend.length_list_friend == 0){
                gtk_widget_set_visible(thong_bao->scrol_thong_bao, FALSE);
            }
            else{
                char id_thong_bao[5];
                row_thong_bao = 0;
                for(i = 0; i < db_list_friend->list_friend.length_list_friend; i++){
                    if(db_list_friend->list_friend.list_friend[i].confirm == 0){
                        sprintf(id_thong_bao,"%d",db_list_friend->list_friend.list_friend[i].ID);
                        thong_bao->btn_thong_bao[row_thong_bao] = gtk_button_new_with_label(db_list_friend->list_friend.list_friend[i].name);
                        gtk_widget_set_name(thong_bao->btn_thong_bao[row_thong_bao], id_thong_bao);
                        g_signal_connect(thong_bao->btn_thong_bao[row_thong_bao],"clicked",G_CALLBACK(on_btn_thong_bao),thong_bao);
                        gtk_grid_insert_row(GTK_GRID(thong_bao->grid_thong_bao),row_thong_bao);
                        gtk_grid_attach(GTK_GRID(thong_bao->grid_thong_bao),thong_bao->btn_thong_bao[row_thong_bao],1,row_thong_bao,1,1);
                        row_thong_bao++;
                    }
                    
                }
                if(row_thong_bao == 0){
                    gtk_widget_set_visible(thong_bao->scrol_thong_bao, FALSE);
                }else{
                    gtk_widget_set_visible(thong_bao->lbl_null_thong_bao, FALSE);
                    gtk_widget_show_all(thong_bao->scrol_thong_bao);
                }
                    
            
            }

        }
    
    //--
    g_object_unref(builder_thong_bao);

    gtk_widget_show(window_thong_bao);                
    gtk_main();

    g_slice_free(w_thong_bao, thong_bao);
    return 0;
}

// called when window is closed
void on_thong_bao_destroy()
{
    gtk_widget_set_visible(window_menu_friend,TRUE);
    gtk_main_quit();
}

//----------------List friend-----------------
void on_btn_list_friend(GtkButton *button,w_list_friend *list_friend);

//main
int list_friend(int argc, char **argv)
{
    GtkBuilder      *builder_list_friend; 
    gtk_init(&argc, &argv);
    w_list_friend *list_friends = g_slice_new(w_list_friend);
    builder_list_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_list_friend, "Glade/menu_list_friend.glade", NULL);

    window_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend, "list_friend"));

    list_friends->entry_add_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend,
                "entry_add_friend"));
    list_friends->lbl_err_add_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend,"lbl_err_add_friend"));
    list_friends->scrol_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend,"scrol_list_friend"));
    list_friends->grid_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend,"grid_list_friend"));
    list_friends->lbl_null_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend,"lbl_null_list_friend"));
    gtk_builder_connect_signals(builder_list_friend, list_friends);
    
    //--
    //Object *obj_list_friend = (Object*)malloc(sizeof(Object));
    
    
        if (db_list_friend->signal == SIGNAL_DB_LIST_FRIEND){
            if(db_list_friend->list_friend.length_list_friend == 0){
                gtk_widget_set_visible(list_friends->scrol_list_friend, FALSE);
                
            }
            else{
                int i;
                char id_friend[5];
                row_list_friend = 0;
                for(i = 0; i < db_list_friend->list_friend.length_list_friend; i++){
                    if(db_list_friend->list_friend.list_friend[i].confirm == 1){
                        sprintf(id_friend,"%d",db_list_friend->list_friend.list_friend[i].ID);
                        list_friends->btn_list_friend[row_list_friend] = gtk_button_new_with_label(db_list_friend->list_friend.list_friend[i].name);
                        gtk_widget_set_name(list_friends->btn_list_friend[row_list_friend],id_friend);
                        g_signal_connect(list_friends->btn_list_friend[row_list_friend],"clicked",G_CALLBACK(on_btn_list_friend),list_friends);
                        gtk_grid_insert_row(GTK_GRID(list_friends->grid_list_friend),row_list_friend);
                        gtk_grid_attach(GTK_GRID(list_friends->grid_list_friend),list_friends->btn_list_friend[row_list_friend],1,row_list_friend,1,1);
                        row_list_friend++;
                    }
                    
                }
                if(row_list_friend == 0){
                     gtk_widget_set_visible(list_friends->scrol_list_friend, FALSE);
                }else{
                    gtk_widget_set_visible(list_friends->lbl_null_list_friend, FALSE);
                    gtk_widget_show_all(list_friends->scrol_list_friend);
                }
                    
            
            }

        }

  
   
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_list_friend);

    gtk_widget_show(window_list_friend);                
    gtk_main();
    g_slice_free(w_list_friend,list_friends);
  
    return 0;
}

void on_btn_list_friend(GtkButton *button,w_list_friend *list_friend){
    g_print("id: %d\tname = %s , username = %s\n",atoi(gtk_widget_get_name(GTK_WIDGET(button))),
    gtk_button_get_label(button),
                 gtk_widget_get_name(GTK_WIDGET(button)));
    gtk_widget_set_sensitive (window_list_friend, FALSE);
    info_friend(argc_friend,&argv_friend,sockfd_friend,atoi(gtk_widget_get_name(GTK_WIDGET(button))),list_friend);
}

// called when window is closed
void on_list_friend_destroy()
{
    gtk_widget_set_visible(window_menu_friend,TRUE);
    gtk_main_quit();
}

void on_btn_add_friend_clicked(GtkButton *b, w_list_friend *w_l_f){
    const char *format_error = "<span foreground='red'>%s</span>";
    const char *format_invalid = "<span foreground='blue'>%s</span>";
    char *markup_message;
    Error err_add_friend;
    
    gtk_widget_set_visible(w_l_f->lbl_err_add_friend,FALSE);
    if(strlen(gtk_entry_get_text(GTK_ENTRY(w_l_f->entry_add_friend))) == 0){
        char err_msg[100];
        err_add_friend = ERR_NULL_USERNAME;
        gtk_widget_set_visible(w_l_f->lbl_err_add_friend,TRUE);
        error_to_string(err_add_friend,err_msg);
        markup_message = g_markup_printf_escaped(format_error,err_msg);
        gtk_label_set_markup(GTK_LABEL(w_l_f->lbl_err_add_friend),markup_message);
        g_free(markup_message);
    }else{
        strcpy(obj_login_list_friend->add_friend.username_friend, gtk_entry_get_text(GTK_ENTRY(w_l_f->entry_add_friend)));
        obj_login_list_friend->signal =SIGNAL_ADD_FRIEND;
    
        if(send(sockfd_friend,obj_login_list_friend,sizeof(Object),0) < 0){
            perror("Err: listfriend");
           exit(0);
        }
        Object *obj_add_friend = (Object*)malloc(sizeof(Object));
        if(recv(sockfd_friend, obj_add_friend, sizeof(Object), 0) < 0){
            perror(" recv add friend");
            exit(0);
        }
        if(obj_add_friend->signal == SIGNAL_ADD_FRIEND){
            if(obj_add_friend->add_friend.err == ERR_NONE){
                char msg[] = "Da gui yeu cau ket ban.";
                gtk_widget_set_visible(w_l_f->lbl_err_add_friend,TRUE);
                markup_message = g_markup_printf_escaped(format_invalid,msg);
                gtk_label_set_markup(GTK_LABEL(w_l_f->lbl_err_add_friend),markup_message);
                g_free(markup_message);
            }else{
                char err_msg[100];
                gtk_widget_set_visible(w_l_f->lbl_err_add_friend,TRUE);
                error_to_string(obj_add_friend->add_friend.err,err_msg);
                markup_message = g_markup_printf_escaped(format_error,err_msg);
                gtk_label_set_markup(GTK_LABEL(w_l_f->lbl_err_add_friend),markup_message);
                g_free(markup_message);
            }
        }
    }
}

//----------confirm friend ------------
//main
int confirm_friend(int argc, char **argv, w_thong_bao *thong_bao,int id_confirm_friend_ok)
{
    GtkBuilder      *builder_confirm_friend; 
    gtk_init(&argc, &argv);
    id_confirm_friend = id_confirm_friend_ok;
    builder_confirm_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_confirm_friend, "Glade/menu_list_friend.glade", NULL);

    window_confirm_friend = GTK_WIDGET(gtk_builder_get_object(builder_confirm_friend, "confirm_friend"));
    gtk_builder_connect_signals(builder_confirm_friend, thong_bao);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_confirm_friend);

    gtk_widget_show(window_confirm_friend);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_confirm_friend_destroy()
{
    gtk_widget_set_sensitive(window_thong_bao,TRUE);
    gtk_main_quit();
}

void on_btn_chap_nhan_clicked(GtkButton *b, w_thong_bao *thongbao){
    int i;
    for(i = 0; i < db_list_friend->list_friend.length_list_friend; i++){
        if(db_list_friend->list_friend.list_friend[i].ID == id_confirm_friend ){
            db_list_friend->list_friend.list_friend[i].confirm = 1;
            break;
        }
    }
    for(i = 0;i < row_thong_bao; i++){
        if(id_confirm_friend == atoi(gtk_widget_get_name(thongbao->btn_thong_bao[i]))){
            gtk_widget_set_visible(thongbao->btn_thong_bao[i],FALSE);
            
            obj_login_list_friend->add_member.ID = id_confirm_friend;
            obj_login_list_friend->signal = SIGNAL_CONFIRM_FRIEND;
            row_thong_bao--;
            if(row_thong_bao == 0){
                gtk_widget_set_visible(thongbao->lbl_null_thong_bao,TRUE);
                gtk_widget_set_visible(thongbao->scrol_thong_bao, FALSE);
            }
            if(send(sockfd_friend, obj_login_list_friend, sizeof(Object), 0) < 0){
                perror("send - confirm friend");
                exit(0);
            }
            break;
        }
    }
    gtk_widget_set_sensitive (window_thong_bao, TRUE);
    gtk_window_close(GTK_WINDOW(window_confirm_friend));
}

void on_btn_khong_chap_nhan_clicked(GtkButton *b, w_thong_bao *thongbao){
    int i;
    delete_btn_friend(id_confirm_friend);
    for(i = 0;i < row_thong_bao; i++){
        if(id_confirm_friend == atoi(gtk_widget_get_name(thongbao->btn_thong_bao[i]))){
            gtk_widget_set_visible(thongbao->btn_thong_bao[i],FALSE);
            obj_login_list_friend->add_member.ID = id_confirm_friend;
            obj_login_list_friend->signal = SIGNAL_NO_CONFIRM_FRIEND;
            if(send(sockfd_friend, obj_login_list_friend, sizeof(Object), 0) < 0){
                perror("send - confirm friend");
                exit(0);
            }
            break;
        }
    }
    gtk_widget_set_sensitive (window_thong_bao, TRUE);
    gtk_window_close(GTK_WINDOW(window_confirm_friend));
}