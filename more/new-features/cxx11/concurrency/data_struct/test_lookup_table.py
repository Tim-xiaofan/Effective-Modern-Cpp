#!/usr/bin/python3
import subprocess
import os
import argparse

KB = 2 ** 10
MB = 2 ** 20

modes = ["random-shuffle-inserts",\
        "random-full-inserts",\
        "random-full-reserve-inserts",\
        "random-full-delete",\
        "random-full-read",\
        "random-full-read-miss",\
        "random-full-read-half",\
        "random-full-iteration"
]
num_keys = [500 * KB,\
           1.0 * MB,\
           1.5 * MB,\
           2.0 * MB,\
           2.5 * MB,\
           3.0 * MB,\
           3.5 * MB,\
           4.5 * MB,\
           5.0 * MB,\
           5.5 * MB,\
           6.0 * MB,\
           6.5 * MB,\
           7.0 * MB,\
           7.5 * MB,\
           8.0 * MB,\
           8.5 * MB,\
           9.0 * MB,\
           9.5 * MB,\
           10.0* MB\
]
num_keys = [str(x) for x in num_keys]

def get_option():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--program', required=True, dest='program')
    parser.add_argument('-m', '--mode', required=False, default="all", dest='mode',\
                help='test mode<random-shuffle-inserts|random-full-inserts|...|all>')
    return parser.parse_args()

def call_program(p, mode):
    print(mode, ': ', p)
    time = []
    memory = []
    for n in num_keys:
        o = subprocess.check_output([p, n, mode]).decode('utf-8')
        words = o.strip().split(',')
        time.append(float(words[0].replace('s', '')))
        memory.append(int(words[1].replace('bytes', '')))
    with open(os.path.basename(p) + '-' + mode + '.csv', 'w') as f:
        print('keys,\ttime,\tmemory')
        f.write('keys,time,memory\n')
        for e in zip(num_keys, time, memory):
            print('{}, {}, {}'.format(e[0], e[1], e[2]))
            f.write('{}, {}, {}\n'.format(e[0], e[1], e[2]))

def mode_test(p, mode):
    if mode == 'all':
      for mode in modes:
        call_program(p, mode)
    else:
        call_program(p, mode)
    
def main():
    args = get_option()
    mode_test(args.program, args.mode)
    pass

if __name__ == '__main__':
    main()
