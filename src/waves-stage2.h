/*
UWORD g_uwWavesInLevel[GAME_STAGES] = {263, 263, 263};
tEnemyWave g_tEnemyWaves[] = {
	{ 0, 3808, -32, 2, FALSE },
	{ 0, 3807, -32, 3, FALSE },
	{ 1, 3792, -32, 2, FALSE },
	{ 1, 3791, -32, 3, FALSE },
	{ 0, 3776, -32, 6, FALSE },
	{ 0, 3775, -32, 7, FALSE },
	{ 0, 3760, -32, 6, FALSE },
	{ 0, 3759, -32, 7, FALSE },
	{ 1, 3745, -32, 6, FALSE },
	{ 1, 3744, -32, 7, FALSE },
	{ 1, 3729, -32, 6, FALSE },
	{ 1, 3728, -32, 7, FALSE },
	{ 0, 3713, -32, 2, FALSE },
	{ 0, 3712, -32, 3, FALSE },
	{ 0, 3697, -32, 2, FALSE },
	{ 0, 3696, -32, 3, FALSE },
	{ 2, 3632, 32, 8, FALSE },
	{ 2, 3630, 32, 9, FALSE },
	{ 5, 3610, 32, 17, FALSE },
	{ 5, 3608, 32, 10, FALSE },
	{ 2, 3585, 32, 8, FALSE },
	{ 2, 3582, 32, 9, FALSE },
	{ 5, 3562, 32, 16, FALSE },
	{ 5, 3560, 32, 11, FALSE },
	{ 2, 3537, 32, 8, FALSE },
	{ 2, 3535, 32, 9, FALSE },
	{ 5, 3514, 32, 15, FALSE },
	{ 5, 3512, 32, 12, FALSE },
	{ 3, 3480, 16, 21, FALSE },
	{ 4, 3464, 0, 20, FALSE },
	{ 4, 3448, 0, 20, FALSE },
	{ 1, 3440, -16, 19, FALSE },
	{ 4, 3433, 0, 20, FALSE },
	{ 0, 3432, -16, 19, FALSE },
	{ 0, 3424, -16, 19, FALSE },
	{ 1, 3416, -16, 19, FALSE },
	{ 1, 3401, -16, 4, FALSE },
	{ 0, 3393, -16, 4, FALSE },
	{ 0, 3385, -16, 4, FALSE },
	{ 1, 3377, -16, 4, FALSE },
	{ 4, 3376, 0, 21, FALSE },
	{ 4, 3329, 0, 20, FALSE },
	{ 1, 3328, -32, 5, FALSE },
	{ 1, 3320, -32, 5, FALSE },
	{ 1, 3312, -32, 5, FALSE },
	{ 1, 3304, -32, 5, FALSE },
	{ 1, 3289, -32, 6, FALSE },
	{ 1, 3281, -32, 6, FALSE },
	{ 1, 3273, -32, 6, FALSE },
	{ 4, 3272, 0, 21, FALSE },
	{ 1, 3265, -32, 6, FALSE },
	{ 4, 3233, 0, 20, FALSE },
	{ 1, 3232, -32, 7, FALSE },
	{ 1, 3224, -32, 7, FALSE },
	{ 1, 3216, -32, 7, FALSE },
	{ 1, 3208, -32, 7, FALSE },
	{ 11, 3192, 16, 8, FALSE },
	{ 1, 3153, -32, 7, FALSE },
	{ 1, 3152, -32, 6, FALSE },
	{ 1, 3137, -32, 7, FALSE },
	{ 1, 3136, -32, 6, FALSE },
	{ 4, 3112, 0, 9, FALSE },
	{ 4, 3110, 0, 8, FALSE },
	{ 1, 3073, -32, 7, FALSE },
	{ 1, 3072, -32, 6, FALSE },
	{ 1, 3057, -32, 7, FALSE },
	{ 1, 3056, -32, 6, FALSE },
	{ 4, 3032, 0, 9, FALSE },
	{ 4, 3030, 0, 8, FALSE },
	{ 1, 2993, -32, 7, FALSE },
	{ 1, 2992, -32, 6, FALSE },
	{ 1, 2977, -32, 7, FALSE },
	{ 1, 2976, -32, 6, FALSE },
	{ 4, 2954, 0, 9, FALSE },
	{ 4, 2952, 0, 8, FALSE },
	{ 1, 2913, -32, 7, FALSE },
	{ 1, 2912, -32, 6, FALSE },
	{ 1, 2897, -32, 7, FALSE },
	{ 1, 2896, -32, 6, FALSE },
	{ 4, 2874, 0, 9, FALSE },
	{ 4, 2872, 0, 8, FALSE },
	{ 1, 2816, -48, 16, FALSE },
	{ 1, 2809, -48, 13, FALSE },
	{ 1, 2800, -48, 17, FALSE },
	{ 1, 2793, -48, 11, FALSE },
	{ 1, 2784, -48, 15, FALSE },
	{ 1, 2777, -48, 12, FALSE },
	{ 1, 2768, -48, 17, FALSE },
	{ 1, 2761, -48, 10, FALSE },
	{ 3, 2728, -48, 21, FALSE },
	{ 1, 2713, -48, 15, FALSE },
	{ 1, 2697, -48, 17, FALSE },
	{ 1, 2681, -48, 16, FALSE },
	{ 1, 2665, -48, 12, FALSE },
	{ 2, 2608, 0, 8, FALSE },
	{ 2, 2606, 0, 9, FALSE },
	{ 4, 2586, 0, 17, FALSE },
	{ 4, 2584, 0, 10, FALSE },
	{ 2, 2561, 0, 8, FALSE },
	{ 2, 2558, 0, 9, FALSE },
	{ 4, 2538, 0, 16, FALSE },
	{ 4, 2536, 0, 11, FALSE },
	{ 2, 2513, 0, 8, FALSE },
	{ 2, 2511, 0, 9, FALSE },
	{ 4, 2490, 0, 15, FALSE },
	{ 4, 2488, 0, 12, FALSE },
	{ 7, 2472, -32, 7, FALSE },
	{ 1, 2432, 0, 0, FALSE },
	{ 1, 2424, 0, 0, FALSE },
	{ 1, 2416, 0, 0, FALSE },
	{ 1, 2408, 0, 0, FALSE },
	{ 1, 2368, 0, 1, FALSE },
	{ 1, 2360, 0, 1, FALSE },
	{ 1, 2352, 0, 1, FALSE },
	{ 1, 2344, 0, 1, FALSE },
	{ 1, 2304, 0, 0, FALSE },
	{ 1, 2296, 0, 0, FALSE },
	{ 1, 2288, 0, 0, FALSE },
	{ 1, 2280, 0, 0, FALSE },
	{ 1, 2240, 0, 1, FALSE },
	{ 1, 2232, 0, 1, FALSE },
	{ 1, 2224, 0, 1, FALSE },
	{ 1, 2216, 0, 1, FALSE },
	{ 3, 2168, -32, 21, FALSE },
	{ 1, 2160, -32, 10, FALSE },
	{ 0, 2144, -32, 11, FALSE },
	{ 0, 2128, -32, 12, FALSE },
	{ 0, 2112, -32, 13, FALSE },
	{ 1, 2096, -32, 17, FALSE },
	{ 0, 2080, -32, 16, FALSE },
	{ 0, 2064, -32, 15, FALSE },
	{ 0, 2048, -32, 14, FALSE },
	{ 1, 2032, -32, 13, FALSE },
	{ 0, 2016, -32, 12, FALSE },
	{ 1, 2000, -32, 11, FALSE },
	{ 0, 1984, -32, 10, FALSE },
	{ 1, 1968, -32, 14, FALSE },
	{ 0, 1952, -32, 15, FALSE },
	{ 1, 1936, -32, 16, FALSE },
	{ 0, 1920, -32, 17, FALSE },
	{ 3, 1904, -32, 20, FALSE },
	{ 0, 1856, -32, 2, FALSE },
	{ 0, 1855, -32, 3, FALSE },
	{ 0, 1840, -32, 2, FALSE },
	{ 0, 1839, -32, 3, FALSE },
	{ 0, 1824, -32, 2, FALSE },
	{ 0, 1823, -32, 3, FALSE },
	{ 0, 1808, -32, 2, FALSE },
	{ 0, 1807, -32, 3, FALSE },
	{ 0, 1793, -32, 2, FALSE },
	{ 0, 1792, -32, 3, FALSE },
	{ 0, 1777, -32, 2, FALSE },
	{ 0, 1776, -32, 3, FALSE },
	{ 0, 1761, -32, 2, FALSE },
	{ 0, 1760, -32, 3, FALSE },
	{ 0, 1745, -32, 2, FALSE },
	{ 0, 1744, -32, 3, FALSE },
	{ 3, 1696, -32, 7, FALSE },
	{ 7, 1680, -32, 6, FALSE },
	{ 0, 1632, 0, 8, FALSE },
	{ 0, 1616, 0, 8, FALSE },
	{ 4, 1608, 0, 17, FALSE },
	{ 1, 1600, 0, 8, FALSE },
	{ 0, 1584, 0, 8, FALSE },
	{ 0, 1569, 0, 8, FALSE },
	{ 2, 1553, 0, 8, FALSE },
	{ 4, 1544, 0, 17, FALSE },
	{ 0, 1537, 0, 8, FALSE },
	{ 0, 1521, 0, 8, FALSE },
	{ 0, 1504, 16, 9, FALSE },
	{ 0, 1488, 16, 9, FALSE },
	{ 4, 1480, 0, 12, FALSE },
	{ 1, 1472, 16, 9, FALSE },
	{ 0, 1456, 16, 9, FALSE },
	{ 0, 1441, 16, 9, FALSE },
	{ 2, 1425, 16, 9, FALSE },
	{ 4, 1416, 0, 12, FALSE },
	{ 0, 1409, 16, 9, FALSE },
	{ 0, 1393, 16, 9, FALSE },
	{ 1, 1376, 16, 8, FALSE },
	{ 0, 1360, 16, 8, FALSE },
	{ 1, 1344, 16, 8, FALSE },
	{ 4, 1336, 0, 17, FALSE },
	{ 0, 1328, 16, 8, FALSE },
	{ 0, 1313, 16, 8, FALSE },
	{ 2, 1297, 16, 8, FALSE },
	{ 4, 1288, 0, 17, FALSE },
	{ 0, 1281, 16, 8, FALSE },
	{ 2, 1264, 16, 8, FALSE },
	{ 0, 1248, 16, 9, FALSE },
	{ 0, 1232, 16, 9, FALSE },
	{ 4, 1224, 0, 12, FALSE },
	{ 1, 1216, 16, 9, FALSE },
	{ 0, 1200, 16, 9, FALSE },
	{ 0, 1185, 16, 9, FALSE },
	{ 2, 1169, 16, 9, FALSE },
	{ 4, 1160, 0, 12, FALSE },
	{ 0, 1153, 16, 9, FALSE },
	{ 0, 1137, 16, 9, FALSE },
	{ 12, 1104, -32, 13, FALSE },
	{ 0, 1056, -16, 4, FALSE },
	{ 2, 1040, -16, 4, FALSE },
	{ 0, 1024, -16, 4, FALSE },
	{ 2, 1007, -16, 4, FALSE },
	{ 1, 991, -16, 6, FALSE },
	{ 0, 975, -16, 6, FALSE },
	{ 1, 959, -16, 6, FALSE },
	{ 0, 943, -16, 6, FALSE },
	{ 0, 927, -16, 2, FALSE },
	{ 2, 911, -16, 2, FALSE },
	{ 0, 895, -16, 2, FALSE },
	{ 2, 878, -16, 2, FALSE },
	{ 0, 864, -16, 7, FALSE },
	{ 2, 848, -16, 7, FALSE },
	{ 0, 832, -16, 7, FALSE },
	{ 2, 815, -16, 7, FALSE },
	{ 1, 799, -16, 5, FALSE },
	{ 0, 783, -16, 5, FALSE },
	{ 1, 767, -16, 5, FALSE },
	{ 0, 751, -16, 5, FALSE },
	{ 0, 735, -16, 3, FALSE },
	{ 2, 719, -16, 3, FALSE },
	{ 0, 703, -16, 3, FALSE },
	{ 2, 686, -16, 3, FALSE },
	{ 11, 664, 16, 8, FALSE },
	{ 0, 624, -16, 4, FALSE },
	{ 2, 608, -16, 4, FALSE },
	{ 0, 592, -16, 4, FALSE },
	{ 2, 575, -16, 4, FALSE },
	{ 1, 559, -16, 6, FALSE },
	{ 0, 543, -16, 6, FALSE },
	{ 6, 536, 16, 1, FALSE },
	{ 1, 527, -16, 6, FALSE },
	{ 0, 511, -16, 6, FALSE },
	{ 0, 495, -16, 2, FALSE },
	{ 2, 479, -16, 2, FALSE },
	{ 6, 472, 16, 1, FALSE },
	{ 0, 463, -16, 2, FALSE },
	{ 2, 446, -16, 2, FALSE },
	{ 0, 432, -16, 7, FALSE },
	{ 2, 416, -16, 7, FALSE },
	{ 0, 400, -16, 7, FALSE },
	{ 2, 383, -16, 7, FALSE },
	{ 1, 367, -16, 5, FALSE },
	{ 0, 351, -16, 5, FALSE },
	{ 6, 344, 16, 0, FALSE },
	{ 1, 335, -16, 5, FALSE },
	{ 0, 319, -16, 5, FALSE },
	{ 0, 303, -16, 3, FALSE },
	{ 2, 287, -16, 3, FALSE },
	{ 6, 280, 16, 0, FALSE },
	{ 0, 271, -16, 3, FALSE },
	{ 2, 254, -16, 3, FALSE },
	{ 2, 224, 16, 0, FALSE },
	{ 2, 208, 16, 1, FALSE },
	{ 2, 192, 16, 2, FALSE },
	{ 2, 176, 16, 3, FALSE },
	{ 2, 160, 16, 4, FALSE },
	{ 2, 144, 16, 5, FALSE },
	{ 2, 128, 16, 0, FALSE }
};
*/