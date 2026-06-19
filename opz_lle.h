#pragma once
#include <stdint.h>

typedef struct {
    int clk;
    int n_rd;
    int n_wr;
    int n_ic;
    int n_cs;
    int a0;
    int data;
} ym2414_input_t;

typedef struct {
    ym2414_input_t input;

    int o_sy;
    int o_so;
    int o_sh1;
    int o_sh2_pull; // pull gnd -> inverted signal
    int o_data;
    int o_data_z;
    int o_ct1;
    int o_ct2;
    int o_irq_pull; // pull gnd

    int ic_async;
    int ic_sync;
    int ic_latch[2];
    int clk_toggle[2];

    int data_l;
    int write0_trig;
    int write0_l[3];
    int write1_trig;
    int write1_l[3];

} ym2141_t;

