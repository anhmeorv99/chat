#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "menu_list_friend.h"
typedef struct {
	GtkTextBuffer *text_buffer_view;
	GtkTextView *text_write;
	//GtkTextBuffer *text_buffer2;
	GtkTextView *txtvw_show;
    GtkWidget *chat_send;
    GtkWidget *lbl_name_friend;
	GtkWidget *scrol_list_friend;
	//GtkLayout *layout;
	GtkWidget *grid;
    GtkWidget *button[100];

    Object *object_chat ;
    int sock_chat_private;
}app_widgets;

//test
Object *obj_chat_private;
int recv_chat;

app_widgets        *widgets;
GtkWidget       *window_chat;
// GtkWidget       *window_friend_chat;

void dup_obj_chat_private(Object *obj){
    obj_chat_private = duplicate_object(obj);
}

void set_obj_chat_private(char *from_username, int sockfd){
    widgets = g_slice_new(app_widgets);
    widgets->object_chat = new_chat_private_object();
    strcpy(widgets->object_chat->chat_private.from_username,from_username);
    widgets->sock_chat_private = sockfd;
    //object->chat_private->to_username
}

char * convert_timestamp_to_date(char* timestamp){
    time_t rawtime = (time_t)atoi(timestamp);
    struct tm  ts;
    char  *buf = (char*)malloc(100*sizeof(char));
                        // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, 100*sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    return buf;
}

void on_btn_list_friend_private(GtkButton *button,app_widgets *widg){
    const char *format_recv = "<span foreground='blue'><u>%s [%s]:</u></span>" ;
    const char *format_send = "<span foreground='red'><u>%s [%s]:</u></span>" ;
    char *markup_message;
    if(gtk_widget_is_visible(widg->lbl_name_friend) == FALSE){
        gtk_widget_set_visible(widg->lbl_name_friend, TRUE);
    }
    gtk_label_set_text(GTK_LABEL(widg->lbl_name_friend),
            gtk_button_get_label(button));
    gtk_widget_set_name(widg->lbl_name_friend,gtk_widget_get_name(GTK_WIDGET(button)));
    if(gtk_widget_is_sensitive(widg->chat_send) == FALSE){
        gtk_widget_set_sensitive(widg->chat_send, TRUE);
    }

    obj_chat_private->chat_private.from_id = obj_chat_private->login.id;
    obj_chat_private->chat_private.to_id = atoi(gtk_widget_get_name(GTK_WIDGET(button)));
    obj_chat_private->signal = SIGNAL_RECV_CHAT_PRIVATE;
    if(send(widg->sock_chat_private, obj_chat_private,sizeof(Object), 0) < 0){
        perror("send - chat private");
        return;
    }
    Data_base *db_recv_chat_private = (Data_base*)malloc(sizeof(Data_base));
    int i;
    if (recv(widg->sock_chat_private,db_recv_chat_private,sizeof(Data_base),0)<0){
        perror("recv chat private");
        return;
    }

    if(db_recv_chat_private->signal == SIGNAL_DB_CHAT_PRIVATE){
        gtk_text_buffer_set_text(widg->text_buffer_view, "", -1);
        for (i=0;i < db_recv_chat_private->chat_private.length_message; i++){
            GtkTextIter iter;
            
            if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(widg->txtvw_show)) != 0){
                        gtk_text_buffer_insert_at_cursor(widg->text_buffer_view,"\n",-1);
                    }
            if(strcmp(obj_chat_private->login.username,db_recv_chat_private->chat_private.msg_private[i].from_username)==0){
                    markup_message = g_markup_printf_escaped(format_send,db_recv_chat_private->chat_private.msg_private[i].from_name
                                    ,convert_timestamp_to_date(db_recv_chat_private->chat_private.msg_private[i].create_at));
            }else{
                markup_message = g_markup_printf_escaped(format_recv,db_recv_chat_private->chat_private.msg_private[i].from_name
                                    ,convert_timestamp_to_date(db_recv_chat_private->chat_private.msg_private[i].create_at));
            }  
                    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
                    gtk_text_buffer_insert_markup(widg->text_buffer_view,&iter,markup_message,-1);

            
                    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
                    gtk_text_buffer_insert(widg->text_buffer_view,&iter,db_recv_chat_private->chat_private.msg_private[i].message,-1);
                    
         }
    }
  
}
void recv_to_friend(void *app){
    //test
    //GtkTextIter iter;
    //char message_recv[250];
    const char *format_recv = "<span foreground='blue'><u>%s [%s]:</u></span>" ;
    char *markup_message;
    app_widgets *widg = (app_widgets*)app;
    Object *obj = (Object*)malloc(sizeof(Object));
    

    while(1){
            GtkTextIter iter;
            recv_chat = recv(widg->sock_chat_private,obj,sizeof(Object),0);
            if(recv_chat < 0 ) break;
            //message_recv[recv_chat] = '\0';
            //puts(message_recv);
            
                if(obj->signal == SIGNAL_ADD_FRIEND){

                }
                
                if (obj->signal == SIGNAL_CHAT_PRIVATE)
                {
                    if(obj->chat_private.from_id == atoi(gtk_widget_get_name(widg->lbl_name_friend))){
                        if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(widg->txtvw_show)) != 0){
                            gtk_text_buffer_insert_at_cursor(widg->text_buffer_view,"\n",-1);
                        }
                        markup_message = g_markup_printf_escaped(format_recv,obj->chat_private.from_name,
                            obj->chat_private.create_at);
                        //gtk_text_view_get_buffer(widg->text_write)
                        gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
                        gtk_text_buffer_insert_markup(widg->text_buffer_view,&iter,markup_message,-1);
                        gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
                        gtk_text_buffer_insert(widg->text_buffer_view,&iter,obj->chat_private.message,-1); 
                    }
                    
                }
                        
    }
    
}

