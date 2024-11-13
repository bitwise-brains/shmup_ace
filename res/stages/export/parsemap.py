import sys
import json
import glob

num_waves_in_level = 'UWORD g_uwWavesInLevel[GAME_STAGES] = {%d, %d, %d};'
waves_in_level = 'tEnemyWave g_tEnemyWaves[] = {\n%s\n};'
wave_string = '\t{ %d, %d, %d, %d, FALSE },\n'
compiled_wave_string = ''

enemy_waves = {}
total_waves = 0


jsonfiles = glob.glob('*.json')

for jsonfilename in jsonfiles:
    jsonfile = open(jsonfilename, 'r')
    jsondata = json.load(jsonfile)
    layers = jsondata['layers']
    for layer in layers:
        if layer['name'] != 'Waves':
            continue

        waves = layer['objects']
        for wave in waves:
            yPos = int(wave['y'])
            if yPos > 4000:
                continue
            enemyType = wave['properties'][0]['value']
            pathType = wave['properties'][1]['value']
            yOffset = wave['properties'][2]['value']

            if yPos not in waves:
                enemy_waves[yPos] = { 'enemyType': enemyType, 'pathType': pathType, 'yOffset': yOffset }
                total_waves += 1
            else:
                print('Overlapping objects: %d', yPos);
                sys.exit(1)

    for idx in sorted(enemy_waves, reverse=True):
        compiled_wave_string += (wave_string % (enemy_waves[idx]['enemyType'], idx, enemy_waves[idx]['yOffset'], enemy_waves[idx]['pathType']))

    compiled_wave_string = compiled_wave_string[:-2]
    print(num_waves_in_level % (total_waves, total_waves, total_waves))
    print(waves_in_level % compiled_wave_string)