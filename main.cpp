#include "Triv_RMQ.hpp"
#include "FH_RMQ.hpp"
#include "Big_RMQ.hpp"
#include "Fast_RMQ.hpp"
#include "Fast2_RMQ.hpp"
#include "Seg_RMQ.hpp"
#include "SegA_RMQ.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

DTidx N_VALUES;
DTidx N_QUERIES;

DT* sequence;
DTidx* queries;

map<string, RMQ*> algorithms;

int const preprocessIterationCount = 1;
int const queryIterationCount = 1;

bool read_input(string path_input, string path_output) {
	// sequence
	ifstream file = ifstream(path_input);

	if (file.is_open()) {
		for (int i = 0; i < N_VALUES; i++) {
			file >> sequence[i];
		}
		file.close();
	}
	else return false;
	
	// queries
	file = ifstream(path_output);

	if (file.is_open()) {

		for (int i = 0; i < (2 * N_QUERIES); i++) {
			file >> queries[i];
		}
		file.close();
	}
	else return false;
	return true;
}

void runAlg(string alg_name) {
	RMQ* alg = algorithms[alg_name];
	cout << alg_name << ": " << endl;
	alg->preprocess();

	DTidx* results = alg->query(queries, N_QUERIES);
	for (int i = 0; i < N_QUERIES; i++) {
		cout << results[i] << endl;
	}
	delete[] results;

	cout << "Preprocessing time: " << alg->getPreprocessingTime() << " seconds" << endl;
	cout << "Query time: " << alg->getQueryTime() << " seconds" << endl;
}

void testAll() {
	DTidx size = algorithms.size();
	DTidx** results = new DTidx*[size]();

	DTidx a = 0;
	DTidx triv = 0;
	for (auto const &alg : algorithms) {
		if (alg.first.compare("Triv") == 0) triv = a;
		alg.second->preprocess();
		results[a++] = alg.second->query(queries, N_QUERIES);
		
	}

	int* wrong = new int[size]();
	for (DTidx i = 0; i < size; i++) {
		if (triv == i) continue;
		for (DTidx j = 0; j < N_QUERIES; j++) {
			if (sequence[results[triv][j]] != sequence[results[i][j]]) {
				wrong[i]++;
				//cout << j << ": " << sequence[results[triv][j]] << " " << sequence[results[i][j]] << endl;
			}
		}
	}

	cout << "Wrong:" << endl;
	a = 0;
	for (auto const &alg : algorithms) {
		cout << alg.first << ": " << wrong[a++] << endl;
	}

	for (int i = 0; i < size; i++) {
		delete[] results[i];
	}
	delete[] results;
	delete[] wrong;
}

void compareAll() {
	for (auto const &alg : algorithms) {
		cout << alg.first << " " << endl;

		alg.second->preprocess(preprocessIterationCount, queryIterationCount);
		cout << "Preprocessing time: " << alg.second->getPreprocessingTime() << " seconds" << endl;

		DTidx* results = alg.second->query(queries, N_QUERIES);
		cout << "Query time: " << alg.second->getQueryTime() << " seconds" << endl << endl;

		delete[] results;
	}
}

void generator(int query_size) {
	for (int i = 0; i < N_VALUES; i++) {
		sequence[i] = 1 + (std::rand() % N_VALUES);
	}

	for (int i = 0; i < N_QUERIES; i++) {
		queries[i * 2] = (std::rand() % (N_VALUES - query_size -1));
		queries[i * 2 + 1] = queries[i * 2] + query_size;
	}
}

/* params: 
* mode : "c" (correctness) / "p" (performance) / "alg name"
* input mode : "f" (file) / "g" (generator)
* size of input array
* number of queries
* input file name (if f mode) / size of query (if g mode)
* query file name (if f mode)
*/

void main(int argc, char* argv[]) {
	srand((unsigned)time(NULL));

	N_VALUES = std::stoi(argv[3]);
	N_QUERIES = std::stoi(argv[4]);

	sequence = new DT[N_VALUES]();
	queries = new DTidx[2 * N_QUERIES]();

	// input mode
	if (std::string(argv[2]).compare("f") == 0) { // file
		cout << "Reading input files..." << endl;
		if (read_input(std::string(argv[5]), std::string(argv[6]))) {
			cout << "Reading complete" << endl;
		}
		else {
			cout << "Input files not found" << endl;
		}
	}
	else if (std::string(argv[2]).compare("g") == 0) {	// generator 
		generator(std::stoi(argv[5]));
	}

	algorithms = map<string, RMQ*>();
	algorithms["Triv"] = new Triv_RMQ(sequence);
	algorithms["FH"] = new FH_RMQ(sequence, N_VALUES);
	algorithms["Big"] = new Big_RMQ(sequence, N_VALUES);
	algorithms["Fast"] = new Fast_RMQ(sequence, N_VALUES);
	algorithms["Fast2"] = new Fast2_RMQ(sequence, N_VALUES);
	algorithms["Seg"] = new Seg_RMQ(sequence, N_VALUES);
	algorithms["SegA"] = new SegA_RMQ(sequence, N_VALUES);

	// run mode
	if (std::string(argv[1]).compare("c") == 0) { // correctness
		testAll();
	}
	else if (std::string(argv[1]).compare("p") == 0) { // performace
		compareAll();
	}
	else {
		runAlg(std::string(argv[1])); // run algorithm "name"
	}

	delete[] queries;
	delete[] sequence;

	for (auto const &alg : algorithms) {
		delete alg.second;
	}
	getchar();
}