import json
import glob

idx = 0
offset = 0
offsets = []
lengths = []

jsonfiles = glob.glob('*.json')

for jsonfilename in jsonfiles:
    print('[%d] %s' % (idx, jsonfilename))
    jsonfile = open(jsonfilename, 'r')
    jsondata = json.load(jsonfile)
    line = '['
    for point in jsondata['originalPoints']:
        line += '%s,%s,' % (point['x'], point['y'])

    line = line[:-1]
    line += ']'
    print(line)
    idx += 1