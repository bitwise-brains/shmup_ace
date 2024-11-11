import sys

wave_string = '{ 0, %d, %d, %d, FALSE },'

count = int(input('Count: '))
start = int(input('Start: '))
spacing = int(input('Spacing: '))
path = int(input('PathType: '))

for i in range(0, int(count)):
    print(wave_string % (start, -32, path))
    start = start+spacing
