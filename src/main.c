/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "TestU01.h"
#include "bitpacking.h"

/*
 * Customized TestU01 batteries.
 */

extern void bbattery_Rabbit31(unif01_Gen *gen, double nb);

/*
 * RNG holder.
 */

typedef void(*rng_init_fn)(uint32_t);
typedef uint32_t(*rng_next_fn)();

struct RNG {
    const char *id;
    const char *description;
    rng_init_fn init;
    rng_next_fn next;
};

/*
 * RNG functions.
 */

#define X(id, desc)                    \
    extern void id ## _init(uint32_t); \
    extern uint32_t id ## _next();

#include "rngs.def"

/*
 * RNG list.
 */

static struct RNG rngs[] = {
    #define X(id, desc) { #id, desc, id ## _init, id ## _next },
    #include "rngs.def"
    { 0 }
};

/*
 * Output modes.
 */

enum mode {
    MODE_WRITE,
    MODE_SMALLCRUSH,
    MODE_CRUSH,
    MODE_BIGCRUSH,
    MODE_RABBIT,
    MODE_RABBIT31,
    MODE_LINCOMP
};

static void do_packed_write(struct RNG *rng)
{
    uint32_t unpacked[32];
    uint8_t packed[124];

    for (;;) {
        for (size_t i = 0; i < 32; i++) {
            unpacked[i] = rng->next() & INT_MAX;
        }

        pack32(unpacked, 32, 31, packed);
        fwrite(packed, 1, 124, stdout);
    }
}

static void do_direct_write(struct RNG *rng)
{
    for (;;) {
        uint32_t value = rng->next() & INT_MAX;
        fwrite(&value, sizeof(value), 1, stdout);
    }
}

/*
 * We need to perform the left shift on output. Ideally the TestU01 generator
 * would save the RNG struct as state and then pass it to the generator
 * function, but it doesn't do that. So we use this global state.
 */
static struct RNG *testu01_rng;

static unsigned testu01_next_direct()
{
    return testu01_rng->next() & INT_MAX;
}

static unsigned testu01_next_fixed()
{
    return testu01_rng->next() << 1;
}

static void do_testu01(struct RNG *rng, bool direct, enum mode mode)
{
    testu01_rng = rng;
    rng_next_fn next = direct ? testu01_next_direct : testu01_next_fixed;
    char *desc = (char *) rng->description; /* non-const for some reason */

    unif01_Gen *gen = unif01_CreateExternGenBits(desc, next);

    switch (mode) {
        case MODE_SMALLCRUSH:
            bbattery_SmallCrush(gen);
            break;
        case MODE_CRUSH:
            bbattery_Crush(gen);
            break;
        case MODE_BIGCRUSH:
            bbattery_BigCrush(gen);
            break;
        case MODE_RABBIT:
            bbattery_Rabbit(gen, 16777216); /* 2^24 bytes == 16 MB */
            break;
        case MODE_RABBIT31:
            bbattery_Rabbit31(gen, 16777216); /* 2^24 bytes == 16 MB */
            break;
        case MODE_LINCOMP: {
            swrite_Basic = TRUE;
            scomp_Res* res = scomp_CreateRes();

            static const int sizes[] = {
                250, 500, 1000, 5000, 25000, 50000, 75000
            };

            for (int i = 0; i < 7; i++) {
                scomp_LinearComp(gen, res, 1, sizes[i], 0, 1);
            }

            scomp_DeleteRes(res);
            break;
        }
    }

    unif01_DeleteExternGenBits(gen);
}

/*
 * Main.
 */

#define ITERATE_RNGS(var) for (struct RNG *var = rngs; var->id; var++)

