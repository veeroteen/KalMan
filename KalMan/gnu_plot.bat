@echo off
gnuplot -p -e "set title 'Kalman Filter 3D'; set xlabel 'X'; set ylabel 'Y'; set zlabel 'Z'; set grid; splot 'true.csv' using 1:2:3 with lines lw 2 lc rgb 'green' title 'True', 'meas.csv' using 1:2:3 with points pt 7 ps 1.2 lc rgb 'red' title 'Measurements (z)', 'est.csv' using 1:2:3 with linespoints pt 5 lw 2 lc rgb 'blue' title 'Kalman Estimate'"
