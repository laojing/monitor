#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "ProjectData.h"

#define PROGRAMNAME "monitor"                               //语言文件名称
#define LANGUAGEDIR "Languages"                             //语言文件位置

gint curpage = 0;
gint pageflag = 0;
GtkWidget *pages[PAGES];

GtkWidget *fixed;

gint oldx = -1;
void Cycle ( void ) 
{
	while ( 1 ) {
		g_usleep ( 10000 );

		// change page
		if ( pageflag == 1 ) {
			gint next = ( curpage + 1 ) % PAGES;
			gtk_fixed_move ( GTK_FIXED(fixed), pages[next], 0, 0 );
			gtk_widget_set_size_request ( pages[next], 0, HEIGHT );
			for ( gint i=0; i<WIDTH; i++ ) {
				g_usleep ( 3000 );
				gtk_fixed_move ( GTK_FIXED(fixed), pages[curpage], i, 0 );
				gtk_widget_set_size_request ( pages[curpage], WIDTH-i, HEIGHT );
				gtk_widget_set_size_request ( pages[next], i, HEIGHT );
			}
			pageflag = 0;
			curpage = next;
		} else if ( pageflag == 2 ) {
			gint next = curpage - 1;
			if ( next < 0 ) next = PAGES - 1;
			gtk_fixed_move ( GTK_FIXED(fixed), pages[next], WIDTH, 0 );
			for ( gint i=0; i<WIDTH; i++ ) {
				g_usleep ( 3000 );
				gtk_fixed_move ( GTK_FIXED(fixed), pages[next], WIDTH-i, 0 );
				gtk_widget_set_size_request ( pages[curpage], WIDTH-i, HEIGHT );
				gtk_widget_set_size_request ( pages[next], i, HEIGHT );
			}
			pageflag = 0;
			curpage = next;
		}
	}
}

gboolean bPress = FALSE;

gboolean 
ChangedPress (
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer gdata )
{
	bPress = TRUE;
	return TRUE;
}

gboolean
ChangePage ( 
		GtkWidget *widget,
		GdkEventMotion *event,
		gpointer gdata ) 
{
	int x, y;
	GdkModifierType state;
	gdk_window_get_device_position ( event->window,
			event->device, &x, &y, &state );
	if ( state & GDK_BUTTON1_MASK ) {
		if ( bPress ) {
			oldx = x;
			bPress = FALSE;
		}
		if ( oldx > 0 ) {
			if ( x - oldx >= 100 ) {
				pageflag = 1;
				oldx = -1;
			}
			if ( x - oldx <= -100 ) {
				pageflag = 2;
				oldx = -1;
			}
		}
	}
	return TRUE;
}

void 
apply_css ( GtkWidget *widget, GtkStyleProvider *provider )
{
	gtk_style_context_add_provider ( gtk_widget_get_style_context ( widget ), provider, G_MAXUINT );
	if ( GTK_IS_CONTAINER ( widget ) ) {
		gtk_container_forall ( GTK_CONTAINER ( widget ), (GtkCallback) apply_css, provider );
	}
}

static void 
window_close ( GtkWidget *widget, gpointer data ) {
	gtk_main_quit();
}

int 
main ( int argc, char *argv[] ) {
	GtkWidget *window, *grid, *nagtive, *child;
	GtkWidget *panedup, *panedleft, *panedright;
	GResource *resource;
	GtkWidget *viewport, *scroll;
	GtkTextIter iter;
	GtkTextBuffer *buffer;
	GtkCellRenderer *renderer;
	GtkStyleProvider *provider;
	GBytes *bytes;
	gsize data_size;
	const guint8 *udata;
	guint length;
	int res;
	pthread_t a_thread;

	chdir ( "/home/laojing/Projects/Monitor/Binary/" );
	gtk_init ( &argc, &argv );

	setlocale ( LC_ALL, "zh_CN.UTF-8" );
	bindtextdomain ( PROGRAMNAME, LANGUAGEDIR );
	bind_textdomain_codeset ( PROGRAMNAME, "UTF-8" );
	textdomain ( PROGRAMNAME );

	window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_default_size ( GTK_WINDOW(window), WIDTH, HEIGHT );
	gtk_window_set_position ( GTK_WINDOW(window), GTK_WIN_POS_CENTER );
	gtk_window_set_title ( GTK_WINDOW(window), _("Wind Turbine Simple Monitor System") );
	gtk_window_set_icon ( GTK_WINDOW(window), gdk_pixbuf_new_from_file ( "Resource/icon.png", NULL ) );

	g_signal_connect ( window, "motion-notify-event", G_CALLBACK(ChangePage), NULL);  
	g_signal_connect ( window, "button-press-event", G_CALLBACK ( ChangedPress ), NULL );

	g_signal_connect ( window, "destroy", G_CALLBACK(window_close), NULL );

	allocProjectData ();

	fixed = gtk_fixed_new ();

	for ( int i=0; i<PAGES; i++ ) {
		pages[i] = createPage(i);
		if ( i == 0 ) {
			gtk_fixed_put ( GTK_FIXED(fixed), pages[i], 0, 0 );
			gtk_widget_set_size_request ( pages[i], WIDTH, HEIGHT );
		} else {
			gtk_fixed_put ( GTK_FIXED(fixed), pages[i], -WIDTH, 0 );
		}
	}
	gtk_window_set_resizable ( GTK_WINDOW(window), FALSE );
	gtk_container_add ( GTK_CONTAINER (window), fixed );

//	gtk_container_add ( GTK_CONTAINER (window), createOverall() );

	provider = GTK_STYLE_PROVIDER ( gtk_css_provider_new () );
	resource = g_resource_load ( "Resource/resource.gresource", NULL );
	g_resources_register ( resource );
	bytes = g_resources_lookup_data ( "/gtk.css", 0, NULL );
	udata = g_bytes_get_data ( bytes, &data_size );
	gtk_css_provider_load_from_data ( GTK_CSS_PROVIDER ( provider ), (gchar*)udata, data_size, NULL );
	g_bytes_unref ( bytes );
	apply_css ( window, provider );

	gtk_widget_show_all ( window );

	GThread *gth = g_thread_new ( "com", (GThreadFunc)Cycle, NULL );
	g_thread_unref ( gth );

	gtk_main ();

	return 0;
}

