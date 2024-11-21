import sys
import json
import glob
import pprint

waves_in_level = 'tEnemyWave g_tEnemyWavesForStage%d[] = {\n%s\n};'
wave_string = '\t{ %d, %d, %d, %d },\n'
compiled_wave_string = ''

enemy_waves = {}

jsonfiles = glob.glob('*.json')

for jsonfilename in jsonfiles:
    jsonfile = open(jsonfilename, 'r')
    jsondata = json.load(jsonfile)
    if jsonfilename not in enemy_waves:
        enemy_waves[jsonfilename] = {}
    layers = jsondata['layers']
    for layer in layers:
        if layer['name'] != 'Waves':
            continue

        waves = layer['objects']
        for wave in waves:
            yPos = int(wave['y'])
            xPos = int(wave['x'])
            enemyType = wave['properties'][0]['value']
            pathType = wave['properties'][1]['value']
            yOffset = wave['properties'][2]['value']

            if enemyType == 13:
                yPos += 32
                pathType = xPos

            if yPos not in waves:
                enemy_waves[jsonfilename][yPos] = { 'enemyType': enemyType, 'pathType': pathType, 'yOffset': yOffset }
            else:
                print('Overlapping objects: %d', yPos);
                sys.exit(1)

for stage in enemy_waves:
    print(stage)

'''
for idx in sorted(enemy_waves, reverse=True):
    compiled_wave_string += (wave_string % (enemy_waves[idx]['enemyType'], idx, enemy_waves[idx]['yOffset'], enemy_waves[idx]['pathType']))

compiled_wave_string = compiled_wave_string[:-2]
print(waves_in_level % compiled_wave_string)
'''