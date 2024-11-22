import sys
import json
import glob
import pprint

waves_in_level = 'tEnemyWave g_tEnemyWavesForStage%d[] = {\n%s\n};\n'
wave_string = '\t{ %d, %d, %d, %d },\n'
compiled_wave_string = ''
all_waves = ''

enemy_waves = {}

jsonfilenames = glob.glob('*.json')

for jsonfilename in jsonfilenames:
    compiled_wave_string = ''
    jsonfile = open(jsonfilename, 'r')
    jsondata = json.load(jsonfile)
    stage_number = int(jsonfilename.replace('.json', ''))
    if stage_number not in enemy_waves:
        enemy_waves[stage_number] = {}

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
                enemy_waves[stage_number][yPos] = { 'enemyType': enemyType, 'pathType': pathType, 'yOffset': yOffset }
            else:
                print('Overlapping objects: %d', yPos);
                sys.exit(1)

    for idx in sorted(enemy_waves[stage_number], reverse=True):
        compiled_wave_string += (wave_string % (enemy_waves[stage_number][idx]['enemyType'], idx, enemy_waves[stage_number][idx]['yOffset'], enemy_waves[stage_number][idx]['pathType']))

    compiled_wave_string = compiled_wave_string[:-2]
    print(waves_in_level % (stage_number, compiled_wave_string))
