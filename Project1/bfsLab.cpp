#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

int findIndex(vector<string>& lookUp, string city);

int main() {
	//we need to read in the cities from the file
	vector<string> lookUpTable;
	vector<int> adjList[10000];
	string nextCity;
	ifstream connectionsFile("connections.txt");
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
	}
	else {
		cout << "Failed to open file. ";
	}

	string userInput;
	int index;
	do {
		cout << "Please enter a departing city name or `quit` to exit: ";
		cin >> userInput; //user enters departing city

		//modify to check if the string inputted matches any of the indices or substrings in the indices
		if (userInput.length() >= 2) {
			bool found = false;
			for (int j = 0; j < lookUpTable.size(); j++) {
				if (lookUpTable[j].find(userInput) != string::npos) { //we need to display the cities that the input matches
					cout << j << ": " << lookUpTable[j] << endl;
					found = true;
				}
			}
			if (found) {
				cout << "Please select a departing city by entering a number from the list above: ";
				cin >> userInput;
				index = stoi(userInput); //casting userInput in the function then the value it returns is used as an index

				//output the "Selected Departure: departure number: city, country"
				cout << "Selected Departure: " << index << ": " << lookUpTable[index] << endl;

				cout << "Please enter a destination city or `quit` to exit: ";
				cin >> userInput; //user enters destination city
				//we need to display the cities that the input matches

				cout << "Please select a destination by entering a number from the list above: ";
				cin >> userInput;
				index = stoi(userInput);
				cout << "Selected Destination: " << index << ": " << lookUpTable[index] << endl;

				//now we have to use bfs to output the shortest route
				cout << "Shortest Route: ";

				cout << "Make another search? (`yes` or `no`): ";
				cin >> userInput;
				if (userInput == "no" || userInput == "NO" || userInput == "No" || userInput == "nO") {
					cout << "Press any key to continue . . .";
					cin.ignore();
					cin.get();
					break;
				}
			}
			else { //if no value is found it should just print this message
				cout << "Found no results." << endl;
			}
		}
		else{
			cout << "Please use at least two characters." << endl;
		}
	} while (userInput != "quit");
	return 0;
}


int findIndex(vector<string>& lookUp, string city) { //use linear search to look for the city in the look up table
	for (int i = 0; i < lookUp.size(); i++) {
		if (lookUp[i] == city) return i;
	}
	return -1;
}