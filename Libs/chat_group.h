#include <gtk/gtk.h>
#include "chat_private.h"

typedef struct {
    GtkWidget *btn_member[15];
    GtkWidget *scrol_member;
    GtkWidget *grid_member;
    GtkTextView *text_view_show;
    GtkTextView *text_view_send;
    GtkTextBuffer *text_buff_show;
    GtkWidget *lbl_id_room;
    GtkWidget *entry_add_member;
    GtkWidget *lbl_err_username;
}w_chat_group;
Data_base *db_list_group;
Object *obj_list_group;
int sockfd_chat_group;
GtkWidget       *window_chat_group;
int id_room;
gboolean check_chat_group = FALSE;

gboolean getCheckChatGroup(){
    return check_chat_group;
}

void setCheckChatGroup(){
    if(getCheckChatGroup() == FALSE)
        check_chat_group = TRUE;
    else
        check_chat_group = FALSE;
}
void dup_login_list_group(Object *obj){
    obj_list_group = duplicate_object(obj);
}



void recv_chat_group(void *app){
    const char *format_recv = "<span foreground='blue'><u>%s [%s]:</u></span>" ;
    const char *format_invalid = "<span foreground='blue'>%s</span>" ;
    const char *format_error = "<span foreground='red'>%s</span>" ;
    
    w_chat_group *widg = (w_chat_group*)app;
    Object *obj = (Object*)malloc(sizeof(Object));
    int recv_chat;

    while(1){
        char *markup_message;
            GtkTextIter iter;
            recv_chat = recv(sockfd_chat_group,obj,sizeof(Object),0);
            if(recv_chat < 0 ){
                perror("recv - chat group");
                exit(0);
            };
            // printf("signal: %d\n",obj->signal);
                //add member
                if(obj->signal == SIGNAL_RECV_ADD_MEMBER){
                   
                    printf("err : %d \n", obj->add_member.err);
                    if(obj->add_member.err == ERR_NONE){ //them member
                        int i, len;
                        for(i =0 ;i < db_list_group->list_group.length_group; i++){
                            if(db_list_group->list_group.group[i].ID_group==id_room){
                                len = db_list_group->list_group.group[i].length_member;
                                printf("log 1\n");
                                char *id_mem = (char*)malloc(5*sizeof(char));
                                sprintf(id_mem,"%d",obj->add_member.ID); 
                                widg->btn_member[len] = gtk_button_new_with_label(obj->add_member.name);
                                gtk_widget_set_name(widg->btn_member[len], id_mem);
                                printf("log 2\n");
                                // g_signal_connect(list_group->btn_group[i],"clicked",G_CALLBACK(on_btn_group),NULL);
                                gtk_grid_insert_row(GTK_GRID(widg->grid_member),len);
                                printf("log 3\n");
                                gtk_grid_attach(GTK_GRID(widg->grid_member),widg->btn_member[len],1,len,1,1);
                                 printf("log 4\n");
                                // gtk_widget_show_all(widg->scrol_member);
                                 printf("log 5\n");
                                db_list_group->list_group.group[i].length_member++;

                                break;
                            }
                        }
                    

                        char err_msg[] = "Them thanh vien thanh cong!";
                        markup_message = g_markup_printf_escaped(format_invalid,err_msg);
                        gtk_label_set_markup(GTK_LABEL(widg->lbl_err_username),markup_message);
                        gtk_widget_set_visible(widg->lbl_err_username, TRUE);
                        // g_free(markup_message);
                                
                    
                    }else{
                        char msg_con[100];
                        error_to_string(obj->add_member.err,msg_con);
                        markup_message = g_markup_printf_escaped(format_error,msg_con);
                        gtk_label_set_markup(GTK_LABEL(widg->lbl_err_username),markup_message);
                        gtk_widget_set_visible(widg->lbl_err_username, TRUE);
                        // g_free(markup_message);
                    }
                }
                //chat room
                if (obj->signal == SIGNAL_CHAT_GROUP)
                {
                    if(obj->chat_group.ID_Room == id_room){
                        if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(widg->text_view_show)) != 0){
                            gtk_text_buffer_insert_at_cursor(widg->text_buff_show,"\n",-1);
                        }
                        markup_message = g_markup_printf_escaped(format_recv,obj->chat_group.from_name,
                            obj->chat_group.created_at);
                        
                        gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->text_view_show),&iter);
                        gtk_text_buffer_insert_markup(widg->text_buff_show,&iter,markup_message,-1);
                        gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->text_view_show),&iter);
                        gtk_text_buffer_insert(widg->text_buff_show,&iter,obj->chat_group.message,-1); 
                    }
                    
                }
                        
    }
    
}


