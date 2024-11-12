UBYTE g_ubWavesInLevel[GAME_STAGES] = {255, 255, 255};
tEnemyWave g_tEnemyWaves[] = {
	{ 0, 3824, -32, 12, FALSE },
	{ 0, 3812, -32, 12, FALSE },
	{ 0, 3800, -32, 12, FALSE },
	{ 0, 3788, -32, 12, FALSE },
	{ 0, 3760, -32, 15, FALSE },
	{ 0, 3748, -32, 15, FALSE },
	{ 0, 3736, -32, 15, FALSE },
	{ 0, 3724, -32, 15, FALSE },
	{ 0, 3696, -32, 11, FALSE },
	{ 0, 3684, -32, 11, FALSE },
	{ 0, 3672, -32, 11, FALSE },
	{ 2, 3660, -32, 11, FALSE },
	{ 0, 3632, -32, 16, FALSE },
	{ 0, 3620, -32, 16, FALSE },
	{ 0, 3608, -32, 16, FALSE },
	{ 2, 3596, -32, 16, FALSE },
	{ 0, 3584, -32, 12, FALSE },
	{ 0, 3572, -32, 12, FALSE },
	{ 0, 3560, -32, 12, FALSE },
	{ 0, 3551, -32, 17, FALSE },
	{ 2, 3548, -32, 12, FALSE },
	{ 0, 3539, -32, 17, FALSE },
	{ 0, 3527, -32, 17, FALSE },
	{ 2, 3515, -32, 17, FALSE },
	{ 0, 3503, -32, 10, FALSE },
	{ 0, 3491, -32, 10, FALSE },
	{ 0, 3479, -32, 10, FALSE },
	{ 0, 3472, -32, 14, FALSE },
	{ 2, 3467, -32, 10, FALSE },
	{ 0, 3460, -32, 14, FALSE },
	{ 0, 3448, -32, 14, FALSE },
	{ 2, 3436, -32, 14, FALSE },
	{ 0, 3424, -32, 12, FALSE },
	{ 0, 3412, -32, 12, FALSE },
	{ 0, 3400, -32, 12, FALSE },
	{ 2, 3388, -32, 12, FALSE },
	{ 3, 3368, -32, 21, FALSE },
	{ 0, 3312, -32, 4, FALSE },
	{ 0, 3296, -32, 4, FALSE },
	{ 0, 3280, -32, 4, FALSE },
	{ 1, 3264, -32, 4, FALSE },
	{ 0, 3248, -32, 5, FALSE },
	{ 0, 3232, -32, 5, FALSE },
	{ 0, 3216, -32, 5, FALSE },
	{ 1, 3200, -32, 5, FALSE },
	{ 1, 3184, -32, 4, FALSE },
	{ 0, 3168, -32, 4, FALSE },
	{ 0, 3152, -32, 4, FALSE },
	{ 0, 3136, -32, 4, FALSE },
	{ 1, 3120, -32, 5, FALSE },
	{ 0, 3104, -32, 5, FALSE },
	{ 0, 3088, -32, 5, FALSE },
	{ 0, 3072, -32, 5, FALSE },
	{ 0, 3056, -32, 2, FALSE },
	{ 0, 3040, -32, 2, FALSE },
	{ 2, 3032, 0, 9, FALSE },
	{ 0, 3024, -32, 2, FALSE },
	{ 1, 3008, -32, 2, FALSE },
	{ 0, 2992, -32, 3, FALSE },
	{ 0, 2976, -32, 3, FALSE },
	{ 2, 2968, 0, 8, FALSE },
	{ 0, 2960, -32, 3, FALSE },
	{ 1, 2944, -32, 3, FALSE },
	{ 1, 2928, -32, 2, FALSE },
	{ 0, 2920, -32, 2, FALSE },
	{ 0, 2912, -32, 2, FALSE },
	{ 1, 2904, -32, 2, FALSE },
	{ 1, 2880, -32, 3, FALSE },
	{ 0, 2872, -32, 3, FALSE },
	{ 0, 2864, -32, 3, FALSE },
	{ 1, 2856, -32, 3, FALSE },
	{ 1, 2832, -32, 18, FALSE },
	{ 0, 2824, -32, 18, FALSE },
	{ 0, 2816, -32, 18, FALSE },
	{ 1, 2808, -32, 18, FALSE },
	{ 1, 2784, -32, 19, FALSE },
	{ 0, 2776, -32, 19, FALSE },
	{ 4, 2768, 0, 20, FALSE },
	{ 1, 2760, -32, 19, FALSE },
	{ 1, 2736, -32, 4, FALSE },
	{ 0, 2728, -32, 4, FALSE },
	{ 4, 2720, 0, 21, FALSE },
	{ 1, 2712, -32, 4, FALSE },
	{ 1, 2672, -32, 5, FALSE },
	{ 4, 2664, 0, 20, FALSE },
	{ 1, 2656, -32, 5, FALSE },
	{ 1, 2648, -32, 5, FALSE },
	{ 1, 2624, -32, 6, FALSE },
	{ 4, 2616, 0, 21, FALSE },
	{ 1, 2608, -32, 6, FALSE },
	{ 1, 2600, -32, 6, FALSE },
	{ 1, 2576, -32, 7, FALSE },
	{ 4, 2568, 0, 20, FALSE },
	{ 1, 2560, -32, 7, FALSE },
	{ 1, 2552, -32, 7, FALSE },
	{ 3, 2520, -32, 20, FALSE },
	{ 0, 2464, -32, 2, FALSE },
	{ 0, 2463, -32, 3, FALSE },
	{ 0, 2448, -32, 2, FALSE },
	{ 0, 2447, -32, 3, FALSE },
	{ 0, 2432, -32, 2, FALSE },
	{ 0, 2431, -32, 3, FALSE },
	{ 0, 2416, -32, 2, FALSE },
	{ 0, 2415, -32, 3, FALSE },
	{ 0, 2401, -32, 2, FALSE },
	{ 0, 2400, -32, 3, FALSE },
	{ 0, 2385, -32, 2, FALSE },
	{ 0, 2384, -32, 3, FALSE },
	{ 0, 2369, -32, 2, FALSE },
	{ 0, 2368, -32, 3, FALSE },
	{ 0, 2353, -32, 2, FALSE },
	{ 0, 2352, -32, 3, FALSE },
	{ 2, 2288, 0, 8, FALSE },
	{ 2, 2286, 0, 9, FALSE },
	{ 4, 2266, 0, 17, FALSE },
	{ 4, 2264, 0, 10, FALSE },
	{ 2, 2241, 0, 8, FALSE },
	{ 2, 2238, 0, 9, FALSE },
	{ 4, 2218, 0, 16, FALSE },
	{ 4, 2216, 0, 11, FALSE },
	{ 2, 2193, 0, 8, FALSE },
	{ 2, 2191, 0, 9, FALSE },
	{ 4, 2170, 0, 15, FALSE },
	{ 4, 2168, 0, 12, FALSE },
	{ 7, 2152, -32, 7, FALSE },
	{ 1, 2112, 0, 0, FALSE },
	{ 1, 2104, 0, 0, FALSE },
	{ 1, 2096, 0, 0, FALSE },
	{ 1, 2088, 0, 0, FALSE },
	{ 1, 2048, 0, 1, FALSE },
	{ 1, 2040, 0, 1, FALSE },
	{ 1, 2032, 0, 1, FALSE },
	{ 1, 2024, 0, 1, FALSE },
	{ 1, 1984, 0, 0, FALSE },
	{ 1, 1976, 0, 0, FALSE },
	{ 1, 1968, 0, 0, FALSE },
	{ 1, 1960, 0, 0, FALSE },
	{ 1, 1920, 0, 1, FALSE },
	{ 1, 1912, 0, 1, FALSE },
	{ 1, 1904, 0, 1, FALSE },
	{ 1, 1896, 0, 1, FALSE },
	{ 3, 1848, -32, 21, FALSE },
	{ 1, 1840, -32, 10, FALSE },
	{ 0, 1824, -32, 11, FALSE },
	{ 0, 1808, -32, 12, FALSE },
	{ 0, 1792, -32, 13, FALSE },
	{ 1, 1776, -32, 17, FALSE },
	{ 0, 1760, -32, 16, FALSE },
	{ 0, 1744, -32, 15, FALSE },
	{ 0, 1728, -32, 14, FALSE },
	{ 1, 1712, -32, 13, FALSE },
	{ 0, 1696, -32, 12, FALSE },
	{ 1, 1680, -32, 11, FALSE },
	{ 0, 1664, -32, 10, FALSE },
	{ 1, 1648, -32, 14, FALSE },
	{ 0, 1632, -32, 15, FALSE },
	{ 1, 1616, -32, 16, FALSE },
	{ 0, 1600, -32, 17, FALSE },
	{ 3, 1584, -32, 20, FALSE },
	{ 1, 1521, -32, 19, FALSE },
	{ 1, 1520, -32, 18, FALSE },
	{ 1, 1489, -32, 19, FALSE },
	{ 1, 1488, -32, 18, FALSE },
	{ 1, 1441, -32, 19, FALSE },
	{ 1, 1440, -32, 18, FALSE },
	{ 2, 1409, -32, 19, FALSE },
	{ 2, 1408, -32, 18, FALSE },
	{ 1, 1361, -32, 19, FALSE },
	{ 1, 1360, -32, 18, FALSE },
	{ 1, 1329, -32, 19, FALSE },
	{ 1, 1328, -32, 18, FALSE },
	{ 1, 1281, -32, 19, FALSE },
	{ 1, 1280, -32, 18, FALSE },
	{ 2, 1249, -32, 19, FALSE },
	{ 2, 1248, -32, 18, FALSE },
	{ 1, 1201, -32, 21, FALSE },
	{ 1, 1200, -32, 20, FALSE },
	{ 1, 1185, -32, 21, FALSE },
	{ 1, 1184, -32, 20, FALSE },
	{ 4, 1160, 0, 17, FALSE },
	{ 4, 1158, 0, 10, FALSE },
	{ 1, 1137, -32, 21, FALSE },
	{ 1, 1136, -32, 20, FALSE },
	{ 1, 1121, -32, 21, FALSE },
	{ 1, 1120, -32, 20, FALSE },
	{ 4, 1096, 0, 16, FALSE },
	{ 4, 1094, 0, 11, FALSE },
	{ 1, 1073, -32, 21, FALSE },
	{ 1, 1072, -32, 20, FALSE },
	{ 1, 1057, -32, 21, FALSE },
	{ 1, 1056, -32, 20, FALSE },
	{ 4, 1034, 0, 15, FALSE },
	{ 4, 1032, 0, 12, FALSE },
	{ 1, 1009, -32, 21, FALSE },
	{ 1, 1008, -32, 20, FALSE },
	{ 1, 993, -32, 21, FALSE },
	{ 1, 992, -32, 20, FALSE },
	{ 0, 912, -32, 2, FALSE },
	{ 0, 911, -32, 3, FALSE },
	{ 0, 896, -32, 2, FALSE },
	{ 0, 895, -32, 3, FALSE },
	{ 0, 880, -32, 2, FALSE },
	{ 0, 879, -32, 3, FALSE },
	{ 0, 864, -32, 2, FALSE },
	{ 0, 863, -32, 3, FALSE },
	{ 0, 849, -32, 2, FALSE },
	{ 0, 848, -32, 3, FALSE },
	{ 0, 833, -32, 2, FALSE },
	{ 0, 832, -32, 3, FALSE },
	{ 0, 817, -32, 2, FALSE },
	{ 0, 816, -32, 3, FALSE },
	{ 0, 801, -32, 2, FALSE },
	{ 0, 800, -32, 3, FALSE },
	{ 3, 752, -32, 7, FALSE },
	{ 7, 736, -32, 6, FALSE },
	{ 0, 688, 0, 8, FALSE },
	{ 0, 672, 0, 8, FALSE },
	{ 4, 664, 0, 17, FALSE },
	{ 1, 656, 0, 8, FALSE },
	{ 0, 640, 0, 8, FALSE },
	{ 0, 625, 0, 8, FALSE },
	{ 2, 609, 0, 8, FALSE },
	{ 4, 600, 0, 17, FALSE },
	{ 0, 593, 0, 8, FALSE },
	{ 0, 577, 0, 8, FALSE },
	{ 0, 560, 16, 9, FALSE },
	{ 0, 544, 16, 9, FALSE },
	{ 4, 536, 0, 12, FALSE },
	{ 1, 528, 16, 9, FALSE },
	{ 0, 512, 16, 9, FALSE },
	{ 0, 497, 16, 9, FALSE },
	{ 2, 481, 16, 9, FALSE },
	{ 4, 472, 0, 12, FALSE },
	{ 0, 465, 16, 9, FALSE },
	{ 0, 449, 16, 9, FALSE },
	{ 1, 432, 16, 8, FALSE },
	{ 0, 416, 16, 8, FALSE },
	{ 1, 400, 16, 8, FALSE },
	{ 4, 392, 0, 17, FALSE },
	{ 0, 384, 16, 8, FALSE },
	{ 0, 369, 16, 8, FALSE },
	{ 2, 353, 16, 8, FALSE },
	{ 4, 344, 0, 17, FALSE },
	{ 0, 337, 16, 8, FALSE },
	{ 2, 320, 16, 8, FALSE },
	{ 0, 304, 16, 9, FALSE },
	{ 0, 288, 16, 9, FALSE },
	{ 4, 280, 0, 12, FALSE },
	{ 1, 272, 16, 9, FALSE },
	{ 0, 256, 16, 9, FALSE },
	{ 0, 241, 16, 9, FALSE },
	{ 2, 225, 16, 9, FALSE },
	{ 4, 216, 0, 12, FALSE },
	{ 0, 209, 16, 9, FALSE },
	{ 0, 193, 16, 9, FALSE }
};
