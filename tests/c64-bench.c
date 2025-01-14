//------------------------------------------------------------------------------
//  c64-bench.c
//  Unthrottled headless C64 emu for benchmarking / profiling.
//------------------------------------------------------------------------------
#include <stdio.h>
#define SOKOL_IMPL
#include "sokol_time.h"
#define CHIPS_IMPL
#include "chips/m6502.h"
#include "chips/m6526.h"
#include "chips/m6569.h"
#include "chips/m6581.h"
#include "chips/beeper.h"
#include "chips/kbd.h"
#include "chips/mem.h"
#include "chips/clk.h"
#include "systems/c64.h"
#include "c64-roms.h"

static struct {
    c64_t c64;
    uint8_t dummy_pixel_buffer[1024*1024];
} state;

#define NUM_USEC (1*1000000)

static void dummy_audio_callback(const float* samples, int num_samples, void* user_data) { };

int main() {
    /* provide "throw-away" pixel buffer and audio callback, so
       that the video and audio generation isn't skipped in the
       emulator
    */
    c64_init(&state.c64, &(c64_desc_t){
        .pixel_buffer = state.dummy_pixel_buffer,
        .pixel_buffer_size = sizeof(state.dummy_pixel_buffer),
        .audio_cb = dummy_audio_callback,
        .rom_char = dump_c64_char_bin,
        .rom_char_size = sizeof(dump_c64_char_bin),
        .rom_basic = dump_c64_basic_bin,
        .rom_basic_size = sizeof(dump_c64_basic_bin),
        .rom_kernal = dump_c64_kernalv3_bin,
        .rom_kernal_size = sizeof(dump_c64_kernalv3_bin)
    });
    stm_setup();
    printf("== running emulation for %.2f emulated secs\n", NUM_USEC / 1000000.0);
    uint64_t start = stm_now();
    c64_exec(&state.c64, NUM_USEC);
    printf("== time: %f sec\n", stm_sec(stm_since(start)));
    return 0;
}
