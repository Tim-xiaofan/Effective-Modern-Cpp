#!/usr/bin/python3
import subprocess
import os

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
    for p in programes:
        o = subprocess.check_output(['./test_lookup_table.py', '-p', path + p]).decode('utf-8')
        print(o)

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
