#ifndef KEYSYM2UCS_H
#define KEYSYM2UCS_H

#include <X11/X.h>

#ifdef __cplusplus
extern "C" {
#endif

long keysym2ucs(KeySym keysym);

#ifdef __cplusplus
}
#endif

#endif // KEYSYM2UCS_H