//

int chat_private(int argc, char **argv)
{
    GtkBuilder      *builder_chat; 
    

  

    gtk_init(&argc, &argv);

    builder_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_chat, "Glade/chat.glade", NULL);

    window_chat = GTK_WIDGET(gtk_builder_get_object(builder_chat, "window_chat"));
    widgets->text_write  = GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat, "txtvw_write"));
    widgets->text_buffer_view  = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_chat, "textbuffer_view"));
    widgets->txtvw_show  = GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat, "txtvw_show"));
    widgets->chat_send = GTK_WIDGET(gtk_builder_get_object(builder_chat,"chat_send"));
    widgets->scrol_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_chat,"scrol_list_friend"));
    widgets->lbl_name_friend = GTK_WIDGET(gtk_builder_get_object(builder_chat,"lbl_name_friend"));
    widgets->grid = GTK_WIDGET(gtk_builder_get_object(builder_chat,"grid_list_friend"));

 
    obj_chat_private->signal = SIGNAL_RECV_LIST_FRIEND;
    if(send(widgets->sock_chat_private,obj_chat_private,sizeof(Object), 0) < 0){
        perror("send - list friend");
        return 0;
    }
    Data_base *db_list_friend = (Data_base*)malloc(sizeof(Data_base));
    if(recv(widgets->sock_chat_private,db_list_friend,sizeof(Data_base), 0) < 0){
        perror("recv - list friend");
        return 0;
    }

     if(db_list_friend->signal == SIGNAL_DB_LIST_FRIEND){
        int i;
        int row = 0;
        char id[5];
        for(i = 0; i < db_list_friend->length_list_friend; i++){
            if(db_list_friend->list_friend[i].confirm == 1){
                widgets->button[row] = gtk_button_new_with_label(db_list_friend->list_friend[i].name);
                sprintf(id,"%d", db_list_friend->list_friend[i].ID);
                gtk_widget_set_name(widgets->button[row], id);
                g_signal_connect(widgets->button[row],"clicked",G_CALLBACK(on_btn_list_friend_private),widgets);
                gtk_grid_insert_row(GTK_GRID(widgets->grid),row);
                gtk_grid_attach(GTK_GRID(widgets->grid),widgets->button[row],1,row,1,1);
                row++;
            }    
        }
        
     }
    

    pthread_t id;
    if(pthread_create(&id,NULL, (void *)recv_to_friend,(void *)widgets) != 0){
        perror("Create pthread error!\n");
        exit(EXIT_FAILURE);
    } 

    gtk_builder_connect_signals(builder_chat, widgets);
   
    g_object_unref(builder_chat);


    gtk_widget_show(window_chat);    
       
    gtk_main();
    pthread_cancel(id);
    g_slice_free(app_widgets, widgets);

    return 0;
}

