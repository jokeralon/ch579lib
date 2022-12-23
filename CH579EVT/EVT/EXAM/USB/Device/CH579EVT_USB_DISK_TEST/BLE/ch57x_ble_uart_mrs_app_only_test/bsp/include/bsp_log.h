#ifndef __LOG_H
#define __LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "CH579SFR.h"
#include "CH57x_common.h"

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

#define LOG_ERROR(fmt, ...)     bsp_log_send_write(LEVEL_ERROR, "[ERROR] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)      bsp_log_send_write(LEVEL_WARN,  "[WARN ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)      bsp_log_send_write(LEVEL_INFO, "[INFO ] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)     bsp_log_send_write(LEVEL_DEBUG, "[DEBUG] %-17s(%d):" fmt, filename(__FILE__), __LINE__, ##__VA_ARGS__)

#endif
