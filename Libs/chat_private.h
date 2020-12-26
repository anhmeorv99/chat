#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	GtkTextBuffer *text_buffer_view;
	GtkTextView *text_write;
	//GtkTextBuffer *text_buffer2;
	GtkTextView *txtvw_show;

	GtkWidget *scrol_list_friend;
	//GtkLayout *layout;
	GtkWidget *grid;
    GtkWidget *button[100];

    Object *object_chat ;
    int sock_chat_private;
}app_widgets;
int row;
//test
Object *obj_chat_private;
int recv_chat;

app_widgets        *widgets;
GtkWidget       *window_friend_chat;

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
            
            if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(widg->txtvw_show)) != 0){
                gtk_text_buffer_insert_at_cursor(widg->text_buffer_view,"\n",-1);
            }
            markup_message = g_markup_printf_escaped(format_recv,obj->chat_private.from_username,
                obj->chat_private.create_at);
            //gtk_text_view_get_buffer(widg->text_write)
            gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
            gtk_text_buffer_insert_markup(widg->text_buffer_view,&iter,markup_message,-1);
            gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->txtvw_show),&iter);
            gtk_text_buffer_insert(widg->text_buffer_view,&iter,obj->chat_private.message,-1);   
  

    }
    
}

//
void on_row(GtkButton *b);
int chat_private(int argc, char **argv)
{
    GtkBuilder      *builder_chat; 
    GtkWidget       *window_chat;

  

    gtk_init(&argc, &argv);

    builder_chat = gtk_builder_new();
    gtk_builder_add_from_file (builder_chat, "Glade/chat.glade", NULL);

    window_chat = GTK_WIDGET(gtk_builder_get_object(builder_chat, "window_chat"));
    widgets->text_write  = GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat, "txtvw_write"));
    widgets->text_buffer_view  = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_chat, "textbuffer_view"));
    widgets->txtvw_show  = GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat, "txtvw_show"));
    
    widgets->scrol_list_friend = GTK_WIDGET(gtk_builder_get_object(builder_chat,"scrol_list_friend"));
    widgets->grid = GTK_WIDGET(gtk_builder_get_object(builder_chat,"grid_list_friend"));

 
    row =0;
   
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
    gtk_widget_set_visible(window_friend_chat,TRUE);
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
    strcpy(widg->object_chat->chat_private.from_username,obj_chat_private->login.username);

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

void on_add_friend_clicked(GtkButton *button, app_widgets *widg){
    int stt = row + 1;
    char str_stt[10];
    sprintf(str_stt,"%d",stt);
    char label[] = "friend ";
    strcat(label,str_stt);
    widg->button[row] = gtk_button_new_with_label(label);
    g_signal_connect(widg->button[row],"clicked",G_CALLBACK(on_row),NULL);
    gtk_grid_insert_row(GTK_GRID(widg->grid),row);
    gtk_grid_attach(GTK_GRID(widg->grid),widg->button[row],1,row,1,1);
    if(gtk_widget_is_visible(widg->scrol_list_friend)){
        gtk_widget_show_all(widg->scrol_list_friend);
    }
    row++;

}

void on_row(GtkButton *b){
    g_print("You clicked button: %s\n",gtk_button_get_label(b));
}

