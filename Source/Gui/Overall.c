#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "ProjectData.h"

GtkTextBuffer *wAlarms;
GtkWidget *wtAlarms;
GtkWidget *wTurbState;
GtkWidget *wAlarmCode;
GtkWidget *wWindDir;
GtkWidget *wWindSpd;
GtkWidget *wPitchAngle;
GtkWidget *wGenSpd;
GtkWidget *wGenPow;

GList *Curves = NULL;

GtkWidget *wSetWindDir;
gfloat setwinddir = M_PI;

void
UpdateAll ()
{
//	GString *str = g_string_new ( "" );
	guint actlen = g_list_length ( pdata->actalarms );
	if ( actlen > 0 ) {
		AlarmItem *actitem = g_list_nth_data ( pdata->actalarms, 0 );
//		gtk_label_set_text ( GTK_LABEL(wtAlarms), GetAlarmName ( actitem->id) );
//		g_print ( "%s\n", GetAlarmName ( actitem->id) );
	} else {
		gtk_label_set_text ( GTK_LABEL(wtAlarms), "System OK!" );
	}
//	for ( int k=0; k<actlen; k++ ) {
//		AlarmItem *actitem = g_list_nth_data ( pdata->actalarms, k );
//		g_string_append_printf ( str, "\n\t%s %s %s\n", 
//				actitem->acttime, 
//				GetLevelName ( actitem->level ),
//				GetAlarmName ( actitem->id ) );
//	}
//	gtk_text_buffer_set_text ( wAlarms, g_strdup(str->str), -1 );                            
//	gtk_text_buffer_set_text ( wAlarms, g_strescape(str->str,NULL), -1 );                            
//	g_string_free ( str, FALSE );
//	g_string_free ( str, TRUE );
	gtk_widget_queue_draw ( wTurbState );
	gtk_widget_queue_draw ( wWindDir );

	guint lens = g_list_length ( Curves );
	for ( int i=0; i<lens; i++ ) {
		RealCurve *rc = g_list_nth_data ( Curves, i );
		rc->update = TRUE;
	}
	gtk_widget_queue_draw ( wWindSpd );
	gtk_widget_queue_draw ( wPitchAngle );
	gtk_widget_queue_draw ( wGenSpd );
	gtk_widget_queue_draw ( wGenPow );
}

int windstate = 0;

