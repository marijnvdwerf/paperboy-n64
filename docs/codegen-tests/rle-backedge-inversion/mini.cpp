typedef unsigned long u32;
extern u32 arr[];
u32 f(u32 n) {
    u32 s = 0;
    for (u32 i = 0; i < n; i++) s += arr[i];
    return s;
}
