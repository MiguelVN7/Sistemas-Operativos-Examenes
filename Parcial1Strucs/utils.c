#include "utils.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

double now_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

size_t memoria_rss_kb() {
    
    FILE *f = fopen("/proc/self/statm","r");
    if (!f) return 0;
    long pages_total=0, pages_rss=0;
    if (fscanf(f, "%ld %ld", &pages_total, &pages_rss) != 2) { fclose(f); return 0; }
    fclose(f);
    long page_kb = sysconf(_SC_PAGESIZE) / 1024;
    return (size_t) (pages_rss * page_kb);
}
