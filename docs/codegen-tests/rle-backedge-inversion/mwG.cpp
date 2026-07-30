typedef unsigned long u32;
extern u32 arr[];
u32 f(u32 n, u32 m) {
    u32 s = 0;
    for (u32 i = n; i != (u32)-1; i--) if (m) { u32 t = arr[i]; s += t; }
    return s;
}
