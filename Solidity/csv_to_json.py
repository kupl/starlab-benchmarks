import argparse
import csv
import os
from os.path import expanduser
import json

HOME = expanduser("~")
soldir = os.path.join(HOME, 'starlab-benchmarks/Solidity/meta.csv')

def main():
    parser = argparse.ArgumentParser(description='Automatically generate bug data in json format from csv data. Just run this script without arguments')
    args = parser.parse_args()

    home = expanduser('~')
    soldir = os.path.join(home, 'starlab-benchmarks/Solidity')

    with open (os.path.join(soldir,'meta.csv'), 'r') as fp: rows = list(csv.DictReader(fp))

    cnt = 0
    for r in rows:
        fid = r['ID']
        assert(os.system(f'mkdir {soldir}/bugs/{fid}') == 0)
        lines = r['LINE'].split('.')
        for (i,line) in enumerate(lines):
            res = {
                'err_type': 'IO',
                'source': None,
                'sink': {'filepath': f'files/{fid}.sol', 'line': line}
            }
            with open(f'{soldir}/bugs/{fid}/bug_{i+1}.json', 'w') as fp: json.dump(res, fp, indent = 4)
            cnt+=1

    print('script successfully terminated.')
    print('#Bugs: ' + str(cnt))

if __name__ == "__main__":
    main()
