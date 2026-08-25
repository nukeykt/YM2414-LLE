#include <string.h>
#include <stdlib.h>
#include "opz_lle.h"


enum {
    eg_state_attack = 0,
    eg_state_decay,
    eg_state_sustain,
    eg_state_release
};

typedef struct {
    int basefreq;
    int approxtype;
    int slope;
} freqtable_t;

static const freqtable_t pg_freqtable[64] = {
    { 1299, 1,  3 }, { 1318, 1,  3 }, { 1337, 1,  3 }, { 1356, 1,  4 },
    { 1376, 1,  4 }, { 1396, 1,  4 }, { 1416, 1,  5 }, { 1437, 1,  4 },
    { 1458, 1,  5 }, { 1479, 1,  5 }, { 1501, 1,  6 }, { 1523, 1,  6 },
    { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 },
    { 1545, 1,  6 }, { 1567, 1,  6 }, { 1590, 1,  7 }, { 1613, 1,  7 },
    { 1637, 1,  7 }, { 1660, 1,  8 }, { 1685, 1,  8 }, { 1709, 1,  8 },
    { 1734, 1,  9 }, { 1759, 1,  9 }, { 1785, 1, 10 }, { 1811, 1, 10 },
    { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 },
    { 1837, 1, 10 }, { 1864, 1, 11 }, { 1891, 1, 11 }, { 1918, 1, 12 },
    { 1946, 1, 12 }, { 1975, 1, 12 }, { 2003, 1, 13 }, { 2032, 1, 14 },
    { 2062, 1, 14 }, { 2092, 1, 14 }, { 2122, 1, 15 }, { 2153, 1, 15 },
    { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 },
    { 2185, 1, 15 }, { 2216, 0, 15 }, { 2249, 0, 15 }, { 2281, 0, 15 },
    { 2315, 0, 15 }, { 2348, 0, 15 }, { 2382, 0, 14 }, { 2417, 0, 14 },
    { 2452, 0, 14 }, { 2488, 0, 14 }, { 2524, 0, 14 }, { 2561, 0, 14 },
    { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 }, { 0,    0,  0 }
};

static const int pow_base[256] = {
    2037, 2026, 2015, 2004, 1993, 1982, 1971, 1960, 1949, 1939, 1928, 1917, 1906, 1896, 1885, 1874,
    1864, 1853, 1843, 1832, 1822, 1811, 1801, 1790, 1780, 1770, 1759, 1749, 1739, 1728, 1718, 1708,
    1698, 1688, 1678, 1668, 1658, 1648, 1638, 1628, 1618, 1608, 1598, 1588, 1578, 1568, 1559, 1549,
    1539, 1529, 1520, 1510, 1500, 1491, 1481, 1472, 1462, 1453, 1443, 1434, 1424, 1415, 1406, 1396,
    1387, 1378, 1368, 1359, 1350, 1341, 1332, 1323, 1313, 1304, 1295, 1286, 1277, 1268, 1259, 1250,
    1241, 1232, 1224, 1215, 1206, 1197, 1188, 1180, 1171, 1162, 1153, 1145, 1136, 1128, 1119, 1110,
    1102, 1093, 1085, 1076, 1068, 1060, 1051, 1043, 1034, 1026, 1018, 1009, 1001, 993, 985, 977,
    968, 960, 952, 944, 936, 928, 920, 912, 904, 896, 888, 880, 872, 864, 856, 848,
    840, 833, 825, 817, 809, 802, 794, 786, 779, 771, 763, 756, 748, 741, 733, 726,
    718, 711, 703, 696, 688, 681, 673, 666, 659, 651, 644, 637, 630, 622, 615, 608,
    601, 594, 586, 579, 572, 565, 558, 551, 544, 537, 530, 523, 516, 509, 502, 495,
    488, 482, 475, 468, 461, 454, 448, 441, 434, 427, 421, 414, 407, 401, 394, 387,
    381, 374, 368, 361, 355, 348, 342, 335, 329, 322, 316, 310, 303, 297, 291, 284,
    278, 272, 265, 259, 253, 247, 240, 234, 228, 222, 216, 210, 204, 197, 191, 185,
    179, 173, 167, 161, 155, 149, 143, 138, 132, 126, 120, 114, 108, 102, 96, 91,
    85, 79, 73, 68, 62, 56, 51, 45, 39, 34, 28, 22, 17, 11, 6, 0
};

static const int pow_delta[3][256] = {
    {
    8, 8, 8, 8, 8, 8, 8, 8, 9, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 7, 8, 8, 8, 8, 7, 8, 8, 7, 8, 8, 8,
    8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 7, 7,
    7, 8, 7, 7, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 7, 6, 7, 6, 6, 7,
    6, 7, 6, 7, 6, 6, 6, 6, 7, 6, 6, 7, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 5, 6, 6, 5, 6, 6, 5, 6, 5, 6, 5,
    6, 5, 6, 5, 6, 5, 6, 6, 5, 6, 6, 5, 5, 6, 6, 5,
    5, 5, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 4, 5,
    5, 4, 5, 5, 5, 4, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 5, 5, 4,
    4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4
    },{
    5, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 6, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 4, 5,
    5, 4, 5, 5, 5, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 5, 5, 4, 4, 5, 5, 4, 5, 4, 4, 5,
    4, 5, 4, 5, 4, 4, 4, 4, 5, 4, 4, 5, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 3, 4, 4, 3, 4, 4, 4, 4, 3, 4, 3,
    4, 3, 4, 3, 4, 4, 4, 4, 3, 4, 4, 3, 3, 4, 4, 4,
    3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4,
    4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 3, 3, 4, 3, 3, 4,
    3, 4, 3, 4, 3, 3, 3, 4, 3, 4, 3, 3, 3, 3, 3, 3,
    3, 3, 4, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 2, 3, 3, 2, 3, 3, 3, 3, 2, 3
    },{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 2, 3, 3,
    2, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2,
    3, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 2, 3,
    2, 2, 3, 3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, 3,
    3, 3, 2, 2, 2, 2, 3, 2, 2, 2, 3, 2, 2, 2, 2, 3,
    2, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1,
    2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2,
    2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2,
    2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 1, 2,
    2, 1, 2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1,
    1, 2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1
    }
};

static const int sin_base[256] = {
    7722, 6596, 5965, 5524, 5185, 4909, 4677, 4477, 4300, 4143, 4000, 3871, 3751, 3641, 3539, 3443,
    3353, 3268, 3188, 3112, 3040, 2971, 2906, 2843, 2783, 2725, 2669, 2616, 2564, 2515, 2467, 2420,
    2375, 2331, 2289, 2248, 2208, 2169, 2131, 2095, 2059, 2024, 1990, 1957, 1924, 1892, 1861, 1831,
    1801, 1772, 1744, 1716, 1689, 1662, 1636, 1610, 1585, 1561, 1536, 1513, 1489, 1466, 1444, 1422,
    1400, 1379, 1358, 1337, 1317, 1297, 1277, 1258, 1239, 1220, 1201, 1183, 1165, 1148, 1130, 1113,
    1096, 1080, 1063, 1047, 1031, 1016, 1000, 985, 970, 955, 940, 926, 912, 898, 884, 870,
    857, 843, 830, 817, 804, 792, 779, 767, 755, 743, 731, 719, 708, 696, 685, 674,
    663, 652, 641, 631, 620, 610, 600, 589, 579, 570, 560, 550, 541, 532, 522, 513,
    504, 495, 486, 478, 469, 461, 452, 444, 436, 428, 420, 412, 404, 396, 389, 381,
    374, 367, 359, 352, 345, 338, 331, 325, 318, 311, 305, 298, 292, 286, 280, 273,
    267, 261, 256, 250, 244, 238, 233, 227, 222, 217, 211, 206, 201, 196, 191, 186,
    181, 177, 172, 167, 163, 158, 154, 150, 145, 141, 137, 133, 129, 125, 121, 117,
    114, 110, 106, 103, 99, 96, 93, 89, 86, 83, 80, 77, 74, 71, 68, 65,
    63, 60, 57, 55, 52, 50, 48, 45, 43, 41, 39, 37, 35, 33, 31, 29,
    27, 25, 24, 22, 21, 19, 18, 16, 15, 14, 12, 11, 10, 9, 8, 7,
    6, 6, 5, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0
};

static const int sin_delta[3][256] = {
    {
    2875, 755, 446, 318, 247, 202, 171, 147, 131, 116, 106, 96, 89, 82, 76, 71,
    67, 63, 60, 57, 54, 52, 49, 47, 45, 43, 42, 40, 39, 37, 35, 35,
    34, 33, 32, 31, 30, 29, 29, 27, 27, 26, 25, 24, 24, 24, 24, 23,
    23, 22, 21, 21, 20, 20, 20, 20, 19, 18, 19, 17, 18, 18, 17, 16,
    16, 16, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 13, 13, 13,
    13, 12, 13, 12, 12, 11, 12, 11, 11, 11, 11, 11, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 8, 9, 8, 8,
    8, 8, 8, 7, 8, 7, 7, 8, 8, 7, 7, 8, 7, 7, 7, 7,
    7, 7, 7, 6, 7, 6, 7, 6, 6, 6, 6, 6, 6, 6, 5, 6,
    5, 5, 6, 6, 5, 5, 6, 5, 5, 5, 5, 5, 5, 4, 4, 5,
    5, 5, 4, 4, 4, 5, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 3, 3, 4, 3, 4, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3,
    2, 3, 3, 3, 3, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0
    },{
    1252, 458, 282, 204, 160, 132, 112, 97, 86, 76, 70, 63, 59, 54, 50, 47,
    44, 42, 40, 38, 36, 34, 32, 31, 29, 29, 28, 26, 26, 24, 23, 23,
    22, 22, 21, 20, 20, 19, 19, 18, 18, 17, 17, 16, 16, 16, 16, 15,
    15, 15, 14, 14, 14, 14, 13, 13, 13, 12, 13, 12, 12, 12, 11, 11,
    11, 10, 10, 10, 10, 10, 10, 9, 9, 9, 10, 9, 9, 8, 9, 9,
    9, 8, 8, 8, 8, 7, 8, 7, 7, 7, 8, 7, 7, 7, 7, 7,
    6, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 6, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5,
    5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 3, 4, 4, 4, 4, 4, 3, 3, 4, 3, 4, 3, 3, 3, 3,
    3, 3, 2, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 3, 2, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 1, 2, 2,
    1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0
    },{
    497, 212, 134, 98, 78, 65, 55, 47, 42, 38, 35, 31, 29, 27, 25, 23,
    22, 21, 20, 19, 18, 17, 16, 15, 15, 14, 14, 13, 13, 12, 11, 12,
    11, 11, 11, 10, 10, 10, 10, 9, 9, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5, 4, 5, 4,
    4, 4, 4, 4, 4, 3, 4, 4, 3, 4, 4, 3, 3, 3, 3, 3,
    3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 2,
    2, 3, 3, 2, 3, 2, 2, 3, 3, 2, 2, 3, 2, 2, 3, 2,
    2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 1, 1, 2,
    2, 2, 1, 1, 1, 2, 1, 2, 1, 1, 2, 2, 1, 1, 1, 1,
    2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0
    }
};

