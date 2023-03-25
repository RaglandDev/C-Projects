#!/bin/bash

touch shell.txt
touch heap.txt
touch batcher.txt
touch quick.txt

for i in {10..10000}
do
    ./sorting -g -s -n $i -p $i >> ./shell.txt
done

for i in {10..10000}
do
    ./sorting -g -h -n $i -p $i >> ./heap.txt
done

for i in {10..10000}
do
    ./sorting -g -b -n $i -p $i >> ./batcher.txt
done

for i in {10..10000}
do
    ./sorting -g -q -n $i -p $i >> ./quick.txt
done

gnuplot <<shell
    set terminal pdf
    set output "shell.pdf"
    set title "Shell Sort"
    set xlabel "Elements"
    set ylabel "Moves"
    set key noautotitle
    set logscale xy
    plot 'shell.txt' using 1:2 with lines lw .25 lc rgb 'red'
shell

gnuplot <<heap
    set terminal pdf
    set output "heap.pdf"
    set title "Heap Sort"
    set xlabel "Elements"
    set ylabel "Moves"
    set key noautotitle
    set logscale xy
    plot 'heap.txt' using 1:2 with lines lw .25 lc rgb 'blue'

heap

gnuplot <<batcher
    set terminal pdf
    set output "batcher.pdf"
    set title "Batcher Sort"
    set xlabel "Elements"
    set ylabel "Moves"
    set key noautotitle
    set logscale xy
    plot 'batcher.txt' using 1:2 with lines lw .25 lc rgb 'green'
batcher

gnuplot <<quick
    set terminal pdf
    set output "quick.pdf"
    set title "Quick Sort"
    set xlabel "Elements"
    set ylabel "Moves"
    set key noautotitle
    set logscale xy
    plot 'quick.txt' using 1:2 with lines lw .25 lc rgb 'orange'
quick

gnuplot <<all
    set terminal pdf
    set output "all.pdf"
    set title "All Sorts"
    set xlabel "Elements"
    set ylabel "Moves"
    set logscale xy
    plot 'shell.txt' using 1:2 with lines lw .25 lc rgb 'red' title 'Shell sort', 'heap.txt' using 1:2 with lines lw .25 lc rgb 'blue' title 'Heap sort', 'batcher.txt' using 1:2 with lines lw .25 lc rgb 'green' title 'Batcher sort', 'quick.txt' using 1:2 with lines lw .25 lc rgb 'orange' title 'Quick sort'
all
