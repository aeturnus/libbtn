#ifndef __ETC_H__
#define __ETC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Miscellaneous functions and macros
 */

#define clamp(x, lo, hi) ((x <= lo) ? lo :\
                          (x >= hi) ? hi :\
                                      x)

#ifdef __cplusplus
}
#endif

#endif//__ETC_H__
