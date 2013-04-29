
#include <doremir/audio_engine.h>
#include <doremir/priority_queue.h>
#include <doremir/thread.h>
#include <doremir/util.h>

#include <unistd.h> // for sysconf(3)


int  version[3] = { 2, 0, 0 };
char *bits      = sizeof(void *) == 4 ? "32-bit" : "64-bit";

void test_section(char *str)
{
    printf("\n\n--------------------\n");
    doremir_audio_engine_log_info(string_dappend(string("Running test: "), string(str)));
}


// --------------------------------------------------------------------------------

void test_value_references()
{
    extern char *doremir_type_str(doremir_ptr_t a);

    test_section("Value references");
    // FIXME leaks

    printf("bool:       %s\n", doremir_type_str(fb(true)));
    assert(tb(fb(true)) == true);
    assert(tb(fb(false)) == false);

    printf("int8:       %s\n", doremir_type_str(i8(62)));
    assert(ti8(i8('h')) == 'h');
    assert(ti8(i8(121)) == 121);
    assert(ti8(i8(-42)) == -42);

    printf("int16:      %s\n", doremir_type_str(i16(12372)));
    printf("int16:      %d\n", ti16(i16(1267)));
    assert(ti16(i16(1267)) == 1267);
    assert(ti16(i16(-8712)) == -8712);

    printf("int32:      %s\n", doremir_type_str(i32(12372)));
    printf("int32:      %d\n", ti32(i32(1267)));
    assert(ti32(i32(2147483646)) == 2147483646);
    assert(ti32(i32(-343646748)) == -343646748);

    printf("int64:      %s\n", doremir_type_str(i64(12372)));
    printf("int64:      %lli\n", ti64(i64(9223372036854775807ll)));
    assert(ti64(i64(4872837827878787871ll)) == 4872837827878787871ll);
    assert(ti64(i64(-6888881236767676711ll)) == -6888881236767676711ll);

    printf("double:     %s\n", doremir_type_str(f64(12372)));
    printf("double:     %f\n", tf64(f64(3.141592653589793)));
    assert(tf64(f64(3.141592653589793)) == 3.141592653589793);
    assert(tf64(f64(-1.4142135623730951)) == -1.4142135623730951);
}


// --------------------------------------------------------------------------------

void test_generic_functions()
{
    test_section("Generic functions");
    // TODO leaks

    printf("2 * 3.2                      ==> %f\n",   tf64(doremir_multiply(f64(2), f64(3.2))));
    printf("1 / 3                        ==> %f\n",   tf64(doremir_divide(f64(1), f64(3))));
    printf("1 + 1.5                      ==> %f\n",   tf64(doremir_add(f64(1), f64(1.5))));

    printf("32                  + 1      ==> %i\n",   ti8(doremir_add(i8(32), i8(1))));
    printf("5123                + 1      ==> %i\n",   ti16(doremir_add(i16(5123), i16(1))));
    printf("2147483646          + 1      ==> %i\n",   ti32(doremir_add(i32(2147483646), i32(1))));
    printf("4872837827878787871 + 1      ==> %lli\n", ti64(doremir_add(i64(4872837827878787871ll), i64(1))));
    printf("32                  - 1      ==> %i\n",   ti8(doremir_subtract(i8(32), i8(1))));
    printf("5123                - 1      ==> %i\n",   ti16(doremir_subtract(i16(5123), i16(1))));
    printf("2147483646          - 1      ==> %i\n",   ti32(doremir_subtract(i32(2147483646), i32(1))));
    printf("4872837827878787871 - 1      ==> %lli\n", ti64(doremir_subtract(i64(4872837827878787871ll), i64(1))));
    printf("3                   / 2      ==> %i\n",   ti8(doremir_divide(i8(33), i8(2))));
    printf("3333                / 2      ==> %i\n",   ti16(doremir_divide(i16(3333), i16(2))));
    printf("3333333333          / 2      ==> %i\n",   ti32(doremir_divide(i32(3333333333l), i32(2))));
    printf("3333333333333333333 / 2      ==> %lli\n", ti64(doremir_divide(i64(3333333333333333333ll), i64(2))));
    printf("3                   / 1      ==> %i\n",   ti8(doremir_divide(i8(32), i8(1))));

    printf("true == false                ==> %s\n", (doremir_equal(fb(true), fb(true))) ? "true" : false);
    printf("32   == 32                   ==> %s\n", (doremir_equal(i8(32), i8(32))) ? "true" : false);
    printf("5123 == 5123                 ==> %s\n", (doremir_equal(i16(5123), i16(5123))) ? "true" : false);


}



// --------------------------------------------------------------------------------

static inline void memdump(void *s, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%x ", *((unsigned char *)(s + i)));
    }

    printf("\n");
}

void test_string()
{
    test_section("Strings");
    {
        string_t s = doremir_string_single('v');
        doremir_dprint("str: %s\n", s);
    }

    {
        // char* cs = " 新隶体 "; // length 5
        char *cs = "höglund";

        string_t s = string(cs);
        printf("len: %i\n", doremir_string_length(s));
        doremir_print("str: %s\n", s);

        printf("charAt 0: %x\n", char_at(0, s));
        printf("charAt 1: %x\n", char_at(1, s));
        printf("charAt 2: %x\n", char_at(2, s));
        doremir_destroy(s);
    }

    {
        string_t s = string("foo");
        string_t t = string("bar");
        string_t u = string_append(s, t);
        doremir_dprint("str: %s\n", s);
        doremir_dprint("str: %s\n", t);
        doremir_dprint("str: %s\n", u);
    }

    {
        string_t s = string("foo");
        string_t t = string("bar");
        doremir_print("str: %s\n", s);
        doremir_print("str: %s\n", t);
        {
            string_t u = string_dappend(s, t);
            doremir_dprint("str: %s\n", u);
        }
    }
    {
        string_t s = string("Foo, Bar, Baz");
        string_t t = doremir_string_copy(s);
        doremir_dprint("str: %s\n", s);
        doremir_dprint("str: %s\n", t);
    }

    {
        string_t s = string("FooBarBaz");
        string_t t = doremir_string_join_map(apply1, doremir_string_single, s);
        doremir_dprint("str: %s\n", s);
        doremir_dprint("str: %s\n", t);
    }

    {
        string_t s = string("A double quote: \", A backslash: \\");
        doremir_dprint("str: %s\n", s);
    }
}


// --------------------------------------------------------------------------------

void test_show()
{
    test_section("Show");
    doremir_print("\n", NULL);
    doremir_dprint("%s\n", fb(0));
    doremir_dprint("%s\n", i8(129));
    doremir_dprint("%s\n", i16(129));
    doremir_dprint("%s\n", i32(64000));
    doremir_dprint("%s\n", f64(3.1415));
    doremir_dprint("%s\n", empty());
    doremir_dprint("%s\n", list(i8(1)));
    doremir_dprint("%s\n", list(i8(1), i8(2), list(i8(1), i8(2), fb(true))));
    doremir_dprint("%s\n", list(
                       pair(string("hans"), string("höglund")),
                       pair(string("lisa"), string("streich")),
                       pair(string("mats"), string("erlandsson"))));
}


