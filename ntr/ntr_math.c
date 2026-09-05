#include "ntr.h"
#include <math.h>

f32 ntr_get_distance(vec2f32 a, vec2f32 b)
{
    return sqrtf( powf((b.x - a.x), 2) + powf((b.y - a.y), 2)); 
}

f32 ntr_get_degree(vec2f32 a, vec2f32 b)
{
    f32 dx = b.x - a.x;
    f32 dy = b.y - a.y;
    if(dx) return atan2f(dy, dx) * 180.0f / PI;
    else   return NAN;
}
