#include <string.h>
#include <stdlib.h>
#include "opz_lle.h"



void OPZLLE_Clock(ym2141_t* chip, int clk) {
    chip->input.clk = clk;

    int clk1 = !clk;
    int clk2 = clk;

    chip->ic_async = !chip->input.n_ic;

    if (clk2) {
        chip->ic_latch[0] = chip->ic_async;
        chip->clk_toggle[0] = !chip->clk_toggle[1] || (!chip->ic_latch[1] && chip->ic_async);
    }
    if (clk1) {
        chip->ic_latch[1] = chip->ic_latch[0];
        chip->clk_toggle[1] = chip->clk_toggle[0];
    }

    chip->o_sy = chip->clk_toggle[1];


    int hclk1 = chip->clk_toggle[1];
    int hclk2 = !(clk1 || chip->clk_toggle[1]);

    if (hclk2) {
        chip->ic_sync = chip->ic_async;
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

        if (chip->ic_async) {
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
                chip->reg_noise_en[0] = (chip->data_l >> 7) & 1;
                chip->reg_noise_freq[0] = chip->data_l & 15;
            } else {
                chip->reg_noise_en[0] = chip->reg_noise_en[1];
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

}

