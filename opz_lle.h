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

    int reg_write_08[2];
    int reg_write_09[2];
    int reg_write_0a[2];
    int reg_write_0b[2];
    int reg_write_0c[2];
    int reg_write_0d[2];
    int reg_write_0e[2];
    int reg_write_0f[2];
    int reg_write_10[2];
    int reg_write_11[2];
    int reg_write_12[2];
    int reg_write_14[2];
    int reg_write_15[2];
    int reg_write_16[2];
    int reg_write_17[2];
    int reg_write_18[2];
    int reg_write_19[2];
    int reg_write_1b[2];
    int reg_write_1c[2];
    int reg_write_1e[2];

    int reg_kon_channel[2];
    int reg_kon_operator[2];
    int reg_test[2];
    int reg_a[2];
    int reg_a3[2];
    int reg_b[2];
    int reg_c[2];
    int reg_d[2];
    int reg_e[2];
    int reg_noise_en[2];
    int reg_noise_freq[2];
    int reg_timer_a[2];
    int reg_timer_b[2];
    int reg_timer_a_load[2];
    int reg_timer_b_load[2];
    int reg_timer_a_irq[2];
    int reg_timer_b_irq[2];
    int reg_15[2];
    int reg_16[2];
    int reg_17_amd[2];
    int reg_17_pmd[2];
    int reg_lfo_freq[2];
    int reg_lfo_amd[2];
    int reg_lfo_pmd[2];
    int reg_lfo_wave[2];
    int reg_lfo2_wave[2];
    int reg_ct[2];
    int reg_1c[2];
    int reg_1e[2];

} ym2141_t;

