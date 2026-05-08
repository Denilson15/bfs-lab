# BFS Lab – CS 20: Data Structures

This project was developed as part of the **CS 20 Data Structures** course. The goal was to implement a **Breadth-First Search (BFS)** algorithm to determine possible flight paths between cities, based on input from a `.txt` file.

## ✨ Overview

The lab reads a list of direct flights between cities from a text file and determines whether there’s a valid route between a starting location and a target destination using BFS.

## ⚙️ How It Works

- The program reads a text file where each line represents a flight:  
  `origin destination`
- A graph is constructed where cities are nodes and flights are edges.
- The BFS algorithm is used to find a path (if one exists) from a starting city to a target city.
- The program outputs the path, or a message stating no path is available.

## 🛠️ Concepts Practiced

- Graph construction using adjacency lists
- Breadth-First Search (BFS)
- File I/O in C++
- Using `queue` and `map` from the STL
- Basic input parsing and dynamic data structures

## 📁 File Structure

- `flights.txt` – Example input file of flight connections
- `main.cpp` – Contains the BFS implementation and main program logic
- `README.md` – Project documentation

## 🚧 Notes

- Created during my learning process in CS 20 while I was also learning GitHub basics.