static void LFO_Clock(ym2414_t* chip, int hclk1, int hclk2, int ic_async, ym2414_lfo_t* lfo, int which) {
    int* whichfreq = which ? chip->reg_lfo_freq : chip->reg_16;
    if (hclk1) {
        int lfrq_h = whichfreq[1] >> 4;

        int load_val = 0x8000 - (1 << (15 - lfrq_h));

        int cnt = lfo->cnt1[1] + lfo->subcnt_of;
        int of = cnt >> 8;
        lfo->cnt1_of_l = of;

        lfo->cnt1_load_val_hi = load_val >> 8;

        if (lfo->cnt1_load[2]) {
            lfo->cnt1[0] = load_val & 255;
        } else {
            lfo->cnt1[0] = ic_async ? 0 : cnt & 255;
        }

        lfo->cnt1_h[1] = lfo->cnt1_h[0];

        lfo->cnt1_load[1] = lfo->cnt1_load[0];
        lfo->cnt1_load[3] = lfo->cnt1_load[2];

        lfo->freq_write[2] = lfo->freq_write[1];
        
        lfo->cnt1_rst = ic_async;

        if (ic_async) {
            lfo->cnt2[0] = 0;
        } else {
            lfo->cnt2[0] = (lfo->cnt2[1] + lfo->cnt2_inc) & 15;
        }

        lfo->cnt2_of[1] = lfo->cnt2_of[0];

        lfo->inc[1] = lfo->inc[0];

        if (chip->lfo_bcnt_rst) {
            lfo->bcnt[0] = 0;
        } else {
            int inc = (chip->lfo_sync[0] >> 2) & 1;
            lfo->bcnt[0] = (lfo->bcnt[1] + inc) & 15;
        }

        int bcnt = lfo->bcnt[1] & 7;
        int b0 = bcnt != 0 && (lfo->out_shifter[1] & 1) != 0;
        int sum = lfo->bit + b0 + lfo->sum_c_in;

        lfo->out_shifter[0] = lfo->out_shifter[1] >> 1;
        lfo->out_shifter[0] |= (sum & 1) << 15;
        lfo->sum_c_out = sum >> 1;

        int cnt3_rst = lfo->cnt3_sync[1] || ic_async || (chip->reg_test[1] & 2) != 0;
        int cnt3 = lfo->cnt3[1] + lfo->cnt3_inc[1];
        lfo->cnt3[0] = cnt3_rst ? 0 : cnt3 & 15;
        int of3 = (cnt3 >> 4) & 1;

        lfo->cnt3_inc[0] = cnt3_rst || (lfo->cnt3_inc[1] && !of3);

        int x = !lfo->wave3 && lfo->inc[0] && (chip->lfo_sync[0] & 8) != 0;
        int w3 = (!lfo->wave3 || !lfo->inc_lock) && (lfo->shifter[1] & 0x8000) != 0 && !lfo->cnt3_inc[1];
        int w2 = x && lfo->wave2;
        int t = (lfo->sum2_c_out[1] && !lfo->wave3 && (chip->lfo_sync[0] & 8) == 0) || x;

        sum = w2 + w3 + t;

        lfo->sum2_c_out[0] = sum >> 1;

        int bit = sum & 1;
        if (lfo->wave3 && lfo->inc_lock) {
            bit |= (chip->noise_lfsr[1] >> 15) & 1;
        }

        lfo->shifter[0] = (lfo->shifter[1] << 1) | bit;

        int bb = lfo->bb ^ w3;
        int sb = lfo->sel ? !chip->lfo_sync2[1] : !lfo->sign_saw_l;
        int mb = chip->fsm_o4[1] && (lfo->wave1 ? sb : bb);

        lfo->premul[0] = (lfo->premul[1] << 1) | mb;
    }
    if (hclk2) {
        lfo->cnt1[1] = lfo->cnt1[0];

        int cnt = lfo->cnt1_h[1] + lfo->cnt1_of_l;
        int of = cnt >> 7;
        lfo->cnt1_of_h = of;
        if (lfo->cnt1_load[3]) {
            lfo->cnt1_h[0] = lfo->cnt1_load_val_hi;
        } else {
            lfo->cnt1_h[0] = lfo->cnt1_rst ? 0 : (cnt & 127);
        }

        lfo->subcnt_of = chip->lfo_subcnt_of[2] || (chip->reg_test[0] & 8) != 0;

        lfo->cnt1_load[0] = lfo->freq_write[2] || of;
        lfo->cnt1_load[2] = lfo->cnt1_load[1];

        if (chip->lfo_sync2[0]) {
            lfo->cnt1_of_h_latch = lfo->cnt1_of_h_lock;
        }
        int cnt2_inc = (chip->lfo_sync[1] & 2) != 0 && lfo->cnt1_of_h_latch;
        lfo->cnt2_inc = cnt2_inc;
        lfo->cnt2[1] = lfo->cnt2[0];

        lfo->cnt2_of[0] = 0;
        if (cnt2_inc)
        {
            int lfrq_l = whichfreq[0] & 15;
            int cnt = lfo->cnt2[0];
            if (lfrq_l & 1)
                lfo->cnt2_of[0] |= (cnt & 15) == 7;
            if (lfrq_l & 2)
                lfo->cnt2_of[0] |= (cnt & 7) == 3;
            if (lfrq_l & 4)
                lfo->cnt2_of[0] |= (cnt & 3) == 1;
            if (lfrq_l & 8)
                lfo->cnt2_of[0] |= (cnt & 1) == 0;
        }

        lfo->freq_write[1] = lfo->freq_write[0];

        lfo->inc[0] = chip->lfo_test || of || lfo->cnt2_of[1];

        lfo->bcnt[1] = lfo->bcnt[0];

        lfo->sum_c_in = lfo->sum_c_out && (chip->lfo_sync[1] & 4) == 0;

        lfo->out_shifter[1] = lfo->out_shifter[0];
        lfo->shifter[1] = lfo->shifter[0];

        lfo->cnt3_inc[1] = lfo->cnt3_inc[0];
        lfo->cnt3[1] = lfo->cnt3[0];

        int whichwave = which ? chip->reg_lfo2_wave[0] : chip->reg_lfo_wave[0];

        lfo->wave1 = whichwave == 1;
        lfo->wave2 = whichwave == 2;
        lfo->wave3 = whichwave == 3;

        int lfo_sel = (lfo->bcnt[0] >> 3) & 1;
        lfo->sel = lfo_sel;

        lfo->sum2_c_out[1] = lfo->sum2_c_out[0];

        lfo->premul[1] = lfo->premul[0];

        int whichamd = which ? chip->reg_17_amd[0] : chip->reg_lfo_amd[0];
        int whichpmd = which ? chip->reg_17_pmd[0] : chip->reg_lfo_pmd[0];
        int depth = lfo_sel ? whichpmd : whichamd;
        int lfo_bit = 0;
        int bcnt = lfo->bcnt[0] & 7;
        switch (bcnt) {
            case 0:
                lfo_bit |= (depth & 64) != 0 && (lfo->premul[0] & 64) != 0;
                break;
            case 1:
                lfo_bit |= (depth & 32) != 0 && (lfo->premul[0] & 32) != 0;
                break;
            case 2:
                lfo_bit |= (depth & 16) != 0 && (lfo->premul[0] & 16) != 0;
                break;
            case 3:
                lfo_bit |= (depth & 8) != 0 && (lfo->premul[0] & 8) != 0;
                break;
            case 4:
                lfo_bit |= (depth & 4) != 0 && (lfo->premul[0] & 4) != 0;
                break;
            case 5:
                lfo_bit |= (depth & 2) != 0 && (lfo->premul[0] & 2) != 0;
                break;
            case 6:
                lfo_bit |= (depth & 1) != 0 && (lfo->premul[0] & 1) != 0;
                break;
        }
        lfo->bit = lfo_bit;

        lfo->pm_sign = (lfo->out_shifter[1] >> 15) & 1;
        int s = whichwave == 2 ? lfo->sign_trig : lfo->sign_saw;
        lfo->pm_sign ^= s;

        if ((chip->lfo_sync[1] & 4) && (chip->lfo_sync[0] & 8))
        {
            lfo->inc_lock = lfo->inc[0];
            if ((lfo->bcnt[0] & 7) == 0) {
                lfo->sign_saw = (lfo->shifter[1] >> 8) & 1;
                lfo->sign_trig = (lfo->shifter[1] >> 7) & 1;
            }
            lfo->cnt1_of_h_lock = lfo->cnt1_of_h;

            int pm_load = lfo_sel && (lfo->bcnt[0] & 7) == 7;
            if (pm_load) {
                lfo->pm = (lfo->out_shifter[1] >> 8) & 127;
                lfo->pm |= lfo->pm_sign << 8;
            }

            int am_load = !lfo_sel && (lfo->bcnt[0] & 7) == 7;
            if (am_load) {
                lfo->am = (lfo->out_shifter[1] >> 4) & 4095;
            }
        }

        lfo->sign_saw_l = lfo->sign_saw;

        lfo->bb = lfo_sel ? lfo->sign_saw : (!lfo->sign_trig || whichwave != 2);

        lfo->pmd_zero = whichpmd == 0;
        lfo->pm_out = lfo->pmd_zero ? 0 : lfo->pm;
    }

}

