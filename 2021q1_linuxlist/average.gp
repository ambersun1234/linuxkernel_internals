reset

set ylabel 'time(sec)'
set xlabel 'size'
set ytics 1
set style fill solid
set title 'Average sorting performance'
set term png enhanced font 'Verdana,10'
set output 'average.png'

plot [:][:]'average.txt' \
   using 2:xtic(1) with linespoints linewidth 2 title 'insertion sort', \
'' using 3:xtic(1) with linespoints linewidth 2 title 'quick sort', \
'' using 4:xtic(1) with linespoints linewidth 2 title 'merge sort'
