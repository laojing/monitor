#include "../Gui/ProjectData.h" 

guint
GetAlarmLevel ( gint index )
{
    if ( index < 0 ) {
    } else if ( index == 0 ) {
        return A_NORMALSTOP;
    } else if ( index == 1 ) {
        return A_NORMALSTOP;
    } else if ( index == 2 ) {
        return A_NORMALSTOP;
    } else if ( index == 3 ) {
        return A_NORMALSTOP;
    } else if ( index == 4 ) {
        return A_NORMALSTOP;
    } else if ( index == 5 ) {
        return A_NORMALSTOP;
    } else if ( index == 6 ) {
        return A_NORMALSTOP;
    } else if ( index == 7 ) {
        return A_NORMALSTOP;
    } else if ( index == 8 ) {
        return A_NORMALSTOP;
    } else if ( index == 9 ) {
        return A_NORMALSTOP;
    } else if ( index == 10 ) {
        return A_NORMALSTOP;
    } else if ( index == 11 ) {
        return A_NORMALSTOP;
    } else if ( index == 12 ) {
        return A_NORMALSTOP;
    } else if ( index == 13 ) {
        return A_NORMALSTOP;
    } else if ( index == 14 ) {
        return A_NORMALSTOP;
    } else if ( index == 15 ) {
        return A_NORMALSTOP;
    } else if ( index == 16 ) {
        return A_WARNING;
    } else if ( index == 17 ) {
        return A_NORMALSTOP;
    }
};
gchar*
GetAlarmName ( gint index )
{
    if ( index < 0 ) {
    } else if ( index == 0 ) {
        return "转速过低停机";
    } else if ( index == 1 ) {
        return "变流器跳闸停机";
    } else if ( index == 2 ) {
        return "风速过低停机";
    } else if ( index == 3 ) {
        return "风速过高停机";
    } else if ( index == 4 ) {
        return "主电源正常";
    } else if ( index == 5 ) {
        return "2级安全链正常";
    } else if ( index == 6 ) {
        return "安全链延迟开关";
    } else if ( index == 7 ) {
        return "安全链主断路器";
    } else if ( index == 8 ) {
        return "安全链主振动开关";
    } else if ( index == 9 ) {
        return "安全链控制柜急停";
    } else if ( index == 10 ) {
        return "1级安全链正常";
    } else if ( index == 11 ) {
        return "顺时针扭缆极限报警";
    } else if ( index == 12 ) {
        return "逆时针扭缆极限报警";
    } else if ( index == 13 ) {
        return "正在解缆";
    } else if ( index == 14 ) {
        return "机舱位置意外移动";
    } else if ( index == 15 ) {
        return "操作台停止命令";
    } else if ( index == 16 ) {
        return "手动模式按钮";
    } else if ( index == 17 ) {
        return "操作锁";
    }
};
gchar*
GetLevelName ( guint level )
{
    if ( level == A_LOG ) { 			
        return "日志信息"; 			
    } else if ( level == A_WARNING ) { 			
        return "警告信息"; 			
    } else if ( level == A_FORBIDSTART ) { 			
        return "禁止启动"; 			
    } else if ( level == A_NORMALSTOP ) { 			
        return "正常停机"; 			
    } else if ( level == A_GRIDERROR ) { 			
        return "电网故障"; 			
    } else if ( level == A_FASTSTOP ) { 			
        return "快速停机"; 			
    } else if ( level == A_BATSTOP ) { 			
        return "电池收桨"; 			
    } else if ( level == A_MANUALERROR ) { 			
        return "手动故障"; 			
    } else if ( level == A_TURBPROTECT ) { 			
        return "风机保护"; 			
    } else if ( level == A_EMERGENCY ) { 			
        return "紧急停机"; 			
    }
}
