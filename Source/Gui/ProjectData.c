#include <glib.h>
#include <glib/gi18n.h>
#include "ProjectData.h"

ProjectData *pdata;

void
allocProjectData () {
	pdata = g_slice_new0 ( ProjectData );
	pdata->hardware = 0;
	pdata->actlev = 2;
	pdata->actalarm = 2;
	pdata->oper = 0;
	pdata->loop = 0;
	pdata->pich = 0;
	pdata->conv = 0;
	pdata->yaws = 0;
	pdata->yawd = 0;

	pdata->winddir = 1.57/3;
	pdata->nacpos = 1.57/2;
	pdata->yawerr = 3.14/5;
	pdata->twist = 0;
	pdata->windspd = 0;
	pdata->pitchangle = 0;
	pdata->genpower = 0;
	pdata->genspd = 0;

	pdata->alarms = NULL;
	guint alarmlen = 0;
}


