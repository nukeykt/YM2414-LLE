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

    int fsm_ic_latch[2];
    int fsm_rst;
    int fsm_cnt[2];
    int fsm_4;
    int fsm_8[2];
    int fsm_13[2];
    int fsm_14;
    int fsm_19[2];
    int fsm_29[2];
    int fsm_30[2];
    int fsm_31[2];
    int fsm_o1[2]; // 31-14
    int fsm_o2[2]; // 9, 25
    int fsm_o3[2]; // 2, 18
    int fsm_o4[2]; // 31-6, 15-22
    int fsm_o5[2]; // 4-19
    int fsm_o6[2]; // 5, 21
    int fsm_o7[2]; // 15-22
    int fsm_o8[2]; // 31-6
    int fsm_o9[2]; // 3, 11, 19, 27
    int fsm_o10[2]; // 31-6
    int fsm_o11[2]; // 31-8
    int fsm_o12[2]; // 9-18
    int fsm_o13[2]; // 0-8
    int fsm_o14[2]; // 22-30
    int fsm_o15[2]; // 21-30
    int fsm_o16; // 11, 27
    int fsm_o17[2]; // 23-28
    int fsm_o18[2]; // 21-22
    int fsm_o19[2]; // 23-30
    int fsm_o20[2]; // 3, 19
    int fsm_o21; // 14, 30
    int fsm_op_sync[2];
    int fsm_op_cnt[2];
    int fsm_alg_latch;
    int fsm_alg_o[9];

    int noise_cnt[2];
    int noise_cnt_match[3];
    int noise_cnt_inc;
    int noise_bit[2];
    int noise_lfsr[2];

} ym2141_t;

