#include <string.h>
#include <stdlib.h>
#include "opz_lle.h"


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

static void LFO_Clock(ym2141_t* chip, int hclk1, int hclk2, int ic_async, ym2414_lfo_t* lfo, int which) {
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

        chip->lfo_sum_c_in = lfo->sum_c_out && (chip->lfo_sync[1] & 4) == 0;

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
        int s = whichwave == 2 ? chip->lfo_sign_trig : lfo->sign_saw;
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
            if (pm_load) {
                lfo->am = (lfo->out_shifter[1] >> 8) & 255;
            }
        }

        lfo->sign_saw_l = lfo->sign_saw;

        lfo->bb = lfo_sel ? lfo->sign_saw : (!lfo->sign_trig || whichwave != 2);

        lfo->pmd_zero = whichpmd == 0;
        lfo->pm_out = lfo->pmd_zero ? 0 : lfo->pm;
    }

}

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
        chip->reg_ch20_l2[0] = (chip->reg_ch_bus >> 16) & 255;

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


        chip->reg_30_0_l[0] = (chip->reg_30_0_l[1] << 1) | (chip->reg_ch30new_l[1] & 1);
        chip->reg_30_1_l[0] = (chip->reg_ch_bus >> 45) & 1;
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
        chip->lfo1.cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data1 & 16) != 0);
        chip->lfo2.cnt3_sync[0] = chip->ic_sync || (wr1b && (chip->data1 & 32) != 0);
        chip->lfo1.freq_write[0] = write1_en && chip->reg_write_18[1];
        chip->lfo2.freq_write[0] = write1_en && chip->reg_write_16[1];

        chip->lfo_pmsel[0] = (chip->reg_ch38new_l[1] >> 2) & 1;
    }
    if (hclk2) {
        chip->lfo_sync[0] = (chip->lfo_sync[1] << 1) | chip->fsm_o16;
        chip->lfo_sync2[1] = chip->lfo_sync2[0];
        chip->lfo_subcnt[1] = chip->lfo_subcnt[0];
        chip->lfo_subcnt_of[1] = chip->lfo_subcnt_of[0];
        chip->lfo_bcnt_rst = chip->fsm_o16 && chip->lfo_subcnt[0] == 2;

        chip->lfo2.cnt3_sync[1] = chip->lfo1.cnt3_sync[0];

        chip->lfo_pmsel[1] = chip->lfo_pmsel[0];
    }

    LFO_Clock(chip, hclk1, hclk2, ic_async, &chip->lfo1, 0);
    LFO_Clock(chip, hclk1, hclk2, ic_async, &chip->lfo2, 1);

    if (hclk1) {
        chip->freq_kc[0] = (chip->reg_ch_bus >> 24) & 127;
        chip->freq_kc[2] = chip->freq_kc[1];
        chip->freq_kf[0] = chip->reg_ch30_l[1];
        chip->freq_kf[2] = chip->freq_kf[1];
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
        chip->freq_freq_frac0 = chip->freq_fre_frac[3] & 1;


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
            if ((chip->freq_rom_slope & 1) != 0 (chip->freq_freq_frac[3] & 12) == 12) {
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
        chip->freq_kc_lfo_sumlof[0] = (suml >> 6) & 1;
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

        chip->freq_rom_base_h = chip->freq_rom_base[0] >> 5;

        int freq_sum_l = (chip->freq_rom_base[0] & 31) + chip->freq_lerp + chip->freq_freq_frac0;

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

        chip->pg_reset[0] = (chip->pg_reset[1] << 1) | x;

        chip->pg_add[0] = (chip->pg_reset[1] & 1) != 0 ? 0 : (chip->pg_add1[3] + chip->pg_add2[3]) & 0xfffff;

        chip->pg_phase_l[0] = chip->pg_bus[3] ^ 0xfffff;

        int phase_sum = ((chip->reg_test[1] & 8) != 0 || (chip->pg_reset & 2) != 0) ? 0 : chip->pg_phase_l[1];
        phase_sum = (phase_sum + chip->pg_add[1]) & 0xfffff;

        chip->pg_phase_l2[0] = phase_sum;
        chip->pg_phase_l2[2] = chip->pg_phase_l2[1];
        chip->pg_phase_l2[4] = chip->pg_phase_l2[3];

        chip->pg_cell_sel[0] = (chip->pg_cell_sel[1] << 1) | chip->fsm_o9[1];

        int i;
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
                chip->pg_cells[3][i] = chip->pg_bus[1];
            }
        }

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

        chip->pg_freq = (fnum << chip->pg_shift[2]) >> 2;

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
}

