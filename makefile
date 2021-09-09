compile: baseline.cpp 
	g++ baseline.cpp -o baseline
test1: baseline
	./baseline data_1111.txt test1_output.txt
stat1: compute_stats.cpp
	g++ compute_stats.cpp -o compute_stats
	./compute_stats data_1111.txt test1_output.txt
all_test1: baseline.cpp compute_stats
	g++ baseline.cpp -o baseline
	./baseline data_1111.txt test1_output.txt
	g++ compute_stats.cpp -o compute_stats
	./compute_stats data_1111.txt test1_output.txt > stats1.csv
