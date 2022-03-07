#ifndef __LOG_H
#define __LOG_H

#include "CH579SFR.h"

#define 	LEVEL_NONE    0
#define 	LEVEL_ERROR 	1
#define 	LEVEL_WARN		2
#define   LEVEL_INFO		3
#define   LEVEL_DEBUG		4

#define LOG_LEVEL			LEVEL_INFO

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)
//#define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %s(%d)<%s()> :"fmt"\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#if LOG_LEVEL	== LOG_ERROR
#define LOG_ERROR(fmt, ...) 		
#define LOG_WARN(fmt, ...) 			
#define LOG_INFO(fmt, ...) 			
#define LOG_DEBUG(fmt, ...) 		

#elif LOG_LEVEL	== LOG_ERROR
#define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %-17s(%d):"fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) 			
#define LOG_INFO(fmt, ...) 			
#define LOG_DEBUG(fmt, ...) 		

#elif LOG_LEVEL == LEVEL_WARN
#define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %-17s(%d):"fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) 			PRINT("[WARN ] %-17s(%d):"fmt,  __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) 			
#define LOG_DEBUG(fmt, ...) 		

#elif LOG_LEVEL == LEVEL_INFO
#define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %-17s(%d):"fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) 			PRINT("[WARN ] %-17s(%d):"fmt,  __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) 			PRINT("[INFO ] %-17s(%d):"fmt,  __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) 

#elif LOG_LEVEL == LEVEL_DEBUG
#define LOG_ERROR(fmt, ...) 		PRINT("[ERROR] %-17s(%d):"fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) 			PRINT("[WARN ] %-17s(%d):"fmt,  __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) 			PRINT("[INFO ] %-17s(%d):"fmt,  __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) 		PRINT("[DEBUG] %-17s(%d):"fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#endif

#endif
