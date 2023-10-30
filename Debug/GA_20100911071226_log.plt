reset;
set terminal jpeg font arial 26 size 1400,750;
set grid xtics x2tics ytics y2tics;
show label;
set xlabel "Geração";
set ylabel "Pontuação";
set output "GA_20100911071226_log.jpg";
plot	"GA_20100911071226_log.dat" with lines title  "fit";
