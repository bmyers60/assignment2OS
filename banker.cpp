//Brian Myers
//OS Assignment 2

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>


const int NUM_PROCESSES = 5;
const int NUM_RESOURCES = 3;

//function definitions
void printMatrix(int[NUM_PROCESSES][NUM_RESOURCES]);
std::vector<int> getProcessRow(int, int[NUM_PROCESSES][NUM_RESOURCES]);
bool isRowLess(std::vector<int>, std::vector<int>);
void updateAvailable(std::vector<int>&, std::vector<int>);

int main(int argc, char* argv[]) {
	// confirm that an input file was passed
	if (argc < 2) {
		std::cout << "Error: must include text file." << std::endl;
		return -1;
	}

	std::ifstream input("input.txt"); 
	// checking that file was opened
	if (!input.is_open()) {
		std::cout << "Error opening input file." << std::endl;
		return -1;
	}

	int allocation[NUM_PROCESSES][NUM_RESOURCES];
	int max[NUM_PROCESSES][NUM_RESOURCES];
	int available[NUM_PROCESSES];
	int need[NUM_PROCESSES][NUM_RESOURCES];

	// freed up resources will be stored in vector
	std::vector<int> newAvailable;
	newAvailable.resize(NUM_RESOURCES);

	// if vector is not 'safe' it will not be used
	std::vector<int> safeSequence;
	// queue if a process could not be completed and must be called again
	std::queue<int> revisit;

	// take data from input file
	for (int i = 0; i < NUM_PROCESSES; ++i) {
		for (int j = 0; j < NUM_RESOURCES; ++j) {
			input >> allocation[i][j];
		}
	}

	for (int i = 0; i < NUM_PROCESSES; ++i) {
		for (int j = 0; j < NUM_RESOURCES; ++j) {
			input >> max[i][j];
		}
	}

	for (int i = 0; i < NUM_PROCESSES; ++i) {
		for (int j = 0; j < NUM_RESOURCES; ++j) {
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}

	for (int i = 0; i < NUM_RESOURCES; ++i)
		input >> available[i];

	
	input.close();


	// print data
	std::cout << "Current Resources:" << std::endl;
	std::cout << "Available Resouces:" << std::endl;
	for (int i = 0; i < NUM_RESOURCES; ++i)
		std::cout << available[i] << ' ';
	std::cout << std::endl;

	std::cout << "Allocation Matrix:" << std::endl;
	printMatrix(allocation);

	std::cout << "Max Matrix:" << std::endl;
	printMatrix(max);

	std::cout << "Need Matrix:" << std::endl;
	printMatrix(need);

	std::cout << std::endl;

	for (int i = 0; i < NUM_RESOURCES; ++i)
		newAvailable[i] = available[i];

	int process = 0; // run first process
	while (process < NUM_PROCESSES) {
		std::vector<int> currNeed = getProcessRow(process, need);

		std::cout << "P" << process << " of need: ";
		for (auto e : currNeed) 
			std::cout << e << ' ';
		std::cout << std::endl;

		std::cout << "Currently Available: ";
		for (int i = 0; i < NUM_RESOURCES; ++i)
			std::cout << newAvailable[i] << ' ';
		std::cout << std::endl;

		std::cout << "Checking if Necessary Resources are Available: ";
		bool result = isRowLess(currNeed, newAvailable);
		result == 0 ? std::cout << "False" : std::cout << "True"; 
		std::cout << std::endl;

		if (result) { 
			safeSequence.push_back(process); 
			auto allocProcess = getProcessRow(process, allocation);
			updateAvailable(newAvailable, allocProcess); 
			std::cout << "Updated Available: ";
			for (auto e : newAvailable)
				std::cout << e << ' ';
			std::cout << std::endl;
		}

		if (!result) // failed: must revisit later on in the code
			revisit.push(process);
		
		std::cout << std::endl;
		++process;
	}


	int trap = 0; // varaibale to avoid infinite loops
	while (!revisit.empty() || trap > NUM_PROCESSES) {

		int revisitProcess = revisit.front(); // revisit processes failed
		std::vector<int> currNeed = getProcessRow(revisitProcess, need);

		std::cout << "P" << revisitProcess << " of need: ";
		for (auto e : currNeed)
			std::cout << e << ' ';
		std::cout << std::endl;

		std::cout << "Currently Available: ";
		for (int i = 0; i < NUM_RESOURCES; ++i)
			std::cout << newAvailable[i] << ' ';
		std::cout << std::endl;

		// confirm that resources are available
		std::cout << "Checking if Necessary Resources are Available: ";
		bool result = isRowLess(currNeed, newAvailable);
		result == 0 ? std::cout << "False" : std::cout << "True";
		std::cout << std::endl;
//succesful processes
		if (result) { 
			safeSequence.push_back(revisit.front()); 
			auto allocProcess = getProcessRow(revisit.front(), allocation); 
			revisit.pop(); 
			updateAvailable(newAvailable, allocProcess); 
			std::cout << "Updated Available: ";
			for (auto e : newAvailable)
				std::cout << e << ' ';
			std::cout << std::endl;
		}
	// failed processes
		if (!result) { 
			int temp = revisit.front(); 
			revisit.pop(); 
			revisit.push(temp); 
			++trap; 
		}
		std::cout << std::endl;

	}

	if (trap < NUM_PROCESSES) {
		std::cout << "The system is in a safe state. The Safe sequence is: ";
		for (int i = 0; i < NUM_PROCESSES; ++i) 
			std::cout << "P" << safeSequence[i] << " ";
		std::cout << std::endl;
	}
	else {
		std::cout << "No safe sequence found. System is not in a safe state";
	}

	return 0;
}


std::vector<int> getProcessRow(int process, int matrix[NUM_PROCESSES][NUM_RESOURCES]) {
	std::vector<int> retVal;
	// returns a specific row of a 2d array representing a proccess
	for (int i = process; i <= process; ++i) {
		for (int j = 0; j < NUM_RESOURCES; ++j) {
			retVal.push_back(matrix[i][j]);
		}
	}
	return retVal;
}

void printMatrix(int arr[NUM_PROCESSES][NUM_RESOURCES]) {
	// prints given matrix
	for (int i = 0; i < NUM_PROCESSES; ++i) {
		for (int j = 0; j < NUM_RESOURCES; ++j) {
			std::cout << arr[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void updateAvailable(std::vector<int>& avail, std::vector<int> alloc) {
	// calculates how much of each resource is now available after each process runs
	for (int i = 0; i < NUM_RESOURCES; ++i)
		avail[i] += alloc[i];
}

bool isRowLess(std::vector<int> currNeed, std::vector<int> avail) {
	// compare 2 rows to see if needed values are less than available
	for (int i = 0; i < NUM_RESOURCES; ++i) {
		if (currNeed[i] > avail[i])
			return false;
	}
	return true;
}


