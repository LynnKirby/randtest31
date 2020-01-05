/*
 * SPDX-License-Identifier: CC0-1.0
 * SPDX-FileCopyrightText: 2020 Lynn Kirby
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "TestU01.h"
#include "bitpacking.h"

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

static void do_practrand(struct RNG *rng)
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

static void do_practrand_direct(struct RNG *rng)
{
    for (;;) {
        uint32_t value = rng->next() & INT_MAX;
        fwrite(&value, sizeof(value), 1, stdout);
    }
}

static struct RNG *testu01_rng;

static unsigned testu01_next()
{
    return testu01_rng->next() << 1;
}

static void do_smallcrush(struct RNG *rng)
{
    /*
     * We need to perform the left shift on output. Ideally the TestU01
     * generator would save the RNG struct as state and then pass it to the
     * generator function, but it doesn't do that. So we use this global state.
     */
    testu01_rng = rng;

    unif01_Gen *gen = unif01_CreateExternGenBits(
            (char *) rng->description, testu01_next);
    bbattery_SmallCrush(gen);
    unif01_DeleteExternGenBits(gen);
}


/*
 * Main.
 */

enum mode {
    MODE_PRACTRAND,
    MODE_SMALLCRUSH
};

#define ITERATE_RNGS(var) for (struct RNG *var = rngs; var->id; var++)

_Noreturn static void show_usage(_Bool error)
{
    FILE *out = error ? stderr : stdout;

    fputs("usage: randtest31 [-v] MODE RNG\n", out);
    fputs("       randtest31 practrand RNG | RNG_test stdin\n", out);
    fputs("       randtest31 smallcrush RNG\n", out);
    fputs("\n", out);
    fputs("positional arguments:\n", out);
    fputs("  MODE        {practrand, smallcrush}\n", out);
    fputs("  RNG         RNG to test, see list below\n", out);
    fputs("\n", out);
    fputs("optional arguments:\n", out);
    fputs("  -h, --help  show this help message and exit\n", out);
    fputs("  -v          verbose output (ignored for practrand)\n", out);
    fputs("\n", out);
    fputs("available generators:\n", out);

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

#define CLI_ERROR "randtest31: error: "

int main(int argc, char** argv)
{
    bool verbose = false;
    const char *mode_str = NULL;
    const char *rng_id_str = NULL;

    for (size_t i = 1; i < argc; i++) {
        const char * cur = argv[i];

        /* Parse help flags */
        if (strcmp(cur, "-h") == 0) show_usage(false);
        if (strcmp(cur, "--help") == 0) show_usage(false);

        /* Parse flags */
        if (strcmp(cur, "-v") == 0) {
            verbose = true;
        } else if (cur[0] == '-') {
            fprintf(stderr, "randtest31: error: unknown argument '%s'\n", cur);
            show_usage(true);
        }

        /* Parse positional arguments */
        if (!mode_str) {
            mode_str = cur;
        } else if (!rng_id_str) {
            rng_id_str = cur;
        } else {
            fputs("randtest31: error: too many arguments\n", stderr);
            show_usage(true);
        }
    }

    if (!mode_str) {
        fputs(CLI_ERROR "missing MODE argument\n", stderr);
        show_usage(true);
    }

    if (!rng_id_str) {
        fputs(CLI_ERROR "missing RNG argument\n", stderr);
        show_usage(true);
    }

    enum mode mode;
    struct RNG* rng_in_use = NULL;

    if (strcmp(mode_str, "practrand") == 0) {
        mode = MODE_PRACTRAND;
    } else if (strcmp(mode_str, "smallcrush") == 0) {
        mode = MODE_SMALLCRUSH;
    } else {
        fprintf(stderr, CLI_ERROR "unknown mode '%s'\n", mode_str);
        show_usage(true);
    }

    ITERATE_RNGS(rng) {
        if (strcmp(rng_id_str, rng->id) == 0) {
            rng_in_use = rng;
            break;
        }
    }

    if (!rng_in_use) {
        fprintf(stderr, CLI_ERROR "unknown generator '%s'\n\n", rng_id_str);
        show_usage(true);
    }

    /* An amazing seed. */
    rng_in_use->init(123456789);

    switch (mode) {
    case MODE_PRACTRAND:
        do_practrand(rng_in_use);
        break;
    case MODE_SMALLCRUSH:
        do_smallcrush(rng_in_use);
        break;
    }

    return 0;
}
