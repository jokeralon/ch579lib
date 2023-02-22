#ifndef __LOG_H
#define __LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "CH579SFR.h"
#include "CH57x_common.h"

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"
 
#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K" //or "\e[1K\r"

#define LEVEL_NONE 0
#define LEVEL_ERROR 1
#define LEVEL_WARN 2
#define LEVEL_INFO 3
#define LEVEL_DEBUG 4

#define LOG_LEVEL LEVEL_DEBUG


int bsp_log_send_write(int level, const char *__format, ...);

// windows:
// #define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux:
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

// #define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
// #define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %s(%d)<%s()> :"fmt"\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

// #if LOG_LEVEL == LOG_ERROR
// #define LOG_ERROR(fmt, ...)
// #define LOG_WARN(fmt, ...)
// #define LOG_INFO(fmt, ...)
// #define LOG_DEBUG(fmt, ...)

// #elif LOG_LEVEL == LOG_ERROR
// #define LOG_ERROR(fmt, ...) PRINT("[ERROR] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_WARN(fmt, ...)
// #define LOG_INFO(fmt, ...)
// #define LOG_DEBUG(fmt, ...)

// #elif LOG_LEVEL == LEVEL_WARN
// #define LOG_ERROR(fmt, ...) PRINT("[ERROR] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_WARN(fmt, ...) PRINT("[WARN ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_INFO(fmt, ...)
// #define LOG_DEBUG(fmt, ...)

// #elif LOG_LEVEL == LEVEL_INFO
// #define LOG_ERROR(fmt, ...) PRINT("[ERROR] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_WARN(fmt, ...) PRINT("[WARN ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_INFO(fmt, ...) PRINT("[INFO ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_DEBUG(fmt, ...)

// #elif LOG_LEVEL == LEVEL_DEBUG
// #define LOG_ERROR(fmt, ...) PRINT("[ERROR] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_WARN(fmt, ...) PRINT("[WARN ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_INFO(fmt, ...) PRINT("[INFO ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #define LOG_DEBUG(fmt, ...) PRINT("[DEBUG] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
// #endif

#define LOG_ERROR(fmt, ...)     bsp_log_send_write(LEVEL_ERROR, RED"[ERROR] %-17s(%d):" fmt NONE, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)      bsp_log_send_write(LEVEL_WARN, YELLOW"[WARN ] %-17s(%d):" fmt NONE, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)      bsp_log_send_write(LEVEL_INFO, GREEN"[INFO ] %-17s(%d):" fmt NONE, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)     bsp_log_send_write(LEVEL_DEBUG, BLUE"[DEBUG] %-17s(%d):" fmt NONE, filename(__FILE__), __LINE__, ##__VA_ARGS__)

#endif
