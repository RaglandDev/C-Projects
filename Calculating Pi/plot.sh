#!/bin/bash
make clean && make all

gnuplot <<e
	set terminal pdf
	set output "e.pdf"
	set title "e Approximation"
	set xlabel "Iteration"
	set ylabel "Value"
	set key noautotitle
	plot 'e.txt' using 2:1 with lines lw .25 lc rgb 'red'
e

gnuplot <<madhava
        set terminal pdf
        set output "madhava.pdf"
        set title "Madhava Pi Approximation"
        set xlabel "Iteration"
        set ylabel "Value"
        set key noautotitle
        plot 'madhava.txt' using 2:1 with lines lw .25 lc rgb 'red'

madhava

gnuplot <<viete
        set terminal pdf
        set output "viete.pdf"
        set title "Viete Pi Approximation"
        set xlabel "Iteration"
        set ylabel "Value"
        set key noautotitle
        plot 'viete.txt' using 2:1 with lines lw .25 lc rgb 'red'

viete

gnuplot <<euler
        set terminal pdf
        set output "euler.pdf"
        set title "Euler Pi Approximation"
        set xlabel "Iteration"
        set ylabel "Value"
        set key noautotitle
        plot 'euler.txt' using 2:1 with lines lw .25 lc rgb 'red'

euler

gnuplot <<bbp
        set terminal pdf
        set output "bbp.pdf"
        set title "BBP Pi Approximation"
        set xlabel "Iteration"
        set ylabel "Value"
        set key noautotitle
        plot 'bbp.txt' using 2:1 with lines lw .25 lc rgb 'red'

bbp

gnuplot <<sqrt
	set terminal pdf
	set output "sqrt.pdf"
	set title "Sqrt(x) Approximations"
	set xlabel "x"
	set ylabel "y"
	set key noautotitle
	plot 'sqrt.txt' using 3:1 with lines lw 3 lc rgb 'purple', 'sqrt.txt' using 3:2 with points pt 7 pointsize .15 lc rgb 'red'
sqrt
