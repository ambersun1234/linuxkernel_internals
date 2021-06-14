#include <pthread.h>
typedef void *(*thread_func_t)(void *);

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <time.h>

#ifndef CROUTINE_CHAN
    #include "chan.c"
#endif

#define mutext_counter 4

enum {
    MSG_MAX = 100000,
    THREAD_MAX = 1024,
};

struct thread_arg {
    size_t id;
    size_t from, to;
    struct chan *ch;
};

static pthread_t reader_tids[THREAD_MAX], writer_tids[THREAD_MAX];
struct thread_arg reader_args[THREAD_MAX], writer_args[THREAD_MAX];
static _Atomic size_t msg_total, msg_count[MSG_MAX];

static pthread_mutex_t mutex[mutext_counter];
static _Atomic time_t sender_seconds[MSG_MAX];
static _Atomic long sender_nano[MSG_MAX];
static _Atomic time_t reader_seconds[MSG_MAX];
static _Atomic long reader_nano[MSG_MAX];

static void *writer(void *arg)
{
    struct thread_arg *a = arg;
    struct timespec tmp;

    for (size_t i = a->from; i < a->to; i++) {
        clock_gettime(CLOCK_MONOTONIC, &tmp);
        if (chan_send(a->ch, (void *) i) == -1) break;
        
        pthread_mutex_lock(&mutex[0]);
        atomic_store_explicit(&sender_seconds[i], tmp.tv_sec, memory_order_relaxed);
        pthread_mutex_unlock(&mutex[0]);
        pthread_mutex_lock(&mutex[1]);
        atomic_store_explicit(&sender_nano[i], tmp.tv_nsec, memory_order_relaxed);
        pthread_mutex_unlock(&mutex[1]);
    }
    return 0;
}

static void *reader(void *arg)
{
    struct thread_arg *a = arg;
    size_t msg, received = 0, expect = a->to - a->from;
    struct timespec tmp;

    while (received < expect) {
        clock_gettime(CLOCK_MONOTONIC, &tmp);
        if (chan_recv(a->ch, (void **) &msg) == -1) break;

        pthread_mutex_lock(&mutex[2]);
        atomic_store_explicit(&reader_seconds[msg], tmp.tv_sec, memory_order_relaxed);
        pthread_mutex_unlock(&mutex[2]);
        pthread_mutex_lock(&mutex[3]);
        atomic_store_explicit(&reader_nano[msg], tmp.tv_nsec, memory_order_relaxed);
        pthread_mutex_unlock(&mutex[3]);

        atomic_fetch_add_explicit(&msg_count[msg], 1, memory_order_relaxed);

        ++received;
    }
    return 0;
}

static long diff_in_ns(struct timespec start, struct timespec end) {
    struct timespec diff;
    if (end.tv_nsec - start.tv_nsec < 0) {
        diff.tv_sec = end.tv_sec - start.tv_sec - 1;
        diff.tv_nsec = end.tv_nsec - start.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = end.tv_sec - start.tv_sec;
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    
    return (diff.tv_sec * 1000000000.0 + diff.tv_nsec);
}

void writeTime(int msg_total, FILE *fp) {
    if (fp == NULL) return;

    for (size_t i = 0; i < msg_total; i++) {
        struct timespec start, end;
        start.tv_sec = sender_seconds[i];
        start.tv_nsec = sender_nano[i];
        end.tv_sec = reader_seconds[i];
        end.tv_nsec = reader_nano[i];

        long time_used = diff_in_ns(start, end);
        fprintf(fp, "%ld\n", time_used);
    }
}

static void create_threads(const size_t n,
                           thread_func_t fn,
                           struct thread_arg *args,
                           pthread_t *tids,
                           struct chan *ch)
{
    size_t each = msg_total / n, left = msg_total % n;
    size_t from = 0;

    for (size_t i = 0; i < n; i++) {
        size_t batch = each;

        if (left > 0) {
            batch++;
            left--;
        }
        args[i] = (struct thread_arg){
            .id = i,
            .ch = ch,
            .from = from,
            .to = from + batch,
        };
        pthread_create(&tids[i], NULL, fn, &args[i]);
        from += batch;
    }
}

static void join_threads(const size_t n, pthread_t *tids)
{
    for (size_t i = 0; i < n; i++) pthread_join(tids[i], NULL);
}

static void test_chan(const size_t repeat,
                      const size_t cap,
                      const size_t total,
                      const size_t n_readers,
                      thread_func_t reader_fn,
                      const size_t n_writers,
                      thread_func_t writer_fn)
{
    if (n_readers > THREAD_MAX || n_writers > THREAD_MAX)
        errx(1, "too many threads to create");
    if (total > MSG_MAX) errx(1, "too many messages to send");

    struct chan *ch = chan_make(cap, malloc);
    if (!ch) errx(1, "fail to create channel");

    msg_total = total;
    for (size_t rep = 0; rep < repeat; rep++) {
        printf("cap=%zu readers=%zu writers=%zu msgs=%zu ... %zu/%zu\n", cap,
               n_readers, n_writers, msg_total, rep + 1, repeat);

        memset(msg_count, 0, sizeof(size_t) * msg_total);
        memset(sender_seconds, 0, sizeof(time_t) * msg_total);
        memset(sender_nano, 0, sizeof(long) * msg_total);
        memset(reader_seconds, 0, sizeof(time_t) * msg_total);
        memset(reader_nano, 0, sizeof(long) * msg_total);
        create_threads(n_readers, reader_fn, reader_args, reader_tids, ch);
        create_threads(n_writers, writer_fn, writer_args, writer_tids, ch);
        join_threads(n_readers, reader_tids);
        join_threads(n_writers, writer_tids);

        for (size_t i = 0; i < msg_total; i++) assert(msg_count[i] == 1);

        FILE *fp = NULL;
        fp = fopen("unbuffer.txt", "w");
        // fp = fopen("buffer.txt", "w");
        writeTime(msg_total, fp);
        fclose(fp);
    }

    chan_close(ch);
    free(ch);
}

void init_mutex() {
    for (int i = 0; i < mutext_counter; i++) {
        pthread_mutex_init(&mutex[i], 0);
    }
}

void clear_mutex() {
    for (int i = 0; i < mutext_counter; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }
}

int main()
{
    init_mutex();

    // repeat, buffer size, msg size, reader count, reader function, writer count, writer function
    test_chan(100, 0, 5000, 100, reader, 1, writer);
    // test_chan(100, 7, 5000, 100, reader, 100, writer);

    clear_mutex();

    return 0;
}