#ifndef __PROJECTDATA_H__
#define __PROJECTDATA_H__

#include <gtk/gtk.h>


#define A_NONE          0
#define A_LOG           1
#define A_WARNING       2
#define A_FORBIDSTART   3
#define A_NORMALSTOP    4
#define A_GRIDERROR     5
#define A_FASTSTOP      6
#define A_BATSTOP       7
#define A_MANUALERROR   8
#define A_TURBPROTECT   9
#define A_EMERGENCY     10

#define PAGES 4

#define WIDTH 600
#define HEIGHT 400

#define POINTS 60

typedef struct _AlarmItem AlarmItem;
struct _AlarmItem
{
	guint id;
	gboolean active;
	guint level;
	gchar *acttime;
};

typedef struct _ProjectData ProjectData;
struct _ProjectData
{
	guint hardware;
	gboolean start;
	GList *actalarms;
	GList *alarms;
	guint actlev;
	guint actalarm;
	guint oper;
	guint loop;
	guint pich;
	guint conv;
	guint yaws;
	guint yawd;
	gfloat winddir;
	gfloat nacpos;
	gfloat yawerr;
	gfloat twist;
	gfloat windspd;
	gfloat pitchangle;
	gfloat genpower;
	gfloat genspd;
};

extern ProjectData *pdata;

typedef struct _RealCurve RealCurve;
struct _RealCurve
{
	gfloat max;
	gfloat min;
	gfloat maxv;
	gfloat minv;
	gfloat *value;
	gchar *name;
	gfloat data[POINTS];
	guint index;
	guint start;
	gboolean update;
};

gchar *GetStateName ( gint state, gchar *machien );
void allocProjectData ();
GtkWidget *createOverall ();
void UpdateAll ();
GtkWidget *createPage ( gint index );

#endif /* __PROJECTDATA_H__ */
