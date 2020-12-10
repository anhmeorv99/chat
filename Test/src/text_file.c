#include <gtk/gtk.h>

typedef struct {
	GtkTextBuffer *text_buffer1;
	GtkTextView *text_view1;
	GtkTextBuffer *text_buffer2;
	GtkTextView *text_view2;

	GtkWidget *scrol_w;
	GtkLayout *layout;
	
}app_widgets;


int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    app_widgets        *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/text_file.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "text_file"));
    widgets->text_view1  = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "txtvw_send"));
    widgets->text_buffer1  = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer1"));
    widgets->text_view2  = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "txtvw_show"));
    //widgets->text_buffer2  = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer2"));
    widgets->scrol_w = GTK_WIDGET(gtk_builder_get_object(builder,"scrol_window"));
    widgets->layout = GTK_LAYOUT(gtk_builder_get_object(builder,"layout"));

    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);
    
    // start 1 second timer
    

    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

// called when window is closed
void on_text_file_destroy()
{
    gtk_main_quit();
}

void on_btn_send_clicked(GtkButton *button, app_widgets *widg){
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(widg->text_view1),&start);
	gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(widg->text_view1),&end);
	//gtk_text_view_set_buffer(widg->text_view2,gtk_text_view_get_buffer(widg->text_view1));
	gtk_text_buffer_insert_at_cursor(widg->text_buffer1,gtk_text_buffer_get_text(gtk_text_view_get_buffer(widg->text_view1),&start,&end,FALSE),-1);
	gtk_text_view_set_buffer(widg->text_view1,gtk_text_buffer_new(NULL));
}

void on_tog_btn_toggled(GtkToggleButton *togglebutton, app_widgets * widg){
	GtkWidget *button_ = gtk_button_new_with_label("button_name");

	if(gtk_widget_is_visible(widg->scrol_w)){
		gtk_widget_set_visible(widg->scrol_w,FALSE);
	}else{
		gtk_widget_set_visible(widg->scrol_w,TRUE);
	}
	
}