// --------------------------------------------------------------------------------

void test_compare()
{
    test_section("Comparison");
    doremir_dprint("\"abc\" <  \"abd\"               ==> %s\n", fb(doremir_less_than(string("abc"), string("abd"))));
    doremir_dprint("\"abc\" <= \"abd\"               ==> %s\n", fb(doremir_less_than_equal(string("abc"), string("abd"))));
    doremir_dprint("\"abc\" >  \"abd\"               ==> %s\n", fb(doremir_greater_than(string("abc"), string("abd"))));
    doremir_dprint("\"abc\" >= \"abd\"               ==> %s\n", fb(doremir_less_than_equal(string("abc"), string("abd"))));
}


// --------------------------------------------------------------------------------

void test_rational()
{
    test_section("Rational numbers");
    doremir_dprint("1/3 <  1/2                   ==> %s\n", fb(doremir_less_than(ratio(1, 3), ratio(1, 2))));
    doremir_dprint("1/3 >  1/2                   ==> %s\n", fb(doremir_greater_than(ratio(1, 3), ratio(1, 2))));
    doremir_dprint("1/3 == 2/6                   ==> %s\n", fb(doremir_equal(ratio(1, 3), ratio(2, 6))));
    doremir_dprint("1/3 == 254/762               ==> %s\n", fb(doremir_equal(ratio(1, 3), ratio(254, 762))));
    doremir_dprint("1/3 <= 7/8                   ==> %s\n", fb(doremir_equal(ratio(1, 3), ratio(254, 762))));
}


// --------------------------------------------------------------------------------

void test_buffer()
{
    test_section("Buffers");

    {
        doremir_buffer_t b = doremir_buffer_create(16);

        doremir_print("b                            ==> %s\n", b);

        for (int i = 0; i < 16; ++i) {
            doremir_buffer_set(b, i, i);
        }

        doremir_print("b                            ==> %s\n", b);

        for (int i = 0; i < 16; ++i) {
            doremir_buffer_set(b, i, 0xff);
        }

        doremir_print("b                            ==> %s\n", b);
        doremir_print("size(b)                      ==> %s\n", i32(doremir_buffer_size(b)));
        doremir_destroy(b);
    }

    {
        doremir_buffer_t b = doremir_buffer_create(1024);

        doremir_print("b                            ==> %s\n", b);

        for (int i = 0; i < 1024; ++i) {
            doremir_buffer_set(b, i, i);
        }

        doremir_print("b                            ==> %s\n", b);

        for (int i = 0; i < 1024; ++i) {
            doremir_buffer_set(b, i, 0xff);
        }

        doremir_print("b                            ==> %s\n", b);
        doremir_print("size(b)                      ==> %s\n", i32(doremir_buffer_size(b)));
        doremir_destroy(b);
    }
}


// --------------------------------------------------------------------------------

void test_time()
{
    test_section("Time");

    doremir_time_t t = doremir_time_create(1, 0, 0, ratio(25, 8));
    doremir_time_t u = doremir_time_create(0, 1, 1, ratio(58, 1));

    doremir_print("t                            ==> %s\n", t);
    doremir_print("u                            ==> %s\n", u);
    doremir_print("t + u                        ==> %s\n", doremir_add(t, u));

    doremir_print("doremir_time_to_iso(t)       ==> %s\n", doremir_time_to_iso(t));
    doremir_print("doremir_time_to_iso(u)       ==> %s\n", doremir_time_to_iso(u));

    doremir_destroy(t);
    doremir_destroy(u);
}


// --------------------------------------------------------------------------------

void test_system_time()
{
    test_section("System time");

    clock_t system_clock = doremir_time_get_system_prec_clock();

    for (int i = 0; i < 10; ++i) {
        // doremir_print("system()                     ==> %s\n", doremir_time_system());
        // doremir_print("cpu()                        ==> %s\n", doremir_time_cpu());
        // doremir_print("system()                     ==> %s\n", doremir_time_from_system(doremir_time_system()));
        // doremir_print("cpu()                        ==> %s\n", doremir_time_from_cpu(doremir_time_cpu()));

        doremir_print("time(systemClock)            ==> %s\n", doremir_time_time(system_clock));
        doremir_print("ticks(systemClock)           ==> %s\n", i64(doremir_time_ticks(system_clock)));

        doremir_thread_sleep(50);
    }
}

// --------------------------------------------------------------------------------

void test_midi()
{
    test_section("Midi");

    {
        doremir_midi_t m = doremir_midi_create_simple(0xa, 60, 127);
        doremir_dprint("m                            ==> %s\n", m);
    }

    {
        doremir_buffer_t b = doremir_buffer_create(32);

        for (int i = 0; i < 32; ++i) {
            doremir_buffer_set(b, i, i);
        }

        doremir_midi_t m = doremir_midi_create_sysex(b);
        doremir_dprint("m                            ==> %s\n", m);
    }
}


// --------------------------------------------------------------------------------

void test_type()
{
    test_section("Types");

    doremir_dprint("type(i8)                  ==> %s\n", type(i8));
    doremir_dprint("size_of(1024,type(i8))    ==> %s\n", i32(doremir_type_size_of(1024, type(i8))));
    doremir_dprint("align_of(1024,type(i8))   ==> %s\n", i32(doremir_type_align_of(type(i8))));
    printf("\n");

    doremir_dprint("type(f64)                 ==> %s\n", type(f64));
    doremir_dprint("size_of(1024,type(f64))   ==> %s\n", i32(doremir_type_size_of(1024, type(f64))));
    doremir_dprint("align_of(1024,type(f64))  ==> %s\n", i32(doremir_type_align_of(type(f64))));
    printf("\n");

    type_t t = type_pair(type(i8), type(f64));
    doremir_dprint("t                            ==> %s\n", t);
    doremir_dprint("size_of(1024,t)              ==> %s\n", i32(doremir_type_size_of(1024, t)));
    doremir_dprint("align_of(1024,t)             ==> %s\n", i32(doremir_type_align_of(t)));
    printf("\n");

    type_t u = type_pair(type_vector(type(i8), 10), type(f64));
    doremir_dprint("u                            ==> %s\n", u);
    doremir_dprint("size_of(1024,u)              ==> %s\n", i32(doremir_type_size_of(1024, u)));
    doremir_dprint("align_of(1024,u)             ==> %s\n", i32(doremir_type_align_of(u)));
    printf("\n");

    type_t u2 = type_pair(type_frame(type(i8)), type(f64));
    doremir_dprint("u2                           ==> %s\n", u2);
    doremir_dprint("size_of(1024,u2)             ==> %s\n", i32(doremir_type_size_of(1024, u2)));
    doremir_dprint("align_of(1024,u2)            ==> %s\n", i32(doremir_type_align_of(u2)));
    printf("\n");

    type_t v = type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                             type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                                       type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                                               type(i8))))))))));

    doremir_print("v                            ==> %s\n", v);
    doremir_dprint("size_of(1024,v)              ==> %s\n", i32(doremir_type_size_of(1024, v)));
    doremir_dprint("align_of(1024,v)             ==> %s\n", i32(doremir_type_align_of(v)));
    doremir_destroy(v);
    printf("\n");
}



