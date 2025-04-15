#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <fstream>
using namespace std;

void printPath(int parents[], int size, int startv, int endv, vector<string>& lookupTable);
void bfs(vector<int> alists[], int size, int start, int target, vector<string>& lookupTable);
int findIndex(vector<string>& lookUp, string city);
string toLowerCase(string s);

int main() {
	//we need to read in the cities from the file
	vector<string> lookUpTable;
	vector<int> adjList[10000];
	string nextCity;
	ifstream connectionsFile("../connections.txt");
	if (connectionsFile.is_open()) {
		int fromIndex = -1;
		while (getline(connectionsFile, nextCity)) {
			//we need to make sure the next city we read in is just the city we can ignore the first 7 char
			string cityName = nextCity.substr(7);
			bool duplicate = false; //this variable is being used to keep track of duplicates
			for (int i = 0; i < lookUpTable.size(); i++) {
				if (lookUpTable[i] == cityName) { //here were looping through the lookuptable
					duplicate = true; //if the city is in the table then we have found a duplicate
					break; // we break out of the loop since we no longer need to check if its a dup
				}
			}
			if (!duplicate) { //if its not a dup then we simply add it to our vector
				lookUpTable.push_back(cityName);
			}
			int cityIndex = findIndex(lookUpTable, cityName); //we use our function that returns the index of the city
			if (nextCity.find("From:") != string::npos) { //check if the city has the word from at the start of it. this is also the purpose of keeping track of two city variables at once
				fromIndex = cityIndex;  //if it does contain "from" we will use that to track our parent cities (departing cities/from cities)
			}
			else { //if it doesnt we just push the city index to the vector at the index of its parent city, mapping them together
				adjList[fromIndex].push_back(cityIndex);
			}
		}
		string userInput;
		int departingCityIndex, destinationCityIndex;
		bool quitProgram = false; // i had to add this because i originally tried to break out of the loop but the if statement I was using to break out was then wrapped in a nested loop so it broke out of the other loop which led to it not ending the program
		do {
			cout << "Please enter a departing city name or `quit` to exit: ";
			getline(cin >> ws, userInput); //user enters departing city

			//modify to check if the string inputted matches any of the indices or substrings in the indices
			if (userInput.length() >= 2) {
				bool foundDeparting = false, foundDestination = false; //used to track if we found the flights
				for (int j = 0; j < lookUpTable.size(); j++) {
					if (toLowerCase(lookUpTable[j]).find(toLowerCase(userInput)) != string::npos) { //we need to display the cities that the input matches
						cout << j << ": " << lookUpTable[j] << endl;
						foundDeparting = true;
					}
				}
				cout << endl; //formatting output
				if (foundDeparting) { //if we find cities that match the user input
					cout << "Please select a departing city by entering a number from the list above: ";
					getline(cin >> ws, userInput);
					departingCityIndex = stoi(userInput); //casting userInput in the function then the value it returns is used as an index
					//output the "Selected Departure: departure number: city, country"
					cout << "Selected Departure: " << departingCityIndex << ": " << lookUpTable[departingCityIndex] << "\n\n";

					while (!foundDestination) { //this is really just in case the user inputs a destination that doesnt exist we just keep prompting
						cout << "Please enter a destination city or `quit` to exit: ";
						getline(cin >> ws, userInput); //user enters destination city
						//we need to display the cities that the input matches

						for (int j = 0; j < lookUpTable.size(); j++) {
							if (toLowerCase(lookUpTable[j]).find(toLowerCase(userInput)) != string::npos) { //same loop logic from departing city
								cout << j << ": " << lookUpTable[j] << endl;
								foundDestination = true;
							}
						}
						if (foundDestination) { //if it gets set to true this will run and then the loop will end afterwards
							cout << "\nPlease select a destination by entering a number from the list above: ";
							getline(cin >> ws, userInput);
							destinationCityIndex = stoi(userInput);
							cout << "Selected Destination: " << destinationCityIndex << ": " << lookUpTable[destinationCityIndex] << "\n\n";
							cout << "Shortest Route:\n";
							bfs(adjList, lookUpTable.size(), departingCityIndex, destinationCityIndex, lookUpTable); //here is the bfs call after the user has chosen the locations
							cout << "\nMake another search? (`yes` or `no`): ";
							cin >> userInput;
							if (toLowerCase(userInput) == "no") {
								cout << "Press any key to continue . . .";
								cin.ignore();
								cin.get();
								quitProgram = true;
								break;
							}
						}
						else cout << "Found no results.\n\n"; //if it does not get set to true meaning we cant find the place the loop will keep running and that means whatever the user is looking for is not being found
					}
				}
				else { //if no value is found it should just print this message
					cout << "Found no results." << "\n\n";
				}
			}
			else {
				cout << "Please use at least two characters." << "\n\n";
			}
		} while (userInput != "quit" && !quitProgram);
	}
	else {
		cout << "Failed to open file. ";
	}
	return 0;
}


int findIndex(vector<string>& lookUp, string city) { //use linear search to look for the city in the look up table
	for (int i = 0; i < lookUp.size(); i++) {
		if (lookUp[i] == city) return i;
	}
	return -1;
}


// traces parent pointers back from endv to startv
void printPath(int parents[], int size, int startv, int endv, vector<string>& lookupTable) {
	if (endv != startv) {
		printPath(parents, size, startv, parents[endv], lookupTable);
		cout << " --> "; //this is for formatting it will only print the arrow as long as we havent reached the base case
	}
	cout << lookupTable[endv]; //from that i modifed print path by adding the lookuptable as a parameter since we need to output the location value
}



void bfs(vector<int> alists[], int size, int start, int target, vector<string>& lookupTable) {
	int* parents = new int[size]; //creates array, parents points to array/first index
	for (int i = 0; i < size; i++) //loop through the array
		parents[i] = -1; //assigning all values in the array -1 to signify no value until updated later on
	
	parents[start] = start; //start index gets updated to reflect an index we have (node that exists at that location so to say)
	queue<int> q; //create a queue
	q.push(start); //we push that first index of start to the queue
	bool found = false; //bool variable set to false
	while (!q.empty() && !found) { //while the queue isnt empty and we havent found what were looking for continue to loop
		int v = q.front(); // by the time this happens, v already has a parent (whatever we have at the front of the queue will be constantly updating v while the loop runs
		q.pop(); //we then pop whatever is at the front
		if (v == target) //if this target were looking for is equivalent to the value that was previously at the front then we set found to true
			found = true; //we found the destination from our departing city
		else  //otherwise we loop through the vector for this nodes adjacency list we created
			for (int i = 0; i < alists[v].size(); i++) {
				int w = alists[v][i]; //this arrays index and vectors index value will be stored in w
				if (parents[w] == -1) { //if that index is empty then v the current node we were just looking at
					parents[w] = v; //will be assigned to update the parents array to mark the current location per se 
					q.push(w); //we can now push w to the queue
				}
			}
	}
	if (found)  //once were out of the loop we check if found is set to true
		printPath(parents, size, start, target, lookupTable); //here is where i modified bfs and print path, we use the lookup table in print path to print the actual location value rather than index
	else //otherwise we print out that the path was not found
		cout << "Not found";

	cout << endl;
	delete[] parents; //then we can delete the array to deallocate the memory
}

string toLowerCase(string s) {
	for (int i = 0; i < s.length(); i++) { //looping to string as a character array
		s[i] = tolower(s[i]); //converting each char to lowercase
	}
	return s; //once all chars have been converted in the string return the updated string.
}