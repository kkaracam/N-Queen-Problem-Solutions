#include <iostream>
#include"board.h"

using namespace std;

int main()
{	
	int success = 0, experiments = 100;
	auto start = chrono::system_clock::now();
	srand(time(NULL));

	// Range of the loop determines the number of experiments.
	for (int i = 0; i < experiments; i++) {
		Board *table = new Board(100);
		//Comment in the below statement to run "Simulated Annealing" algorithm
		//table->runSA(10000, 0.00001);

		//Comment in the below statement to run "First Choice Hill Climbing" algorithm
		table->run(true);

		//Comment in the below statement to run "Hill Climbing" algorithm
		//table->run(false);

		success += table->isSuccess();
	}
			
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "Total Running Time:\t" << elapsed_seconds.count() << " seconds.\n\n";
	cout << "Average Running Time:\t" << elapsed_seconds.count() / experiments << " seconds.\n\n";
	cout << "Success rate:\t" << success / experiments * 100 << "% \n\n";
    return 0;
}

