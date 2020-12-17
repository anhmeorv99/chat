#include <gtk/gtk.h>

GtkWidget *window_menu_friend;
GtkWidget *window_thong_bao;
GtkWidget *window_list_friend;
GtkWidget *window_confirm_friend;
int sockfd_friend;
int argc_friend;
char *argv_friend;

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

    builder_list_friend = gtk_builder_new();
    gtk_builder_add_from_file (builder_list_friend, "Glade/menu_list_friend.glade", NULL);

    window_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_list_friend, "list_friend"));
    gtk_builder_connect_signals(builder_list_friend, NULL);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
    
    //--
    g_object_unref(builder_list_friend);

    gtk_widget_show(window_list_friend);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_list_friend_destroy()
{
    gtk_widget_set_visible(window_menu_friend,TRUE);
    gtk_main_quit();
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