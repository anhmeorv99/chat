#include <gtk/gtk.h>
#include "chat_group.h"
#include <pthread.h>
typedef struct {
    GtkWidget *lbl_null_list;
    GtkWidget *btn_group[100];
    GtkWidget *grid_list;
    GtkWidget *scrol_list;
    GtkWidget *lbl_err_id;
    GtkWidget *entry_id_room;
}w_list_group;

GtkWidget *window_group_chat;
GtkWidget *window_create_group_chat;
int sockfd_group_chat;
int argc_group_chat;
char *argv_group_chat;
 gboolean check_list_group = FALSE;

gboolean getCheckListGroup(){
    return check_list_group;
}

void setCheckListGroup(){
    if(getCheckListGroup() == FALSE)
        check_list_group = TRUE;
    else
        check_list_group = FALSE;
}

int create_group_chat(int argc, char **argv);
void on_btn_group(GtkButton *b){
    if(getCheckChatGroup() == FALSE){
        int id_ = atoi(gtk_widget_get_name(GTK_WIDGET(b)));
        setCheckChatGroup();
        chat_group(argc_group_chat,&argv_group_chat,sockfd_group_chat,id_);
    }
    
}

//---------------------------------
//main
int list_group_chat(int argc, char **argv,int sockfd)
{
    GtkBuilder      *builder_group_chat; 
    sockfd_group_chat = sockfd;
   	argc_group_chat = argc;
   	argv_group_chat = *argv;
    gtk_init(&argc, &argv);
    w_list_group *list_group = g_slice_new(w_list_group);
    builder_group_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_group_chat, "Glade/list_group_chat.glade", NULL);

    window_group_chat = GTK_WIDGET(gtk_builder_get_object(builder_group_chat, "list_group_chat"));
    list_group->grid_list = GTK_WIDGET(gtk_builder_get_object(builder_group_chat, "grid_list"));
    list_group->scrol_list = GTK_WIDGET(gtk_builder_get_object(builder_group_chat, "scrol_list"));
    list_group->lbl_null_list = GTK_WIDGET(gtk_builder_get_object(builder_group_chat, "lbl_null_list"));
    
    
    gtk_builder_connect_signals(builder_group_chat, list_group);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
   
         obj_list_group->signal = SIGNAL_RECV_LIST_GROUP;
    if(send(sockfd_group_chat,obj_list_group,sizeof(Object), 0) < 0){
        perror("send - list group");
        exit(0);
    }
    db_list_group = (Data_base*)malloc(sizeof(Data_base));
    if(recv(sockfd_group_chat,db_list_group,sizeof(Data_base), MSG_WAITALL) < 0){
        perror("recv - list group");
        exit(0);
        }

        if (db_list_group->signal == SIGNAL_RECV_DB_LIST_GROUP){
            if(db_list_group->list_group.length_group < 0){
                gtk_widget_set_visible(list_group->scrol_list, FALSE);
               gtk_widget_set_visible(list_group->lbl_null_list, TRUE);
            }
            else{
                
                int i;
                char id[5];
                for(i = 0; i < db_list_group->list_group.length_group; i++){
                    
                        list_group->btn_group[i] = gtk_button_new_with_label(db_list_group->list_group.group[i].name);

                        sprintf(id, "%d", db_list_group->list_group.group[i].ID_group);
                        gtk_widget_set_name(list_group->btn_group[i], id);
                        g_signal_connect(list_group->btn_group[i],"clicked",G_CALLBACK(on_btn_group),NULL);
                        gtk_grid_insert_row(GTK_GRID(list_group->grid_list),i);
                        gtk_grid_attach(GTK_GRID(list_group->grid_list),list_group->btn_group[i],1,i,1,1);
                    
                }
                    gtk_widget_show_all(list_group->scrol_list);
            }

        }

    g_object_unref(builder_group_chat);

    gtk_widget_show(window_group_chat);                
    gtk_main();
    g_slice_free(w_list_group,list_group);
    return 0;
}

// called when window is closed
void on_list_group_chat_destroy()
{
    setCheckListGroup();
    gtk_main_quit();
}

void on_btn_create_group_clicked(){
	create_group_chat(argc_group_chat,&argv_group_chat);
}



//-----------------------create group chat----------------------------
//main
int create_group_chat(int argc, char **argv)
{
    GtkBuilder      *builder_create_group_chat; 
   	
    gtk_init(&argc, &argv);

    builder_create_group_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_create_group_chat, "Glade/list_group_chat.glade", NULL);

    window_create_group_chat = GTK_WIDGET(gtk_builder_get_object(builder_create_group_chat, "create_group_chat"));
    gtk_builder_connect_signals(builder_create_group_chat, NULL);
    //--
    //gtk_window_set_decorated(GTK_WINDOW(window_signup),FALSE);
        
    //--
    g_object_unref(builder_create_group_chat);

    gtk_widget_show(window_create_group_chat);                
    gtk_main();

 
    return 0;
}

// called when window is closed
void on_create_group_chat_destroy()
{
    gtk_main_quit();
}
