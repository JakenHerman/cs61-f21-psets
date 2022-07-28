#define M61_DISABLE 1
#include "m61.hh"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>
#include <iostream>

m61_statistics g_stats = {0, 0, 0, 0, 0, 0, 0, 0};

struct header_information {
    size_t sz;
    header_information* next;
    header_information* prev;
    void* payload;
};

/// m61_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc must
///    return a unique, newly-allocated pointer value. The allocation
///    request was at location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings

    size_t size_with_header = sizeof(struct header_information) + sz;

    // Attempt to allocate memory.
    void* ptr = base_malloc(size_with_header);

    if (ptr == nullptr) {
        // Memory allocation failed.
        ++g_stats.nfail;
        g_stats.fail_size += sz;
        return nullptr;
    } else {
        // Memory succesfully allocated.
        ++g_stats.ntotal;
        ++g_stats.nactive;
        g_stats.total_size += sz;

        // attach header metadata to pointer
        header_information* header = (header_information*) ptr;
        header->sz = sz;
        header->next = nullptr;
        header->prev = nullptr;

        void *payload = (void*) ((char*) ptr + sizeof(struct header_information));
        header->payload = payload;
        
        g_stats.active_size += header->sz;

        if ((uintptr_t)&ptr <= g_stats.heap_min) {
            g_stats.heap_min = (uintptr_t) &ptr;
        }

        if ((uintptr_t)&ptr + sz >= g_stats.heap_max) {
            g_stats.heap_max = (uintptr_t) &ptr + sz;
        }
        return payload;
    }
}


/// m61_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to m61_malloc. If `ptr == NULL`,
///    does nothing. The free was called at location `file`:`line`.

void m61_free(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    
    if (ptr == nullptr) {
        return;
    }

    struct header_information *alloc_information = (struct header_information*) ptr - 1;

    if (alloc_information->payload == ptr) {
        // Track that the active size has decreased, as well as the active count.
        g_stats.active_size -= alloc_information->sz;
        --g_stats.nactive;

        base_free(alloc_information);
    }
}


/// m61_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. If `sz == 0`,
///    then must return a unique, newly-allocated pointer value. Returned
///    memory should be initialized to zero. The allocation request was at
///    location `file`:`line`.

void* m61_calloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test019).
    void* ptr = m61_malloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}


/// m61_get_statistics(stats)
///    Store the current memory statistics in `*stats`.

void m61_get_statistics(m61_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 0, sizeof(m61_statistics));
    stats->ntotal = g_stats.ntotal;
    stats->nactive = g_stats.nactive;
    stats->total_size = g_stats.total_size;
    stats->nfail = g_stats.nfail;
    stats->fail_size = g_stats.fail_size;
    stats->heap_min = g_stats.heap_min;
    stats->heap_max = g_stats.heap_max;
    stats->active_size = g_stats.active_size;
}


/// m61_print_statistics()
///    Print the current memory statistics.

void m61_print_statistics() {
    m61_statistics stats;
    m61_get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_print_leak_report()
///    Print a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_print_leak_report() {
    // Your code here.
}


/// m61_print_heavy_hitter_report()
///    Print a report of heavily-used allocation locations.

void m61_print_heavy_hitter_report() {
    // Your heavy-hitters code here
}