// called when window is closed
void on_window_chat_destroy()
{
    gtk_main_quit();
}

void on_chat_send_clicked(GtkButton *button, app_widgets *widg){
    GtkTextIter start, end, iter;
    GDateTime *date_time;
    char *markup_message;
    gchar *send_name_dtime;
    gchar *time_;
    const char *format_error = "<span foreground='red'><u>%s</u></span>" ;
    date_time = g_date_time_new_now_local();
    send_name_dtime = g_date_time_format(date_time,"YOU [%F %T]: ");
    time_ = g_date_time_format(date_time,"%F %T");
    strcpy(widg->object_chat->chat_private.create_at,time_);
    markup_message = g_markup_printf_escaped(format_error,send_name_dtime);
    gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(widg->text_write),&start);
    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->text_write),&end);
    gtk_text_buffer_get_end_iter(widg->text_buffer_view,&iter);
    if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(widg->txtvw_show)) != 0){
        gtk_text_buffer_insert(widg->text_buffer_view,&iter,"\n",-1);
    }

    strcpy(widg->object_chat->chat_private.message,gtk_text_buffer_get_text(
        gtk_text_view_get_buffer(widg->text_write),&start,&end,FALSE));
    widg->object_chat->chat_private.to_id = atoi(gtk_widget_get_name(widg->lbl_name_friend));
    printf("to id : %d ",widg->object_chat->chat_private.to_id );
    widg->object_chat->chat_private.from_id = obj_chat_private->login.id;
    strcpy(widg->object_chat->chat_private.from_username,obj_chat_private->login.username);
    strcpy(widg->object_chat->chat_private.from_name, obj_chat_private->login.name);
    strcpy(widg->object_chat->chat_private.to_name, gtk_label_get_text(GTK_LABEL(widg->lbl_name_friend)));
    if(send(widg->sock_chat_private,widg->object_chat,sizeof(Object),0) < 0){
        perror("send - chat private");
        return;
    }
    
    gtk_text_buffer_get_end_iter(widg->text_buffer_view,&iter);
    gtk_text_buffer_insert_markup(widg->text_buffer_view,&iter,markup_message,-1);
    gtk_text_buffer_get_end_iter(widg->text_buffer_view,&iter);
    gtk_text_buffer_insert(widg->text_buffer_view,&iter,gtk_text_buffer_get_text(
        gtk_text_view_get_buffer(widg->text_write),&start,&end,FALSE),-1);
    gtk_text_view_set_buffer(widg->text_write,gtk_text_buffer_new(NULL));

}

void on_togbtn_list_friend_toggled(GtkToggleButton *togglebutton, app_widgets * widg){
	//GtkWidget *button_ = gtk_button_new_with_label("button_name");
    
	if(gtk_widget_is_visible(widg->scrol_list_friend)){
		gtk_widget_set_visible(widg->scrol_list_friend,FALSE);
	}else{
		gtk_widget_set_visible(widg->scrol_list_friend,TRUE);
        gtk_widget_show_all(widg->scrol_list_friend);
	}
	
}

// void on_add_friend_clicked(GtkButton *button, app_widgets *widg){
//     int stt = row + 1;
//     char str_stt[10];
//     sprintf(str_stt,"%d",stt);
//     char label[] = "friend ";
//     strcat(label,str_stt);
//     widg->button[row] = gtk_button_new_with_label(label);
//     g_signal_connect(widg->button[row],"clicked",G_CALLBACK(on_row),NULL);
//     gtk_grid_insert_row(GTK_GRID(widg->grid),row);
//     gtk_grid_attach(GTK_GRID(widg->grid),widg->button[row],1,row,1,1);
//     if(gtk_widget_is_visible(widg->scrol_list_friend)){
//         gtk_widget_show_all(widg->scrol_list_friend);
//     }
//     row++;

// }

// void on_row(GtkButton *b){
//     g_print("You clicked button: %s\n",gtk_button_get_label(b));
// }

