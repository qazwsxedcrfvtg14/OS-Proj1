all: process schedule

process: process.c
	gcc process.c -o process -O2

schedule: schedule.c
	gcc schedule.c -o schedule -O2
