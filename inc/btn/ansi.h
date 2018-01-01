#ifndef __BTN_ANSI_H__
#define __BTN_ANSI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This file contains macros for ansi terminal colors and control
 */
#define ANSI_RESET      "\x1B[0m"
#define ANSI_BOLD       "\x1B[1m"
#define ANSI_UNDER      "\x1B[4m"
#define ANSI_BLINK      "\x1B[5m"
#define ANSI_REVERSE    "\x1B[5m"
#define ANSI_CONCEALED  "\x1B[5m"

#define ANSI_F_BLK     "\x1B[30m"
#define ANSI_F_RED     "\x1B[31m"
#define ANSI_F_GRN     "\x1B[32m"
#define ANSI_F_YEL     "\x1B[33m"
#define ANSI_F_BLU     "\x1B[34m"
#define ANSI_F_MAG     "\x1B[35m"
#define ANSI_F_CYN     "\x1B[36m"
#define ANSI_F_WHT     "\x1B[37m"

#define ANSI_B_BLK     "\x1B[40m"
#define ANSI_B_RED     "\x1B[41m"
#define ANSI_B_GRN     "\x1B[42m"
#define ANSI_B_YEL     "\x1B[43m"
#define ANSI_B_BLU     "\x1B[44m"
#define ANSI_B_MAG     "\x1B[45m"
#define ANSI_B_CYN     "\x1B[46m"
#define ANSI_B_WHT     "\x1B[47m"

#ifdef __cplusplus
}
#endif

#endif//__BTN_ANSI_H__
