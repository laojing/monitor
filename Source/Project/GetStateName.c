#include "../Gui/ProjectData.h" 

gchar *
GetStateName ( gint state, gchar *machine )
{
    if ( g_ascii_strcasecmp ( machine, "Oper" ) == 0 ) {
        if ( state == 0 ) {
            return "上电初始化";
        } else if ( state == 1 ) {
            return "上电变桨距";
        } else if ( state == 2 ) {
            return "上电变流器";
        } else if ( state == 3 ) {
            return "上电延时中";
        } else if ( state == 4 ) {
            return "检查状态码";
        } else if ( state == 5 ) {
            return "启动变桨距";
        } else if ( state == 6 ) {
            return "启动变流器";
        } else if ( state == 7 ) {
            return "选工作模式";
        } else if ( state == 8 ) {
            return "手动速度升";
        } else if ( state == 9 ) {
            return "手动功率升";
        } else if ( state == 10 ) {
            return "速度上升中";
        } else if ( state == 11 ) {
            return "手动速度中";
        } else if ( state == 12 ) {
            return "手动功率中";
        } else if ( state == 13 ) {
            return "正常发电中";
        } else if ( state == 14 ) {
            return "手动初始化";
        } else if ( state == 15 ) {
            return "手动运行中";
        } else if ( state == 16 ) {
            return "手动退出中";
        } else if ( state == 17 ) {
            return "正常停机中";
        } else if ( state == 18 ) {
            return "失网停机中";
        } else if ( state == 19 ) {
            return "快速停机中";
        } else if ( state == 20 ) {
            return "电池停机中";
        } else if ( state == 21 ) {
            return "紧急停机中";
        } else if ( state == 22 ) {
            return "触发紧急停机";
        } else if ( state == 23 ) {
            return "触发电池停机";
        } else if ( state == 24 ) {
            return "触发快速停机";
        } else if ( state == 25 ) {
            return "触发失网停机";
        } else if ( state == 26 ) {
            return "触发正常停机";

        }
    } else if ( g_ascii_strcasecmp ( machine, "Loop" ) == 0 ) {
        if ( state == 0 ) {
            return "上电初始化";
        } else if ( state == 1 ) {
            return "上电完成";
        } else if ( state == 2 ) {
            return "手动速度进入";
        } else if ( state == 3 ) {
            return "手动速度运行";
        } else if ( state == 4 ) {
            return "手动功率进入";
        } else if ( state == 5 ) {
            return "手动功率运行";
        } else if ( state == 6 ) {
            return "进入启动";
        } else if ( state == 7 ) {
            return "开环升速";
        } else if ( state == 8 ) {
            return "闭环升速";
        } else if ( state == 9 ) {
            return "并网";
        } else if ( state == 10 ) {
            return "正常发电中";
        } else if ( state == 11 ) {
            return "进入正常停机";
        } else if ( state == 12 ) {
            return "脱网";
        } else if ( state == 13 ) {
            return "收桨";
        } else if ( state == 14 ) {
            return "正常停机完成";
        } else if ( state == 15 ) {
            return "触发正常停机";

        }
    } else if ( g_ascii_strcasecmp ( machine, "Pich" ) == 0 ) {
        if ( state == 0 ) {
            return "上电初始化";
        } else if ( state == 1 ) {
            return "上电完成";
        } else if ( state == 2 ) {
            return "寻找定位原点";
        } else if ( state == 3 ) {
            return "定位完成";
        } else if ( state == 4 ) {
            return "进入使能";
        } else if ( state == 5 ) {
            return "使能";
        } else if ( state == 6 ) {
            return "给定变桨位置";
        } else if ( state == 7 ) {
            return "给启动命令";
        } else if ( state == 8 ) {
            return "给停止命令";
        } else if ( state == 9 ) {
            return "手动等命令";
        } else if ( state == 10 ) {
            return "手动开桨";
        } else if ( state == 11 ) {
            return "手动关桨";

        }
    } else if ( g_ascii_strcasecmp ( machine, "Conv" ) == 0 ) {
        if ( state == 0 ) {
            return "上电初始化";
        } else if ( state == 1 ) {
            return "上电完成";
        } else if ( state == 2 ) {
            return "待机初始化";
        } else if ( state == 3 ) {
            return "待机完成";
        } else if ( state == 4 ) {
            return "开始同步";
        } else if ( state == 5 ) {
            return "并网完成";
        } else if ( state == 6 ) {
            return "变流器脱网";
        } else if ( state == 7 ) {
            return "清除错误延时";
        } else if ( state == 8 ) {
            return "清除错误";
        } else if ( state == 9 ) {
            return "触发变流器错误";

        }
    } else if ( g_ascii_strcasecmp ( machine, "Wind" ) == 0 ) {
    } else if ( g_ascii_strcasecmp ( machine, "Ctrl" ) == 0 ) {
    } else if ( g_ascii_strcasecmp ( machine, "Drag" ) == 0 ) {
        if ( state == 0 ) {
            return "待机";
        } else if ( state == 1 ) {
            return "工作";
        } else if ( state == 2 ) {
            return "错误";
        }
    } else if ( g_ascii_strcasecmp ( machine, "Grid" ) == 0 ) {

    }
	return " ";
}
