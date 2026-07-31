#include <string.h>
#include <stdlib.h>
#include "opz_lle.h"



void OPZLLE_Clock(ym2141_t* chip, int clk) {
    int i;

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
            if (write1_en && chip->reg_write_18[1]) {
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
        chip->fsm_alg_latch = chip->reg_alg[1];
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
            if (!ic_async) {
                int rst = (chip->noise_lfsr[1] & 0xffff) == 0 && !chip->noise_bit[1];
                int xr = (chip->noise_lfsr[1] >> 13) & 1;
                xr ^= chip->noise_bit[1];
                chip->noise_lfsr[0] |= rst | xr;
            }
        } else {
            chip->noise_lfsr[0] |= (chip->noise_lfsr[1] >> 15) & 1;
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

    if (wr_hclk1) {
        int addr_write = ((chip->data_l & 0xe0) != 0 || (chip->data_l & 0xf8) == 0) && write0_en;
        if (chip->ic_sync) {
            chip->reg_address[0] = 0;
        } else if (addr_write) {
            chip->reg_address[0] = chip->data_l;
        } else {
            chip->reg_address[0] = chip->reg_address[1];
        }
        chip->reg_address_valid[0] = addr_write || (chip->reg_address_valid[1] && !write0_en);

        int data_write = chip->reg_address_valid[1] && write1_en;
        if (chip->ic_sync) {
            chip->reg_data[0] = 0;
        } else if (data_write) {
            chip->reg_data[0] = chip->data_l;
        } else {
            chip->reg_data[0] = chip->reg_data[1];
        }
    }
    if (hclk2) {
        chip->reg_address[1] = chip->reg_address[0];
        chip->reg_address_valid[1] = chip->reg_address_valid[0];
        chip->reg_data[1] = chip->reg_data[0];
        chip->reg_data_valid[1] = chip->reg_data_valid[0];
    }

    if (hclk1) {
        int data_write = chip->reg_address_valid[1] && write1_en;
        chip->reg_data_valid[0] = data_write || (chip->reg_data_valid[1] && !write0_en);
        if (chip->fsm_30[1]) {
            chip->reg_counter[0] = 0;
        } else {
            chip->reg_counter[0] = (chip->reg_counter[1] + 1) & 31;
        }

        chip->reg_op_sel[0] = (chip->reg_op_sel[1] << 1) | chip->fsm_o20[1];
        chip->reg_ch_sel[0] = (chip->reg_ch_sel[1] << 1) | chip->fsm_o9[1];

        for (i = 0; i < 8; i++) {
            if (chip->reg_ch_sel[1] & (1 << i)) {
                chip->reg_ch_bus |= chip->reg_ch_cell[i];
            }
        }

        uint64_t reg_ch_in = 0;
        reg_ch_in |= (uint64_t)chip->reg_ch00_l[1];
        reg_ch_in |= (uint64_t)chip->reg_ch20_l[1] << 8;
        reg_ch_in |= (chip->reg_ch_bus << 8) & 0xff0000ull;
        reg_ch_in |= (uint64_t)chip->reg_ch28_l[1] << 24;
        reg_ch_in |= (chip->reg_ch_bus << 7) & 0x3f80000000ull;
        reg_ch_in |= (uint64_t)chip->reg_ch30_l[1] << 38;
        reg_ch_in |= (uint64_t)chip->reg_ch30new_l[1] << 44;
        reg_ch_in |= (uint64_t)chip->reg_ch38_l[1] << 46;
        reg_ch_in |= (uint64_t)chip->reg_ch38new_l[1] << 51;

        for (i = 0; i < 8; i++) {
            if (chip->reg_ch_sel[1] & (2 << i)) {
                chip->reg_ch_cell[i] = reg_ch_in;
            }
        }

        chip->reg_alg[0] = (chip->reg_ch_bus >> 8) & 7;


        int unkaddr = (chip->reg_address[0] & 6) != 6;

        chip->reg_match20_l[0] = chip->reg_match20_l[1] << 1;
        if (chip->reg_match20 && (!chip->reg_unkmode || unkaddr || !chip->reg_unksel1)) {
            chip->reg_match20_l[0] |= 1;
        }
        
        if (chip->reg_match00 || ic_async) {
            chip->reg_ch00_l[0] = chip->reg_data[1];
        } else {
            chip->reg_ch00_l[0] = chip->reg_ch_bus & 255;
        }
        if ((chip->reg_match20_l[1] & 16) != 0 || ic_async) {
            chip->reg_ch20_l[0] = chip->reg_data[1];
        } else {
            chip->reg_ch20_l[0] = chip->reg_ch20_l2[1];
        }

        int match28 = chip->ic_sync || (chip->reg_match28 && (!chip->reg_unkmode || unkaddr || !chip->reg_unksel2));
        if (match28 || ic_async) {
            chip->reg_ch28_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_ch28_l[0] = chip->reg_ch28_l2[1];
        }
        chip->reg_ch28_l2[0] = (chip->reg_ch_bus >> 31) & 127;

        if (chip->reg_match30 || ic_async) {
            chip->reg_ch30_l[0] = (chip->reg_data[1] >> 2) & 63;
        } else {
            chip->reg_ch30_l[0] = (chip->reg_ch_bus >> 38) & 63;
        }

        int newm = chip->reg_15[0] & 1;

        if ((chip->reg_match30 && newm) || ic_async) {
            chip->reg_ch30new_l[0] = chip->reg_data[1] & 3;
        } else {
            chip->reg_ch30new_l[0] = (chip->reg_ch_bus >> 44) & 3;
        }

        if (chip->reg_match38 || ic_async) {
            chip->reg_ch38_l[0] = chip->reg_data[1] & 3; // ams
            chip->reg_ch38_l[0] |= (chip->reg_data[1] >> 2) & 28; // pms
        } else {
            chip->reg_ch38_l[0] = (chip->reg_ch_bus >> 46) & 31;
        }

        if ((chip->reg_match38 && newm) || ic_async) {
            chip->reg_ch38new_l[0] = (chip->reg_data[1] >> 2) & 3;
            chip->reg_ch38new_l[0] |= (chip->reg_data[1] >> 5) & 4;
        } else {
            chip->reg_ch38new_l[0] = (chip->reg_ch_bus >> 51) & 7;
        }

        int bank = !newm || (chip->reg_data[1] & 128) != 0;
        if ((chip->reg_match40 && !bank) || ic_async) {
            chip->reg_op40_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_op40_l[0] = chip->reg_op1_bus_l[1] & 127;
        }

        if ((chip->reg_match40 && bank) || ic_async) {
            chip->reg_op40new_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_op40new_l[0] = (chip->reg_op1_bus_l[1] >> 7) & 127;
        }

        if (chip->reg_match60 || ic_async) {
            chip->reg_op60_l[0] = chip->reg_data[1];
        } else {
            chip->reg_op60_l[0] = (chip->reg_op1_bus_l[1] >> 14) & 255;
        }

        chip->reg_op80_l[0] = 0;
        if (chip->reg_match80 || ic_async) {
            chip->reg_op80_l[0] |= chip->reg_data[1] & 0xdf;
        } else {
            chip->reg_op80_l[0] |= chip->reg_op2_bus_l[1] & 0xdf;
        }
        if ((chip->reg_match80 && newm) || ic_async) {
            chip->reg_op80_l[0] |= chip->reg_data[1] & 0x20;
        } else {
            chip->reg_op80_l[0] |= chip->reg_op2_bus_l[1] & 0x20;
        }

        chip->reg_opa0_l[0] = 0;
        if (chip->reg_matcha0 || ic_async) {
            chip->reg_opa0_l[0] |= chip->reg_data[1] & 0x9f;
        } else {
            chip->reg_opa0_l[0] |= (chip->reg_op2_bus_l[1] >> 8) & 0x9f;
        }
        if ((chip->reg_matcha0 && newm) || ic_async) {
            chip->reg_opa0_l[0] |= chip->reg_data[1] & 0x60;
        } else {
            chip->reg_opa0_l[0] |= (chip->reg_op2_bus_l[1] >> 8) & 0x60;
        }

        bank = !newm || (chip->reg_data[1] & 32) != 0;
        if ((chip->reg_matchc0 && !bank) || ic_async) {
            chip->reg_opc0_l[0] = chip->reg_data[1] & 31;
            chip->reg_opc0_l[0] |= (chip->reg_data[1] >> 1) & 0x60;
        } else {
            chip->reg_opc0_l[0] = (chip->reg_op2_bus_l[1] >> 16) & 127;
        }

        if ((chip->reg_matchc0 && bank) || ic_async) {
            chip->reg_opc0new_l[0] = chip->reg_data[1] & 31;
            chip->reg_opc0new_l[0] |= (chip->reg_data[1] >> 1) & 0x60;
        } else {
            chip->reg_opc0new_l[0] = (chip->reg_op2_bus_l[1] >> 23) & 127;
        }

        for (i = 0; i < 16; i++) {
            if (chip->reg_op_sel[1] & (1 << i)) {
                chip->reg_op1_bus[0] |= chip->reg_op1_cell[0][i];
                chip->reg_op1_bus[1] |= chip->reg_op1_cell[1][i];
                chip->reg_op2_bus[0] |= chip->reg_op2_cell[0][i];
                chip->reg_op2_bus[1] |= chip->reg_op2_cell[1][i];
            }
        }

        uint64_t reg_op1_in = 0;
        reg_op1_in |= (uint64_t)chip->reg_op40_l[1];
        reg_op1_in |= (uint64_t)chip->reg_op40new_l[1] << 7;
        reg_op1_in |= (uint64_t)chip->reg_op60_l[1] << 14;
        reg_op1_in |= (uint64_t)x << 22;

        uint64_t reg_op2_in = 0;
        reg_op2_in |= (uint64_t)chip->reg_op80_l[1];
        reg_op2_in |= (uint64_t)chip->reg_opa0_l[1] << 8;
        reg_op2_in |= (uint64_t)chip->reg_opc0_l[1] << 16;
        reg_op2_in |= (uint64_t)chip->reg_opc0new_l[1] << 23;

        for (i = 0; i < 16; i++) {
            if (chip->reg_op_sel[1] & (2 << i)) {
                chip->reg_op1_cell[0][i] = reg_op1_in;
                chip->reg_op1_cell[1][i] = chip->reg_op1_bus[0];
                chip->reg_op2_cell[0][i] = reg_op2_in;
                chip->reg_op2_cell[1][i] = chip->reg_op2_bus[0];
            }
        }

        chip->reg_op1_bus_l[0] = chip->reg_op1_bus[1] & 0x3fffff;
        chip->reg_op2_bus_l[0] = chip->reg_op2_bus[1];
    }
    if (hclk2) {
        chip->reg_data_valid[1] = chip->reg_data_valid[0];
        chip->reg_counter[1] = chip->reg_counter[0];
        int ch_match = (chip->reg_counter[0] & 7) == (chip->reg_address[0] & 7) && chip->reg_data_valid[0];
        chip->reg_match00 = ch_match && (chip->reg_address[0] & 0x38) == 0;
        chip->reg_match20 = ch_match && (chip->reg_address[0] & 0x38) == 0x20;
        chip->reg_match28 = ch_match && (chip->reg_address[0] & 0x38) == 0x28;
        chip->reg_match30 = ch_match && (chip->reg_address[0] & 0x38) == 0x30;
        chip->reg_match38 = ch_match && (chip->reg_address[0] & 0x38) == 0x38;
        int op_match = chip->reg_counter[0] == (chip->reg_address[0] & 31) && chip->reg_data_valid[0];
        chip->reg_match40 = op_match && (chip->reg_address[0] & 0xe0) == 0x40;
        chip->reg_match60 = op_match && (chip->reg_address[0] & 0xe0) == 0x60;
        chip->reg_match80 = op_match && (chip->reg_address[0] & 0xe0) == 0x80;
        chip->reg_matcha0 = op_match && (chip->reg_address[0] & 0xe0) == 0xa0;
        chip->reg_matchc0 = op_match && (chip->reg_address[0] & 0xe0) == 0xc0;
        chip->reg_matche0 = op_match && (chip->reg_address[0] & 0xe0) == 0xe0;

        chip->reg_op_sel[1] = chip->reg_op_sel[0];
        chip->reg_ch_sel[1] = chip->reg_ch_sel[0];

        chip->reg_ch_bus = 0;
        chip->reg_op1_bus[0] = 0;
        chip->reg_op1_bus[1] = 0;
        chip->reg_op2_bus[0] = 0;
        chip->reg_op2_bus[1] = 0;
        chip->reg_ch00_l[1] = chip->reg_ch00_l[0];
        chip->reg_ch20_l[1] = chip->reg_ch20_l[0];
        chip->reg_ch20_l2[1] = chip->reg_ch20_l2[0];
        chip->reg_ch28_l[1] = chip->reg_ch28_l[0];
        chip->reg_ch28_l2[1] = chip->reg_ch28_l2[0];
        chip->reg_ch30_l[1] = chip->reg_ch30_l[0];
        chip->reg_ch30new_l[1] = chip->reg_ch30new_l[0];
        chip->reg_ch38_l[1] = chip->reg_ch38_l[0];
        chip->reg_ch38new_l[1] = chip->reg_ch38new_l[0];
        chip->reg_op40_l[1] = chip->reg_op40_l[0];
        chip->reg_op40new_l[1] = chip->reg_op40new_l[0];
        chip->reg_op60_l[1] = chip->reg_op60_l[0];
        chip->reg_op80_l[1] = chip->reg_op80_l[0];
        chip->reg_opa0_l[1] = chip->reg_opa0_l[0];
        chip->reg_opc0_l[1] = chip->reg_opc0_l[0];
        chip->reg_opc0new_l[1] = chip->reg_opc0new_l[0];

        chip->reg_op1_bus_l[1] = chip->reg_op1_bus_l[0];
        chip->reg_op2_bus_l[1] = chip->reg_op2_bus_l[0];

        chip->reg_unkmode = (chip->reg_15[0] & 3) == 3;
        chip->reg_unksel1 = (chip->reg_data[0] ^ (chip->reg_counter[0] >> 3)) & 1;
        chip->reg_unksel2 = ((chip->reg_data[0] >> 7) ^ (chip->reg_counter[0] >> 3)) & 1;

        chip->reg_match20_l[1] = chip->reg_match20_l[0];

        chip->reg_alg[1] = chip->reg_alg[0];
    }

    if (hclk1) {
        chip->reg_kon[0][0] = chip->reg_kon[0][1] << 1;
        chip->reg_kon[1][0] = chip->reg_kon[1][1] << 1;
        chip->reg_kon[2][0] = chip->reg_kon[2][1] << 1;
        chip->reg_kon[3][0] = chip->reg_kon[3][1] << 1;
        if (chip->reg_kon_match) {
            chip->reg_kon[0][0] |= (chip->reg_kon_operator[1] >> 0) & 1;
            chip->reg_kon[1][0] |= (chip->reg_kon_operator[1] >> 3) & 1;
            chip->reg_kon[2][0] |= (chip->reg_kon_operator[1] >> 1) & 1;
            chip->reg_kon[3][0] |= (chip->reg_kon_operator[1] >> 2) & 1;
        } else {
            if (!chip->ic_sync) {
                chip->reg_kon[0][0] |= (chip->reg_kon[3][1] >> 7) & 1;
            }
            chip->reg_kon[1][0] |= (chip->reg_kon[0][1] >> 7) & 1;
            chip->reg_kon[2][0] |= (chip->reg_kon[1][1] >> 7) & 1;
            chip->reg_kon[3][0] |= (chip->reg_kon[2][1] >> 7) & 1;
        }
    }
    if (hclk2) {
        chip->reg_kon_match = chip->reg_counter[0] == chip->reg_kon_channel[0];

        chip->reg_kon[0][1] = chip->reg_kon[0][0];
        chip->reg_kon[1][1] = chip->reg_kon[1][0];
        chip->reg_kon[2][1] = chip->reg_kon[2][0];
        chip->reg_kon[3][1] = chip->reg_kon[3][0];
    }

    if (hclk1) {
        chip->lfo_sync[1] = chip->lfo_sync[0];
        chip->lfo_sync2[0] = chip->fsm_o6[1];
        int subcnt = chip->lfo_subcnt[1] + (chip->lfo_sync[0] & 1);
        int sub_of = subcnt >> 4;
        if (ic_async) {
            chip->lfo_subcnt[0] = 0;
        } else {
            chip->lfo_subcnt[0] = subcnt & 15;
        }
        chip->lfo_subcnt_of[0] = sub_of;
        chip->lfo_subcnt_of[2] = chip->lfo_subcnt_of[1];

        chip->lfo_test = (chip->reg_test[1] >> 2) & 1;

        int newm = chip->reg_15[0] & 1;
        int wr1b = write1_en && chip->reg_write_1b[1] && newm;
        chip->lfo1_cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data1 & 16) != 0);
        chip->lfo2_cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data1 & 32) != 0);
    }
    if (hclk2) {
        chip->lfo_sync[0] = (chip->lfo_sync[1] << 1) | chip->fsm_o16;
        chip->lfo_sync2[1] = chip->lfo_sync2[0];
        chip->lfo_subcnt[1] = chip->lfo_subcnt[0];
        chip->lfo_subcnt_of[1] = chip->lfo_subcnt_of[0];
        chip->lfo_bcnt_rst = chip->fsm_o16 && chip->lfo_subcnt[0] == 2;

        chip->lfo1_cnt3_sync[1] = chip->lfo1_cnt3_sync[0];
    }


    if (hclk1) {
        int lfrq_h = chip->reg_lfo_freq[1] >> 4;

        int load_val = 0x8000 - (1 << (15 - lfrq_h));

        int cnt = chip->lfo1_cnt1[1] + chip->lfo1_subcnt_of;
        int of = cnt >> 8;
        chip->lfo1_cnt1_of_l = of;

        chip->lfo1_cnt1_load_val_hi = load_val >> 8;

        if (chip->lfo1_cnt1_load[2]) {
            chip->lfo1_cnt1[0] = load_val & 255;
        } else {
            chip->lfo1_cnt1[0] = ic_async ? 0 : cnt & 255;
        }

        chip->lfo1_cnt1_h[1] = chip->lfo1_cnt1_h[0];

        chip->lfo1_cnt1_load[1] = chip->lfo1_cnt1_load[0];
        chip->lfo1_cnt1_load[3] = chip->lfo1_cnt1_load[2];

        chip->lfo1_freq_write[0] = write1_en && chip->reg_write_18[1];
        chip->lfo1_freq_write[2] = chip->lfo1_freq_write[1];
        
        chip->lfo1_cnt1_rst = ic_async;

        if (ic_async) {
            chip->lfo1_cnt2[0] = 0;
        } else {
            chip->lfo1_cnt2[0] = (chip->lfo1_cnt2[1] + chip->lfo1_cnt2_inc) & 15;
        }

        chip->lfo1_cnt2_of[1] = chip->lfo1_cnt2_of[0];

        chip->lfo1_inc[1] = chip->lfo1_inc[0];

        if (chip->lfo_bcnt_rst) {
            chip->lfo1_bcnt[0] = 0;
        } else {
            int inc = (chip->lfo_sync[0] >> 2) & 1;
            chip->lfo1_bcnt[0] = (chip->lfo1_bcnt[1] + inc) & 15;
        }

        int bcnt = chip->lfo1_bcnt[1] & 7;
        int b0 = bcnt != 0 && (chip->lfo1_out_shifter[1] & 1) != 0;
        int sum = chip->lfo1_bit + b0 + chip->lfo1_sum_c_in;

        chip->lfo1_out_shifter[0] = chip->lfo1_out_shifter[1] >> 1;
        chip->lfo1_out_shifter[0] |= (sum & 1) << 15;
        chip->lfo1_sum_c_out = sum >> 1;

        int cnt3_rst = chip->lfo1_cnt3_sync[1] || ic_async || (chip->reg_test[1] & 2) != 0;
        int cnt3 = chip->lfo1_cnt3[1] + chip->lfo1_cnt3_inc[1];
        chip->lfo1_cnt3[0] = cnt3_rst ? 0 : cnt3 & 15;
        int of3 = (cnt3 >> 4) & 1;

        chip->lfo1_cnt3_inc[0] = cnt3_rst || (chip->lfo1_cnt3_inc[1] && !of3);

        int x = !chip->lfo1_wave3 && chip->lfo1_inc[0] && (chip->lfo_sync[0] & 8) != 0;
        int w3 = (!chip->lfo1_wave3 || !chip->lfo1_inc_lock) && (chip->lfo1_shifter[1] & 0x8000) != 0 && !chip->lfo1_cnt3_inc[1];
        int w2 = x && chip->lfo1_wave2;
        int t = (chip->lfo1_sum2_c_out[1] && !chip->lfo1_wave3 && (chip->lfo_sync[0] & 8) == 0) || x;

        sum = w2 + w3 + t;

        chip->lfo1_sum2_c_out[0] = sum >> 1;

        int bit = sum & 1;
        if (chip->lfo1_wave3 && chip->lfo1_inc_lock) {
            bit |= (chip->noise_lfsr[1] >> 15) & 1;
        }

        chip->lfo1_shifter[0] = (chip->lfo1_shifter[1] << 1) | bit;

        int bb = chip->lfo1_bb ^ w3;
        int sb = chip->lfo1_sel ? !chip->lfo_sync2[1] : !chip->lfo1_sign_saw_l;
        int mb = chip->fsm_o4[1] && (chip->lfo1_wave1 ? sb : bb);

        chip->lfo1_premul[0] = (chip->lfo1_premul[1] << 1) | mb;

    }
    if (hclk2) {
        chip->lfo1_cnt1[1] = chip->lfo1_cnt1[0];

        int cnt = chip->lfo1_cnt1_h[1] + chip->lfo1_cnt1_of_l;
        int of = cnt >> 7;
        chip->lfo1_cnt1_of_h = of;
        if (chip->lfo1_cnt1_load[3]) {
            chip->lfo1_cnt1_h[0] = chip->lfo1_cnt1_load_val_hi;
        } else {
            chip->lfo1_cnt1_h[0] = chip->lfo1_cnt1_rst ? 0 : (cnt & 127);
        }

        chip->lfo1_subcnt_of = chip->lfo_subcnt_of[2] || (chip->reg_test[0] & 8) != 0;

        chip->lfo1_cnt1_load[0] = chip->lfo1_freq_write[2] || of;
        chip->lfo1_cnt1_load[2] = chip->lfo1_cnt1_load[1];

        if (chip->lfo_sync2[0]) {
            chip->lfo1_cnt1_of_h_latch = chip->lfo1_cnt1_of_h_lock;
        }
        int cnt2_inc = (chip->lfo_sync[1] & 2) != 0 && chip->lfo1_cnt1_of_h_latch;
        chip->lfo1_cnt2_inc = cnt2_inc;
        chip->lfo1_cnt2[1] = chip->lfo1_cnt2[0];

        chip->lfo1_cnt2_of[0] = 0;
        if (cnt2_inc)
        {
            int lfrq_l = chip->reg_lfo_freq[0] & 15;
            int cnt = chip->lfo1_cnt2[0];
            if (lfrq_l & 1)
                chip->lfo1_cnt2_of[0] |= (cnt & 15) == 7;
            if (lfrq_l & 2)
                chip->lfo1_cnt2_of[0] |= (cnt & 7) == 3;
            if (lfrq_l & 4)
                chip->lfo1_cnt2_of[0] |= (cnt & 3) == 1;
            if (lfrq_l & 8)
                chip->lfo1_cnt2_of[0] |= (cnt & 1) == 0;
        }

        chip->lfo1_freq_write[1] = chip->lfo1_freq_write[0];

        chip->lfo1_inc[0] = chip->lfo_test || of || chip->lfo1_cnt2_of[1];

        chip->lfo1_bcnt[1] = chip->lfo1_bcnt[0];

        chip->lfo_sum_c_in = chip->lfo1_sum_c_out && (chip->lfo_sync[1] & 4) == 0;

        chip->lfo1_out_shifter[1] = chip->lfo1_out_shifter[0];
        chip->lfo1_shifter[1] = chip->lfo1_shifter[0];

        chip->lfo1_cnt3_inc[1] = chip->lfo1_cnt3_inc[0];
        chip->lfo1_cnt3[1] = chip->lfo1_cnt3[0];

        chip->lfo1_wave1 = chip->reg_lfo_wave[0] == 1;
        chip->lfo1_wave2 = chip->reg_lfo_wave[0] == 2;
        chip->lfo1_wave3 = chip->reg_lfo_wave[0] == 3;

        int lfo_sel = (chip->lfo1_bcnt[0] >> 3) & 1;
        chip->lfo1_sel = lfo_sel;

        chip->lfo1_sum2_c_out[1] = chip->lfo1_sum2_c_out[0];

        chip->lfo1_premul[1] = chip->lfo1_premul[0];

        int depth = lfo_sel ? chip->reg_lfo_pmd[0] : chip->reg_lfo_amd[0];
        int lfo_bit = 0;
        int bcnt = chip->lfo1_bcnt[0] & 7;
        switch (bcnt) {
            case 0:
                lfo_bit |= (depth & 64) != 0 && (chip->lfo1_premul[0] & 64) != 0;
                break;
            case 1:
                lfo_bit |= (depth & 32) != 0 && (chip->lfo1_premul[0] & 32) != 0;
                break;
            case 2:
                lfo_bit |= (depth & 16) != 0 && (chip->lfo1_premul[0] & 16) != 0;
                break;
            case 3:
                lfo_bit |= (depth & 8) != 0 && (chip->lfo1_premul[0] & 8) != 0;
                break;
            case 4:
                lfo_bit |= (depth & 4) != 0 && (chip->lfo1_premul[0] & 4) != 0;
                break;
            case 5:
                lfo_bit |= (depth & 2) != 0 && (chip->lfo1_premul[0] & 2) != 0;
                break;
            case 6:
                lfo_bit |= (depth & 1) != 0 && (chip->lfo1_premul[0] & 1) != 0;
                break;
        }
        chip->lfo1_bit = lfo_bit;

        if ((chip->lfo_sync[1] & 4) && (chip->lfo_sync[0] & 8))
        {
            chip->lfo1_inc_lock = chip->lfo1_inc[0];
            if ((chip->lfo1_bcnt[0] & 7) == 0) {
                chip->lfo1_sign_saw = (chip->lfo1_shifter[1] >> 8) & 1;
                chip->lfo1_sign_trig = (chip->lfo1_shifter[1] >> 7) & 1;
            }
            chip->lfo1_cnt1_of_h_lock = chip->lfo1_cnt1_of_h;
        }

        chip->lfo1_sign_saw_l = chip->lfo1_sign_saw;

        chip->lfo1_bb = lfo_sel ? chip->lfo1_sign_saw : (!chip->lfo1_sign_trig || chip->reg_lfo_wave[0] != 2);

    }
}

