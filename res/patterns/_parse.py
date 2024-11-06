import json
import glob

idx = 0
offset = 0
offsets = []
lengths = []
paths = 'UBYTE g_ubPathData[] = {'

jsonfiles = glob.glob('*.json')

for jsonfilename in jsonfiles:
    print('[%d] %s' % (idx, jsonfilename))
    jsonfile = open(jsonfilename, 'r')
    jsondata = json.load(jsonfile)
    offsets.append(offset)
    lengths.append(jsondata['length'])
    path = ''
    if (offset != 0):
        path += '                        '

    path += jsondata['path'].replace('{', '').replace('};', ',')
    paths += (path)
    paths += '\n'
    offset += jsondata['length']
    idx += 1

paths = paths[:-2]
paths += '};'
print('\n')
print('UWORD g_uwPathOffset[] = ' + str(offsets).replace('[', '{').replace(']', '};'))
print('UWORD g_uwPathLength[] = ' + str(lengths).replace('[', '{').replace(']', '};'))
print(paths)