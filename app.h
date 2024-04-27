#ifndef APP_H_
#define APP_H_

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef int i32;
typedef int b32;
typedef float f32;

typedef enum {
     DEF_CELL= 0,
     DEF_SIGNAL
} Def;

typedef struct {
    Def type;
    u32 initial;
} NodeRef;

#define DEFS_CAP (100)
typedef struct {
    NodeRef items[DEFS_CAP];
    u32 begin;
    u32 size;
} Defs;


void platform_panic(const char *file_path, i32 line, const char *message);
void platform_log(const char *message);

void app_init(u32 width, u32 height);
u32 getValue(u32 node);
u32 cell(u32 value);
void foo(u32 value);

#endif // APP_H_
