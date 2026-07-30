typedef unsigned long u32;
extern u32 arr[];
u32 f(u32 n, u32 m) {
    u32 s = 0;
    for (u32 i = 0; i < n; i++) {
        u32 j = 0;
        if (m != 0) {
            do { s += arr[j]; j++; } while (j < m);
        }
    }
    return s;
}