// --------------------------------------------------------------------------------

#pragma mark -

ptr_t add10(ptr_t x, ptr_t _)
{
    return (ptr_t)((int) x + 10);
}

void test_atomic()
{
    test_section("Atomic");

    // treat as integer
    {
        doremir_atomic_t a = doremir_atomic_create();
        doremir_print("a                            ==> %s\n", a);

        doremir_atomic_set(a, (ptr_t) 0x5);
        doremir_print("a                            ==> %s\n", a);

        doremir_atomic_modify(a, add10, 0);
        doremir_print("a                            ==> %s\n", a);

        // doremir_atomic_add(a, (ptr_t) - 0xf);
        // doremir_print("a                            ==> %s\n", a);

        doremir_atomic_exchange(a, (ptr_t) 1, (ptr_t) 0xfe);
        doremir_print("a                            ==> %s\n", a); // fails, still 0

        doremir_atomic_exchange(a, (ptr_t) 0, (ptr_t) 0xff);
        doremir_print("a                            ==> %s\n", a); // now ff
    }
}


// --------------------------------------------------------------------------------

struct reader_args {
    doremir_atomic_queue_t queue;
    atomic_t active;
};

doremir_ptr_t queue_reader(doremir_ptr_t x)
{
    struct reader_args *args = x;
    doremir_atomic_queue_t q = args->queue;
    atomic_t               a = args->active;
    ptr_t                  v;

    while (true) {
        if (!tb(doremir_atomic_get(a))) {
            return v;
        }

        if ((v = doremir_atomic_queue_read(q))) {
            printf("         |- %5d    \n", ti32(v));
        }

        doremir_thread_sleep(random() % 100);
    }
}

void test_atomic_queue(int iter, long sleepTime)
{
    test_section("Atomic queues");
    {
        doremir_atomic_queue_t q = doremir_atomic_queue_create();

        struct reader_args args = { q, atomic() };
        doremir_atomic_set(args.active, fb(true));

        thread_t t = doremir_thread_create(queue_reader, &args);

        doremir_print("q                            ==> %s\n", q);

        for (int i = 0; i < iter; ++i) {
            doremir_thread_sleep(random() % 100);
            doremir_atomic_queue_write(q, i32(i));
            printf("  %5d -|  \n", i);
        }

        doremir_thread_sleep(sleepTime);
        doremir_atomic_set(args.active, fb(false));
        doremir_thread_join(t); // TODO how to kill?
        doremir_destroy(q);
    }
}


// --------------------------------------------------------------------------------

struct stack_reader_args {
    doremir_atomic_stack_t stack;
    atomic_t active;
};

doremir_ptr_t stack_reader(doremir_ptr_t x)
{
    struct stack_reader_args *args = x;
    doremir_atomic_stack_t q = args->stack;
    atomic_t               a = args->active;
    ptr_t                  v;

    while (true) {
        if (!tb(doremir_atomic_get(a))) {
            return v;
        }

        if ((v = doremir_atomic_stack_read(q))) {
            printf("         |- %5d    \n", ti32(v));
        }

        srand(time(NULL));
        doremir_thread_sleep(random() % 100);
    }
}

void test_atomic_stack(int iter, long sleepTime)
{
    test_section("Atomic stacks");
    {
        doremir_atomic_stack_t q = doremir_atomic_stack_create();

        struct stack_reader_args args = { q, atomic() };
        doremir_atomic_set(args.active, fb(true));

        thread_t t = doremir_thread_create(stack_reader, &args);

        doremir_print("q                            ==> %s\n", q);

        for (int i = 0; i < iter; ++i) {
            if (i % 10) {
                doremir_thread_sleep(random() % 100);
            }

            doremir_atomic_stack_write(q, i32(i));
            printf("  %5d -|  \n", i);
        }

        doremir_thread_sleep(sleepTime);
        doremir_atomic_set(args.active, fb(false));
        doremir_thread_join(t);
        doremir_destroy(q);
    }
}


// --------------------------------------------------------------------------------

void test_atomic_ring_buffer(int inter, long sleepTime)
{
    test_section("Ring buffer");
}


// --------------------------------------------------------------------------------

ptr_t printer(ptr_t data)
{
    int n = 0;

    while (n < 100) {
        printf("%d\n", n);
        n = n + ((int) data);
        doremir_thread_sleep(100);
    }

    return 0;
}

void test_thread()
{
    test_section("Threads");

    doremir_thread_t t, t2;
    t  = doremir_thread_create(printer, (ptr_t) 10);
    t2 = doremir_thread_create(printer, (ptr_t) 11);

    doremir_thread_sleep(1000);
    doremir_thread_join(t);
    doremir_thread_join(t2);
}


// --------------------------------------------------------------------------------

typedef struct {
    doremir_thread_mutex_t mut;
    int val;
} lock_index;

ptr_t locker(ptr_t x)
{
    lock_index *i = (lock_index *) x;

    doremir_thread_lock(i->mut);
    printf("Acquired lock in thread %d\n", i->val);
    doremir_thread_sleep(200);
    doremir_thread_unlock(i->mut);
    printf("Released lock in thread %d\n", i->val);

    return 0;
}

void test_mutex()
{
    test_section("Mutexes");

    doremir_thread_mutex_t m = doremir_thread_create_mutex();

    for (int j = 0; j < 10; ++j) {
        lock_index i = { m, j };
        doremir_thread_t t = doremir_thread_create(locker, (ptr_t) &i);
        doremir_thread_sleep(100);
        doremir_thread_detach(t);
    }

    doremir_thread_sleep(1200);
}


// --------------------------------------------------------------------------------

#pragma mark -

void test_for_each()
{
    test_section("For each loops");

    doremir_let(x, 33) {
        doremir_let(y, 1)
        doremir_let(z, x + y)
        doremir_print("%s\n", i32(z));
    }

    printf("\n");
    doremir_with(list, list(i32(1), i32(2), i32(3), i32(4)), doremir_destroy(list)) {
        doremir_for_each(x, list) {
            doremir_print(">    %s\n", x);
        }
    }

    printf("\n");
    doremir_with(set, set(i32(1), i32(1), i32(2), i32(1)), doremir_destroy(set)) {
        doremir_for_each(x, doremir_set_to_list(set)) {
            doremir_print(">    %s\n", x);
        }
    }

    printf("\n");

    doremir_with(map, map(
                     string("foo"), i16(1),
                     string("bar"), list(i16(1), i16(2), i16(3))),
                 doremir_destroy(map)) {
        doremir_for_each(x, doremir_map_to_list(map)) {
            doremir_print(">    %s\n", x);
        }
    }
}



// --------------------------------------------------------------------------------

bool is_even16(ptr_t data, ptr_t p)
{
    return ti16(p) % 2 == 0;
}

