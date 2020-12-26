#include <gtk/gtk.h>

typedef struct {
    GtkWidget *entry_add_friend;
    GtkWidget *lbl_err_add_friend;
    //GtkWidget *btn_add_friend;
}w_list_friend;

GtkWidget *window_menu_friend;
GtkWidget *window_thong_bao;
GtkWidget *window_list_friend;
GtkWidget *window_confirm_friend;
int sockfd_friend;
int argc_friend;
char *argv_friend;

Object *obj_login_list_friend;

void dup_obj_list_friend(Object *obj){
    obj_login_list_friend = duplicate_object(obj);
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
//main
int thong_bao(int argc, char **argv)
{
    GtkBuilder      *builder_thong_bao; 
    gtk_init(&argc, &argv);

    builder_thong_bao = gtk_builder_new();
    gtk_builder_add_from_file (builder_thong_bao, "Glade/menu_list_friend.glade", NULL);

    window_thong_bao = GTK_WIDGET(gtk_builder_get_object(builder_thong_bao, "thong_bao"));
    gtk_builder_connect_signals(builder_thong_bao, NULL);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_thong_bao);

    gtk_widget_show(window_thong_bao);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_thong_bao_destroy()
{
    gtk_widget_set_visible(window_menu_friend,TRUE);
    gtk_main_quit();
}

//----------------List friend-----------------
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
    gtk_builder_connect_signals(builder_list_friend, list_friends);

    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_list_friend);

    gtk_widget_show(window_list_friend);                
    gtk_main();
    g_slice_free(w_list_friend,list_friends);
 
    return 0;
}

// called when window is closed
void on_list_friend_destroy()
{
    gtk_widget_set_visible(window_menu_friend,TRUE);
    gtk_main_quit();
}

void on_btn_add_friend_clicked(GtkButton *b, w_list_friend *w_l_f){
    const char *format_error = "<span foreground='red'>%s</span>";
    char *markup_message;
    Error err_add_friend;
    Object *obj = (Object*)malloc(sizeof(Object));
    obj->signal = SIGNAL_ADD_FRIEND;
    strcpy(obj->add_friend.username,gtk_entry_get_text(GTK_ENTRY(w_l_f->entry_add_friend)));
    strcpy(obj->add_friend.username_friend, obj_login_list_friend->login.username);
    gtk_widget_set_visible(w_l_f->lbl_err_add_friend,FALSE);
    if(strlen(obj->add_friend.username) == 0){
        char err_msg[100];
        err_add_friend = ERR_NULL_USERNAME;
        gtk_widget_set_visible(w_l_f->lbl_err_add_friend,TRUE);
        error_to_string(err_add_friend,err_msg);
        markup_message = g_markup_printf_escaped(format_error,err_msg);
        gtk_label_set_markup(GTK_LABEL(w_l_f->lbl_err_add_friend),markup_message);
        g_free(markup_message);
    }else{
        /*
        if(send(sockfd_friend,obj,sizeof(Object),0) < 0){
            perror("Err: listfriend");
            return;
        }*/
    }
    free(obj);
}

//----------confirm friend ------------
//main
int confirm_friend(int argc, char **argv)
{
    GtkBuilder      *builder_confirm_friend; 
    gtk_init(&argc, &argv);

    builder_confirm_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_confirm_friend, "Glade/menu_list_friend.glade", NULL);

    window_confirm_friend = GTK_WIDGET(gtk_builder_get_object(builder_confirm_friend, "confirm_friend"));
    gtk_builder_connect_signals(builder_confirm_friend, NULL);
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