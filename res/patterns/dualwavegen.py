import sys

wave_string = '{ 0, %d, %d, %d, FALSE },'

count = int(input('Count: '))
start = int(input('Start: '))
path = int(input('PathType: '))
secondpath = int(input('PathType2: '))
spacing = int(input('Spacing: '))
secondspacing = int(input('Spacing2: '))

spacing = abs(spacing) * -1
secondspacing = abs(secondspacing) * -1

if (spacing == secondspacing):
    print('Spacing value are equal')
    sys.exit(1)

for i in range(0, int(count)):
    print(wave_string % (start, -32, path))
    print(wave_string % (start+secondspacing, -32, secondpath))

    start = start+spacing