bool is_odd16(ptr_t data, ptr_t p)
{
    return ti16(p) % 2 != 0;
}

ptr_t times2(ptr_t data, ptr_t p)
{
    return i16(2 * ti16(p));
}

ptr_t times10(ptr_t data, ptr_t p)
{
    return i16(10 * ti16(p));
}

// x = [x,x]
ptr_t dup_list(ptr_t ct, ptr_t x)
{
    return list(x, x);
}

void test_list()
{
    test_section("List");

    {
        printf("\n");

        list_t as = empty();
        doremir_print("empty()                      ==> %s\n", as);
        doremir_destroy(as);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));
        list_t bs = doremir_list_cons(i16(0), as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("cons(0,as)                   ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));
        list_t bs = doremir_list_append(as, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("append(as,as)                ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));
        list_t bs = doremir_list_copy(as);
        doremir_print("as                           ==> %s\n", as);
        doremir_print("copy(as)                     ==> %s\n", bs);
        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));
        list_t bs = doremir_list_init(as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("init(as)                     ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));
        ptr_t v = doremir_list_last(as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("last(as)                     ==> %s\n", v);

        doremir_destroy(as);
        doremir_destroy(v);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3));

        doremir_print("as                           ==> %s\n", as);
        doremir_print("length(as)                   ==> %s\n", i16(doremir_list_length(as)));

        doremir_destroy(as);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_reverse(as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("reverse(as)                  ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }

    {
        printf("\n");

        // list_t as = list(i16(1), i16(-2), i16(0), i16(4), i16(123));
        list_t as = doremir_list_enumerate(0, 10);
        as = doremir_list_reverse(as);
        as = doremir_list_dmap(apply1, i32, as);

        list_t bs = doremir_list_sort(as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("sort(as)                     ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }

    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_take(3, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("take(3,as)                   ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_drop(3, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("drop(3,as)                   ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        ptr_t v = doremir_list_index(1, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("index(1,as)                  ==> %s\n", v);

        doremir_destroy(as);
    }

    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_range(1, 3, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("range(1,3,as)                ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_remove_range(1, 3, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("removeRange(1,3,as)          ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }

    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t xs = list(i16(0), i16(0));
        list_t bs = doremir_list_insert_range(2, xs, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("insertRange(2,list(0,0),as)  ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(xs);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_insert(2, i16(0), as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("insert(2,0,as)               ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_insert(0, i16(0), as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("insert(0,1,as)               ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }
    {
        printf("\n");

        list_t as = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t bs = doremir_list_remove(2, as);

        doremir_print("as                           ==> %s\n", as);
        doremir_print("remove(2,as)                 ==> %s\n", bs);

        doremir_destroy(as);
        doremir_destroy(bs);
    }

    // has
    // find
    // findIndex

    {
        printf("\n");

        list_t as = list(i16(1), i16(3), i16(5));

        doremir_print("as                           ==> %s\n", as);
        doremir_print("indexOf(0,as)                ==> %s\n", i16(doremir_list_index_of(i16(0), as)));
        doremir_print("indexOf(1,as)                ==> %s\n", i16(doremir_list_index_of(i16(1), as)));
        doremir_print("indexOf(2,as)                ==> %s\n", i16(doremir_list_index_of(i16(2), as)));
        doremir_print("indexOf(3,as)                ==> %s\n", i16(doremir_list_index_of(i16(3), as)));
        doremir_print("indexOf(4,as)                ==> %s\n", i16(doremir_list_index_of(i16(4), as)));
        doremir_print("indexOf(5,as)                ==> %s\n", i16(doremir_list_index_of(i16(5), as)));

        doremir_destroy(as);
    }
    {
        printf("\n");

        list_t xs = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t ys = doremir_list_filter(is_odd16, 0, xs);

        doremir_print("xs                           ==> %s\n", xs);
        doremir_print("filter(is_odd,ys)            ==> %s\n", ys);

        doremir_destroy(xs);
        doremir_destroy(ys);
    }

    {
        printf("\n");

        list_t xs = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t ys = doremir_list_map(times10, 0, xs);

        doremir_print("xs                           ==> %s\n", xs);
        doremir_print("map(times10,ys)              ==> %s\n", ys);

        doremir_destroy(xs);
        doremir_destroy(ys);
    }

    {
        printf("\n");

        list_t xs = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t ys = list(i16(66), i16(77));
        list_t zss = list(xs, ys, xs);

        doremir_print("[xs,ys]                      ==> %s\n", zss);
        doremir_print("join([xs,ys])              ==> %s\n", doremir_list_join(zss));

        doremir_destroy(xs);
        doremir_destroy(ys);
    }


    {
        printf("\n");

        list_t xs = list(i16(1), i16(2), i16(3), i16(4), i16(5));
        list_t ys = doremir_list_join_map(dup_list, 0, xs);

        doremir_print("xs                           ==> %s\n", xs);
        doremir_print("joinMap(\\x -> [x,x])       ==> %s\n", ys);

        doremir_destroy(xs);
        doremir_destroy(ys);
    }


    {
        printf("\n");

        list_t xs = doremir_list_enumerate(0, 50000);

        xs = doremir_list_dreverse(xs);
        // doremir_print("reverse(xs)                  ==> %s\n", xs);

        xs = doremir_list_dmap(apply1, i16, xs);
        // doremir_print("xs                           ==> %s\n", xs);

        xs = doremir_list_dfilter(is_odd16, 0, xs);
        // doremir_print("filter(is_odd,xs)            ==> %s\n", xs);

        xs = doremir_list_dmap(times10, 0, xs);
        // doremir_print("map(times10, xs)             ==> %s\n", xs);

        doremir_destroy(xs);
    }
    {
        printf("\n");

        list_t xs = doremir_list_enumerate(0, 12);
        xs = doremir_list_dmap(apply1, i8, xs);

        doremir_print("xs                           ==> %s\n", xs);
        ptr_t sum = doremir_list_dfold_left(apply2, doremir_add, i8(0), xs);
        doremir_print("sum(xs)                      ==> %s\n", sum);
    }
}


// --------------------------------------------------------------------------------

void test_set()
{
    test_section("Set");
    {
        printf("\n");

        set_t a = set(i16(1), i16(3), i16(2));

        a = doremir_set_dadd(i16(1), a);
        a = doremir_set_dadd(i16(5), a);
        a = doremir_set_dadd(i16(3), a);
        a = doremir_set_dremove(i16(3), a);

        doremir_print("a                            ==> %s\n", a);
        doremir_print("size(a)                      ==> %s\n", i16(doremir_set_size(a)));
        doremir_destroy(a);
    }

    {
        printf("\n");

        set_t a = set(i16(1), i16(2), i16(3));
        set_t b = set(i16(3), i16(4));

        doremir_print("a                            ==> %s\n", a);
        doremir_print("b                            ==> %s\n", b);
        doremir_dprint("a + b                        ==> %s\n", doremir_set_sum(a, b));

        doremir_destroy(a);
        doremir_destroy(b);
    }

    {
        printf("\n");

        set_t a = set(i16(1), i16(2), i16(3));
        set_t b = set(i16(3), i16(4));

        doremir_print("a                            ==> %s\n", a);
        doremir_print("b                            ==> %s\n", b);
        doremir_dprint("a - b                        ==> %s\n", doremir_set_difference(a, b));

        doremir_destroy(a);
        doremir_destroy(b);
    }

    {
        printf("\n");

        set_t a = set(i16(1), i16(2), i16(3));
        set_t b = set(i16(3), i16(4));

        doremir_print("a                            ==> %s\n", a);
        doremir_print("b                            ==> %s\n", b);
        doremir_dprint("a x b                        ==> %s\n", doremir_set_product(a, b));

        doremir_destroy(a);
        doremir_destroy(b);
    }

    {
        printf("\n");

        set_t a = set(string("foo"), string("bar"));
        set_t b = set(string("hi"), string("ho"));
        set_t c = set(i16(0), i16(1));

        doremir_print("a                            ==> %s\n", a);
        doremir_print("b                            ==> %s\n", b);
        doremir_dprint("a x b                        ==> %s\n", doremir_set_product(a, b));

        set_t ab = doremir_set_product(a, b);
        doremir_dprint("a x b x c                    ==> %s\n", doremir_set_product(ab, c));
        doremir_destroy(ab);

        doremir_destroy(a);
        doremir_destroy(b);
        doremir_destroy(c);
    }
}


// --------------------------------------------------------------------------------

void test_map()
{
    test_section("Map");
    {
        printf("\n");

        map_t a = doremir_map_empty();

        a = doremir_map_dadd(string("name"), string("Hans"), a);
        a = doremir_map_dset(string("name"), string("Sven"), a);
        a = doremir_map_dset(string("age"), i16(22), a);
        a = doremir_map_dset(string("age"), i16(22), a);

        // a = doremir_map_dremove(string("age"), a);
        // a = doremir_map_dadd(string("age"), i16(25), a);
        a = doremir_map_dset(string("skills"), list(string("programming"), string("composition")), a);

        // a = doremir_map_dadd(string("happy"), fb(true), a);
        // a = doremir_map_dadd(string("pair"), pair(fb(true), f64(3.1415)), a);
        // a = doremir_map_dadd(string("ratio"), ratio(1, 3), a);
        // a = doremir_map_dadd(string("ratio2"), doremir_multiply(ratio(4, 4444), ratio(1, 2)), a);

        doremir_print("a                            ==> %s\n", a);
        doremir_print("size(a)                      ==> %s\n", i16(doremir_map_size(a)));

        doremir_print("a.name                       ==> %s\n", doremir_map_get(string("name"), a));
        doremir_print("a.age                        ==> %s\n", doremir_map_get(string("age"), a));
        doremir_print("a.skills                     ==> %s\n", doremir_map_get(string("skills"), a));
        doremir_print("a.happy                      ==> %s\n", doremir_map_get(string("happy"), a));
        doremir_print("a.pair                       ==> %s\n", doremir_map_get(string("pair"), a));

        doremir_destroy(a);
    }
}


// --------------------------------------------------------------------------------

void test_graph(string_t path)
{
    test_section("Graph");
    {
        graph_t a = doremir_graph_empty();


        a = doremir_graph_insert(string("foo"), a);
        a = doremir_graph_connect(string("foo"), string("foo"), string("(1)"), a);

        // a = doremir_graph_insert(pair(string("a"), string("b")), a);
        // a = doremir_graph_connect(
        //     pair(string("a"), string("b")),
        //     pair(string("a"), string("b")),
        //     string("(1)"), a);

        doremir_print("a                            ==> %s\n", a);
        doremir_system_directory_write_file(path, doremir_graph_to_dot(
                                                string("#include \"doc/graphs/header.dot\""),
                                                string(""),
                                                a));
    }
}


// --------------------------------------------------------------------------------

void test_priority_queue(int iter)
{
    test_section("Priority queue");

    priority_queue_t q = doremir_priority_queue_empty();
    srand(time(NULL));

    for (int i = 0; i < iter; ++i) {
        doremir_priority_queue_insert(doremir_add(hours(rand() % 24), seconds(rand() % 3600)), q);
    }

    while (doremir_priority_queue_peek(q)) {
        doremir_dprint("     -> %s \n", doremir_priority_queue_pop(q));
    }

}


// --------------------------------------------------------------------------------

void test_json(string_t path)
{
    extern void doremir_puts(doremir_string_t string);

    test_section("JSON conversion");

    string_t json = doremir_system_directory_read_file(path);
    // printf("%s\n", unstring(json));

    ptr_t data = doremir_string_from_json(json);
    doremir_print("data                         ==> %s\n", data);

    string_t json2 = doremir_string_to_json(data);
    doremir_puts(json2);

    doremir_puts(doremir_string_to_json(
                     pair(i32(1), i32(2))));

    doremir_puts(doremir_string_to_json(
                     list(pair(i32(1), i32(2)), pair(i32(3), i32(4)))));

    doremir_puts(doremir_string_to_json(
                     set(pair(i32(1), i32(2)), pair(i32(1), i32(2)))));

    doremir_puts(doremir_string_to_json(
                     map(
                         string("foo"), i32(1),
                         string("bar"), list(i32(1), i32(2), i32(3)))));

}



// --------------------------------------------------------------------------------

#pragma mark -

void test_dispatcher()
{
    test_section("Dispatcher");

    dispatcher_t disp = lockfree_dispatcher();

    ptr_t val = map(
                    string("lyrics"), list(string("Help"), string("me"), string("if"), string("you"), string("can")),
                    string("pitches"), list(ratio(60, 1), ratio(62, 1))
                );

    doremir_message_send((receiver_t) disp, i16(1), val);
    doremir_message_send((receiver_t) disp, i16(2), string("World!"));
    doremir_message_send((receiver_t) disp, i16(2), string("World!"));
    doremir_message_send((receiver_t) disp, i16(2), string("World!"));
    doremir_message_send((receiver_t) disp, i16(2), string("World!"));

    list_t msgs = doremir_list_empty();

    while (true) {
        doremir_message_sync((sender_t) disp);
        msgs = doremir_message_receive((sender_t) disp, i16(1));

        if (doremir_list_is_empty(msgs)) {
            break;
        }

        doremir_print("             | 1: %s\n", msgs);
    }

    doremir_destroy(disp);
}


// --------------------------------------------------------------------------------

void test_system_event()
{
    test_section("System events");

    doremir_message_sender_t s =
        doremir_system_event_receive(
            list(
                // i16(mouse_move_event)
                // i16(mouse_down_event)
                i16(key_down_event),
                i16(key_up_event)

            ));
    doremir_message_receiver_t r =
        doremir_system_event_send_std();

    for (int i = 0; i < 100000; ++i) {
        doremir_message_sync(s);
        doremir_for_each(x, doremir_message_receive(s, i16(0))) {
            // doremir_print("    Received: %s\n", x);
            doremir_message_send(r, i16(0), x);
        }
        doremir_thread_sleep(5);
    }
}


// --------------------------------------------------------------------------------

void test_event()
{
    // {
    //     test_section("Events");
    //
    //     doremir_time_t t = seconds(0);
    //     event_t a = delay_event(seconds(5), delay_event(seconds(5), now(string("fix"))));
    //     event_t b = delay_event(seconds(0),
    //                             merge_event(
    //                                 delay_event(seconds(3),  now(string("foo"))),
    //                                 delay_event(seconds(12), now(string("bar"))))); // too early!
    //
    //     doremir_print("\n", NULL);
    //     doremir_print("t                            ==> %s\n", t);
    //
    //     doremir_print("\n", NULL);
    //     doremir_print("a                            ==> %s\n", a);
    //     doremir_print("offset(a)                    ==> %s\n", doremir_event_offset(a));
    //     doremir_print("hasValue(a)                  ==> %s\n", fb(doremir_event_has_value(t, a)));
    //     doremir_print("value(a)                     ==> %s\n", doremir_event_value(a));
    //
    //     doremir_print("\n", NULL);
    //     doremir_print("b                            ==> %s\n", b);
    //     doremir_print("offset(b)                    ==> %s\n", doremir_event_offset(b));
    //     doremir_print("hasValue(b)                  ==> %s\n", fb(doremir_event_has_value(t, b)));
    //     doremir_print("value(b)                     ==> %s\n", doremir_event_value(b));
    //
    //     doremir_print("\n", NULL);
    //     doremir_print("min(a,b)                     ==> %s\n", doremir_min(a, b));
    //     doremir_print("offset(min(a,b))             ==> %s\n", doremir_event_offset(doremir_min(a, b)));
    // }

    {
        doremir_time_t t = seconds(0);

        event_t ha = now(string("höglund"));
        event_t ho = now(string("holmgren"));

        event_t a = merge_event(ha,
            delay_event(milliseconds(200*2), merge_event(ha,
            delay_event(milliseconds(200*2), merge_event(ha,
            delay_event(milliseconds(200*2), merge_event(ha,
            delay_event(milliseconds(200*2), merge_event(ha,
            delay_event(milliseconds(200*2), merge_event(ha, never())))))))))));

        event_t b = merge_event(ho,
            delay_event(milliseconds(240*2), merge_event(ho,
            delay_event(milliseconds(240*2), merge_event(ho,
            delay_event(milliseconds(240*2), merge_event(ho,
            delay_event(milliseconds(240*2), merge_event(ho,
            delay_event(milliseconds(240*2), merge_event(ho, never())))))))))));

        event_t s1 = doremir_event_later(seconds(1), NULL);
        event_t s3 = doremir_event_later(seconds(3), NULL);

        event_t mm = doremir_system_event_mouse_move();
        event_t md = doremir_system_event_mouse_down();
        event_t mu = doremir_system_event_mouse_up();
        event_t kd = doremir_system_event_key_down();
        event_t ku = doremir_system_event_key_up();
        event_t mouseX = doremir_event_map(apply1, doremir_pair_fst, mm);
        event_t mouseY = doremir_event_map(apply1, doremir_pair_snd, mm);

        // event_t y2 = merge_event(switch_event(kd, merge_event(a, mm), merge_event(b, md)), later(seconds(5), list(string("flux"))));
        // event_t y2 = switch_event(ku, switch_event(kd,never(),mm), merge_event(delay_event(seconds(3),b),md));
        // event_t y2 = switch_event(kd,mm,merge_event(md,mu));
        // event_t y2 = doremir_event_filter(doremir_less_than, f64(500), mouseX);
        event_t y2 = mm;
        // doremir_print("The event: %s\n", mouseX);
        event_t z  = doremir_system_event_write_std(y2);

        {
            clock_t     clk = doremir_time_get_system_prec_clock();
            scheduler_t sched = doremir_scheduler_create(clk);
            doremir_scheduler_schedule(sched, z);
            doremir_scheduler_loop(sched);
        }
    }

}


// --------------------------------------------------------------------------------

void test_scheduler()
{
    test_section("Scheduler");



}


// --------------------------------------------------------------------------------

ptr_t add1234(ptr_t c, ptr_t x)
{
    return i8(ti8(x) + 1234);
}

void test_processor_graphs(string_t path)
{
    test_section("Processors");

    inform(string_append(string("Writing "), path));

    processor_t p, q, chain, rchain;
    p = unary(type(i8), type(i8), add1234, NULL);
    chain = seq(p, seq(p, seq(p, seq(p, seq(p, p)))));
    rchain = seq(seq(seq(seq(p, p), p), p), p);
    q =
        seq(split(type(i8)),

            par(
                seq(seq(seq(p, p), p), p),

                seq(
                    split(type(i8)),
                    par(seq(split(type(i8)), par(chain, seq(split(type(i8)), par(chain, chain)))),
                        seq(split(type(i8)), par(p, seq(p, p))))
                )

            )


           )

        ;

    doremir_processor_write_graph(q, path);


    /*

        {
            processor_t p, q;
            p = doremir_processor_unary(type(i8), type(i8), add1234, NULL);
            q = doremir_processor_sequence(p, p);
            doremir_print("p                            ==> %s\n", p);
            doremir_print("bufferSize(p)                ==> %s\n", i32(doremir_processor_buffer_size(1, p)));
        }

        {
            processor_t p, q;
            p = doremir_processor_unary(type(i8), type(i8), add1234, NULL);
            q = doremir_processor_sequence(p, p);
            doremir_print("p                            ==> %s\n", p);
            doremir_print("seq(p,p)                     ==> %s\n", q);
            doremir_print("bufferSize(seq(p,p))         ==> %s\n", i32(doremir_processor_buffer_size(1, q)));
        }

        {
            processor_t p, q;
            p = doremir_processor_unary(type(i8), type(i8), add1234, NULL);
            q = doremir_processor_parallel(p, p);
            doremir_print("p                            ==> %s\n", p);
            doremir_print("q                            ==> %s\n", p);
            doremir_print("par(p,p)                     ==> %s\n", q);
            doremir_print("bufferSize(par(p,q))         ==> %s\n", i32(doremir_processor_buffer_size(1, q)));
        }

        {
            processor_t p0, p, q;
            p0 = doremir_processor_unary(type(i8), type(i8), add1234, NULL);
            p = doremir_processor_parallel(p0, p0);
            q = doremir_processor_loop(p);
            doremir_print("p                            ==> %s\n", p);
            doremir_print("loop(p)                      ==> %s\n", q);
            doremir_print("bufferSize(loop(p))          ==> %s\n", i32(doremir_processor_buffer_size(1, q)));
        }

        {
            type_t t = type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                                     type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                                               type_pair(type(i8), type_pair(type(i8), type_pair(type(i8),
                                                                       type(i8))))))))));

            processor_t p = doremir_processor_identity(t);
            doremir_print("p                            ==> %s\n", p);

        }  */

}


// --------------------------------------------------------------------------------

ptr_t cont(ptr_t x)
{
    printf("Continuing...\n");
    return x;
}

double f1(void *ct, int i, double t, double x)
{
    double pi  = 3.141592653589793;
    double tau = 2 * pi;
    double t0  = x;
    double t2  = t + x;
    t2 = t2;

#define step(p) ((float)((int)fmod(t,p)%p))/p

    switch (i) {
    case 3:
        return step(5);

    case 2:
        return -0.5 * cos(tau * t0 * 0.5 + pi);

    case 1:
        return  0.5 * cos(tau * t0 * 0.5 + pi);

    case 0:
        return  0.5 * cos(tau * t0 * 0.5 + pi) * sin(tau * t0 * 3);

    default:
        return 0;
    }
}

void test_plot()
{
    test_section("Plot");
    doremir_plot_continous(f1, NULL, NULL, NULL);
}


// --------------------------------------------------------------------------------

void test_plot_buffer()
{
    buffer_t buf = doremir_buffer_create(44100 * sizeof(double));

    for (int i = 0; i < 44100; ++i) {
        double r = (double) random() / RAND_MAX;
        double x = (double) i / 44100;
        doremir_buffer_set_double(buf, i, (r * 2 - 1) * sin(x * 10));
    }

    doremir_plot_buffer_double(buf, NULL, NULL);
}


// --------------------------------------------------------------------------------

void test_plot_file(string_t path)
{
    test_section("Plot file");

    pair_t res = doremir_buffer_read_audio(path);

    if (doremir_error_check(res)) {
        doremir_error_log(NULL, (error_t) res);
        return;
    }

    doremir_print("%s\n", res);

    buffer_t buf = doremir_pair_snd(res);
    doremir_plot_buffer_double(buf, NULL, NULL);
    doremir_destroy(buf);
    doremir_destroy(res);
}


// --------------------------------------------------------------------------------

void test_error()
{

}


// --------------------------------------------------------------------------------

void test_log()
{
    test_section("Logging");
    doremir_audio_engine_log_info(string("---------------"));
    doremir_audio_engine_log_info(string("Log test: Do not take these seriously"));

    for (int i = 0; i < 3; ++i) {

        doremir_audio_engine_log_info(string("We have a problem"));
        // doremir_audio_engine_log_warning(string("We have a problem"));
        doremir_audio_engine_log_error(string("We have a problem"));

        doremir_audio_engine_log(NULL,
                                 doremir_error_create_simple(
                                     error,
                                     string("We have a problem"),
                                     string("Doremir.FooBar")));
        doremir_thread_sleep(50);
    }

    doremir_audio_engine_log_info(string("---------------"));
}


// --------------------------------------------------------------------------------

void test_system_directory()
{
    test_section("Directory");

    doremir_print("home()                       ==> %s\n", doremir_system_directory_home());
    doremir_print("current()                    ==> %s\n", doremir_system_directory_current());
}



// --------------------------------------------------------------------------------

void test_regex()
{
    test_section("Regular expressions");

    char exp[] = ".* Hans H.*";
    char str[] = "A Hans Höglund";

    doremir_print("exp                          ==> %s\n", string(exp));
    doremir_print("str                          ==> %s\n", string(str));
    doremir_print("matches(exp,str)             ==> %s\n", fb(doremir_string_matches(string(exp), string(str))));
}

// --------------------------------------------------------------------------------

void test_file_stream(string_t in_path, string_t out_path)
{
    test_section("File streams");

    file_device_t    input, output;
    file_result_t    result;
    processor_t proc;

    // Processor to use
    proc    = doremir_processor_identity(type_pair(type_frame(type(f32)), type_frame(type(f32))));

    // Open streams
    input   = doremir_device_file_open(in_path);
    output  = doremir_device_file_open(out_path);

    // Handle possible errors
    if (doremir_check(input)) {
        log_error((error_t) input);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    if (doremir_check(output)) {
        log_error((error_t) output);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    result = doremir_device_file_run(input, proc, output);

    // Handle possible error
    if (doremir_check(result)) {
        log_error((error_t) result);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

cleanup:
    doremir_device_file_close(input);
    doremir_device_file_close(output);
}


// --------------------------------------------------------------------------------

void test_buffer_stream()
{
    test_section("Buffer streams");
}


// --------------------------------------------------------------------------------

void print_audio_devices(audio_session_t session)
{
    doremir_print("\n", NULL);
    doremir_print("    Listing audio devices: \n", NULL);
    doremir_for_each(x, doremir_device_audio_all(session)) {
        doremir_print("        Device: %s\n", x);
        doremir_print("            Input:  %s\n", doremir_device_audio_input_type(x));
        doremir_print("            Output: %s\n", doremir_device_audio_output_type(x));
    }
    doremir_print("    Default input is : %s\n", doremir_device_audio_default_input(session));
    doremir_print("    Default output is : %s\n", doremir_device_audio_default_output(session));
    doremir_print("\n", NULL);
}


// --------------------------------------------------------------------------------

ptr_t status_changed(ptr_t ct)
{
    printf("Status changed: %s!\n", unstring(ct));
    return 0;
}

void test_audio_stream()
{
    test_section("Audio streams");

    audio_session_t session;
    audio_device_t  input, output;
    audio_stream_t  stream;
    processor_t     proc1, proc2;

    // Processor to use
    proc1    = id(type_pair(type_frame(type(f32)), type_frame(type(f32))));
    proc2    = seq(proc1, proc1);

    // Begin session
    session = doremir_device_audio_begin_session();

    // Handle possible error
    if (doremir_check(session)) {
        log_error((error_t) session);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    // Session obtained, we can now access devices
    print_audio_devices(session);

    input = doremir_device_audio_default_input(session);
    output = doremir_device_audio_default_output(session);

    // Start stream
    stream = doremir_device_audio_open_stream(input, proc2, output);

    // Handle possible error
    if (doremir_check(stream)) {
        log_error((error_t) stream);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    doremir_device_audio_set_status_callback(status_changed, string("foobar"), session);
    doremir_thread_sleep(3000);

cleanup:
    doremir_device_audio_close_stream(stream);
    doremir_device_audio_end_session(session);
    doremir_destroy(proc1);
    doremir_destroy(proc2);
}



// --------------------------------------------------------------------------------

void print_midi_devices(midi_session_t session)
{
    doremir_print("\n", NULL);
    doremir_print("    Listing midi devices: \n", NULL);
    doremir_for_each(x, doremir_device_midi_all(session)) {
        doremir_print("        Device: %s\n", x);
        doremir_print("            Input:  %s\n", fb(doremir_device_midi_has_input(x)));
        doremir_print("            Output: %s\n", fb(doremir_device_midi_has_output(x)));
    }
    doremir_print("    Default input is : %s\n", doremir_device_midi_default_input(session));
    doremir_print("    Default output is : %s\n", doremir_device_midi_default_output(session));
    doremir_print("\n", NULL);
}

ptr_t to_note_on(ptr_t occ) {
    // doremir_print("%s\n", occ);
    int16_t kc = ti16(doremir_list_head(occ));
    return midi(0x90, 48 + kc, 120);
}

ptr_t to_note_off(ptr_t occ) {
    // doremir_print("%s\n", occ);
    int16_t kc = ti16(doremir_list_head(occ));
    return midi(0x80, 48 + kc, 120);
}

ptr_t to_control(ptr_t occ) {
    // doremir_print("%s\n", occ);
    double x = tf64(doremir_pair_fst(occ));
    return midi(0xb0, 7, x/1900 * 127);
}
ptr_t to_control2(ptr_t occ) {
    // doremir_print("%s\n", occ);
    double y = tf64(doremir_pair_snd(occ));
    return midi(0xb0, 1, y/1200 * 127);
}


void test_midi_stream()
{
    test_section("Midi streams");

    midi_session_t session;
    midi_device_t  input, output;
    midi_stream_t  in_stream, out_stream;

    // Begin session
    session = doremir_device_midi_begin_session();

    // Handle possible error
    if (doremir_check(session)) {
        log_error((error_t) session);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    // Session obtained, we can now access devices
    print_midi_devices(session);

    input = doremir_list_index(2, doremir_device_midi_all(session));
    // output = doremir_device_midi_default_output(session);
    output = doremir_list_index(6, doremir_device_midi_all(session));

    // Start streams
    in_stream  = doremir_device_midi_open_stream(input);
    out_stream = doremir_device_midi_open_stream(output);

    // Handle possible errors
    if (doremir_check(in_stream)) {
        log_error((error_t) in_stream);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    if (doremir_check(out_stream)) {
        log_error((error_t) out_stream);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    // TODO
    // doremir_device_midi_set_status_callback(status_changed, string("foobar"), session);

    // event_t notes  =
    //     merge_event(later(divisions(1,10), midi(0x90, 48, 10)),
    //     merge_event(later(divisions(2,10), midi(0x90, 50, 20)),
    //     merge_event(later(divisions(3,10), midi(0x90, 52, 30)),
    //     merge_event(later(divisions(4,10), midi(0x90, 53, 40)),
    //     merge_event(later(divisions(5,10), midi(0x90, 55, 50)),
    //     merge_event(later(divisions(6,10), midi(0x90, 57, 60)),
    //     merge_event(later(divisions(7,10), midi(0x90, 59, 70)),
    //     merge_event(later(divisions(8,10), midi(0x90, 60, 80)),
    //     never()))))))));

    // event_t notes =
    //     merge_event(doremir_event_map(apply1, to_note_on,  doremir_system_event_key_down()),
    //     merge_event(doremir_event_map(apply1, to_note_off, doremir_system_event_key_up()),
    //     merge_event(doremir_event_map(apply1, to_control,  doremir_system_event_mouse_move()),
    //                 doremir_event_map(apply1, to_control2, doremir_system_event_mouse_move()))));

    // event_t notes2 = doremir_event_before(later(seconds(3),0), notes);

    event_t notes   = doremir_event_receive((sender_t) in_stream, i32(0));
    event_t sender  = doremir_event_send((receiver_t) out_stream, i32(0), notes);
    event_t sender2 = doremir_system_event_write_std(notes);

    scheduler_t sched = doremir_scheduler_create(doremir_time_get_system_prec_clock());
    doremir_scheduler_schedule(sched, sender);
    doremir_scheduler_schedule(sched, sender2);
    doremir_scheduler_loop(sched);

    // for (int i = 0; i < 30; ++i) {
    //     doremir_message_send((receiver_t) out_stream, 0, midi(0x90, 48 + i * 2, 100));
    //     doremir_thread_sleep(100);
    // }

cleanup:
    // doremir_device_midi_close_stream(stream);
    doremir_device_midi_end_session(session);
}           



void test_midi_hotplug()
{
    test_section("Midi hot-plugging");

    midi_session_t session;

    // Begin session
    session = doremir_device_midi_begin_session();

    // Handle possible error
    if (doremir_check(session)) {
        log_error((error_t) session);
        warn(string("Aborting test due to error"));
        goto cleanup;
    }

    doremir_device_midi_set_status_callback(status_changed, string("hello"), session);
    
    // CFRunLoopRun();
    // doremir_thread_sleep(20000);
cleanup:
    // doremir_device_midi_close_stream(stream);
    doremir_device_midi_end_session(session);
}


// --------------------------------------------------------------------------------

static const int  iterations_k = 1;
static const bool stop_k       = false;

// --------------------------------------------------------------------------------

int main(int argc, char const *argv[])
{
    printf("DoReMIR Audio engine %s v%d.%d.%d\n", bits, version[0], version[1], version[2]);

    printf("sizeof(doremir_ptr_t) = %d\n", (unsigned int) sizeof(doremir_ptr_t));
    printf("sizeof(int32_t) = %d\n", (unsigned int) sizeof(int32_t));
    printf("sizeof(int64_t) = %d\n", (unsigned int) sizeof(int64_t));
    printf("sizeof(wchar_t) = %d\n", (unsigned int) sizeof(wchar_t));

    printf("The page size for this system is %ld bytes.\n", sysconf(_SC_PAGESIZE));
    
    for (int i = 0; i < iterations_k; ++i) {
        if (stop_k) {
            getchar();
        }

        doremir_audio_engine_set_log_std();
        doremir_plot_use_gnu();
        // doremir_audio_engine_set_log_file(string("/Users/hans/Library/Logs/DoReMIRAudio.log"));
        // doremir_plot_use_core();

        doremir_audio_engine_initialize();

        // goto begin;
        test_value_references();
        test_generic_functions();
        test_string();
        test_show();
        test_compare();
        test_rational();
        test_buffer();
        test_time();
        test_system_time();
        test_type();
        test_midi();

        // test_thread();
        // test_mutex();

        test_atomic();
        test_atomic_queue(5, 2);
        // test_atomic_queue(10, 10);
        // test_atomic_queue(300, 2);
        test_atomic_stack(5, 2);
        // test_atomic_stack(10, 10);
        // test_atomic_stack(300, 2);
        test_atomic_ring_buffer(5, 2);

        test_for_each();
        test_list();
        test_set();
        test_map();
        test_graph(string_dappend(doremir_system_directory_current(), string("/test/gen.dot")));
        test_priority_queue(10);
        test_json(
            string_dappend(doremir_system_directory_current(), string("/test/example.json")));

        test_log();
        test_error();
        test_system_directory();
        test_regex();
        // test_plot(NULL, NULL);
        // test_plot_buffer();
        // test_plot_file(string_dappend(doremir_system_directory_current(), string("/test/in.wav")));

        // test_processor_graphs(string_dappend(doremir_system_directory_current(), string("/test/proc.dot")));
        // test_dispatcher();
        // test_system_event();

        // test_event();
        // goto end;
        // test_scheduler();
        // test_processor();

        test_file_stream(
            string_dappend(doremir_system_directory_current(), string("/test/in.wav")),
            string_dappend(doremir_system_directory_current(), string("/test/out.wav")));
        test_buffer_stream();
        // test_audio_stream();
        // test_midi_stream();
        
        
// begin:
        // test_midi_hotplug();

// end:
        doremir_audio_engine_terminate();
    }

    return 0;
}