static gboolean 
DrawTurbState (
		GtkWidget *widget, 
		cairo_t *cr, 
    	gpointer user_data )
{
	guint width = gtk_widget_get_allocated_width (widget);
	guint height = gtk_widget_get_allocated_height (widget);

	cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
	cairo_set_line_width(cr, 1);
	cairo_select_font_face ( cr, "YaHei Consolas Hybrid",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size ( cr, 14 );

	cairo_move_to ( cr, 20, 30 );
	gchar *caption = g_strdup_printf ( "主控制器:%s", GetStateName ( pdata->oper, "Oper" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
	cairo_move_to ( cr, width/2, 30 );
	caption = g_strdup_printf ( "闭环控制:%s", GetStateName ( pdata->loop, "Loop" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
	cairo_move_to ( cr, 20, 30 + height/3 );
	caption = g_strdup_printf ( "变桨系统:%s", GetStateName ( pdata->pich, "Pich" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
	cairo_move_to ( cr, width/2, 30 + height/3 );
	caption = g_strdup_printf ( "变流系统:%s", GetStateName ( pdata->conv, "Conv" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
	cairo_move_to ( cr, 20, 30 + height*2/3 );
	caption = g_strdup_printf ( "偏航系统:%s", GetStateName ( pdata->yaws, "YawS" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
	cairo_move_to ( cr, width/2, 30 + height*2/3 );
	caption = g_strdup_printf ( "偏航驱动:%s", GetStateName ( pdata->yawd, "YawD" ) );
	cairo_show_text ( cr, caption );  
	g_free ( caption );
}
		
static void 
Draw_Nacelle ( 
		cairo_t *cr, 
		guint centerx, 
		guint centery, 
		guint radius )
{
	gfloat nacpos = 0.0;
	gfloat winddir = 0.0;
	if ( pdata->nacpos <= M_PI ) nacpos = M_PI - pdata->nacpos;
	else nacpos = 3*M_PI - pdata->nacpos;
	if ( pdata->winddir <= M_PI ) winddir = M_PI - pdata->winddir;
	else winddir = 3*M_PI - pdata->winddir;
	guint cx = (guint)( centerx + sin ( nacpos ) * radius / 3 );
	guint cy = (guint)( centery + cos ( nacpos ) * radius / 3 );
	guint rx = (guint)( centerx - sin ( nacpos ) * radius / 3 );
	guint ry = (guint)( centery - cos ( nacpos ) * radius / 3 );

	cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
	cairo_set_line_width(cr, radius/3);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE); 
	cairo_move_to(cr, centerx, centery); 
	cairo_line_to(cr, rx, ry);
	cairo_stroke(cr);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 
	cairo_move_to(cr, centerx, centery); 
	cairo_line_to(cr, cx, cy);
	cairo_stroke(cr);

	cx = (guint)( centerx + sin ( nacpos-M_PI/3 ) * radius / 2 );
	cy = (guint)( centery + cos ( nacpos-M_PI/3 ) * radius / 2 );
	rx = (guint)( centerx + sin ( nacpos+M_PI/3 ) * radius / 2 );
	ry = (guint)( centery + cos ( nacpos+M_PI/3 ) * radius / 2 );
	cairo_set_line_width(cr, radius/9);
	cairo_move_to(cr, cx, cy); 
	cairo_line_to(cr, rx, ry);
	cairo_stroke(cr);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE); 
	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_set_line_width(cr, 10);
	cairo_arc(cr, centerx, centery, radius-15, nacpos-abs(pdata->yawerr), nacpos+abs(pdata->yawerr) );
//	g_print ( "%f - %f - %f == %f\n", nacpos, pdata->yawerr, nacpos-abs(pdata->yawerr), nacpos+abs(pdata->yawerr) );
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	guint x = (guint)( centerx + sin ( winddir ) * ( radius - 10 ) );
	guint y = (guint)( centery + cos ( winddir ) * ( radius - 10 ) );
	cairo_arc(cr, x, y, 5, 0, 2*M_PI);
    cairo_fill(cr);

	cairo_set_line_width(cr, 1);
}

static gboolean 
DrawAlarmCode (
		GtkWidget *widget, 
		cairo_t *cr, 
    	gpointer user_data )
{
	double dash[2] = { 1.0, 2.0 };
	guint width = gtk_widget_get_allocated_width (widget);
	guint height = gtk_widget_get_allocated_height (widget);
	guint radius = MIN (width, height) / 2. - 30;

	cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
	cairo_select_font_face ( cr, "LiHei Pro",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size ( cr, 14 );

	cairo_move_to ( cr, 20, 30 );
	cairo_show_text ( cr, "报警码" );  
	cairo_move_to(cr, 20, 40 ); 
	cairo_line_to(cr, width-20, 40 );

	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	cairo_set_line_width(cr, 1);
	cairo_set_dash ( cr, dash, 2, 0 );
	for ( int i=1; i<9; i++ ) {
		guint x1 = 20;
		guint y1 = 40+i*30;
		guint x2 = width - 20;
		guint y2 = 40+i*30;
		cairo_move_to(cr, x1, y1); 
		cairo_line_to(cr, x2, y2);
		cairo_move_to ( cr, x1+10, y1-10 );
		cairo_show_text ( cr, "报警码" );  
	}
	cairo_stroke(cr);

  	return FALSE;
}


static gboolean 
DrawWindDir (
		GtkWidget *widget, 
		cairo_t *cr, 
    	gpointer user_data )
{
	guint width = gtk_widget_get_allocated_width (widget);
	guint height = gtk_widget_get_allocated_height (widget);
	guint radius = MIN (width, height) / 2. - 30;

	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	cairo_set_line_width(cr, 2);
	cairo_arc(cr, width/2., height/2., radius - 10, 0, 2*M_PI);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
	cairo_set_line_width(cr, 1);
	cairo_arc(cr, width/2., height/2., radius, 0, 2*M_PI);
	cairo_stroke(cr);

	for ( int i=0; i<360; i+=3 ) {
		if ( i % 30 == 0 ) continue;
		guint x1 = (guint)( width/2 + sin ( M_PI*i/180 ) * ( radius - 10 ) );
		guint y1 = (guint)( height/2 + cos ( M_PI*i/180 ) * ( radius - 10 ) );
		guint x2 = (guint)( width/2 + sin ( M_PI*i/180 ) * radius );
		guint y2 = (guint)( height/2 + cos ( M_PI*i/180 ) * radius );
		cairo_move_to(cr, x1, y1); 
		cairo_line_to(cr, x2, y2);
	}
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	cairo_set_line_width(cr, 2);
	for ( int i=0; i<360; i+=30 ) {
		guint x1 = (guint)( width/2 + sin ( M_PI*i/180 ) * ( radius - 10 ) );
		guint y1 = (guint)( height/2 + cos ( M_PI*i/180 ) * ( radius - 10 ) );
		guint x2 = (guint)( width/2 + sin ( M_PI*i/180 ) * ( radius + 3 ) );
		guint y2 = (guint)( height/2 + cos ( M_PI*i/180 ) * ( radius + 3 ) );
		cairo_move_to(cr, x1, y1); 
		cairo_line_to(cr, x2, y2);
	}
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_select_font_face ( cr, "LiHei Pro",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size ( cr, 10 );

	gint x3 = (gint)( width/2 + sin ( M_PI ) * ( radius + 8 ) );
	gint y3 = (gint)( height/2 + cos ( M_PI ) * ( radius + 8 ) );
	cairo_translate ( cr, x3, y3 );
	cairo_rotate ( cr, 0 );
	cairo_move_to ( cr, -20, 0 );
	cairo_show_text ( cr, "360°0°" );  

	x3 = (gint) ( sin ( M_PI/6 ) * ( radius + 8 ) );
	y3 = (gint) ( ( 1 - cos ( M_PI/6 ) ) * ( radius + 8 ) );

	for ( int i=0; i<330; i+=30 ) {
		gchar *caption = g_strdup_printf ( "%d°", i+30 );
		cairo_translate ( cr, x3, y3 );
		cairo_rotate ( cr, M_PI/6 );
		cairo_move_to ( cr, -6, 0 );
		cairo_show_text ( cr, caption );
		g_free ( caption );
	}
	cairo_translate ( cr, x3, y3 );
	cairo_rotate ( cr, M_PI/6 );
	x3 = - (gint)( width/2 + sin ( M_PI ) * ( radius + 8 ) );
	y3 = - (gint)( height/2 + cos ( M_PI ) * ( radius + 8 ) );
	cairo_translate ( cr, x3, y3 );

	Draw_Nacelle ( cr, width/2., height/2., radius );

  	return FALSE;
}

static gboolean 
DrawRealCurve (
		GtkWidget *widget, 
		cairo_t *cr, 
    	RealCurve *rc )
{
	guint width = gtk_widget_get_allocated_width (widget);
	guint height = gtk_widget_get_allocated_height (widget);
	double dash[2] = { 1.0, 2.0 };
	double indentx = 40;
	double indentytop = 20;
	double indentybottom = 20;
	double dely = ( height - indentytop - indentybottom ) / 5;
	double delx = ( width - indentx - 10 ) / 10;

	cairo_select_font_face ( cr, "YaHei Consolas Hybrid",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL );
	cairo_set_font_size ( cr, 14 );

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	gchar *caption = g_strdup_printf ( "%s %-5.1f 最大值:%-5.1f 最小值:%-5.1f", rc->name, *(rc->value), rc->maxv, rc->minv );
	cairo_move_to ( cr, 40, 15 );
	cairo_show_text ( cr, caption );
	g_free ( caption );

	cairo_set_font_size ( cr, 10 );

	cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
	cairo_set_line_width ( cr, 1.5 );
	double px = ( width - indentx ) / POINTS;
	double realheight = ( height - indentytop - indentybottom );
	double span = rc->max - rc->min;

	if ( pdata->start ) {
		if ( rc->update ) {
			rc->update = FALSE;
			rc->data[rc->index++] = *(rc->value);
//			g_print ( "index:%4d - %f\n", rc->index, *(rc->value) );
			if  ( rc->minv > *(rc->value ) ) rc->minv = *(rc->value );
			if  ( rc->maxv < *(rc->value ) ) rc->maxv = *(rc->value );
			if ( rc->index >= POINTS ) {
				rc->index = 0;
				rc->start = FALSE;
			}
		}
		if ( rc->start ) {
			cairo_move_to ( cr, 
					(guint)(indentx + ( POINTS - rc->index ) * px), 
					(guint)(indentytop + ( 1.0f - (rc->data[0]-rc->min)/span ) * realheight ));
			for ( int i=1; i<rc->index; i++ ) {
				cairo_line_to ( cr, 
					(guint)(indentx + ( POINTS - rc->index + i ) * px), 
					(guint)(indentytop + ( 1.0f - (rc->data[i]-rc->min)/span ) * realheight ));
			}
		} else {
			cairo_move_to ( cr, indentx, 
					indentytop + ( 1.0f - (rc->data[rc->index]-rc->min)/span ) * realheight );
			for ( int i=1; i<POINTS; i++ ) {
				int ii = ( rc->index + i ) % POINTS;
				cairo_line_to ( cr, 
					indentx + i * px, 
					indentytop + ( 1.0f - (rc->data[ii]-rc->min)/span ) * realheight );
			}
		}
	}
	cairo_stroke ( cr );


	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	cairo_set_line_width ( cr, 1.0 );
	cairo_set_dash ( cr, dash, 2, 0 );

	for ( int i=0; i<=5; i++ ) {
		gchar *caption = g_strdup_printf ( "%5.0f", rc->max - i * ( (rc->max-rc->min) / 5 ) );
		cairo_move_to ( cr, 0, indentytop + i * dely + 3 );
		cairo_show_text ( cr, caption );
		g_free ( caption );

		cairo_move_to ( cr, indentx, indentytop + i * dely );
		cairo_line_to ( cr, width - 10, indentytop + i* dely );
	}

	for ( int i=0; i<=10; i++ ) {
		if ( i==0 ) {
			gchar *caption = g_strdup_printf ( "时间(s)" );
			cairo_move_to ( cr, indentx + i*delx, height - indentybottom + 10 );
			cairo_show_text ( cr, caption );
			g_free ( caption );
		} else {
			gchar *caption = g_strdup_printf ( "%d", 6*(10-i) );
			cairo_move_to ( cr, indentx + i*delx - 3, height - indentybottom + 10 );
			cairo_show_text ( cr, caption );
			g_free ( caption );
		}

		cairo_move_to ( cr, indentx + i*delx, indentytop );
		cairo_line_to ( cr, indentx + i*delx, height - indentybottom );
	}
	cairo_stroke ( cr );
}

static gboolean 
DrawSetWindDir (
		GtkWidget *widget, 
		cairo_t *cr, 
    	gpointer user_data )
{
	guint width = gtk_widget_get_allocated_width (widget);
	guint height = gtk_widget_get_allocated_height (widget);
	guint radius = MIN (width, height) / 2. - 15;

	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	cairo_set_line_width(cr, 1);
	cairo_arc(cr, width/2., height/2., radius, 0, 2*M_PI);
	cairo_stroke(cr);

	guint x = (guint)( width/2 + sin ( setwinddir ) * radius );
	guint y = (guint)( height/2 + cos ( setwinddir ) * radius );
	cairo_arc(cr, x, y, 5, 0, 2*M_PI);
    cairo_fill(cr);
}

GtkWidget *createCurve ( gchar *title, 
				gint max, 
				gint min,
				gfloat *value )
{
	RealCurve *rc = g_slice_new0 ( RealCurve );
	Curves = g_list_append ( Curves, rc );
	rc->name = title;
	rc->max = max;
	rc->min = min;
	rc->index = 0;
	rc->start = TRUE;
	rc->update = FALSE;
	rc->value = value;
	GtkWidget *result = gtk_drawing_area_new();
	gtk_widget_set_hexpand ( result, TRUE );
	gtk_widget_set_vexpand ( result, TRUE );
	g_signal_connect(G_OBJECT(result), "draw", G_CALLBACK(DrawRealCurve), rc);  
	gtk_widget_set_events ( result, gtk_widget_get_events ( result ) 
			| GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK
			| GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );

	return result;
}

GtkWidget *createPage ( gint index )
{
	GtkWidget *curvegrid = gtk_grid_new ();
	if ( index == 0 ) {
		wWindSpd = createCurve ( "风速(m/s)", 30, 0, &(pdata->windspd) );
		wGenSpd = createCurve ( "发电机转速(rpm)", 1500, 0, &(pdata->genspd) );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wWindSpd, 0, 0, 1, 1 );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wGenSpd, 0, 1, 1, 1 );
	} else if ( index == 1 ) {
		wPitchAngle = createCurve ( "桨距角(°)", 30, 0, &(pdata->pitchangle) );
		wGenPow = createCurve ( "发电机功率(Kw)", 15, 0, &(pdata->genpower) );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wPitchAngle, 0, 0, 1, 1 );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wGenPow, 0, 1, 1, 1 );
	} else if ( index == 2 ) {
		wTurbState = gtk_drawing_area_new();
		gtk_widget_set_hexpand ( wTurbState, TRUE );
		gtk_widget_set_vexpand ( wTurbState, TRUE );
		g_signal_connect(G_OBJECT(wTurbState), "draw", G_CALLBACK(DrawTurbState), NULL);  
		gtk_widget_set_events ( wTurbState, gtk_widget_get_events ( wTurbState ) 
				| GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK
				| GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wTurbState, 0, 0, 1, 1 );
	} else if ( index == 3 ) {
		wAlarmCode = gtk_drawing_area_new();
		gtk_widget_set_hexpand ( wAlarmCode, TRUE );
		gtk_widget_set_vexpand ( wAlarmCode, TRUE );
		g_signal_connect(G_OBJECT(wAlarmCode), "draw", G_CALLBACK(DrawAlarmCode), NULL);  
		gtk_widget_set_events ( wAlarmCode, gtk_widget_get_events ( wAlarmCode ) 
				| GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK
				| GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wAlarmCode, 0, 0, 1, 1 );

		/*
		wWindDir = gtk_drawing_area_new();
		gtk_widget_set_hexpand ( wWindDir, TRUE );
		gtk_widget_set_vexpand ( wWindDir, TRUE );
		g_signal_connect(G_OBJECT(wWindDir), "draw", G_CALLBACK(DrawWindDir), NULL);  
		gtk_widget_set_events ( wWindDir, gtk_widget_get_events ( wWindDir ) 
				| GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK
				| GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
		gtk_grid_attach ( GTK_GRID ( curvegrid ), wWindDir, 0, 0, 1, 1 );
		*/
	}

	return curvegrid;
}
