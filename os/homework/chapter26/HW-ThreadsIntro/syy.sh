#!/bin/bash
for ((var = 0; var < 10; var++));
do
python2 ./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s $var -c
done