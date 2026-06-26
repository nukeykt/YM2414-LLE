#include <string.h>
#include <stdlib.h>
#include "opz_lle.h"



void OPZLLE_Clock(ym2141_t* chip, int clk) {
    chip->input.clk = clk;

    int clk1 = !clk;
    int clk2 = clk;

    int ic_async = !chip->input.n_ic;

    if (clk2) {
        chip->ic_latch[0] = ic_async;
        chip->clk_toggle[0] = !chip->clk_toggle[1] || (!chip->ic_latch[1] && ic_async);
    }
    if (clk1) {
        chip->ic_latch[1] = chip->ic_latch[0];
        chip->clk_toggle[1] = chip->clk_toggle[0];
    }

    chip->o_sy = chip->clk_toggle[1];


    int hclk1 = chip->clk_toggle[1];
    int hclk2 = !(clk1 || chip->clk_toggle[1]);

    if (hclk2) {
        chip->ic_sync = ic_async;
    }

    int data_l_en = !chip->input.n_cs && !chip->input.n_wr;
    if (data_l_en) {
        chip->data_l = chip->input.data;
    }

    chip->o_data_z = !(!chip->ic_sync && chip->input.a0 && !chip->input.n_cs && !chip->input.n_rd);

    int wr0 = chip->ic_sync && (!chip->input.n_cs && !chip->input.n_wr && !chip->input.a0);
    int wr1 = !chip->ic_sync && !chip->input.n_cs && !chip->input.n_wr && chip->input.a0;

    if (wr0) {
        chip->write0_trig = 1;
    } else if (chip->write0_l[2]) {
        chip->write0_trig = 0;
    }

    if (hclk2) {
        chip->write0_l[0] = chip->write0_trig;
        //chip->write0_l[1] = chip->write0_l[1];
        chip->write0_l[2] = chip->write0_l[1];
    }
    if (hclk1) {
        //chip->write0_l[0] = chip->write0_l[0];
        chip->write0_l[1] = chip->write0_l[0] && !wr0;
    }

    if (wr1) {
        chip->write1_trig = 1;
    } else if (chip->write1_l[2]) {
        chip->write1_trig = 0;
    }

    if (hclk2) {
        chip->write1_l[0] = chip->write1_trig;
        //chip->write1_l[1] = chip->write1_l[1];
        chip->write1_l[2] = chip->write1_l[1];
    }
    if (hclk1) {
        //chip->write1_l[0] = chip->write1_l[0];
        chip->write1_l[1] = chip->write1_l[0] && !wr1;
    }

    int write0_en = chip->write0_l[2];
    int write1_en = chip->write1_l[2];

    int wr_hclk1 = hclk1 && !clk1;

    if (wr_hclk1) {
        if (write0_en) {
            chip->reg_write_08[0] = chip->data_l == 8;
            chip->reg_write_09[0] = chip->data_l == 9;
            chip->reg_write_0a[0] = chip->data_l == 0xa;
            chip->reg_write_0b[0] = chip->data_l == 0xb;
            chip->reg_write_0c[0] = chip->data_l == 0xc;
            chip->reg_write_0d[0] = chip->data_l == 0xd;
            chip->reg_write_0e[0] = chip->data_l == 0xe;
            chip->reg_write_0f[0] = chip->data_l == 0xf;
            chip->reg_write_10[0] = chip->data_l == 0x10;
            chip->reg_write_11[0] = chip->data_l == 0x11;
            chip->reg_write_12[0] = chip->data_l == 0x12;
            chip->reg_write_14[0] = chip->data_l == 0x14;
            chip->reg_write_15[0] = chip->data_l == 0x15;
            chip->reg_write_16[0] = chip->data_l == 0x16;
            chip->reg_write_17[0] = chip->data_l == 0x17;
            chip->reg_write_18[0] = chip->data_l == 0x18;
            chip->reg_write_19[0] = chip->data_l == 0x19;
            chip->reg_write_1b[0] = chip->data_l == 0x1b;
            chip->reg_write_1c[0] = chip->data_l == 0x1c;
            chip->reg_write_1e[0] = chip->data_l == 0x1e;
        } else {
            chip->reg_write_08[0] = chip->reg_write_08[1];
            chip->reg_write_09[0] = chip->reg_write_09[1];
            chip->reg_write_0a[0] = chip->reg_write_0a[1];
            chip->reg_write_0b[0] = chip->reg_write_0b[1];
            chip->reg_write_0c[0] = chip->reg_write_0c[1];
            chip->reg_write_0d[0] = chip->reg_write_0d[1];
            chip->reg_write_0f[0] = chip->reg_write_0f[1];
            chip->reg_write_10[0] = chip->reg_write_10[1];
            chip->reg_write_11[0] = chip->reg_write_11[1];
            chip->reg_write_12[0] = chip->reg_write_12[1];
            chip->reg_write_14[0] = chip->reg_write_14[1];
            chip->reg_write_15[0] = chip->reg_write_15[1];
            chip->reg_write_16[0] = chip->reg_write_16[1];
            chip->reg_write_17[0] = chip->reg_write_17[1];
            chip->reg_write_18[0] = chip->reg_write_18[1];
            chip->reg_write_19[0] = chip->reg_write_19[1];
            chip->reg_write_1b[0] = chip->reg_write_1b[1];
            chip->reg_write_1c[0] = chip->reg_write_1c[1];
            chip->reg_write_1e[0] = chip->reg_write_1e[1];
        }

        if (chip->ic_sync) {
            chip->reg_kon_channel[0] = 0;
            chip->reg_kon_operator[0] = 0;
            chip->reg_test[0] = 0;
            chip->reg_a[0] = 0;
            chip->reg_a3[0] = 0;
            chip->reg_b[0] = 0;
            chip->reg_c[0] = 0;
            chip->reg_d[0] = 0;
            chip->reg_e[0] = 0;
            chip->reg_noise_en[0] = 0;
            chip->reg_noise_freq[0] = 0;
            chip->reg_timer_a[0] = 0;
            chip->reg_timer_b[0] = 0;
            chip->reg_timer_a_load[0] = 0;
            chip->reg_timer_b_load[0] = 0;
            chip->reg_timer_a_irq[0] = 0;
            chip->reg_timer_b_irq[0] = 0;
            chip->reg_15[0] = 0;
            chip->reg_16[0] = 0;
            chip->reg_17_amd[0] = 0;
            chip->reg_17_pmd[0] = 0;
            chip->reg_lfo_freq[0] = 0;
            chip->reg_lfo_amd[0] = 0;
            chip->reg_lfo_pmd[0] = 0;
            chip->reg_lfo_wave[0] = 0;
            chip->reg_lfo2_wave[0] = 0;
            chip->reg_ct[0] = 0;
            chip->reg_1c[0] = 0;
            chip->reg_1e[0] = 0;
        } else {
            int newm = (chip->reg_15[1] & 1) != 0;
            if (write1_en && chip->reg_write_08[1]) {
                chip->reg_kon_channel[0] = chip->data_l & 7;
                chip->reg_kon_operator[0] = (chip->data_l >> 3) & 15;
            } else {
                chip->reg_kon_channel[0] = chip->reg_kon_channel[1];
                chip->reg_kon_operator[0] = chip->reg_kon_operator[1];
            }
            if (write1_en && chip->reg_write_09[1]) {
                chip->reg_test[0] = chip->data_l & 7;
            } else {
                chip->reg_test[0] = chip->reg_test[1];
            }
            if (write1_en && chip->reg_write_0a[1]) {
                chip->reg_a[0] = chip->data_l & 7;
                chip->reg_a3[0] = (chip->data_l >> 3) & 1;
            } else {
                chip->reg_a[0] = chip->reg_a[1];
                chip->reg_a3[0] = chip->reg_a3[1];
            }
            if (write1_en && chip->reg_write_0b[1]) {
                chip->reg_b[0] = chip->data_l;
            } else {
                chip->reg_b[0] = chip->reg_b[1];
            }
            if (write1_en && chip->reg_write_0c[1]) {
                chip->reg_c[0] = chip->data_l;
            } else {
                chip->reg_c[0] = chip->reg_c[1];
            }
            if (write1_en && chip->reg_write_0d[1]) {
                chip->reg_d[0] = chip->data_l;
            } else {
                chip->reg_d[0] = chip->reg_d[1];
            }
            if (write1_en && chip->reg_write_0e[1]) {
                chip->reg_e[0] = chip->data_l;
            } else {
                chip->reg_e[0] = chip->reg_e[1];
            }
            if (write1_en && chip->reg_write_0f[1]) {
                chip->reg_noise_freq[0] = chip->data_l & 31;
            } else {
                chip->reg_noise_freq[0] = chip->reg_noise_freq[1];
            }
            chip->reg_timer_a[0] = chip->reg_timer_a[1];
            if (write1_en && chip->reg_write_11[1]) {
                chip->reg_timer_a[0] &= ~0x3;
                chip->reg_timer_a[0] |= chip->data_l & 3;
            }
            if (write1_en && chip->reg_write_12[1]) {
                chip->reg_timer_b[0] = chip->data_l;
            } else {
                chip->reg_timer_b[0] = chip->reg_timer_b[1];
            }
            if (write1_en && chip->reg_write_14[1]) {
                chip->reg_timer_a_load[0] = chip->data_l & 1;
                chip->reg_timer_b_load[0] = (chip->data_l >> 1) & 1;
                chip->reg_timer_a_irq[0] = (chip->data_l >> 2) & 1;
                chip->reg_timer_b_irq[0] = (chip->data_l >> 3) & 1;
            } else {
                chip->reg_timer_a_load[0] = chip->reg_timer_a_load[1];
                chip->reg_timer_b_load[0] = chip->reg_timer_b_load[1];
                chip->reg_timer_a_irq[0] = chip->reg_timer_a_irq[1];
                chip->reg_timer_b_irq[0] = chip->reg_timer_b_irq[1];
            }
            if (write1_en && chip->reg_write_15[1]) {
                chip->reg_15[0] = chip->data_l & 3;
            } else {
                chip->reg_15[0] = chip->reg_15[1];
            }
            if (write1_en && chip->reg_write_16[1] && newm) {
                chip->reg_16[0] = chip->data_l;
            } else {
                chip->reg_16[0] = chip->reg_16[1];
            }
            if (write1_en && chip->reg_write_17[1] && newm && (chip->data_l & 128) == 0) {
                chip->reg_17_amd[0] = chip->data_l & 127;
            } else {
                chip->reg_17_amd[0] = chip->reg_17_amd[1];
            }
            if (write1_en && chip->reg_write_17[1] && newm && (chip->data_l & 128) != 0) {
                chip->reg_17_pmd[0] = chip->data_l & 127;
            } else {
                chip->reg_17_pmd[0] = chip->reg_17_pmd[1];
            }
            if (write1_en && chip->reg_write_18[1] && newm) {
                chip->reg_lfo_freq[0] = chip->data_l;
            } else {
                chip->reg_lfo_freq[0] = chip->reg_lfo_freq[1];
            }
            if (write1_en && chip->reg_write_19[1] && (chip->data_l & 128) == 0) {
                chip->reg_lfo_amd[0] = chip->data_l & 127;
            } else {
                chip->reg_lfo_amd[0] = chip->reg_lfo_amd[1];
            }
            if (write1_en && chip->reg_write_19[1] && (chip->data_l & 128) != 0) {
                chip->reg_lfo_pmd[0] = chip->data_l & 127;
            } else {
                chip->reg_lfo_pmd[0] = chip->reg_lfo_pmd[1];
            }
            if (write1_en && chip->reg_write_1b[1]) {
                chip->reg_lfo_wave[0] = chip->data_l & 3;
                chip->reg_ct[0] = (chip->data1 >> 6) & 3;
            } else {
                chip->reg_lfo_wave[0] = chip->reg_lfo_wave[1];
                chip->reg_ct[0] = chip->reg_ct[1];
            }
            if (write1_en && chip->reg_write_1b[1] && newm) {
                chip->reg_lfo2_wave[0] = (chip->data_l >> 2) & 3;
            } else {
                chip->reg_lfo2_wave[0] = chip->reg_lfo2_wave[1];
            }
            if (write1_en && chip->reg_write_1c[1] && newm) {
                chip->reg_1c[0] = chip->data_l;
            } else {
                chip->reg_1c[0] = chip->reg_1c[1];
            }
            if (write1_en && chip->reg_write_1e[1] && newm) {
                chip->reg_1e[0] = chip->data_l;
            } else {
                chip->reg_1e[0] = chip->reg_1e[1];
            }
        }
    }
    if (hclk2) {
        chip->reg_write_08[1] = chip->reg_write_08[0];
        chip->reg_write_09[1] = chip->reg_write_09[0];
        chip->reg_write_0a[1] = chip->reg_write_0a[0];
        chip->reg_write_0b[1] = chip->reg_write_0b[0];
        chip->reg_write_0c[1] = chip->reg_write_0c[0];
        chip->reg_write_0d[1] = chip->reg_write_0d[0];
        chip->reg_write_0e[1] = chip->reg_write_0e[0];
        chip->reg_write_0f[1] = chip->reg_write_0f[0];
        chip->reg_write_11[1] = chip->reg_write_11[0];
        chip->reg_write_12[1] = chip->reg_write_12[0];
        chip->reg_write_14[1] = chip->reg_write_14[0];
        chip->reg_write_15[1] = chip->reg_write_15[0];
        chip->reg_write_16[1] = chip->reg_write_16[0];
        chip->reg_write_17[1] = chip->reg_write_17[0];
        chip->reg_write_18[1] = chip->reg_write_18[0];
        chip->reg_write_19[1] = chip->reg_write_19[0];
        chip->reg_write_1b[1] = chip->reg_write_1b[0];
        chip->reg_write_1c[1] = chip->reg_write_1c[0];
        chip->reg_write_1e[1] = chip->reg_write_1e[0];

        chip->reg_kon_channel[1] = chip->reg_kon_channel[0];
        chip->reg_kon_operator[1] = chip->reg_kon_operator[0];
        chip->reg_test[1] = chip->reg_test[0];
        chip->reg_a[1] = chip->reg_a[0];
        chip->reg_a3[1] = chip->reg_a3[0];
        chip->reg_b[1] = chip->reg_b[0];
        chip->reg_c[1] = chip->reg_c[0];
        chip->reg_d[1] = chip->reg_d[0];
        chip->reg_e[1] = chip->reg_e[0];
        chip->reg_noise_en[1] = chip->reg_noise_en[0];
        chip->reg_noise_freq[1] = chip->reg_noise_freq[0];
        chip->reg_timer_a[1] = chip->reg_timer_a[0];
        chip->reg_timer_b[1] = chip->reg_timer_b[0];
        chip->reg_timer_a_load[1] = chip->reg_timer_a_load[0];
        chip->reg_timer_b_load[1] = chip->reg_timer_b_load[0];
        chip->reg_timer_a_irq[1] = chip->reg_timer_a_irq[0];
        chip->reg_timer_b_irq[1] = chip->reg_timer_b_irq[0];
        chip->reg_15[1] = chip->reg_15[0];
        chip->reg_16[1] = chip->reg_16[0];
        chip->reg_17_amd[1] = chip->reg_17_amd[0];
        chip->reg_17_pmd[1] = chip->reg_17_pmd[0];
        chip->reg_lfo_freq[1] = chip->reg_lfo_freq[0];
        chip->reg_lfo_pmd[1] = chip->reg_lfo_pmd[0];
        chip->reg_lfo_amd[1] = chip->reg_lfo_amd[0];
        chip->reg_lfo2_wave[1] = chip->reg_lfo2_wave[0];
        chip->reg_lfo_wave[1] = chip->reg_lfo_wave[0];
        chip->reg_ct[1] = chip->reg_ct[0];
        chip->reg_1c[1] = chip->reg_1c[0];
        chip->reg_1e[1] = chip->reg_1e[0];
    }

    if (clk2) {
        chip->fsm_ic_latch[0] = (chip->fsm_ic_latch[1] << 1) | ic_async;
        chip->fsm_rst = ic_async && (chip->fsm_ic_latch[1] & 2) == 0;
    }
    if (clk1) {
        chip->fsm_ic_latch[1] = chip->fsm_ic_latch[0];
    }

    if (hclk1) {
        chip->fsm_cnt[0] = chip->fsm_rst ? 0 : ((chip->fsm_cnt[1] + 1) & 31);

        int cnt = chip->fsm_cnt[1];
        chip->fsm_4 = cnt == 4;
        chip->fsm_8[0] = cnt == 7;
        chip->fsm_13[0] = cnt == 12;
        chip->fsm_14 = cnt == 14;
        chip->fsm_19[0] = cnt == 19;
        chip->fsm_29[0] = cnt == 28;
        chip->fsm_30[0] = cnt == 29;
        chip->fsm_31[0] = cnt == 30;
        chip->fsm_o1[0] = cnt == 30 || cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5 || cnt == 6 || cnt == 7 || cnt == 8 || cnt == 9 || cnt == 10 || cnt == 11 || cnt == 12 || cnt == 13;
        chip->fsm_o2[0] = cnt == 8 || cnt == 24;
        chip->fsm_o3[0] = cnt == 1 || cnt == 17;
        chip->fsm_o4[0] = cnt == 30 || cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5 || cnt == 14 || cnt == 15 || cnt == 16 || cnt == 17 || cnt == 18 || cnt == 19 || cnt == 20 || cnt == 21;
        chip->fsm_o5[0] = cnt == 3 || cnt == 4 || cnt == 5 || cnt == 6 || cnt == 7 || cnt == 8 || cnt == 9 || cnt == 10 || cnt == 11 || cnt == 12 || cnt == 13 || cnt == 14 || cnt == 15 || cnt == 16 || cnt == 17 || cnt == 18;
        chip->fsm_o6[0] = cnt == 4 || cnt == 20;
        chip->fsm_o7[0] = cnt == 14 || cnt == 15 || cnt == 16 || cnt == 17 || cnt == 18 || cnt == 19 || cnt == 20 || cnt == 21;
        chip->fsm_o8[0] = cnt == 30 || cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5;
        chip->fsm_o9[0] = cnt == 2 || cnt == 10 || cnt == 18 || cnt == 26;
        chip->fsm_o10[0] = cnt == 30 || cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5;
        chip->fsm_o11[0] = cnt == 30 || cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5 || cnt == 6 || cnt == 7;
        chip->fsm_o12[0] = cnt == 8 || cnt == 9 || cnt == 10 || cnt == 11 || cnt == 12 || cnt == 13 || cnt == 14 || cnt == 15 || cnt == 16 || cnt == 17;
        chip->fsm_o13[0] = cnt == 31 || cnt == 0 || cnt == 1 || cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5 || cnt == 6 || cnt == 7;
        chip->fsm_o14[0] = cnt == 21 || cnt == 22 || cnt == 23 || cnt == 24 || cnt == 25 || cnt == 26 || cnt == 27 || cnt == 28 || cnt == 29;
        chip->fsm_o15[0] = cnt == 20 || cnt == 21 || cnt == 22 || cnt == 23 || cnt == 24 || cnt == 25 || cnt == 26 || cnt == 27 || cnt == 28 || cnt == 29;
        chip->fsm_o16 = cnt == 11 || cnt == 27;
        chip->fsm_o17[0] = cnt == 22 || cnt == 23 || cnt == 24 || cnt == 25 || cnt == 26 || cnt == 27;
        chip->fsm_o18[0] = cnt == 20 || cnt == 21;
        chip->fsm_o19[0] = cnt == 22 || cnt == 23 || cnt == 24 || cnt == 25 || cnt == 26 || cnt == 27 || cnt == 28 || cnt == 29;
        chip->fsm_o20[0] = cnt == 2 || cnt == 18;
        chip->fsm_o21 = cnt == 14 || cnt == 30;

        chip->fsm_op_sync[0] = (chip->fsm_op_sync[1] << 1) | chip->fsm_o9[1];

        if (chip->fsm_13[1]) {
            chip->fsm_op_cnt[0] = 0;
        } else {
            chip->fsm_op_cnt[0] = chip->fsm_op_cnt[1] + ((chip->fsm_op_sync[1] >> 1) & 1);
        }
        chip->fsm_alg_latch = x;
    }
    if (hclk2) {
        chip->fsm_cnt[1] = chip->fsm_cnt[0];
        chip->fsm_8[1] = chip->fsm_8[0];
        chip->fsm_13[1] = chip->fsm_13[0];
        chip->fsm_19[1] = chip->fsm_19[0];
        chip->fsm_29[1] = chip->fsm_29[0];
        chip->fsm_30[1] = chip->fsm_30[0];
        chip->fsm_31[1] = chip->fsm_31[0];
        chip->fsm_o1[1] = chip->fsm_o1[0];
        chip->fsm_o2[1] = chip->fsm_o2[0];
        chip->fsm_o3[1] = chip->fsm_o3[0];
        chip->fsm_o4[1] = chip->fsm_o4[0];
        chip->fsm_o5[1] = chip->fsm_o5[0];
        chip->fsm_o6[1] = chip->fsm_o6[0];
        chip->fsm_o7[1] = chip->fsm_o7[0];
        chip->fsm_o8[1] = chip->fsm_o8[0];
        chip->fsm_o9[1] = chip->fsm_o9[0];
        chip->fsm_o10[1] = chip->fsm_o10[0];
        chip->fsm_o11[1] = chip->fsm_o11[0];
        chip->fsm_o12[1] = chip->fsm_o12[0];
        chip->fsm_o13[1] = chip->fsm_o13[0];
        chip->fsm_o14[1] = chip->fsm_o14[0];
        chip->fsm_o15[1] = chip->fsm_o15[0];
        chip->fsm_o16[1] = chip->fsm_o16[0];
        chip->fsm_o17[1] = chip->fsm_o17[0];
        chip->fsm_o18[1] = chip->fsm_o18[0];
        chip->fsm_o19[1] = chip->fsm_o19[0];
        chip->fsm_o20[1] = chip->fsm_o20[0];
        chip->fsm_op_sync[1] = chip->fsm_op_sync[0];
        chip->fsm_op_cnt[1] = chip->fsm_op_cnt[0];
        int op = chip->fsm_op_cnt[0];
        int alg = chip->fsm_alg_latch;
        int unk = (chip->fsm_o9[0] & 3) != 0 && (chip->reg_15[0] & 3) == 3;
        chip->fsm_alg_o[0] = op == 0;
        chip->fsm_alg_o[1] = 0;
        chip->fsm_alg_o[2] = 0;
        chip->fsm_alg_o[3] = 0;
        chip->fsm_alg_o[4] = 0;
        chip->fsm_alg_o[5] = 0;
        chip->fsm_alg_o[6] = 0;
        chip->fsm_alg_o[7] = 0;
        chip->fsm_alg_o[8] = 0;
        switch (op) {
            case 0:
                if (unk) {
                    chip->fsm_alg_o[6] = 1;
                } else {
                    chip->fsm_alg_o[6] = alg == 0 || alg == 3 || alg == 4 || alg == 5 || alg == 6;
                    chip->fsm_alg_o[8] = alg == 7;
                }
                break;
            case 1:
                if (unk) {
                    chip->fsm_alg_o[2] = 1;
                    chip->fsm_alg_o[6] = 1;
                } else {
                    chip->fsm_alg_o[3] = alg == 2 || alg == 5;
                    chip->fsm_alg_o[4] = alg == 3;
                    chip->fsm_alg_o[6] = alg == 0 || alg == 1 || alg == 2 || alg == 3 || alg == 4;
                    chip->fsm_alg_o[8] = alg == 5 || alg == 6 || alg == 7;
                }
                break;
            case 2:
                chip->fsm_alg_o[1] = 1;
                chip->fsm_alg_o[3] = 1;
                chip->fsm_alg_o[7] = 1;
                if (unk) {
                    chip->fsm_alg_o[8] = 1;
                } else {
                    chip->fsm_alg_o[2] = alg == 0 || alg == 1 || alg == 2 || alg == 3 || alg == 5 || alg == 6 || alg == 7;
                    chip->fsm_alg_o[8] = alg == 4 || alg == 5 || alg == 6 || alg == 7;
                }
                break;
            case 3:
                chip->fsm_alg_o[8] = 1;
                if (unk) {
                    chip->fsm_alg_o[1] = 1;
                    chip->fsm_alg_o[4] = 1;
                    chip->fsm_alg_o[5] = 1;
                } else {
                    chip->fsm_alg_o[3] = alg == 5;
                    chip->fsm_alg_o[4] = alg == 0 || alg == 1 || alg == 2;
                    chip->fsm_alg_o[7] = alg == 1;
                }
                break;
        }
    }

    if (hclk1) {
        int rst = ic_async || (chip->noise_cnt_inc && chip->noise_cnt_match[0]);
        if (rst) {
            chip->noise_cnt[0] = 0;
        } else {
            chip->noise_cnt[0] = (chip->noise_cnt[1] + chip->noise_cnt_inc) & 31;
        }
        chip->noise_cnt_match[1] = chip->noise_cnt_match[0];

        int noise_step = ic_async || chip->noise_cnt_match[2];

        if (noise_step) {
            chip->noise_bit[0] = (chip->noise_lfsr[1] >> 15) & 1;
        } else {
            chip->noise_bit[0] = chip->noise_bit[1];
        }

        chip->noise_lfsr[0] = chip->noise_lfsr[1] << 1;
        if (noise_step) {
            int rst = (chip->noise_lfsr[1] & 0xffff) == 0 && !chip->noise_bit[1];
            int xr = (chip->noise_lfsr[1] >> 13) & 1;
            xr ^= chip->noise_bit[1];
        } else {
        }

    }
    if (hclk2) {
        chip->noise_cnt[1] = chip->noise_cnt[0];
        chip->noise_cnt_inc = chip->fsm_o21;
        chip->noise_cnt_match[0] = chip->noise_cnt[0] == (chip->reg_noise_freq[0] ^ 31);
        chip->noise_cnt_match[2] = chip->noise_cnt_match[1];
        chip->noise_bit[1] = chip->noise_bit[0];
        chip->noise_lfsr[1] = chip->noise_lfsr[0];
    }

}

