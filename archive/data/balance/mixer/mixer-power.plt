set y2range [0:140]
set y2tics 20
set yrange [-16:-2]
set xrange [0:1000]
set xlabel "LO Power (nW)"
set ylabel "Gain (dB)"
set y2label "Noise Temperature (K)"
set grid
set key bottom left
plot "test.dat" using 1:(20*log10($2)) title "LSB Gain" with lines
replot "test.dat" using 1:(20*log10($3)) title "USB Gain" with lines
set terminal postscript color solid
set output "test.ps"
replot "test.dat" using 1:((297-77*($6/$5))/(($6/$5)-1)) axes x1y2 title "DSB Noise" with lines

