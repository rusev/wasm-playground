#include "./app.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

static char logf_buf[4096] = {0};
#define LOGF(...) \
    do { \
        stbsp_snprintf(logf_buf, sizeof(logf_buf), __VA_ARGS__); \
        platform_log(logf_buf); \
    } while(0)

static void platform_assert(const char *file, i32 line, b32 cond, const char *message)
{
    if (!cond) platform_panic(file, line, message);
}
#define ASSERT(cond, message) platform_assert(__FILE__, __LINE__, cond, message)
#define UNREACHABLE() platform_panic(__FILE__, __LINE__, "unreachable")

#define ring_empty(ring) ((ring)->size == 0)

#define ring_cap(ring) (sizeof((ring)->items)/sizeof((ring)->items[0]))

#define ring_push_back(ring, item) \
    do { \
        ASSERT((ring)->size < ring_cap(ring), "Ring buffer overflow"); \
        u32 index = ((ring)->begin + (ring)->size)%ring_cap(ring); \
        (ring)->items[index] = (item); \
        (ring)->size += 1; \
    } while (0)

#define ring_displace_back(ring, item) \
    do { \
        u32 index = ((ring)->begin + (ring)->size)%ring_cap(ring); \
        (ring)->items[index] = (item); \
        if ((ring)->size < ring_cap(ring)) { \
            (ring)->size += 1; \
        } else { \
            (ring)->begin = ((ring)->begin + 1)%ring_cap(ring); \
        } \
    } while (0)

#define ring_pop_front(ring) \
    do { \
        ASSERT((ring)->size > 0, "Ring buffer underflow"); \
        (ring)->begin = ((ring)->begin + 1)%ring_cap(ring); \
        (ring)->size -= 1; \
    } while (0)

#define ring_back(ring) \
    (ASSERT((ring)->size > 0, "Ring buffer is empty"), \
     &(ring)->items[((ring)->begin + (ring)->size - 1)%ring_cap(ring)])
#define ring_front(ring) \
    (ASSERT((ring)->size > 0, "Ring buffer is empty"), \
     &(ring)->items[(ring)->begin])
#define ring_get(ring, index) \
    (ASSERT((ring)->size > 0, "Ring buffer is empty"), \
     &(ring)->items[((ring)->begin + (index))%ring_cap(ring)])
    


Defs defs = {0};

u32 getValue(u32 node) 
{
    return defs.items[node].initial;
//    for (u32 i = 0; i < defs.size; ++i) {
//         if (defs.items[i].uid == node.uid) {
//             return defs.items[i].initial;
//         }
//     } 

//     UNREACHABLE();
}

void foo(u32 value) 
{
    LOGF("foo %u", value);
}

u32 cell(u32 value) 
{
    LOGF("New Cell from WASM %u", value);
    NodeRef ref = {
        .type =  DEF_CELL,
        .initial = value,
    };

    ring_push_back(&defs, ref);
    LOGF("after New Cell from WASM %u", defs.size);


    return defs.size - 1;

    // return ref;
}

void app_init(u32 width, u32 height)
{
    LOGF("APP initialized %u %u", width, height);
    cell(42);
    // NodeRef node = cell(42);
    // LOGF("Cell from WASM %u, %u", defs.size, getValue(node));
}

// export function Cell<T>(value: T, init: (r: Realm) => void = noop, distinct: Distinct<T> = true): NodeRef<T> {
//   return tap(Symbol(), (id) => {
//     nodeDefs$$.set(id, { type: 'cell', distinct, initial: value, init })
//   }) as NodeRef<T>
// }