_Noreturn static void show_usage_and_exit(bool error, bool show_help)
{
    FILE *out = error ? stderr : stdout;

    fputs("usage: randtest31 [-h] [-v] [-d] MODE RNG\n", out);
    if (show_help) exit(error);

    fputs("\n", out);
    fputs("positional arguments:\n", out);
    fputs("  MODE        output mode\n", out);
    fputs("  RNG         random number generator under test\n", out);
    fputs("\n", out);
    fputs("optional arguments:\n", out);
    fputs("  -h, --help  show this help message and exit\n", out);
    fputs("  -d          do not adjust for 31-bit resolution (direct output)\n", out);
    fputs("  -v          verbose messages\n", out);
    fputs("\n", out);
    fputs("modes:\n", out);
    fputs("  write       write RNG output to stdout\n", out);
    fputs("  smallcrush  TestU01 SmallCrush test battery\n", out);
    fputs("  crush       TestU01 Crush test battery\n", out);
    fputs("  bigcrush    TestU01 BigCrush test battery\n", out);
    fputs("  rabbit      TestU01 Rabbit test battery (16 MB)\n", out);
    fputs("  rabbit31    TestU01 31-bit Rabbit test battery (16 MB)\n", out);
    fputs("  lincomp     TestU01 LinearComp test\n", out);
    fputs("\n", out);
    fputs("generators:\n", out);

    size_t longest_gen_id = 0;

    ITERATE_RNGS(rng) {
        size_t len = strlen(rng->id);
        if (len > longest_gen_id) longest_gen_id = len;
    }

    ITERATE_RNGS(rng) {
        size_t space_count = longest_gen_id - strlen(rng->id);
        fprintf(out, "  %s", rng->id);
        for (size_t i = 0; i < space_count; i++) {
            fputs(" ", out);
        }
        fprintf(out, "  %s\n", rng->description);
    }

    exit(error);
}

_Noreturn static void exit_failed_with_usage()
{
    show_usage_and_exit(true, true);
}

_Noreturn static void exit_success_with_help()
{
    show_usage_and_exit(false, false);
}

#define CLI_ERROR "randtest31: error: "

int main(int argc, char** argv)
{
    /* Default settings for TestU01 */
    swrite_Basic = FALSE;

    bool verbose = false;
    bool direct = false;
    const char *mode_str = NULL;
    const char *rng_id_str = NULL;

    /*
     * Extract arguments.
     */

    for (size_t i = 1; i < argc; i++) {
        const char * cur = argv[i];

        /* Extract help flags */
        if (strcmp(cur, "-h") == 0) exit_success_with_help();
        if (strcmp(cur, "--help") == 0) exit_success_with_help();

        if (cur[0] == '-') {
            /* Extract flags */
            if (strcmp(cur, "-v") == 0) {
                verbose = true;
            } else if (strcmp(cur, "-d") == 0) {
                direct = true;
            } else {
                fprintf(stderr, CLI_ERROR "unknown argument '%s'\n", cur);
                exit_failed_with_usage();
            }
        } else {
            /* Extract positional arguments */
            if (!mode_str) {
                mode_str = cur;
            } else if (!rng_id_str) {
                rng_id_str = cur;
            } else {
                fputs(CLI_ERROR "too many positional arguments\n", stderr);
                exit_failed_with_usage();
            }
        }
    }

    if (!mode_str) {
        fputs(CLI_ERROR "missing MODE argument\n", stderr);
        exit_failed_with_usage();
    }

    if (!rng_id_str) {
        fputs(CLI_ERROR "missing RNG argument\n", stderr);
        exit_failed_with_usage();
    }

    /*
     * Parse positional arguments.
     */

    enum mode mode;
    struct RNG* rng_in_use = NULL;

    if (strcmp(mode_str, "write") == 0) {
        mode = MODE_WRITE;
    } else if (strcmp(mode_str, "smallcrush") == 0) {
        mode = MODE_SMALLCRUSH;
    } else if (strcmp(mode_str, "crush") == 0) {
        mode = MODE_CRUSH;
    } else if (strcmp(mode_str, "bigcrush") == 0) {
        mode = MODE_BIGCRUSH;
    } else if (strcmp(mode_str, "rabbit") == 0) {
        mode = MODE_RABBIT;
    } else if (strcmp(mode_str, "rabbit31") == 0) {
        mode = MODE_RABBIT31;
    } else if (strcmp(mode_str, "lincomp") == 0) {
        mode = MODE_LINCOMP;
    } else {
        fprintf(stderr, CLI_ERROR "unknown mode '%s'\n", mode_str);
        exit_failed_with_usage();
    }

    ITERATE_RNGS(rng) {
        if (strcmp(rng_id_str, rng->id) == 0) {
            rng_in_use = rng;
            break;
        }
    }

    if (!rng_in_use) {
        fprintf(stderr, CLI_ERROR "unknown generator '%s'\n", rng_id_str);
        exit_failed_with_usage();
    }

    /*
     * Handle other flags.
     */

    if (verbose) {
        swrite_Basic = TRUE;
    }

    /*
     * Run the specified mode.
     */

    /* An amazing seed. */
    rng_in_use->init(123456789);

    if (mode == MODE_WRITE) {
        if (direct) {
            do_direct_write(rng_in_use);
        } else {
            do_packed_write(rng_in_use);
        }
    } else {
        do_testu01(rng_in_use, direct, mode);
    }

    return 0;
}
