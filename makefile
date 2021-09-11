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
all: baseline.cpp compute_stats
	g++ baseline.cpp -o baseline
	g++ compute_stats.cpp -o compute_stats
	./baseline data_1111.txt test1_output.txt
	./compute_stats data_1111.txt test1_output.txt >> stats1.csv
	./baseline data_2222.txt test2_output.txt
	./compute_stats data_2222.txt test2_output.txt >> stats1.csv
	./baseline data_3333.txt test3_output.txt
	./compute_stats data_3333.txt test3_output.txt >> stats1.csv
clear: stats1.csv test1_output.txt test2_output.txt test3_output.txt 
	rm stats1.csv test1_output.txt test2_output.txt test3_output.txt 

