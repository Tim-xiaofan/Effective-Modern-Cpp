#!/usr/bin/python3
import subprocess
import os
import argparse

modes = ["random-shuffle-inserts",\
        "random-full-inserts",\
        "random-full-reserve-inserts",\
        "random-full-delete",\
        "random-full-read",\
        "random-full-read-miss",\
        "random-full-read-half",\
        "random-full-iteration"
]

programes = ["test_lookup_table",\
            "test_threadsafe_lookup_table1",\
            "test_threadsafe_lookup_table2"]

path = './concurrency/data_struct/'
    
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-r', '--regenerate',\
                action='store_true',\
                help='regenerate csv files')
    args = parser.parse_args();
    if(args.regenerate):
        print("regenerate csv files first")
        for p in programes:
            o = subprocess.check_output(\
                        ['./test_lookup_table.py', '-p', path + p])\
                            .decode('utf-8')
            print(o)
    else:
        print("generate images from existing csv files")

    for m in modes:
        args = ['./script.py', '-f']
        for p in programes:
            args.append(p + '-' + m + '.csv')
        args.append('-o')
        args.append(m + '.png')
        print(' '.join(args))
        subprocess.check_output(args)
    pass

if __name__ == '__main__':
    main()
