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
    int cnt1[2];
    int cnt1_of_l;
    int subcnt_of;
    int cnt1_load[4];
    int freq_write[3];
    int cnt1_rst;
    int cnt1_h[2];
    int cnt1_of_h;
    int cnt1_load_val_hi;
    int inc[2];
    int cnt1_of_h_lock;
    int cnt1_of_h_latch;
    int cnt2_inc;
    int cnt2[2];
    int cnt2_of[2];
    int bcnt[2];
    int premul[2];
    int bit;
    int out_shifter[2];
    int sum_c_out;
    int sum_c_in;
    int wave1;
    int wave2;
    int wave3;
    int sel;
    int inc_lock;
    int sign_saw;
    int sign_saw_l;
    int sign_trig;
    int shifter[2];
    int sum2_c_out[2];
    int cnt3_sync[2];
    int cnt3_inc[2];
    int cnt3[2];
    int bb;
    int pm;
    int pmd_zero;
    int pm_out;
    int pm_sign;
    int am;
} ym2414_lfo_t;

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

    int reg_address[2];
    int reg_address_valid[2];
    int reg_data[2];
    int reg_data_valid[2];
    int reg_counter[2];
    int reg_match00;
    int reg_match20;
    int reg_match20_l[2];
    int reg_match28;
    int reg_match30;
    int reg_match38;
    int reg_match40;
    int reg_match60;
    int reg_match80;
    int reg_matcha0;
    int reg_matchc0;
    int reg_matche0;
    int reg_op_sel[2];
    int reg_ch_sel[2];
    int reg_unkmode;
    int reg_unksel1;
    int reg_unksel2;

    int reg_kon[4][2];
    int reg_kon_match;

    int reg_ch00_l[2];
    int reg_ch20_l[2];
    int reg_ch20_l2[2];
    int reg_ch28_l[2];
    int reg_ch28_l2[2];
    int reg_ch30_l[2];
    int reg_ch30new_l[2];
    int reg_ch38_l[2];
    int reg_ch38new_l[2];
    int reg_op40_l[2];
    int reg_op40new_l[2];
    int reg_op60_l[2];
    int reg_op80_l[2];
    int reg_opa0_l[2];
    int reg_opc0_l[2];
    int reg_opc0new_l[2];

    int reg_alg[2];

    uint64_t reg_ch_cell[8];
    uint64_t reg_ch_bus;
    uint64_t reg_op1_cell[2][16];
    uint64_t reg_op1_bus[2];
    uint64_t reg_op1_bus_l[2];
    uint64_t reg_op2_cell[2][16];
    uint64_t reg_op2_bus[2];
    uint64_t reg_op2_bus_l[2];

    int lfo_subcnt[2];
    int lfo_subcnt_of[3];
    int lfo_sync[2];
    int lfo_test;
    int lfo_sync2[2];
    int lfo_pmsel[2];

    ym2414_lfo_t lfo1, lfo2;

    int reg_30_0_l[2];
    int reg_30_1_l[2];

    int freq_kc[8];
    int freq_kf[5];
    int freq_km;
    int freq_kc_add_sign;
    int freq_kc_add_c;
    int freq_kc_add;
    int freq_kc_clip_h[2];
    int freq_kc_clip_l[2];
    int freq_lfo_pm;
    int freq_lfo_pms[4];
    int freq_lfo_sign[4];
    int freq_lfo_add;
    int freq_lfo_add_shift;
    int freq_lfo_add_shift_h;
    int freq_kc_lfo_suml[4];
    int freq_kc_lfo_sumlof;
    int freq_kc_lfo_sumh[5];
    int freq_kc_lfo_sumh_add;
    int freq_kc_lfo_sumh_sub;
    int freq_kc_ch;
    int freq_kc_cl;
    int freq_kc_ch2;
    int freq_kc_cl2;
    int freq_dt2_del[2][2];
    int freq_dt2_del2[2];
    int freq_dt_sum_c;
    int freq_dt_sumh_cin;
    int freq_dt_sumh_b1;
    int freq_kf_dt[3];
    int freq_kc_dt;
    int freq_kc_dt_of;
    int freq_rom_addr[3];
    int freq_rom_base[2];
    int freq_rom_base_h;
    int freq_rom_type;
    int freq_rom_slope;
    int freq_freq_frac[4];
    int freq_freq_frac0;
    int freq_lerp;
    int freq_fnum_l;
    int freq_fnum_l_of;
    int freq_fnum;
    int freq_block[4];
} ym2141_t;