void OPZLLE_Clock(ym2414_t* chip, int clk) {
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

    int data_z = !(!chip->ic_sync && chip->input.a0 && !chip->input.n_cs && !chip->input.n_rd);
    chip->o_data_z = data_z;

    int wr0 = chip->ic_sync || (!chip->input.n_cs && !chip->input.n_wr && !chip->input.a0);
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
                chip->reg_test[0] = chip->data_l;
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
            if (write1_en && chip->reg_write_10[1]) {
                chip->reg_timer_a[0] &= ~0x3fc;
                chip->reg_timer_a[0] |= chip->data_l << 2;
            }
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
                chip->reg_ct[0] = (chip->data_l >> 6) & 3;
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
        chip->fsm_19[0] = cnt == 18;
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
            chip->fsm_op_cnt[0] = (chip->fsm_op_cnt[1] + ((chip->fsm_op_sync[1] >> 1) & 1)) & 3;
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
        if ((chip->reg_match20_l[1] & 16) != 0 || chip->ic_sync) {
            chip->reg_ch20_l[0] = chip->reg_data[1];
        } else {
            chip->reg_ch20_l[0] = chip->reg_ch20_l2[1];
        }
        chip->reg_ch20_l2[0] = (chip->reg_ch_bus >> 16) & 255;

        int match28 = chip->ic_sync || (chip->reg_match28 && (!chip->reg_unkmode || unkaddr || !chip->reg_unksel2));
        if (match28) {
            chip->reg_ch28_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_ch28_l[0] = chip->reg_ch28_l2[1];
        }
        chip->reg_ch28_l2[0] = (chip->reg_ch_bus >> 31) & 127;

        if (chip->reg_match30 || chip->ic_sync) {
            chip->reg_ch30_l[0] = (chip->reg_data[1] >> 2) & 63;
        } else {
            chip->reg_ch30_l[0] = (chip->reg_ch_bus >> 38) & 63;
        }

        int newm = chip->reg_15[0] & 1;

        if ((chip->reg_match30 && newm) || chip->ic_sync) {
            chip->reg_ch30new_l[0] = chip->reg_data[1] & 3;
        } else {
            chip->reg_ch30new_l[0] = (chip->reg_ch_bus >> 44) & 3;
        }

        if (chip->reg_match38 || chip->ic_sync) {
            chip->reg_ch38_l[0] = chip->reg_data[1] & 3; // ams
            chip->reg_ch38_l[0] |= (chip->reg_data[1] >> 2) & 28; // pms
        } else {
            chip->reg_ch38_l[0] = (chip->reg_ch_bus >> 46) & 31;
        }

        if ((chip->reg_match38 && newm) || chip->ic_sync) {
            chip->reg_ch38new_l[0] = (chip->reg_data[1] >> 2) & 3;
            chip->reg_ch38new_l[0] |= (chip->reg_data[1] >> 5) & 4;
        } else {
            chip->reg_ch38new_l[0] = (chip->reg_ch_bus >> 51) & 7;
        }

        int bank = newm && (chip->reg_data[1] & 128) != 0;
        if ((chip->reg_match40 && !bank) || chip->ic_sync) {
            chip->reg_op40_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_op40_l[0] = chip->reg_op1_bus_l[1] & 127;
        }

        if ((chip->reg_match40 && bank) || chip->ic_sync) {
            chip->reg_op40new_l[0] = chip->reg_data[1] & 127;
        } else {
            chip->reg_op40new_l[0] = (chip->reg_op1_bus_l[1] >> 7) & 127;
        }

        if (chip->reg_match60 || chip->ic_sync) {
            chip->reg_op60_l[0] = chip->reg_data[1];
        } else {
            chip->reg_op60_l[0] = (chip->reg_op1_bus_l[1] >> 14) & 255;
        }

        chip->reg_op80_l[0] = 0;
        if (chip->reg_match80 || chip->ic_sync) {
            chip->reg_op80_l[0] |= chip->reg_data[1] & 0xdf;
        } else {
            chip->reg_op80_l[0] |= chip->reg_op2_bus_l[1] & 0xdf;
        }
        if ((chip->reg_match80 && newm) || chip->ic_sync) {
            chip->reg_op80_l[0] |= chip->reg_data[1] & 0x20;
        } else {
            chip->reg_op80_l[0] |= chip->reg_op2_bus_l[1] & 0x20;
        }

        chip->reg_opa0_l[0] = 0;
        if (chip->reg_matcha0 || chip->ic_sync) {
            chip->reg_opa0_l[0] |= chip->reg_data[1] & 0x9f;
        } else {
            chip->reg_opa0_l[0] |= (chip->reg_op2_bus_l[1] >> 8) & 0x9f;
        }
        if ((chip->reg_matcha0 && newm) || chip->ic_sync) {
            chip->reg_opa0_l[0] |= chip->reg_data[1] & 0x60;
        } else {
            chip->reg_opa0_l[0] |= (chip->reg_op2_bus_l[1] >> 8) & 0x60;
        }

        bank = newm && (chip->reg_data[1] & 32) != 0;
        if ((chip->reg_matchc0 && !bank) || chip->ic_sync) {
            chip->reg_opc0_l[0] = chip->reg_data[1] & 31;
            chip->reg_opc0_l[0] |= (chip->reg_data[1] >> 1) & 0x60;
        } else {
            chip->reg_opc0_l[0] = (chip->reg_op2_bus_l[1] >> 16) & 127;
        }

        if ((chip->reg_matchc0 && bank) || chip->ic_sync) {
            chip->reg_opc0new_l[0] = chip->reg_data[1] & 15;
            chip->reg_opc0new_l[0] |= (chip->reg_data[1] >> 2) & 0x30;
        } else {
            chip->reg_opc0new_l[0] = (chip->reg_op2_bus_l[1] >> 23) & 63;
        }

        if (chip->reg_matche0 || chip->ic_sync) {
            chip->reg_ope0_l[0] = chip->reg_data[1];
        } else {
            chip->reg_ope0_l[0] = (chip->reg_op2_bus_l[1] >> 29) & 0xff;
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
        reg_op1_in |= (uint64_t)chip->ramp_tl_cell_in << 22;

        uint64_t reg_op2_in = 0;
        reg_op2_in |= (uint64_t)chip->reg_op80_l[1];
        reg_op2_in |= (uint64_t)chip->reg_opa0_l[1] << 8;
        reg_op2_in |= (uint64_t)chip->reg_opc0_l[1] << 16;
        reg_op2_in |= (uint64_t)chip->reg_opc0new_l[1] << 23;
        reg_op2_in |= (uint64_t)chip->reg_ope0_l[1] << 29;

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

        chip->ramp_tl_cell_out = (chip->reg_op1_bus[1] >> 22) & 1023;

        chip->reg_30_0_l[0] = (chip->reg_30_0_l[1] << 1) | (chip->reg_ch30new_l[1] & 1);
        chip->reg_30_1_l[0] = (chip->reg_ch_bus >> 45) & 1;
    }
    if (hclk2) {
        chip->reg_data_valid[1] = chip->reg_data_valid[0];
        chip->reg_counter[1] = chip->reg_counter[0];
        int ch_match = (chip->reg_counter[0] & 7) == (chip->reg_address[0] & 7) && chip->reg_data_valid[0];
        chip->reg_match00 = ch_match && (chip->reg_address[0] & 0xf8) == 0;
        chip->reg_match20 = ch_match && (chip->reg_address[0] & 0xf8) == 0x20;
        chip->reg_match28 = ch_match && (chip->reg_address[0] & 0xf8) == 0x28;
        chip->reg_match30 = ch_match && (chip->reg_address[0] & 0xf8) == 0x30;
        chip->reg_match38 = ch_match && (chip->reg_address[0] & 0xf8) == 0x38;
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
        chip->reg_ope0_l[1] = chip->reg_ope0_l[0];

        chip->reg_op1_bus_l[1] = chip->reg_op1_bus_l[0];
        chip->reg_op2_bus_l[1] = chip->reg_op2_bus_l[0];

        chip->reg_unkmode = (chip->reg_15[0] & 3) == 3;
        chip->reg_unksel1 = (chip->reg_data[0] ^ (chip->reg_counter[0] >> 3)) & 1;
        chip->reg_unksel2 = ((chip->reg_data[0] >> 7) ^ (chip->reg_counter[0] >> 3)) & 1;

        chip->reg_match20_l[1] = chip->reg_match20_l[0];

        chip->reg_alg[1] = chip->reg_alg[0];

        chip->reg_30_0_l[1] = chip->reg_30_0_l[0];
        chip->reg_30_1_l[1] = chip->reg_30_1_l[0];
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
        chip->lfo1.cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data_l & 16) != 0);
        chip->lfo2.cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data_l & 32) != 0);
        chip->lfo1.freq_write[0] = write1_en && chip->reg_write_18[1];
        chip->lfo2.freq_write[0] = write1_en && chip->reg_write_16[1];

        chip->lfo_pmsel[0] = (chip->reg_ch38new_l[1] >> 2) & 1;
        chip->lfo_amsel[0] = (chip->reg_ch38new_l[1] >> 0) & 1;

        chip->lfo_amen[0] = chip->lfo_amen[1] << 1;
        if (chip->reg_op2_bus[1] & (1 << 15)) {
            chip->lfo_amen[0] |= 1;
        }

        chip->lfo_ams[0] = (chip->reg_ch_bus >> 46) & 3;

        chip->lfo_amse[0] = 0;
        if (chip->lfo_amen[1] & 2) {
            chip->lfo_amse[0] = chip->lfo_ams[1];
        }
    }
    if (hclk2) {
        chip->lfo_sync[0] = (chip->lfo_sync[1] << 1) | chip->fsm_o16;
        chip->lfo_sync2[1] = chip->lfo_sync2[0];
        chip->lfo_subcnt[1] = chip->lfo_subcnt[0];
        chip->lfo_subcnt_of[1] = chip->lfo_subcnt_of[0];
        chip->lfo_bcnt_rst = chip->fsm_o16 && chip->lfo_subcnt[0] == 2;

        chip->lfo2.cnt3_sync[1] = chip->lfo1.cnt3_sync[0];

        chip->lfo_pmsel[1] = chip->lfo_pmsel[0];
        chip->lfo_amsel[1] = chip->lfo_amsel[0];

        chip->lfo_amen[1] = chip->lfo_amen[0];
        chip->lfo_ams[1] = chip->lfo_ams[0];
        chip->lfo_amse[1] = chip->lfo_amse[0];
    }

    LFO_Clock(chip, hclk1, hclk2, ic_async, &chip->lfo1, 0);
    LFO_Clock(chip, hclk1, hclk2, ic_async, &chip->lfo2, 1);

    if (hclk1) {
        chip->freq_kc[0] = (chip->reg_ch_bus >> 24) & 127;
        chip->freq_kc[2] = chip->freq_kc[1];
        chip->freq_kf[0] = chip->reg_ch30_l[1];
        chip->freq_kf[2] = chip->freq_kf[1];
        chip->freq_kf[4] = chip->freq_kf[3];
        chip->freq_lfo_pms[0] = (chip->reg_ch38_l[1] >> 2) & 7;
        chip->freq_lfo_pms[2] = chip->freq_lfo_pms[1];

        chip->freq_km = 0;
        if ((chip->reg_a[1] & 4) == 0) {
            chip->freq_km = (chip->reg_30_1_l[1] << 1) | ((chip->reg_30_0_l[1] >> 7) & 1);
        }
        int kc = chip->freq_kc[3] + chip->freq_kc_add + chip->freq_kc_add_c;
        chip->freq_kc[4] = kc & 127;

        chip->freq_kc_clip_h[0] = (kc & 128) && !chip->freq_kc_add_sign;
        chip->freq_kc_clip_l[0] = !(kc & 128) && chip->freq_kc_add_sign;

        chip->freq_kc[6] = chip->freq_kc_clip_h[1] ? 0x7f : (chip->freq_kc_clip_l[1] ? 0 : chip->freq_kc[5]);


        chip->freq_lfo_pm = chip->lfo_pmsel[1] ? chip->lfo2.pm_out : chip->lfo1.pm_out;

        int lfo_add = 0;
        int pms = chip->freq_lfo_pms[3];
        if (pms)
        {
            if (pms < 6)
                lfo_add = ((chip->freq_lfo_add & 127) << pms) >> 6;
            else
                lfo_add = (chip->freq_lfo_add << pms) >> 5;
        }
        chip->freq_lfo_sign[1] = chip->freq_lfo_sign[0];
        chip->freq_lfo_sign[3] = chip->freq_lfo_sign[2];
        if (chip->freq_lfo_sign[0]) {
            lfo_add ^= 0x1fff;
        }
        chip->freq_lfo_add_shift = lfo_add;

        chip->freq_kc_lfo_suml[1] = chip->freq_kc_lfo_suml[0];

        int kc_lfo_h = (chip->freq_kc[7] & 3) + (chip->freq_lfo_add_shift_h & 3) + chip->freq_kc_lfo_sumlof;
        int of2 = (kc_lfo_h >> 2) & 1;
        kc_lfo_h += (chip->freq_kc[7] & 124) + (chip->freq_lfo_add_shift_h & 124);

        chip->freq_kc_lfo_sumh[0] = kc_lfo_h & 127;
        int of = (kc_lfo_h >> 7) & 1;
        chip->freq_kc_ch = of && !chip->freq_lfo_sign[2];
        chip->freq_kc_cl = !of && chip->freq_lfo_sign[2];

        chip->freq_kc_lfo_sumh_add = !chip->freq_lfo_sign[2] && (of2 || (kc_lfo_h & 3) == 3);
        chip->freq_kc_lfo_sumh_sub = !of2 && chip->freq_lfo_sign[2] && (chip->freq_lfo_add_shift_h & 3) != 0;

        chip->freq_kc_lfo_suml[3] = chip->freq_kc_lfo_suml[2];
        chip->freq_kc_lfo_sumh[2] = chip->freq_kc_lfo_sumh[1];
        if (chip->freq_kc_cl2) {
            chip->freq_kc_lfo_suml[3] = 0;
            chip->freq_kc_lfo_sumh[2] = 0;
        }
        if (chip->freq_kc_ch2) {
            chip->freq_kc_lfo_suml[3] = 63;
            chip->freq_kc_lfo_sumh[2] = 126;
        }
        chip->freq_kc_lfo_sumh[4] = chip->freq_kc_lfo_sumh[3];

        chip->freq_dt2_del[0][0] = (chip->freq_dt2_del[0][1] << 1) | ((chip->reg_op2_bus[0] >> 21) & 1);
        chip->freq_dt2_del[1][0] = (chip->freq_dt2_del[1][1] << 1) | ((chip->reg_op2_bus[0] >> 22) & 1);

        chip->freq_dt2_del2[1] = chip->freq_dt2_del2[0];

        chip->freq_kf_dt[1] = chip->freq_kf_dt[0];

        int dt2 = chip->freq_dt2_del2[0] == 2;
        int c6 = (chip->freq_kc_lfo_sumh[3] & 1) != 0;
        int c7 = (chip->freq_kc_lfo_sumh[3] & 2) != 0;
        int t = (dt2 && c6) || c7;
        chip->freq_dt_sumh_b1 = chip->freq_dt_sum_c ? t : 0;
        chip->freq_dt_sumh_cin = chip->freq_dt_sum_c ? !t : (c7 && dt2);

        int kcf = (chip->freq_kc_dt << 6) | chip->freq_kf_dt[2];
        if (chip->freq_kc_dt_of) {
            kcf = 8127;
        }

        chip->freq_rom_addr[0] = (kcf >> 4) & 63;
        chip->freq_rom_addr[2] = chip->freq_rom_addr[1];

        chip->freq_freq_frac[0] = kcf & 15;
        chip->freq_freq_frac[2] = chip->freq_freq_frac[1];
        chip->freq_freq_frac0 = chip->freq_freq_frac[3] & 1;


        chip->freq_rom_base[1] = chip->freq_rom_base[0];

        int lerp1 = 0;
        if (chip->freq_freq_frac[3] & 8) {
            lerp1 |= chip->freq_rom_slope;
            if (!chip->freq_rom_type) {
                lerp1 |= 1;
            }
            lerp1 |= 16;
        }
        int lerp2 = 0;
        if (chip->freq_freq_frac[3] & 4) {
            lerp2 |= 8 | (chip->freq_rom_slope >> 1);
        }
        int lerp3 = 0;
        if (chip->freq_rom_type) {
            if ((chip->freq_rom_slope & 4) != 0 && (chip->freq_freq_frac[3] & 2) != 0) {
                lerp3 |= 1;
            }
            if ((chip->freq_rom_slope & 8) != 0 && (chip->freq_freq_frac[3] & 2) != 0) {
                lerp3 |= 2;
            }
            if ((chip->freq_freq_frac[3] & 2) != 0) {
                lerp3 |= 4;
            }
        } else {
            if ((chip->freq_freq_frac[3] & 8) != 0) {
                lerp3 |= 1;
            }
            if ((chip->freq_freq_frac[3] & 1) != 0) {
                lerp3 |= 2;
            }
            if ((chip->freq_rom_slope & 1) != 0 && (chip->freq_freq_frac[3] & 12) == 12) {
                lerp3 |= 4;
            }
            if ((chip->freq_freq_frac[3] & 2) != 0) {
                lerp3 |= 8;
            }
        }
        int lerp4 = (chip->freq_rom_slope & 8) != 0 && (chip->freq_freq_frac[3] & 1) != 0;

        chip->freq_lerp = ((lerp1 + lerp2 + lerp3 + lerp4) >> 1) & 31;

        int fnumh = (chip->freq_rom_base_h + chip->freq_fnum_l_of) & 127;

        chip->freq_fnum = (fnumh << 5) | chip->freq_fnum_l;

        chip->freq_kcode[0] = (kcf >> 8) & 31;
        chip->freq_kcode[2] = chip->freq_kcode[1];
    }
    if (hclk2) {
        chip->freq_kc[1] = chip->freq_kc[0];
        chip->freq_kc[3] = chip->freq_kc[2];
        chip->freq_kc[5] = chip->freq_kc[4];
        chip->freq_kc[7] = chip->freq_kc[6];
        chip->freq_kf[1] = chip->freq_kf[0];
        chip->freq_kf[3] = chip->freq_kf[2];
        chip->freq_lfo_pms[1] = chip->freq_lfo_pms[0];

        int km0 = chip->freq_km & 1;
        int km1 = (chip->freq_km >> 1) & 1;
        int w1 = (!km0 && !km1) || (km0 && km1);
        int kc = chip->freq_kc[2];

        int w2 = !w1 || (km0 && (kc & 1));// !((w1 && !(kc & 1)) || (w1 && !km0));
        chip->freq_kc_add_sign = km1;
        chip->freq_kc_add_c = (kc & 2) != 0 && (km0 || km1); //!((!km0 && !km1) || !(kc & 2));
        chip->freq_kc_add = ((km0 && km1) << 1) | w2;
        if (km1) {
            chip->freq_kc_add |= 0x7c;
        }
        // 01 -> add 1 + kc1
        // 10 -> -3 + kc1
        // 11 -> -2 + kc0 + kc1
        
        // 0  1 -d -e 
        // 1  2 -e  0
        // 2  4  0  1
        // 4  5  1  2
        // 5  6  2  4
        // 6  8  4  5
        // 8  9  5  6
        // 9  a  6  8
        // a  c  8  9
        // c  d  9  a
        // d  e  a  c
        // e +0  c  d


        chip->freq_kc_clip_h[1] = chip->freq_kc_clip_h[0];
        chip->freq_kc_clip_l[1] = chip->freq_kc_clip_l[0];

        int lfo_pm_out = chip->freq_lfo_pm;

        int pms = chip->freq_lfo_pms[2];
        chip->freq_lfo_pms[3] = pms;
        chip->freq_lfo_sign[0] = pms != 0 && (lfo_pm_out & 128) != 0;
        chip->freq_lfo_sign[2] = chip->freq_lfo_sign[1];

        int ps7 = pms == 7;
        int hi = ps7 ? (lfo_pm_out >> 4) & 7 : (lfo_pm_out >> 5) & 3;
        int hi2 = (hi >> 2) & 1;
        int add = ((hi & 6) != 0 && pms == 7) || (pms >= 6 && (hi & 3) == 3);

        int pm_sum = hi + hi2 + add;
        int lfo_add2 = ps7 ? pm_sum & 15 : ((pm_sum & 7) << 1) | ((lfo_pm_out >> 4) & 1);
        chip->freq_lfo_add = (lfo_add2 << 4) | (lfo_pm_out & 15);

        chip->freq_lfo_add_shift_h = chip->freq_lfo_add_shift >> 6;

        int suml = chip->freq_kf[4] + (chip->freq_lfo_add_shift & 63) + chip->freq_lfo_sign[1];
        chip->freq_kc_lfo_sumlof = (suml >> 6) & 1;
        chip->freq_kc_lfo_suml[0] = suml & 63;
        chip->freq_kc_lfo_suml[2] = chip->freq_kc_lfo_suml[1];

        int sumh = chip->freq_kc_lfo_sumh[0] + chip->freq_kc_lfo_sumh_add;
        if (chip->freq_kc_lfo_sumh_sub) {
            sumh += 127;
        }
        chip->freq_kc_lfo_sumh[1] = sumh & 127;
        chip->freq_kc_lfo_sumh[3] = chip->freq_kc_lfo_sumh[2];
        int ofh = (sumh >> 7) & 1;

        chip->freq_kc_cl2 = (!ofh && chip->freq_kc_lfo_sumh_sub) || chip->freq_kc_cl;
        chip->freq_kc_ch2 = (ofh && !chip->freq_lfo_sign[3]) || chip->freq_kc_ch;

        chip->freq_dt2_del[0][1] = chip->freq_dt2_del[0][0];
        chip->freq_dt2_del[1][1] = chip->freq_dt2_del[1][0];

        int dt2 = (chip->freq_dt2_del[0][0] >> 13) & 1;
        dt2 |= (chip->freq_dt2_del[1][0] >> 12) & 2;


        chip->freq_dt2_del2[0] = dt2;

        int dt_suml = chip->freq_kc_lfo_suml[3];
        int dt_addl = 0;
        if (dt2 == 2) {
            dt_addl |= 52;
        }
        if (dt2 == 3) {
            dt_addl |= 32;
        }

        dt_suml += dt_addl;
        chip->freq_dt_sum_c = (dt_suml >> 6) & 1;

        chip->freq_kf_dt[0] = dt_suml & 63;
        chip->freq_kf_dt[2] = chip->freq_kf_dt[1];

        int dt_sumh = chip->freq_kc_lfo_sumh[4];
        int dt_addh = chip->freq_dt_sumh_b1 << 1;
        if (chip->freq_dt2_del2[1] == 2) {
            dt_addh |= 1;
        }
        if (chip->freq_dt2_del2[1] == 3) {
            dt_addh |= 4;
        }
        if (chip->freq_dt2_del2[1] != 0) {
            dt_addh |= 8;
        }

        dt_sumh += dt_addh + chip->freq_dt_sumh_cin;

        chip->freq_kc_dt = dt_sumh & 127;
        chip->freq_kc_dt_of = (dt_sumh >> 7) & 1;


        chip->freq_rom_addr[1] = chip->freq_rom_addr[0];

        const freqtable_t* frq = &pg_freqtable[chip->freq_rom_addr[2]];
        chip->freq_rom_base[0] = frq->basefreq;
        chip->freq_rom_type = frq->approxtype;
        chip->freq_rom_slope = frq->slope;

        chip->freq_freq_frac[1] = chip->freq_freq_frac[0];
        chip->freq_freq_frac[3] = chip->freq_freq_frac[2];

        chip->freq_rom_base_h = chip->freq_rom_base[1] >> 5;

        int freq_sum_l = (chip->freq_rom_base[1] & 31) + chip->freq_lerp + chip->freq_freq_frac0;

        chip->freq_fnum_l = freq_sum_l & 31;
        chip->freq_fnum_l_of = (freq_sum_l >> 5) & 1;

        chip->freq_kcode[1] = chip->freq_kcode[0];
        chip->freq_kcode[3] = chip->freq_kcode[2];
    }

    if (hclk1) {

        chip->pg_fix[0] = (chip->reg_op80_l[1] >> 5) & 1;
        chip->pg_fix[2] = chip->pg_fix[1];

        chip->pg_shift[1] = chip->pg_shift[0];

        chip->pg_dt1 = (chip->reg_op40_l[1] >> 4) & 7;

        chip->pg_block = chip->freq_kcode[3] >> 2;
        chip->dt_note[0] = chip->freq_kcode[3] & 3;
        chip->dt_note[2] = chip->dt_note[1];
        chip->dt_sign[1] = chip->dt_sign[0];

        chip->dt_sum = chip->dt_add1 + chip->dt_add2 + 1;

        chip->dt_enabled2 = chip->dt_enabled && !chip->pg_fix[1];

        chip->pg_freqdt[0] = (chip->pg_freq + chip->pg_dt_add) & 0x1ffff;
        chip->pg_freqdt[2] = chip->pg_freqdt[1];

        chip->pg_multi1[0] = chip->reg_op40_l[1] & 15;
        chip->pg_multi1[2] = chip->pg_multi1[1];
        chip->pg_multi1[4] = chip->pg_multi1[3];
        chip->pg_multi1[6] = chip->pg_multi1[5];

        chip->pg_multi2[0] = chip->reg_op40new_l[1] & 15;
        chip->pg_multi2[2] = chip->pg_multi2[1];
        chip->pg_multi2[4] = chip->pg_multi2[3];
        chip->pg_multi2[6] = chip->pg_multi2[5];

        chip->pg_add1[0] = chip->pg_multi1[7] ? chip->pg_freqdt[3] * chip->pg_multi1[7] :
            (chip->pg_freqdt[3] >> 1);
        chip->pg_add1[0] &= 0xfffff;
        chip->pg_add1[2] = chip->pg_add1[1];

        chip->pg_add2[0] = ((chip->pg_multi2[7] * chip->pg_freqdt[3]) & 0xfffff) >> 4;
        chip->pg_add2[2] = chip->pg_add2[1];

        int kon = (chip->eg_kon[0] & 8) != 0;
        chip->pg_reset[0] = (chip->pg_reset[1] << 1) | kon;

        chip->pg_add[0] = (chip->pg_reset[1] & 1) != 0 ? 0 : (chip->pg_add1[3] + chip->pg_add2[3]) & 0xfffff;

        int phase_sum = ((chip->reg_test[1] & 8) != 0 || (chip->pg_reset[1] & 2) != 0) ? 0 : chip->pg_phase_l[1];
        phase_sum = (phase_sum + chip->pg_add[1]) & 0xfffff;

        chip->pg_phase_l2[0] = phase_sum;
        chip->pg_phase_l2[2] = chip->pg_phase_l2[1];
        chip->pg_phase_l2[4] = chip->pg_phase_l2[3];

        chip->pg_cell_sel[0] = (chip->pg_cell_sel[1] << 1) | chip->fsm_o9[1];

        for (i = 0; i < 8; i++) {
            if (chip->pg_cell_sel[1] & (1 << i)) {
                chip->pg_bus[0] |= chip->pg_cells[0][i];
                chip->pg_bus[1] |= chip->pg_cells[1][i];
            }
            if (chip->pg_cell_sel[1] & (512 << i)) {
                chip->pg_bus[2] |= chip->pg_cells[2][i];
                chip->pg_bus[3] |= chip->pg_cells[3][i];
            }
        }
        for (i = 0; i < 8; i++) {
            if (chip->pg_cell_sel[1] & (2 << i)) {
                chip->pg_cells[0][i] = chip->pg_phase_l2[5] ^ 0xfffff;
                chip->pg_cells[1][i] = chip->pg_bus[3];
            }
            if (chip->pg_cell_sel[1] & (1024 << i)) {
                chip->pg_cells[2][i] = chip->pg_bus[0];
                chip->pg_cells[3][i] = chip->pg_bus[2];
            }
        }

        chip->pg_phase_l[0] = chip->pg_bus[1] ^ 0xfffff;

        chip->pg_dbg[0] = chip->pg_dbg[1] >> 1;
        if (chip->pg_dbgload) {
            chip->pg_dbg[0] |= chip->pg_phase_l[1] & 0xff;
        }

        chip->pg_out[0] = (chip->pg_bus[3] ^ 0xfffff) >> 8;
        chip->pg_out[2] = chip->pg_out[1];
        if (chip->reg_a3[1]) {
            chip->pg_out[2] |= 1023;
        }
    }
    if (hclk2) {
        chip->pg_fix[1] = chip->pg_fix[0];

        int fnum = chip->pg_fix[2] ? 1200 : chip->freq_fnum;
        chip->pg_shift[0] = chip->pg_fix[0] ? chip->pg_dt1 : chip->pg_block;

        chip->pg_freq = (fnum << chip->pg_shift[1]) >> 2;

        chip->dt_note[1] = chip->pg_block == 7 ? 0 : chip->dt_note[0];
        chip->dt_add1 = chip->pg_block;
        chip->dt_enabled = (chip->pg_dt1 & 3) != 0;
        if (chip->dt_enabled) {
            chip->dt_add1 |= 8;
        }
        chip->dt_add2 = 0;
        if ((chip->pg_dt1 & 3) == 3) {
            chip->dt_add2 |= 1;
        }
        if (chip->pg_dt1 & 2) {
            chip->dt_add2 |= 2;
        }

        chip->dt_sign[0] = (chip->pg_dt1 >> 2) & 1;

        int dt_l = ((chip->dt_sum & 1) << 2) | chip->dt_note[2];
        int dt_h = chip->dt_sum >> 1;

        static const int pg_detune[8] = { 16, 17, 19, 20, 22, 24, 27, 29 };

        int dt_freq = chip->dt_enabled2 ? pg_detune[dt_l] >> (9 - dt_h) : 0;

        if (chip->dt_sign[1]) {
            dt_freq = -dt_freq;
        }

        chip->pg_dt_add = dt_freq;

        chip->pg_freqdt[1] = chip->pg_freqdt[0];
        chip->pg_freqdt[3] = chip->pg_freqdt[2];

        chip->pg_multi1[1] = chip->pg_multi1[0];
        chip->pg_multi1[3] = chip->pg_multi1[2];
        chip->pg_multi1[5] = chip->pg_multi1[4];
        chip->pg_multi1[7] = chip->pg_multi1[6];

        chip->pg_multi2[1] = chip->pg_multi2[0];
        chip->pg_multi2[3] = chip->pg_multi2[2];
        chip->pg_multi2[5] = chip->pg_multi2[4];
        chip->pg_multi2[7] = chip->pg_multi2[6];

        chip->pg_add1[1] = chip->pg_add1[0];
        chip->pg_add1[3] = chip->pg_add1[2];

        chip->pg_add2[1] = chip->pg_add2[0];
        chip->pg_add2[3] = chip->pg_add2[2];

        chip->pg_reset[1] = chip->pg_reset[0];

        chip->pg_add[1] = chip->pg_add[0];

        chip->pg_phase_l[1] = chip->pg_phase_l[0];

        chip->pg_phase_l2[1] = chip->pg_phase_l2[0];
        chip->pg_phase_l2[3] = chip->pg_phase_l2[2];
        chip->pg_phase_l2[5] = chip->pg_phase_l2[4];

        chip->pg_cell_sel[1] = chip->pg_cell_sel[0];


        chip->pg_bus[0] = 0;
        chip->pg_bus[1] = 0;
        chip->pg_bus[2] = 0;
        chip->pg_bus[3] = 0;

        chip->pg_dbgload = chip->fsm_4;

        chip->pg_dbg[1] = chip->pg_dbg[0];

        chip->pg_out[1] = chip->pg_out[0];
    }

    if (hclk1) {
        int ramp = chip->reg_ch00_l[1];
        int match = ramp == chip->ramp_cnt[1][7];

        int step = chip->fsm_o10[1];
        int rst = ic_async || (step && match);

        chip->ramp_cnt[0][0] = rst ? 0 : (chip->ramp_cnt[1][7] + step);
        memcpy(&chip->ramp_cnt[0][1], &chip->ramp_cnt[1][0], 7 * sizeof(uint8_t));
        chip->ramp_step[0] = match;

        chip->ramp_tl_reg[0] = chip->reg_op60_l[1];
        chip->ramp_tl_reg[2] = chip->ramp_tl_reg[1];

        int of = (chip->ramp_tl_cmp >> 10) & 1;

        chip->ramp_tl_add1 = of && chip->ramp_step[1];
        chip->ramp_tl_add2 = !of && (chip->ramp_tl_cmp & 1023) != 0 && chip->ramp_step[1];

        chip->ramp_tl_out_l = chip->ramp_tl_out;
    }
    if (hclk2) {
        memcpy(&chip->ramp_cnt[1][0], &chip->ramp_cnt[0][0], 8 * sizeof(uint8_t));
        chip->ramp_step[1] = chip->ramp_step[0];

        chip->ramp_tl_reg[1] = chip->ramp_tl_reg[0];
        
        chip->ramp_tl_out = chip->ramp_tl_cell_out;

        chip->ramp_tl_cmp = (chip->ramp_tl_cell_out ^ 1023) + ((chip->ramp_tl_reg[0] & 127) << 3);

        int sum = chip->ramp_tl_out_l + chip->ramp_tl_add1;
        if (chip->ramp_tl_add2) {
            sum += 1023;
        }

        chip->ramp_tl_cell_in = (chip->ramp_tl_reg[2] & 128) != 0 ? (sum & 1023) : ((chip->ramp_tl_reg[2] & 127) << 3);
    }

    if (hclk1) {
        chip->eg_sync[0] = chip->fsm_30[1];

        if (ic_async || chip->eg_subcnt_reset) {
            chip->eg_subcnt[0] = 0;
        } else {
            chip->eg_subcnt[0] = chip->eg_subcnt[1] + chip->eg_sync[1];
        }
        chip->eg_clock[1] = chip->eg_clock[0];

        chip->eg_half[0] = chip->fsm_o1[1];

        int inc = chip->eg_clock[0] && chip->eg_half[1] && (chip->eg_timer_carry[1] || chip->eg_sync2[2]);
        int timer_bit = (chip->eg_timer[1] & 1) + inc;

        int next_bit = !ic_async && (timer_bit & 1) != 0;
        chip->eg_timer[0] = (chip->eg_timer[1] >> 1) | (next_bit << 15);
        int masked_bit = next_bit && chip->eg_masking[1];
        chip->eg_timer_masked[0] = (chip->eg_timer_masked[1] >> 1) | (masked_bit << 15);

        chip->eg_masking[0] = ic_async || chip->eg_sync2[0] || (chip->eg_masking[1] && !next_bit);

        chip->eg_timer_carry[0] = timer_bit >> 1;

        chip->eg_sync2[1] = chip->eg_sync2[0];


        chip->eg_ar[0] = chip->reg_op2_bus[1] & 0x1f;
        chip->eg_d1r[0] = (chip->reg_op2_bus[1] >> 8) & 0x1f;
        chip->eg_d2r[0] = (chip->reg_op2_bus[1] >> 16) & 0x1f;
        chip->eg_rr[0] = (chip->reg_op2_bus[1] >> 30) & 0xf;
        int revrate = (chip->reg_op2_bus[1] >> 23) & 7;
        chip->eg_rev[0] = revrate;
        chip->eg_zero_rev[0] = revrate == 0;



        chip->eg_cell_sel[0] = (chip->eg_cell_sel[1] << 1) | chip->fsm_o9[1];

        int key = (chip->reg_kon[3][1] >> 7) & 1;

        chip->eg_key_l[0] = (chip->eg_key_l[1] << 1) | key;

        int cell_in = 0;
        if ((chip->eg_key_l[1] & 4) == 0) {
            cell_in |= 1;
        }
        cell_in |= chip->eg_state_next << 1;
        cell_in |= chip->eg_level_next << 3;

        for (i = 0; i < 8; i++) {
            if (chip->eg_cell_sel[1] & (1 << i)) {
                chip->eg_bus[0] |= chip->eg_cells[0][i];
                chip->eg_bus[1] |= chip->eg_cells[1][i];
            }
            if (chip->eg_cell_sel[1] & (512 << i)) {
                chip->eg_bus[2] |= chip->eg_cells[2][i];
                chip->eg_bus[3] |= chip->eg_cells[3][i];
            }
        }
        for (i = 0; i < 8; i++) {
            if (chip->eg_cell_sel[1] & (2 << i)) {
                chip->eg_cells[0][i] = cell_in;
                chip->eg_cells[1][i] = chip->eg_bus[3];
            }
            if (chip->eg_cell_sel[1] & (1024 << i)) {
                chip->eg_cells[2][i] = chip->eg_bus[0];
                chip->eg_cells[3][i] = chip->eg_bus[2];
            }
        }

        chip->eg_cell_o[0] = chip->eg_bus[1];

        int okeyn = chip->eg_cell_o[1] & 1;
        int ratesel = (chip->eg_cell_o[1] >> 1) & 3;
        if (key && okeyn) {
            ratesel = 0;
        }

        int rrzero = chip->eg_rr[1] == 0;

        int newm = (chip->reg_15[1] & 1) != 0;
        int reverbsel = (chip->eg_rev_l2[1] & 32) == 0 && !chip->eg_zero_rev[1] && newm && !rrzero && ratesel != 0;

        int rrrate = newm && rrzero ? 11 : chip->eg_rr[1];

        int rate = 0;
        if (reverbsel) {
            rate = (chip->eg_rev[1] << 1) | 1;
        } else {
            switch (ratesel) {
                case eg_state_attack:
                    rate = chip->eg_ar[1];
                    break;
                case eg_state_decay:
                    rate = chip->eg_d1r[1];
                    break;
                case eg_state_sustain:
                    rate = chip->eg_d2r[1];
                    break;
                case eg_state_release:
                    rate = (rrrate << 1) | 1;
                    break;
            }
        }
        chip->eg_rate[0] = rate;

        chip->eg_sl[0] = chip->reg_ope0_l[1] >> 4;
        chip->eg_sl[2] = chip->eg_sl[1];

        chip->eg_ks[0] = (chip->reg_op2_bus[1] >> 6) & 3;
        chip->eg_ks[2] = chip->eg_ks[1];

        chip->eg_kcode = chip->freq_kcode[1];

        chip->eg_zerorate[1] = chip->eg_zerorate[0];

        chip->eg_rateks = chip->eg_rate[1] * 2 + chip->eg_rks;

        chip->eg_maxrate[1] = chip->eg_maxrate[0];

        int shiftsum = chip->eg_shift_add1 + chip->eg_shift_add2;

        int incl = (shiftsum == 10 && !chip->eg_zerorate2)
            || (shiftsum == 11 && (chip->eg_rate_lo & 2) != 0)
            || (shiftsum == 12 && (chip->eg_rate_lo & 1) != 0);

        chip->eg_incp1 = (chip->eg_rate10 && !chip->eg_stephi) || (!chip->eg_ratehi && !chip->eg_zerorate[2] && incl);
        chip->eg_incp2 = (chip->eg_rate10 && chip->eg_stephi) || (chip->eg_rate11 && !chip->eg_stephi);
        chip->eg_incp3 = (chip->eg_rate11 && chip->eg_stephi) || (chip->eg_rate12 && !chip->eg_stephi);
        chip->eg_incp4 = (chip->eg_rate12 && chip->eg_stephi) || (chip->eg_rate13 && !chip->eg_stephi);
        chip->eg_incp5 = (chip->eg_rate13 && chip->eg_stephi) || (chip->eg_rate14 && !chip->eg_stephi);
        chip->eg_incp6 = (chip->eg_rate14 && chip->eg_stephi) || chip->eg_rate15;

        chip->eg_state_l[0] = (chip->eg_cell_o[1] >> 1) & 3;
        chip->eg_state_l[2] = chip->eg_state_l[1];
        chip->eg_state_l[4] = chip->eg_state_l[3];

        chip->eg_okeyn_l[0] = (chip->eg_okeyn_l[1] << 1) | (chip->eg_cell_o[1] & 1);

        chip->eg_level_l[0] = (chip->eg_cell_o[1] >> 3) & 4095;
        chip->eg_level_l[2] = chip->eg_level_l[1];

        chip->eg_kon[1] = chip->eg_kon[0];

        int mute = chip->eg_mute | ic_async;
        int kon = chip->eg_kon[0] & 1;

        int level = 0;
        if (mute) {
            level |= 4095;
        }
        if (!kon) {
            level |= chip->eg_level_l[3];
        } else if (!chip->eg_maxrate[2]){
            level |= 2560;
        }

        chip->eg_level_add1 = level;

        int einc = 0;
        if (chip->eg_linear) {
            einc |= chip->eg_inc1 << 0;
            einc |= chip->eg_inc2 << 1;
            einc |= chip->eg_inc3 << 2;
            einc |= chip->eg_inc4 << 3;
            einc |= chip->eg_inc5 << 4;
        }
        if (chip->eg_exp) {
            if (chip->eg_inc1) {
                einc |= (~chip->eg_level_l[3] >> 6) & 4095;
            }
            if (chip->eg_inc2) {
                einc |= (~chip->eg_level_l[3] >> 5) & 4095;
            }
            if (chip->eg_inc3) {
                einc |= (~chip->eg_level_l[3] >> 4) & 4095;
            }
            if (chip->eg_inc4) {
                einc |= (~chip->eg_level_l[3] >> 3) & 4095;
            }
            if (chip->eg_inc5) {
                einc |= (~chip->eg_level_l[3] >> 2) & 4095;
            }
        }

        chip->eg_level_add2 = einc;

        chip->eg_lev_shift = (chip->reg_opc0new_l[1] >> 4) & 3;

        chip->eg_level_shifted[1] = chip->eg_level_shifted[0];

        chip->eg_rev_sel = (chip->reg_opc0new_l[1] >> 3) & 1;

        chip->eg_o17[0] = chip->fsm_o17[1];
        chip->eg_o18[0] = chip->fsm_o18[1];
        chip->eg_o19[0] = chip->fsm_o19[1];

        chip->eg_o17[2] = chip->eg_o17[1];
        chip->eg_o18[2] = chip->eg_o18[1];
        chip->eg_o19[2] = chip->eg_o19[1];

        chip->eg_int_en[0] = chip->eg_int_en[1] << 1;
        if (chip->reg_ch20_l[1] & 64) {
            chip->eg_int_en[0] |= 1;
        }

        chip->eg_rev_l1[0] = chip->eg_rev_l1[1] << 1;
        if (chip->eg_kon[0] & 0x80000000) {
            chip->eg_rev_l1[0] |= 1;
        }
        if (!chip->eg_rev1) {
            chip->eg_rev_l1[0] |= (chip->eg_rev_l2[1] >> 7) & 1;
        }
        chip->eg_rev_l2[0] = chip->eg_rev_l2[1] << 1;
        if (!chip->eg_rev2) {
            chip->eg_rev_l2[0] |= (chip->eg_rev_l1[1] >> 7) & 1;
        }

        chip->eg_lfo[0] = chip->lfo_amsel[1] ? chip->lfo2.am : chip->lfo1.am;

        int lfo_shift = 0;
        if (chip->lfo_amse[1]) {
            lfo_shift = chip->eg_lfo[1] >> (3 - chip->lfo_amse[1]);
        }
        chip->eg_lfo_sh[0] = lfo_shift;

        chip->eg_lev_lfo_l[1] = chip->eg_lev_lfo_l[0];

        int eg_lev_lfo_h = chip->eg_lfo_sh[1] + chip->eg_level_shifted[2] + chip->eg_lev_lfo_c;
        chip->eg_lev_lfo_h[0] = eg_lev_lfo_h & 63;
        chip->eg_lev_lfo_c2[0] = eg_lev_lfo_h >> 6;
        chip->eg_lev_lfo_c2[2] = chip->eg_lev_lfo_c2[1];

        chip->eg_tl[0] = chip->ramp_tl_out;

        int trem_shift = 0;
        if (chip->trem_sens[1]) {
            int trem = chip->trem_sel[1] ? chip->trem_latch_b : chip->trem_latch_a;
            trem_shift = trem >> (3 - chip->trem_sens[1]);
        }
        chip->eg_trem_shift[0] = trem_shift;

        int trem_tl_h = chip->eg_tl[1] + chip->eg_trem_shift[1] + chip->eg_tl_trem_c1;

        int c2 = trem_tl_h >> 6;

        if (c2) {
            chip->eg_tl_trem[1] = 0xfff;
        } else {
            chip->eg_tl_trem[1] = ((trem_tl_h & 63) << 6) | chip->eg_tl_trem[0];
        }
        int eg_sum_h = chip->eg_tl_trem[2] + chip->eg_lev_lfo_h[1] + chip->eg_sum_c1;
        chip->eg_sum_c2 = eg_sum_h >> 6;
        chip->eg_sum[1] = ((eg_sum_h & 63) << 6) | chip->eg_sum[0];

        chip->eg_dbg_load[0] = chip->fsm_29[1];

        if (chip->eg_dbg_load[1]) {
            chip->eg_dbg_shifter[0] = chip->eg_sum_clip;
        } else {
            chip->eg_dbg_shifter[0] = chip->eg_dbg_shifter[1] << 1;
        }
    }
    if (hclk2) {
        chip->eg_sync[1] = chip->eg_sync[0];

        chip->eg_subcnt_reset = (chip->eg_subcnt[0] & 2) != 0 && chip->eg_sync[0];
        chip->eg_subcnt[1] = chip->eg_subcnt[0];

        chip->eg_timer_carry[1] = chip->eg_timer_carry[0];

        int clock = (chip->eg_subcnt[0] & 2) != 0 || (chip->reg_test[0] & 1) != 0;
        chip->eg_clock[0] = clock;

        chip->eg_half[1] = chip->eg_half[0];

        chip->eg_timer[1] = chip->eg_timer[0];
        chip->eg_timer_masked[1] = chip->eg_timer_masked[0];
        chip->eg_masking[1] = chip->eg_masking[0];

        chip->eg_sync2[0] = chip->fsm_o21;
        chip->eg_sync2[2] = chip->eg_sync2[1];


        int timer_lock = chip->eg_clock[0] && !chip->fsm_o1[1] && chip->eg_sync2[1];

        if (!chip->eg_timerlock_l && timer_lock) {
            chip->eg_timer_lo_lock = chip->eg_timer[0] & 3;
            chip->eg_shift_lock = 0;
            if (chip->eg_timer_masked[0] & 0x1555) {
                chip->eg_shift_lock |= 1;
            }
            if (chip->eg_timer_masked[0] & 0x2666) {
                chip->eg_shift_lock |= 2;
            }
            if (chip->eg_timer_masked[0] & 0x3878) {
                chip->eg_shift_lock |= 4;
            }
            if (chip->eg_timer_masked[0] & 0x3f80) {
                chip->eg_shift_lock |= 8;
            }
        }

        chip->eg_timerlock_l = timer_lock;


        chip->eg_ar[1] = chip->eg_ar[0];
        chip->eg_d1r[1] = chip->eg_d1r[0];
        chip->eg_d2r[1] = chip->eg_d2r[0];
        chip->eg_rr[1] = chip->eg_rr[0];
        chip->eg_rev[1] = chip->eg_rev[0];
        chip->eg_zero_rev[1] = chip->eg_zero_rev[0];


        chip->eg_cell_sel[1] = chip->eg_cell_sel[0];


        chip->eg_bus[0] = 0;
        chip->eg_bus[1] = 0;
        chip->eg_bus[2] = 0;
        chip->eg_bus[3] = 0;

        chip->eg_cell_o[1] = chip->eg_cell_o[0];

        chip->eg_sl[1] = chip->eg_sl[0];

        chip->eg_ks[1] = chip->eg_ks[0];

        chip->eg_rate[1] = chip->eg_rate[0];

        int rks = 0;
        switch (chip->eg_ks[2]) {
            case 0:
                if (chip->eg_rate[0] != 0) {
                    rks = chip->eg_kcode >> 3;
                }
                break;
            case 1:
                rks = chip->eg_kcode >> 2;
                break;
            case 2:
                rks = chip->eg_kcode >> 1;
                break;
            case 3:
                rks = chip->eg_kcode;
                break;
        }

        chip->eg_zerorate[0] = chip->eg_rate[0] == 0;
        chip->eg_zerorate[2] = chip->eg_zerorate[1];

        chip->eg_rks = rks;

        int rateks = chip->eg_rateks;
        if (rateks & 64) {
            rateks = 63;
        }

        chip->eg_zerorate2 = rateks == 0;
        chip->eg_maxrate[0] = rateks == 63;

        const static int stephi[4][4] = {
            0,0,0,0,
            1,0,0,0,
            1,0,1,0,
            1,1,1,0
        };

        chip->eg_stephi = stephi[rateks & 3][chip->eg_timer_lo_lock];

        chip->eg_rate10 = (rateks >> 2) == 10;
        chip->eg_rate11 = (rateks >> 2) == 11;
        chip->eg_rate12 = (rateks >> 2) == 12;
        chip->eg_rate13 = (rateks >> 2) == 13;
        chip->eg_rate14 = (rateks >> 2) == 14;
        chip->eg_rate15 = (rateks >> 2) == 15;

        chip->eg_ratehi = chip->eg_rate10 || chip->eg_rate11 || chip->eg_rate12 || chip->eg_rate13 || chip->eg_rate14 || chip->eg_rate15;

        chip->eg_inc1 = chip->eg_clock[1] ? chip->eg_incp1 : (chip->eg_incp2 || chip->eg_incp3);
        chip->eg_inc2 = chip->eg_clock[1] ? chip->eg_incp3 : chip->eg_incp4;
        chip->eg_inc3 = chip->eg_clock[1] ? chip->eg_incp4 : chip->eg_incp5;
        chip->eg_inc4 = chip->eg_clock[1] ? chip->eg_incp5 : chip->eg_incp6;
        chip->eg_inc5 = chip->eg_clock[1] && chip->eg_incp6;

        chip->eg_shift_add1 = rateks >> 2;
        chip->eg_shift_add2 = chip->eg_shift_lock;

        chip->eg_rate_lo = rateks & 3;

        chip->eg_state_l[1] = chip->eg_state_l[0];
        chip->eg_state_l[3] = chip->eg_state_l[2];

        chip->eg_key_l[1] = chip->eg_key_l[0];

        int state = chip->eg_state_l[4];
        int key = (chip->eg_key_l[0] & 4) != 0;
        int kon = (chip->eg_okeyn_l[0] & 4) != 0 && key;
        int sl = chip->eg_sl[2];
        if (sl == 15) {
            sl |= 16;
        }
        int slreach = (chip->eg_level_l[2] >> 6) == (sl << 1);
        int zero = chip->eg_level_l[2] == 0;
        int quiet = (chip->eg_level_l[2] & 0xf00) == 0xf00;

        int next_state = eg_state_attack;

        int mute = !kon && state != eg_state_attack && quiet;
        chip->eg_mute = mute;

        if (!kon && !key) {
            next_state |= eg_state_release;
        }
        if (mute) {
            next_state |= eg_state_release;
        }
        if (!kon && state == eg_state_release) {
            next_state |= eg_state_release;
        }
        if (!kon && state == eg_state_sustain) {
            next_state |= eg_state_sustain;
        }
        if (!kon && state == eg_state_decay && slreach) {
            next_state |= eg_state_sustain;
        }
        if (!kon && state == eg_state_decay && !slreach) {
            next_state |= eg_state_decay;
        }
        if (!kon && state == eg_state_attack && zero) {
            next_state |= eg_state_decay;
        }
        if (ic_async) {
            next_state |= eg_state_release;
        }

        chip->eg_linear = (!kon && !quiet && (state & 2) != 0) || (!kon && !quiet && !slreach && state == eg_state_decay);
        chip->eg_exp = state == eg_state_attack && !zero && key && !chip->eg_maxrate[1];

        chip->eg_maxrate[2] = chip->eg_maxrate[1];

        chip->eg_state_next = next_state;
        chip->eg_okeyn_l[1] = chip->eg_okeyn_l[0];

        chip->eg_kon[0] = (chip->eg_kon[1] << 1) | kon;
        
        chip->eg_level_l[1] = chip->eg_level_l[0];
        chip->eg_level_l[3] = chip->eg_level_l[2];

        chip->eg_level_next = (chip->eg_level_add1 + chip->eg_level_add2) & 4095;

        int lev_shifted = 0;
        if ((chip->reg_test[0] & 32) == 0) {
            switch (chip->eg_lev_shift) {
                case 0:
                    lev_shifted = chip->eg_level_l[0];
                    break;
                case 1:
                    lev_shifted = chip->eg_level_l[0] >> 1;
                    break;
                case 2:
                    lev_shifted = chip->eg_level_l[0] >> 2;
                    break;
                case 3:
                    lev_shifted = chip->eg_level_l[0] >> 3;
                    break;
            }
        }

        chip->eg_level_shifted[0] = lev_shifted;
        chip->eg_level_shifted[2] = chip->eg_level_shifted[1] >> 6;

        int int_threshold = (lev_shifted & 2048) != 0 && (lev_shifted & (1024 + 512)) != 0;
        int rev_threshold_n = (lev_shifted & (2048+1024)) == 0 && ((lev_shifted & 512) == 0 || ((lev_shifted & 256) == 0 && chip->eg_rev_sel));

        chip->eg_o17[1] = chip->eg_o17[0];
        chip->eg_o18[1] = chip->eg_o18[0];
        chip->eg_o19[1] = chip->eg_o19[0];

        chip->eg_int_en[1] = chip->eg_int_en[0];

        int special = (chip->reg_15[0] & 3) == 3;

        chip->eg_int = (chip->eg_int_en[0] & 8) != 0 && int_threshold && ((chip->eg_o18[2] && special) || chip->eg_o19[2]);

        int notattack = chip->eg_level_l[2] != eg_state_attack;

        chip->eg_rev1 = notattack && !rev_threshold_n && ((chip->eg_o18[2] && special) || chip->eg_o19[2]);
        chip->eg_rev2 = notattack && !rev_threshold_n && ((chip->eg_o17[2] && special) || (chip->eg_o19[2] && !special));

        chip->eg_rev_l1[1] = chip->eg_rev_l1[0];
        chip->eg_rev_l2[1] = chip->eg_rev_l2[0];

        chip->eg_lfo[1] = chip->eg_lfo[0];
        chip->eg_lfo_sh[1] = chip->eg_lfo_sh[0] >> 6;

        int eg_lev_lfo_l = (chip->eg_lfo_sh[0] & 63) + (chip->eg_level_shifted[1] & 63);
        chip->eg_lev_lfo_l[0] = eg_lev_lfo_l & 63;
        chip->eg_lev_lfo_c = eg_lev_lfo_l >> 6;

        chip->eg_lev_lfo_h[1] = chip->eg_lev_lfo_h[0];
        chip->eg_lev_lfo_c2[1] = chip->eg_lev_lfo_c2[0];

        int trem_tl_l = (chip->eg_trem_shift[0] & 63) + ((chip->eg_tl[0] & 15) << 2);
        chip->eg_tl_trem[0] = trem_tl_l & 63;

        chip->eg_tl_trem_c1 = trem_tl_l >> 6;

        chip->eg_tl[1] = chip->eg_tl[0] >> 4;
        chip->eg_trem_shift[1] = chip->eg_trem_shift[0] >> 6;

        chip->eg_tl_trem[2] = chip->eg_tl_trem[1] >> 6;

        int eg_sum_l = (chip->eg_tl_trem[1] & 63) + chip->eg_lev_lfo_l[1];
        chip->eg_sum[0] = eg_sum_l & 62;
        chip->eg_sum_c1 = (eg_sum_l >> 6) & 1;

        if (chip->eg_sum_c2 || chip->eg_lev_lfo_c2[2]) {
            chip->eg_sum_clip = 4095;
        } else {
            chip->eg_sum_clip = chip->eg_sum[1];
        }

        chip->eg_dbg_load[1] = chip->eg_dbg_load[0];

        chip->eg_dbg_shifter[1] = chip->eg_dbg_shifter[0];
    }

    if (hclk1) {
        chip->trem_subcnt[0] = ic_async ? 0 : (chip->trem_subcnt[1] + chip->trem_subcnt_inc) & 255;

        int reg1c_7 = (chip->reg_1c[1] >> 7) & 1;
        int reg1e_7 = (chip->reg_1e[1] >> 7) & 1;

        chip->trem_ctrl1[0] = reg1c_7 ? (chip->trem_state[1][1] ? chip->trem_subcnt_sel2 : chip->trem_subcnt_sel3) : chip->trem_subcnt_sel1;
        chip->trem_ctrl2[0] = reg1e_7 ? (chip->trem_state[0][1] ? chip->trem_subcnt_sel2 : chip->trem_subcnt_sel3) : chip->trem_subcnt_sel1;

        int a = chip->trem_shifter[1] & 1;
        int c = (!chip->trem_state[0][1] && (chip->trem_fsm_19_l[1] & 2) != 0 && chip->trem_ctrl2[1])
            || (!chip->trem_state[1][1] && chip->fsm_31[1] && chip->trem_ctrl1[1])
            || (chip->trem_adder_c[1] && (chip->fsm_o14[1] || chip->fsm_o13[1]));
        int b = (!chip->trem_state[3][1] && chip->trem_state[1][1] && chip->fsm_o11[1] && chip->trem_ctrl1[1])
            || (!chip->trem_state[2][1] && chip->trem_state[0][1] && chip->fsm_o15[1] && chip->trem_ctrl2[1]);
        int adder = a + b + c;

        chip->trem_shifter[0] = (chip->trem_shifter[1] >> 1);
        chip->trem_shifter[0] |= (adder & 1) << 31;
        if (chip->fsm_8[1]) {
            chip->trem_shifter[0] &= ~0x3ff;
            chip->trem_shifter[0] |= (chip->reg_1c[1] & 127) << 3;
        }

        chip->trem_shifter2[0] = chip->trem_shifter2[1] >> 1;
        if (chip->fsm_30[1]) {
            chip->trem_shifter2[0] &= ~0x3ff;
            chip->trem_shifter2[0] |= (chip->reg_1e[1] & 127) << 3;
        }


        chip->trem_adder_c[0] = (adder >> 1);

        int cmp_a = !(((chip->trem_shifter[1] & 1) != 0 && chip->fsm_o12[1]) || (chip->fsm_o11[1] && (chip->trem_shifter2[1] & 1) != 0));
        int cmp_b = (chip->trem_shifter[1] >> 22) & 1;
        int cmp_c = chip->trem_cmp_adder_c[1] || chip->trem_fsm_9[1] || chip->fsm_31[1];
        int cmp_adder = cmp_a + cmp_b + cmp_c;
        int cmp_o = cmp_adder & 1;
        int cmp_co = cmp_adder >> 1;

        chip->trem_cmp_adder_c[0] = cmp_co;

        if (!chip->trem_cmp_adder_c[1] && chip->trem_fsm_9[1]) {
            chip->trem_state[0][0] = 0;
        } else if (chip->trem_cmp_adder_c[1] && chip->trem_fsm_9[1]) {
            chip->trem_state[0][0] = 1;
        } else {
            chip->trem_state[0][0] = chip->trem_state[0][1];
        }
        if (!chip->trem_cmp_adder_c[1] && chip->fsm_19[1]) {
            chip->trem_state[1][0] = 0;
        } else if (chip->trem_cmp_adder_c[1] && chip->fsm_19[1]) {
            chip->trem_state[1][0] = 1;
        } else {
            chip->trem_state[1][0] = chip->trem_state[1][1];
        }
        if (chip->fsm_o11[1] && cmp_o) {
            chip->trem_state[2][0] = 0;
        } else if (chip->fsm_31[1] && !cmp_o) {
            chip->trem_state[2][0] = 1;
        } else {
            chip->trem_state[2][0] = chip->trem_state[2][1];
        }
        if (chip->fsm_o12[1] && cmp_o) {
            chip->trem_state[3][0] = 0;
        } else if (chip->trem_fsm_9[1] && !cmp_o) {
            chip->trem_state[3][0] = 1;
        } else {
            chip->trem_state[3][0] = chip->trem_state[3][1];
        }


        chip->trem_fsm_19_l[0] = (chip->trem_fsm_19_l[1] << 1) | chip->fsm_19[1];
        chip->trem_fsm_9[0] = chip->fsm_8[1];

        chip->trem_fsm_31_l = chip->fsm_31[1];
        chip->trem_fsm_9_l = chip->trem_fsm_9[1];

        chip->trem_sel[0] = (chip->reg_ch38new_l[1] >> 1) & 1;

        chip->trem_sens[0] = (chip->reg_opa0_l[1] >> 5) & 3;
    }
    if (hclk2) {
        chip->trem_subcnt[1] = chip->trem_subcnt[0];
        chip->trem_subcnt_inc = chip->fsm_14;

        chip->trem_subcnt_sel1 = (chip->trem_subcnt[0] & 3) != 0;
        chip->trem_subcnt_sel2 = (chip->trem_subcnt[0] & 3) == 3;
        chip->trem_subcnt_sel3 = (chip->trem_subcnt[0] & 31) == 31 && ((chip->trem_subcnt[0] & 32) != 0 || (chip->trem_subcnt[0] & 192) == 192);

        chip->trem_shifter[1] = chip->trem_shifter[0];
        chip->trem_shifter2[1] = chip->trem_shifter2[0];

        chip->trem_ctrl1[1] = chip->trem_ctrl1[0];
        chip->trem_ctrl2[1] = chip->trem_ctrl2[0];

        chip->trem_adder_c[1] = chip->trem_adder_c[0];

        chip->trem_state[0][1] = chip->trem_state[0][0];
        chip->trem_state[1][1] = chip->trem_state[1][0];
        chip->trem_state[2][1] = chip->trem_state[2][0];
        chip->trem_state[3][1] = chip->trem_state[3][0];

        chip->trem_fsm_19_l[1] = chip->trem_fsm_19_l[0];

        chip->trem_cmp_adder_c[1] = chip->trem_cmp_adder_c[0];
        chip->trem_fsm_9[1] = chip->trem_fsm_9[0];

        chip->trem_sel[1] = chip->trem_sel[0];
        chip->trem_sens[1] = chip->trem_sens[0];
    }

    if (ic_async) {
        chip->trem_latch_a = 0;
        chip->trem_latch_b = 0;
    }
    if (chip->fsm_31[1] && !chip->trem_fsm_31_l) {
        chip->trem_latch_a = (chip->trem_shifter[1] >> 22) & 0x3ff;
    }
    if (chip->trem_fsm_9[1] && !chip->trem_fsm_9_l) {
        chip->trem_latch_b = (chip->trem_shifter[1] >> 22) & 0x3ff;
    }


    if (hclk1) {
        // nofb - 0-11
        // 0 - none
        // 1 - 8-15
        // 2 - 7-15
        // 3 - 6-15
        // 4 - 5-15
        // 5 - 4-15
        // 6 - 3-14
        // 7 - 2-13
        int mod = (short)chip->op_modsum[2];
        if (chip->op_mod_shift[1] != 0) {
            chip->op_mod_in[0] = (mod >> (9 - chip->op_mod_shift[1])) & 4095;
        } else {
            chip->op_mod_in[0] = 0;
        }

        int sh = 0;
        if (chip->op_dofb[1]) {
            sh = chip->op_fb[5];
        }
        chip->op_mod_shift[0] = sh;

        chip->op_fb[0] = (chip->reg_ch_bus >> 11) & 7;
        chip->op_fb[2] = chip->op_fb[1];
        chip->op_fb[4] = chip->op_fb[3];

        chip->op_dofb[0] = chip->fsm_alg_o[1];

        int phase_l = (chip->op_mod_in[1] & 63) + (chip->op_phase_in[0] & 63);
        chip->op_phase_c = phase_l >> 6;
        chip->op_phase[0] = phase_l & 63;

        chip->op_mod_in[2] = chip->op_mod_in[1] >> 6;
        chip->op_phase_in[1] = chip->op_phase_in[0] >> 6;

        chip->op_wf12[0] = (chip->reg_op40new_l[1] >> 5) & 3;
        chip->op_wf12[2] = chip->op_wf12[1];
        chip->op_wf0[0] = chip->op_wf0[1] << 1;
        chip->op_wf0[0] |= (chip->reg_op40new_l[1] >> 4) & 1;

        int phase0 = (chip->op_wf12[3] & 2) != 0 ? (chip->op_phase[1] << 1) & 2047 : chip->op_phase[1] & 2047;
        int phase1 = (phase0 & 1024) != 0 ? phase0 ^ 2047 : phase0 & 1023;

        int sign = chip->op_wf12[3] == 2 ? (chip->op_phase[1] & 1024) != 0 : (chip->op_phase[1] & 2048) != 0;
        int mute = chip->op_wf12[3] != 0 && (chip->op_phase[1] & 2048) != 0;

        chip->op_sign[0] = (chip->op_sign[1] << 1) | sign;
        chip->op_mute[0] = (chip->op_mute[1] << 1) | mute;

        chip->op_logsin_index = phase1;

        int logsin_l = (chip->op_logsin_base[0] & 63) + (chip->op_logsin_delta[0] & 63);
        chip->op_logsin_c1 = logsin_l >> 6;
        chip->op_logsin[0] = logsin_l & 63;

        chip->op_logsin_base[1] = chip->op_logsin_base[0] >> 6;
        chip->op_logsin_delta[1] = chip->op_logsin_delta[0] >> 6;

        chip->op_logsin_wf[0] = (chip->op_wf0[1] & 8) != 0 ? chip->op_logsin[1] << 1 : chip->op_logsin[1];
        chip->op_atten[0] = chip->eg_sum_clip;
        int atten_h = chip->op_logsin_atten_c1 + (chip->op_logsin_wf[1] & 63) + chip->op_atten[1];

        chip->op_logsin_atten[1] = chip->op_logsin_atten[0] | ((atten_h & 63) << 8);

        chip->op_logsin_atten_clip = (atten_h & 64) != 0 || (chip->op_logsin_wf[1] & 64) != 0 || (chip->op_mute[1] & 4) != 0;

        chip->op_pow_base[0] = pow_base[chip->op_pow_index >> 2];
        switch (chip->op_pow_index & 3) {
        case 0:
            chip->op_pow_delta[0] = pow_delta[0][chip->op_pow_index >> 2];
            break;
        case 1:
            chip->op_pow_delta[0] = pow_delta[1][chip->op_pow_index >> 2];
            break;
        case 2:
            chip->op_pow_delta[0] = pow_delta[2][chip->op_pow_index >> 2];
            break;
        default:
            chip->op_pow_delta[0] = 0;
            break;
        }

        int pow_l = (chip->op_pow_base[1] & 15) + chip->op_pow_delta[1];

        chip->op_pow[0] = pow_l & 15;
        chip->op_pow_c = pow_l >> 4;
        chip->op_pow_base[2] = chip->op_pow_base[1] >> 4;

        chip->op_pow_shift[1] = chip->op_pow_shift[0];
        chip->op_pow_shift[3] = chip->op_pow_shift[2];
        chip->op_pow_shift[5] = chip->op_pow_shift[4];
        

        chip->op_pow_shifted_l[0] = (chip->op_pow[1] | 2048) << (3 - chip->op_pow_shift_l);

        int shifted_h = chip->op_pow_shifted_l[1] >> (4 * chip->op_pow_shift[6]);
        if (chip->reg_test[1] & 16) {
            shifted_h |= 1 << 15;
        }
        if (chip->op_sign[1] & 64) {
            shifted_h ^= 65535;
        }

        chip->op_value[0] = shifted_h;
        chip->op_value[2] = chip->op_value[1];
        chip->op_value[4] = chip->op_value[3];
        int corr = (chip->op_sign[1] & 512) != 0;
        chip->op_value[6] = (chip->op_value[5] + corr) & 65535;
        chip->op_value[8] = chip->op_value[7];

        chip->op_cell_sel[0] = (chip->op_cell_sel[1] << 1) | chip->fsm_o9[1];

        chip->op_cell_in[0] = chip->fsm_alg_o[2] ? chip->op_value[9] : chip->op_cell_bus_l[0][1];
        chip->op_cell_in[1] = chip->fsm_alg_o[2] ? chip->op_cell_bus_l[0][1] : chip->op_cell_bus_l[1][1];
        chip->op_cell_in[2] = chip->fsm_alg_o[0] ? chip->op_value[9] : chip->op_cell_bus_l[2][1];
        chip->op_cell_in[3] = chip->fsm_alg_o[0] ? chip->op_cell_bus_l[2][1] : chip->op_cell_bus_l[3][1];

        for (i = 0; i < 8; i++) {
            if (chip->op_cell_sel[1] & (1 << i)) {
                chip->op_cell_bus[0] |= chip->op_cells[0][i];
                chip->op_cell_bus[1] |= chip->op_cells[1][i];
                chip->op_cell_bus[2] |= chip->op_cells[2][i];
                chip->op_cell_bus[3] |= chip->op_cells[3][i];
            }
        }


        for (i = 0; i < 8; i++) {
            if (chip->op_cell_sel[1] & (2 << i)) {
                chip->op_cells[0][i] = chip->op_cell_in[0];
                chip->op_cells[1][i] = chip->op_cell_in[1];
                chip->op_cells[2][i] = chip->op_cell_in[2];
                chip->op_cells[3][i] = chip->op_cell_in[3];
            }
        }

        chip->op_cell_bus_l[0][0] = chip->op_cell_bus[0];
        chip->op_cell_bus_l[1][0] = chip->op_cell_bus[1];
        chip->op_cell_bus_l[2][0] = chip->op_cell_bus[2];
        chip->op_cell_bus_l[3][0] = chip->op_cell_bus[3];

        int mod1 = 0;
        int mod2 = 0;
        if (chip->fsm_alg_o[5]) {
            mod1 |= chip->op_cell_bus_l[1][1];
        }
        if (chip->fsm_alg_o[6]) {
            mod1 |= chip->op_value[9];
        }
        if (chip->fsm_alg_o[7]) {
            mod1 |= chip->op_cell_bus_l[3][1];
        }
        if (chip->fsm_alg_o[4]) {
            mod2 |= chip->op_cell_bus_l[0][1];
        }
        if (chip->fsm_alg_o[3]) {
            mod2 |= chip->op_cell_bus_l[2][1];
        }

        chip->op_mod1[0] = mod1;
        chip->op_mod2[0] = mod2;

        int sm1 = chip->op_mod1[1];
        int sm2 = chip->op_mod2[1];

        if (sm1 & 128) {
            sm1 |= 256;
        }
        if (sm2 & 128) {
            sm2 |= 256;
        }

        int mod_h = sm1 + sm2 + chip->op_modsum_c;
        chip->op_modsum[1] = chip->op_modsum[0] | ((mod_h & 511) << 7);
    }
    if (hclk2) {
        chip->op_phase_in[0] = chip->pg_out[2];
        chip->op_mod_in[1] = chip->op_mod_in[0];

        chip->op_mod_shift[1] = chip->op_mod_shift[0];

        chip->op_dofb[1] = chip->op_dofb[0];

        chip->op_fb[1] = chip->op_fb[0];
        chip->op_fb[3] = chip->op_fb[2];
        chip->op_fb[5] = chip->op_fb[4];

        int phase_h = (chip->op_mod_in[2] + chip->op_phase_in[1] + chip->op_phase_c) & 63;
        chip->op_phase[1] = chip->op_phase[0] | (phase_h << 6);
        chip->op_wf12[1] = chip->op_wf12[0];
        chip->op_wf12[3] = chip->op_wf12[2];
        chip->op_wf0[1] = chip->op_wf0[0];

        chip->op_sign[1] = chip->op_sign[0];
        chip->op_mute[1] = chip->op_mute[0];

        chip->op_logsin_base[0] = sin_base[chip->op_logsin_index >> 2];
        switch (chip->op_logsin_index & 3) {
            case 0:
                chip->op_logsin_delta[0] = sin_delta[0][chip->op_logsin_index >> 2];
                break;
            case 1:
                chip->op_logsin_delta[0] = sin_delta[1][chip->op_logsin_index >> 2];
                break;
            case 2:
                chip->op_logsin_delta[0] = sin_delta[2][chip->op_logsin_index >> 2];
                break;
            default:
                chip->op_logsin_delta[0] = 0;
                break;
        }

        int logsin_h = chip->op_logsin_base[1] + chip->op_logsin_delta[1] + chip->op_logsin_c1;

        chip->op_logsin[1] = (logsin_h << 6) | chip->op_logsin[0];


        chip->op_logsin_atten[0] = chip->op_logsin_wf[0] & 3;
        int atten_l = 1 + ((chip->op_logsin_wf[0] >> 2) & 63) + (chip->op_atten[0] & 63);

        chip->op_logsin_atten_c1 = atten_l >> 6;
        chip->op_logsin_atten[0] |= (atten_l & 63) << 2;

        chip->op_logsin_wf[1] = chip->op_logsin_wf[0] >> 8;
        chip->op_atten[1] = chip->op_atten[0] >> 6;

        int powindex = chip->op_logsin_atten_clip ? 0x3fff : chip->op_logsin_atten[1];

        chip->op_pow_index = powindex & 1023;
        chip->op_pow_shift[0] = powindex >> 10;
        chip->op_pow_shift[2] = chip->op_pow_shift[1];
        chip->op_pow_shift[4] = chip->op_pow_shift[3] >> 2;
        chip->op_pow_shift[6] = chip->op_pow_shift[5];

        chip->op_pow_base[1] = chip->op_pow_base[0];
        chip->op_pow_delta[1] = chip->op_pow_delta[0];

        int pow_h = chip->op_pow_base[2] + chip->op_pow_c;

        chip->op_pow[1] = (pow_h << 4) | chip->op_pow[0];

        chip->op_pow_shift_l = chip->op_pow_shift[3] & 3;

        chip->op_pow_shifted_l[1] = chip->op_pow_shifted_l[0];

        chip->op_value[1] = chip->op_value[0];
        chip->op_value[3] = chip->op_value[2];
        chip->op_value[5] = chip->op_value[4];
        chip->op_value[7] = chip->op_value[6];
        chip->op_value[9] = chip->op_value[8];

        chip->op_cell_sel[1] = chip->op_cell_sel[0];

        chip->op_cell_bus[0] = 0;
        chip->op_cell_bus[1] = 0;
        chip->op_cell_bus[2] = 0;
        chip->op_cell_bus[3] = 0;

        chip->op_cell_bus_l[0][1] = chip->op_cell_bus_l[0][0];
        chip->op_cell_bus_l[1][1] = chip->op_cell_bus_l[1][0];
        chip->op_cell_bus_l[2][1] = chip->op_cell_bus_l[2][0];
        chip->op_cell_bus_l[3][1] = chip->op_cell_bus_l[3][0];

        int mod_l = (chip->op_mod1[0] & 255) + (chip->op_mod2[0] & 255);

        chip->op_modsum[0] = (mod_l >> 1) & 127;
        chip->op_modsum_c = mod_l >> 8;

        chip->op_mod1[1] = chip->op_mod1[0] >> 8;
        chip->op_mod2[1] = chip->op_mod2[0] >> 8;

        chip->op_modsum[2] = chip->op_modsum[1];
    }

    int dmode = chip->reg_a[1] & 1;

    int rclip = !dmode || (dmode && chip->ac_sh2_xr_nz[1] && (chip->ac_sh2_sync[1] & 16) == 0);
    int rch = !rclip && chip->ac_rcliph;
    int rcl = !rclip && chip->ac_rclipl;
    int rbit = rch || (!rcl && (chip->ac_serial_r[1] & 1) != 0);

    int lclip = !dmode || (dmode && chip->ac_sh2_xr_nz[1] && (chip->ac_sh2_sync[1] & 16) == 0);
    int lch = !lclip && chip->ac_lcliph;
    int lcl = !lclip && chip->ac_lclipl;
    int lbit = lch || (!lcl && (chip->ac_serial_l[1] & 1) != 0);

    if (hclk1) {
        int ro = chip->fsm_alg_o[8] && (chip->ac_pan[1] || chip->ac_mono);

        chip->ac_add_r[0] = ro ? (short)chip->op_value[9] : 0;
        chip->ac_sum_r_lo[0] = chip->fsm_13[1] ? 0 : chip->ac_sum_r_lo[1];
        chip->ac_sum_r_l[0] = chip->ac_sum_r_lo[1];
        chip->ac_sum_r_hi[0] = (chip->ac_sum_r_c + chip->ac_add_r[1] + chip->ac_sum_r_hi[1]) & 0x3ff;

        int lo = chip->fsm_alg_o[8] && (!chip->ac_pan[1] || chip->ac_mono);
        chip->ac_add_l[0] = lo ? (short)chip->op_value[9] : 0;
        chip->ac_sum_l_lo[0] = chip->fsm_29[1] ? 0 : chip->ac_sum_l_lo[1];
        chip->ac_sum_l_l[0] = chip->ac_sum_l_lo[1];
        chip->ac_sum_l_hi[0] = (chip->ac_sum_l_c + chip->ac_add_l[1] + chip->ac_sum_l_hi[1]) & 0x3ff;

        chip->ac_pan[0] = (chip->reg_ch20_l[1] >> 7) & 1;

        chip->ac_fsm14[0] = chip->fsm_13[1];
        chip->ac_fsm15[0] = chip->ac_fsm14[1];

        chip->ac_fsm30[0] = chip->fsm_29[1];
        chip->ac_fsm31[0] = chip->ac_fsm30[1];

        chip->ac_out_sync[0] = (chip->ac_out_sync[1] << 1) | chip->fsm_o3[1];

        int bit = 0;
        if (chip->fsm_o3[1]) {
            bit |= chip->ac_shifter_sign2;
        }
        if (chip->ac_out_sync[1] & 1) {
            bit |= (chip->ac_shifter_shift >> 0) & 1;
        }
        if (chip->ac_out_sync[1] & 2) {
            bit |= (chip->ac_shifter_shift >> 1) & 1;
        }
        if (chip->ac_out_sync[1] & 4) {
            bit |= (chip->ac_shifter_shift >> 2) & 1;
        }
        if ((chip->ac_out_sync[1] & 7) == 0 && !chip->fsm_o3[1]) {
            if (chip->ac_shifter_shift) {
                bit |= (chip->ac_shifter[1] >> (chip->ac_shifter_shift - 1)) & 1;
            }
        }

        chip->ac_shifter[0] = chip->ac_shifter[1] >> 1;
        chip->ac_shifter[0] |= chip->ac_shifter_in << 20;

        chip->ac_so_l[0] = bit;
        chip->ac_so_l[2] = chip->ac_so_l[1];

        chip->ac_rbit[0] = !dmode && rbit;
        chip->ac_rbit[2] = chip->ac_rbit[1];

        chip->ac_lbit[0] = !dmode && lbit;
        chip->ac_lbit[2] = chip->ac_lbit[1];

        chip->ac_sh1_l[0] = chip->fsm_o7[1];
        chip->ac_sh2_l[0] = chip->fsm_o8[1];
    }
    if (hclk2) {

        int addr_l = (chip->ac_add_r[0] & 0x3ff) + (chip->ac_sum_r_lo[0] & 0x3ff);

        chip->ac_sum_r_lo[1] = addr_l & 0x3ff;
        chip->ac_sum_r_c = addr_l >> 10;

        chip->ac_add_r[1] = chip->ac_add_r[0] >> 10;

        chip->ac_sum_r_l[1] = chip->ac_sum_r_l[0] | (chip->ac_sum_r_hi[1] << 10);

        chip->ac_sum_r_hi[1] = chip->ac_fsm14[0] ? 0 : chip->ac_sum_r_hi[0];

        addr_l = (chip->ac_add_l[0] & 0x3ff) + (chip->ac_sum_l_lo[0] & 0x3ff);

        chip->ac_sum_l_lo[1] = addr_l & 0x3ff;
        chip->ac_sum_l_c = addr_l >> 10;

        chip->ac_add_l[1] = chip->ac_add_l[0] >> 10;

        chip->ac_sum_l_l[1] = chip->ac_sum_l_l[0] | (chip->ac_sum_l_hi[1] << 10);

        chip->ac_sum_l_hi[1] = chip->ac_fsm30[0] ? 0 : chip->ac_sum_l_hi[0];

        chip->ac_mono = (chip->reg_a[0] & 4) != 0 && (chip->reg_30_0_l[0] & 32) != 0;
        chip->ac_pan[1] = chip->ac_pan[0];

        chip->ac_fsm14[1] = chip->ac_fsm14[0];
        chip->ac_fsm15[1] = chip->ac_fsm15[0];

        chip->ac_fsm30[1] = chip->ac_fsm30[0];
        chip->ac_fsm31[1] = chip->ac_fsm31[0];

        chip->ac_out_sync[1] = chip->ac_out_sync[0];
        chip->ac_shifter[1] = chip->ac_shifter[0];

        chip->ac_so_l[1] = chip->ac_so_l[0];
        chip->ac_so_l[3] = chip->ac_so_l[2];

        chip->ac_rbit[1] = chip->ac_rbit[0];
        chip->ac_rbit[3] = chip->ac_rbit[2];

        chip->ac_lbit[1] = chip->ac_lbit[0];
        chip->ac_lbit[3] = chip->ac_lbit[2];

        int shifter_in = chip->fsm_o5[1] ? chip->ac_lbit[3] : chip->ac_rbit[3];
        chip->ac_shifter_in = shifter_in;

        if ((chip->ac_out_sync[0] & 1) && (chip->ac_out_sync[1] & 1)) {
            chip->ac_shifter_sign = shifter_in;
            chip->ac_shifter_high = (chip->ac_shifter[1] >> 15) & 63;
        }
        if (chip->fsm_o2[1]) {
            chip->ac_shifter_sign2 = chip->ac_shifter_sign;
            int top = chip->ac_shifter_high;
            if (!chip->ac_shifter_sign) {
                top ^= 63;
            }

            int shift = 0;
            if (top & 32) {
                shift = 7;
            } else if (top & 16) {
                shift = 6;
            } else if (top & 8) {
                shift = 5;
            } else if (top & 4) {
                shift = 4;
            } else if (top & 2) {
                shift = 3;
            } else if (top & 1) {
                shift = 2;
            } else {
                shift = 1;
            }
            chip->ac_shifter_shift = shift;
        }

        chip->ac_sh1_l[1] = chip->ac_sh1_l[0] || ic_async;
        chip->ac_sh2_l[1] = chip->ac_sh2_l[0] || ic_async;

    }
    if (clk1) {
        chip->ac_fsm14_l[0] = chip->ac_fsm14[1];
        chip->ac_fsm30_l[0] = chip->ac_fsm30[1];
        chip->ac_rbit2[0] = rbit;
        chip->ac_lbit2[0] = lbit;

        chip->ac_sh2i_1[0] = (chip->ac_sh2i_1[1] << 1) | (chip->input.sh2 != 0);

        chip->ac_sh2i_2[0] = (chip->ac_sh2i_2[1] << 1);
        if ((chip->ac_sh2i_1[1] & 3) == 2) {
            chip->ac_sh2i_2[0] |= 1;
        }

        if (chip->ac_sh2i_2[1] & 32) {
            chip->ac_sh2i_cnt[0] = 9;
        } else if (ic_async || (chip->ac_sh2i_2[1] & 16) != 0 || chip->ac_sh2i_cnt[1] == 63) {
            chip->ac_sh2i_cnt[0] = 0;
        } else {
            chip->ac_sh2i_cnt[0] = (chip->ac_sh2i_cnt[1] + 1) & 63;
        }

        chip->ac_sh2_sync[0] = chip->ac_sh2_sync[1] << 1;
        chip->ac_sh2_sync[0] |= (chip->ac_sh2i_cnt[1] >> 5) & 1;

        int xr = ((chip->ac_sh2_sync[1] >> 4) ^ (chip->ac_sh2_sync[1] >> 3)) & 1;

        chip->ac_sh2_xr[0] = chip->ac_sh2_xr[1] << 1;
        chip->ac_sh2_xr[0] |= xr;

        chip->ac_sh2_xr_nz[0] = (chip->ac_sh2_xr[1] & 0x1fff) != 0;
    }
    if (clk2) {
        chip->ac_fsm14_l[1] = chip->ac_fsm14_l[0];
        chip->ac_fsm30_l[1] = chip->ac_fsm30_l[0];
        chip->ac_rbit2[1] = chip->ac_rbit2[0];
        chip->ac_lbit2[1] = chip->ac_lbit2[0];

        chip->ac_sh2i_1[1] = chip->ac_sh2i_1[0];
        chip->ac_sh2i_2[1] = chip->ac_sh2i_2[0];

        chip->ac_sh2i_cnt[1] = chip->ac_sh2i_cnt[0];
        chip->ac_sh2_sync[1] = chip->ac_sh2_sync[0];

        chip->ac_sh2_xr[1] = chip->ac_sh2_xr[0];
        chip->ac_sh2_xr_nz[1] = chip->ac_sh2_xr_nz[0];
    }

    int sclk1 = dmode ? clk1 : hclk1;
    int sclk2 = dmode ? clk2 : hclk2;

    if (chip->ac_fsm14[1] && !chip->ac_fsm14_l[1]) {
        chip->ac_sum_r_lock = chip->ac_sum_r_l[1];
    }

    if (chip->ac_fsm30[1] && !chip->ac_fsm30_l[1]) {
        chip->ac_sum_l_lock = chip->ac_sum_l_l[1];
    }

    int rload = dmode ? (chip->ac_sh2_sync[1] & 48) == 32 : chip->ac_fsm15[1];
    int lload = dmode ? (chip->ac_sh2_sync[1] & 48) == 16 : chip->ac_fsm31[1];

    // 000 - nop
    // 001 - cliph
    // 010 - cliph
    // 011 - cliph
    // 111 - nop
    // 110 - clipl
    // 101 - clipl
    // 100 - clipl

    if (rload && !chip->ac_rload_l) {
        int hi = (chip->ac_sum_r_lock >> 17) & 7;
        int clipl = (hi & 4) != 0 && ((hi & 1) == 0 || (hi & 3) == 1);
        int cliph = (hi & 4) == 0 && ((hi & 1) != 0 || (hi & 3) == 2);


        chip->ac_rclipl = clipl;
        chip->ac_rcliph = cliph;
    }

    if (lload && !chip->ac_lload_l) {
        int hi = (chip->ac_sum_l_lock >> 17) & 7;
        int clipl = (hi & 4) != 0 && ((hi & 1) == 0 || (hi & 3) == 1);
        int cliph = (hi & 4) == 0 && ((hi & 1) != 0 || (hi & 3) == 2);


        chip->ac_lclipl = clipl;
        chip->ac_lcliph = cliph;
    }

    if (sclk1) {
        int rmsb = (chip->ac_sum_r_lock >> 19) & 1;
        int rb = rload && (rmsb ^ !dmode);

        chip->ac_serial_r[0] = chip->ac_serial_r[1] >> 1;
        if (rload) {
            chip->ac_serial_r[0] |= chip->ac_sum_r_lock & 0x1ffff;
        }
        chip->ac_serial_r[0] |= (rb && (chip->ac_ext_r[1] & 1) != 0) << 17;

        chip->ac_rload_l = rload;

        chip->ac_ext_r[0] = chip->ac_ext_r[1];
        if (rb) {
            chip->ac_ext_r[0] |= 511;
        }

        int lmsb = (chip->ac_sum_l_lock >> 19) & 1;
        int lb = lload && (lmsb ^ !dmode);

        chip->ac_serial_l[0] = chip->ac_serial_l[1] >> 1;
        if (lload) {
            chip->ac_serial_l[0] |= chip->ac_sum_l_lock & 0x1ffff;
        }
        chip->ac_serial_l[0] |= (lb && (chip->ac_ext_l[1] & 1) != 0) << 17;

        chip->ac_lload_l = lload;

        chip->ac_ext_l[0] = chip->ac_ext_l[1];
        if (lb) {
            chip->ac_ext_l[0] |= 511;
        }
    }
    if (sclk2) {
        chip->ac_serial_r[1] = chip->ac_serial_r[0];
        chip->ac_ext_r[1] = chip->ac_ext_r[0];
        chip->ac_serial_l[1] = chip->ac_serial_l[0];
        chip->ac_ext_l[1] = chip->ac_ext_l[0];
    }

    if (hclk1) {
        int cnt_a = chip->timer_a_cnt[1] + chip->timer_a_inc;
        chip->timer_a_of = (cnt_a >> 10) & 1;
        if (chip->timer_a_load) {
            chip->timer_a_cnt[0] = chip->reg_timer_a[1];
        } else {
            chip->timer_a_cnt[0] = chip->timer_a_en[0] ? cnt_a & 0x3ff : 0;
        }
        chip->timer_a_en[1] = chip->timer_a_en[0];
        chip->timer_a_reset[0] = write1_en && chip->reg_write_14[1] && (chip->data_l & 16) != 0;
        chip->timer_a_status[0] = chip->timer_a_set || (chip->timer_a_status[1] && !chip->timer_a_reset[1]);

        int cnt_b = chip->timer_b_cnt[1] + chip->timer_b_inc;
        chip->timer_b_of = (cnt_b >> 8) & 1;
        if (chip->timer_b_load) {
            chip->timer_b_cnt[0] = chip->reg_timer_b[1];
        } else {
            chip->timer_b_cnt[0] = chip->timer_b_en[0] ? cnt_b & 0xff : 0;
        }
        chip->timer_b_en[1] = chip->timer_b_en[0];
        chip->timer_b_reset[0] = write1_en && chip->reg_write_14[1] && (chip->data_l & 32) != 0;
        chip->timer_b_status[0] = chip->timer_b_set || (chip->timer_b_status[1] && !chip->timer_b_reset[1]);

        chip->timer_clk[0] = chip->fsm_30[1];

        int subcnt = chip->timer_b_subcnt[1] + chip->timer_clk[1];
        chip->timer_b_subcnt[0] = ic_async ? 0 : subcnt & 31;
        chip->timer_b_subcnt_of = (subcnt >> 5) & 1;
    }
    if (hclk2) {
        int test = (chip->reg_test[0] & 4) != 0;
        chip->timer_a_cnt[1] = chip->timer_a_cnt[0];
        chip->timer_a_load = chip->timer_a_of || (!chip->timer_a_en[1] && chip->reg_timer_a_load[0]);
        chip->timer_a_en[0] = chip->reg_timer_a_load[0];
        chip->timer_a_inc = (chip->reg_timer_a_load[0] && chip->timer_clk[0]) || test;
        int rst_a = chip->timer_a_reset[0] || ic_async;
        chip->timer_a_reset[1] = rst_a;
        chip->timer_a_set = chip->timer_a_of && !rst_a && chip->reg_timer_a_irq[1];

        chip->timer_b_cnt[1] = chip->timer_b_cnt[0];
        chip->timer_b_load = chip->timer_b_of || (!chip->timer_b_en[1] && chip->reg_timer_b_load[0]);
        chip->timer_b_en[0] = chip->reg_timer_b_load[0];
        chip->timer_b_inc = (chip->reg_timer_b_load[0] && chip->timer_b_subcnt_of) || test;
        int rst_b = chip->timer_b_reset[0] || ic_async;
        chip->timer_b_reset[1] = rst_b;
        chip->timer_b_set = chip->timer_b_of && !rst_b && chip->reg_timer_b_irq[1];
        chip->timer_b_status[1] = chip->timer_b_status[0];

        chip->timer_b_subcnt[1] = chip->timer_b_subcnt[0];
        chip->timer_clk[1] = chip->timer_clk[0];
    }

    if (data_z) {
        chip->st_latch &= ~0xa3;
        chip->st_latch |= chip->timer_a_status[0] << 0;
        chip->st_latch |= chip->timer_b_status[0] << 1;
        chip->st_latch |= chip->st_ch_irq[0] << 5;
        chip->st_latch |= chip->st_busy_en[0] << 7;
    }

    if ((chip->st_ch_irq_l[1] & 16) != 0 && (chip->st_ch_irq_l[0] & 32) == 0) {
        chip->st_latch &= ~0x5c;
        chip->st_latch |= (chip->st_ch_cnt[0] & 7) << 2;
        chip->st_latch |= (chip->st_ch_cnt[0] & 8) << 3;
    }

    if (hclk1) {
        chip->st_test = (chip->reg_test[1] & 64) != 0;
        if (chip->reg_test[1] & 128) {
            chip->st_dbg = (chip->op_value[9] >> 8) & 63;
            if ((chip->reg_a[1] & 2) == 0) {
                chip->st_dbg |= (chip->op_value[9] >> 8) & 192;
            } else {
                if (chip->eg_dbg_shifter[1] & 2048) {
                    chip->st_dbg |= 64;
                }
                if (chip->pg_dbg[1] & 1) {
                    chip->st_dbg |= 128;
                }
            }
        } else {
            chip->st_dbg = chip->op_value[9] & 255;
        }

        int rst = ic_async || (write1_en && chip->reg_write_14[1] && (chip->data_l & 64) != 0);
        chip->st_ch_irq[0] = !rst && (chip->st_ch_irq[1] || chip->eg_int);

        if (chip->st_ch_cnt_rst) {
            chip->st_ch_cnt[0] = 0;
        } else {
            chip->st_ch_cnt[0] = (chip->st_ch_cnt[1] + 1) & 15;
        }

        int busy = chip->st_busy_cnt[1] + chip->st_busy_en[1];
        int busy_of = (busy >> 5) & 1;

        chip->st_busy_cnt[0] = chip->ic_sync ? 0 : busy & 31;

        chip->st_busy_en[0] = (!(busy_of || chip->ic_sync) && chip->st_busy_en[1]) || write1_en;
    }
    if (hclk2) {
        chip->st_ch_irq[1] = chip->st_ch_irq[0];
        chip->st_ch_irq_l[0] = (chip->st_ch_irq_l[1] << 1) | chip->st_ch_irq[0];

        chip->st_ch_cnt[1] = chip->st_ch_cnt[0];

        chip->st_irq = (chip->st_latch & 3) != 0 || chip->st_ch_irq[0];

        chip->st_ch_cnt_rst = chip->fsm_o21;

        chip->st_busy_cnt[1] = chip->st_busy_cnt[0];
        chip->st_busy_en[1] = chip->st_busy_en[0];
    }

    chip->o_data = chip->st_test ? chip->st_dbg : chip->st_latch;
    chip->o_irq_pull = chip->st_irq;

    if (hclk1) {
        chip->unk_write = chip->reg_write_0a[1] && write1_en && (chip->data_l & 128) != 0;
        chip->unk_write_l[1] = chip->unk_write_l[0];
    }
    if (hclk2) {
        chip->unk_write_l[0] = chip->unk_write;
        chip->unk_write_en2[0] = chip->unk_write_en[0];
    }
    if (clk1) {
        if (!chip->unk_write_en2[5] && chip->unk_write_en2[3] && (chip->reg_a[1] & 1) != 0) {
            chip->unk_sh1_data[0] =
                ((uint64_t)chip->reg_b[1] << 1) | (1ULL << 9) |
                ((uint64_t)chip->reg_c[1] << 11) | (1ULL << 19) |
                ((uint64_t)chip->reg_d[1] << 21) | (1ULL << 29) |
                ((uint64_t)chip->reg_e[1] << 31) | (1ULL << 39);
        } else {
            chip->unk_sh1_data[0] = (chip->unk_sh1_data[1] >> 1) | (1ULL << 39);
        }

        chip->unk_write_en[1] = chip->unk_write_en[0];
        chip->unk_write_en2[2] = chip->unk_write_en2[1];
        chip->unk_write_en2[4] = chip->unk_write_en2[3];
    }
    if (clk2) {
        chip->unk_write_en[0] = chip->unk_write && !chip->unk_write_l[1];
        chip->unk_sh1_data[1] = chip->unk_sh1_data[0];
        chip->unk_write_en2[1] = chip->unk_write_en2[0];
        chip->unk_write_en2[3] = chip->unk_write_en2[2];
        chip->unk_write_en2[5] = chip->unk_write_en2[4];
    }

    if (hclk2) {
        int dbg = (chip->reg_a[0] & 4) != 0 ? chip->lfo1.inc[1] : chip->lfo2.inc[1];
        chip->ct_data1 = (chip->reg_test[0] & 8) != 0 ? dbg : chip->reg_ct[0] & 1;
    }

    chip->o_so = dmode ? chip->ac_rbit2[1] || chip->ac_lbit2[1] : chip->ac_so_l[3];
    chip->o_sh1 = dmode ? (chip->unk_sh1_data[0] & 1) : chip->ac_sh1_l[1];
    chip->o_sh2_pull = dmode ? !(chip->ac_sh2_sync[1] & 1) : !chip->ac_sh2_l[1];
    chip->o_ct1 = chip->ct_data1;
    chip->o_ct2 = dmode ? !chip->unk_write_en[1] : (chip->reg_ct[1] >> 1) & 1;
}