int chat_group(int argc, char **argv,int sockfd,int id)
{
    int i;
    GtkBuilder      *builder_chat_group; 
    

    w_chat_group *chat_group_ = g_slice_new(w_chat_group);
    id_room = id;
    sockfd_chat_group = sockfd;
    gtk_init(&argc, &argv);

    builder_chat_group = gtk_builder_new();
    gtk_builder_add_from_file (builder_chat_group, "Glade/list_group_chat.glade", NULL);

    window_chat_group = GTK_WIDGET(gtk_builder_get_object(builder_chat_group, "chat_group"));
    chat_group_->scrol_member = GTK_WIDGET(gtk_builder_get_object(builder_chat_group,"scrol_member"));
    chat_group_->grid_member = GTK_WIDGET(gtk_builder_get_object(builder_chat_group,"grid_member"));
    chat_group_->text_buff_show = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_chat_group,"text_buffer_chat_group"));
    chat_group_->text_view_show = GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat_group,"text_view_chat_group"));
    chat_group_->text_view_send =  GTK_TEXT_VIEW(gtk_builder_get_object(builder_chat_group,"text_view_send"));
    chat_group_->lbl_id_room = GTK_WIDGET(gtk_builder_get_object(builder_chat_group,"lbl_id_room"));
    chat_group_->lbl_err_username = GTK_WIDGET(gtk_builder_get_object(builder_chat_group,"lbl_err_user"));
    chat_group_->entry_add_member = GTK_WIDGET(gtk_builder_get_object(builder_chat_group,"entry_add_username"));
    //////////
    gtk_builder_connect_signals(builder_chat_group, chat_group_);
   
    
    for(i = 0; i < db_list_group->list_group.length_group; i++){
        if (db_list_group->list_group.group[i].ID_group == id){
                int row;
                char strId[20];
                sprintf(strId,"ID_ROOM: %d",id);
                gtk_label_set_text(GTK_LABEL(chat_group_->lbl_id_room),strId);
                for(row = 0; row < db_list_group->list_group.group[i].length_member; row++){
                    
                        chat_group_->btn_member[row] = gtk_button_new_with_label(db_list_group->list_group.group[i].members[row].name);
                        // sprintf(id, "%d", db_list_group->list_group.group[i].ID_group);
                        // gtk_widget_set_name(list_group->btn_group[i], id);
                        // g_signal_connect(list_group->btn_group[i],"clicked",G_CALLBACK(on_btn_group),NULL);
                        gtk_grid_insert_row(GTK_GRID(chat_group_->grid_member),row);
                        gtk_grid_attach(GTK_GRID(chat_group_->grid_member),chat_group_->btn_member[row],1,row,1,1);
                    
                }
                const char *format_recv = "<span foreground='blue'><u>%s [%s]:</u></span>" ;
                const char *format_send = "<span foreground='red'><u>%s [%s]:</u></span>" ;
                char *markup_message;
                
                for(row = 0; row < db_list_group->list_group.group[i].length_msg_public; row++){
                    GtkTextIter iter;
                    if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(chat_group_->text_view_show)) != 0){
                        gtk_text_buffer_insert_at_cursor(chat_group_->text_buff_show,"\n",-1);
                    }
                    if(obj_list_group->login.id == db_list_group->list_group.group[i].msg_public[row].from_id){
                        markup_message = g_markup_printf_escaped(format_send,db_list_group->list_group.group[i].msg_public[row].from_name
                                    ,convert_timestamp_to_date(db_list_group->list_group.group[i].msg_public[row].create_at));
                    }else{
                        markup_message = g_markup_printf_escaped(format_recv,db_list_group->list_group.group[i].msg_public[row].from_name
                                    ,convert_timestamp_to_date(db_list_group->list_group.group[i].msg_public[row].create_at));
                    }
                    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(chat_group_->text_view_show),&iter);
                    gtk_text_buffer_insert_markup(chat_group_->text_buff_show,&iter,markup_message,-1);

                    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(chat_group_->text_view_show),&iter);
                    gtk_text_buffer_insert(chat_group_->text_buff_show,&iter,db_list_group->list_group.group[i].msg_public[row].message,-1);
                }

                break;
                //if(gtk_widget_is_visible(list_friends->scrol_list_friend)){
                    // gtk_widget_show_all(chat_group_->scrol_member);
        }
    }
    GtkTextIter end;
    GtkTextMark *mark;
        gtk_text_buffer_get_end_iter(chat_group_->text_buff_show,&end);
        mark = gtk_text_buffer_create_mark(chat_group_->text_buff_show,NULL,&end,FALSE);
        gtk_text_view_scroll_to_mark(chat_group_->text_view_show, mark,0,FALSE,0,0);
    pthread_t id_chat_group_ok;
    if(pthread_create(&id_chat_group_ok,NULL, (void *)recv_chat_group,(void *)chat_group_) != 0){
        perror("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }


    g_object_unref(builder_chat_group);
    
    // start 1 second timer
    

    gtk_widget_show(window_chat_group);    

    //gtk_widget_set_visible(widgets->scrol_list_friend,FALSE);   
           
    gtk_main();
    pthread_cancel(id_chat_group_ok);
    g_slice_free(w_chat_group,chat_group_);
    return 0;
}

// called when window is closed
void on_chat_group_destroy()
{
    setCheckChatGroup();
    gtk_main_quit();
}
void on_btn_member_toggled(GtkToggleButton *togglebutton, w_chat_group * widg){
    if(gtk_widget_is_visible(widg->scrol_member)){
		gtk_widget_set_visible(widg->scrol_member,FALSE);
	}else{
		gtk_widget_set_visible(widg->scrol_member,TRUE);
        gtk_widget_show_all(widg->scrol_member);
	}
}
void on_btn_send_group_clicked(GtkButton *b, w_chat_group *chat_group_ok){
    int i;
    GtkTextIter start, end, iter;
    GDateTime *date_time;
    char *markup_message;
    // gchar *send_name_dtime;
    gchar *time_;
    const char *format_error = "<span foreground='red'><u>%s [%s]: </u></span>" ;
    Object *send_obj = (Object*)malloc(sizeof(Object));
    send_obj->signal = SIGNAL_CHAT_GROUP;
    send_obj->chat_group.from_id = obj_list_group->login.id;
    send_obj->chat_group.ID_Room = id_room;
    strcpy(send_obj->chat_group.from_name, obj_list_group->login.name);
    for(i =0 ;i < db_list_group->list_group.length_group; i++){
        if(db_list_group->list_group.group[i].ID_group == id_room){
            int j, len = 0;
            for(j = 0; j < db_list_group->list_group.group[i].length_member; j++){
                if(db_list_group->list_group.group[i].members[j].ID != obj_list_group->login.id){
                    send_obj->chat_group.to_id_member[len] = db_list_group->list_group.group[i].members[j].ID;
                    len++;
                }
            }
            send_obj->chat_group.length_to_member = len;
            break;
        }
    }
    
    date_time = g_date_time_new_now_local();
    //send_name_dtime = g_date_time_format(date_time,"YOU [%F %T]: ");
    time_ = g_date_time_format(date_time,"%F %T");
    strcpy(send_obj->chat_group.created_at,time_);
    markup_message = g_markup_printf_escaped(format_error,obj_list_group->login.name,time_);
    gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(chat_group_ok->text_view_send),&start);
    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(chat_group_ok->text_view_send),&end);
    gtk_text_buffer_get_end_iter(chat_group_ok->text_buff_show,&iter);
    if(gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(chat_group_ok->text_view_show)) != 0){
        gtk_text_buffer_insert(chat_group_ok->text_buff_show,&iter,"\n",-1);
    }

    strcpy(send_obj->chat_group.message,gtk_text_buffer_get_text(
        gtk_text_view_get_buffer(chat_group_ok->text_view_send),&start,&end,FALSE));
    if(send(sockfd_chat_group,send_obj,sizeof(Object),0) < 0){
        perror("send - chat group");
        exit(0);
    }
    
    gtk_text_buffer_get_end_iter(chat_group_ok->text_buff_show,&iter);
    gtk_text_buffer_insert_markup(chat_group_ok->text_buff_show,&iter,markup_message,-1);
    gtk_text_buffer_get_end_iter(chat_group_ok->text_buff_show,&iter);
    gtk_text_buffer_insert(chat_group_ok->text_buff_show,&iter,gtk_text_buffer_get_text(
        gtk_text_view_get_buffer(chat_group_ok->text_view_send),&start,&end,FALSE),-1);
    gtk_text_view_set_buffer(chat_group_ok->text_view_send,gtk_text_buffer_new(NULL));
    //scroll bottom
    
         GtkTextMark *mark;
         gtk_text_buffer_get_end_iter(chat_group_ok->text_buff_show,&end);
         mark = gtk_text_buffer_create_mark(chat_group_ok->text_buff_show,NULL,&end,FALSE);
         gtk_text_view_scroll_to_mark(chat_group_ok->text_view_show, mark,0,FALSE,0,0);

}

void on_add_member_clicked(GtkButton *b, w_chat_group *chat_group){
    
    const char *format_error = "<span foreground='red'>%s</span>" ;
    char *markup_message;
    if(strlen(gtk_entry_get_text(GTK_ENTRY(chat_group->entry_add_member))) == 0){
        char err_msg[] = "Ban chua nhap USERNAME!";
        markup_message = g_markup_printf_escaped(format_error,err_msg);
        gtk_label_set_markup(GTK_LABEL(chat_group->lbl_err_username),markup_message);
        gtk_widget_set_visible(chat_group->lbl_err_username, TRUE);
        g_free(markup_message);
    }else{
        
        
        obj_list_group->add_member.ID_Room = id_room;
        strcpy(obj_list_group->add_member.username, gtk_entry_get_text(GTK_ENTRY(chat_group->entry_add_member)));
        obj_list_group->signal = SIGNAL_ADD_ROOM;
        if(send(sockfd_chat_group,obj_list_group, sizeof(Object),0) < 0){
            perror("send - add room");
            exit(0);
        }
        printf("helllo");
        // Object *obj = (Object*)malloc(sizeof(Object));
        
    }
    
}