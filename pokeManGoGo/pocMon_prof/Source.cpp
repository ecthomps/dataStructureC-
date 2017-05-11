/*
Name	   : Eranus Christian Thompson
Date	   : 1-17-2017
Description: The program creates a Pokemon Go Go that access Pokemon stop
			 locations and Pokemon near every current location. Using vectors,
			 the program stores the location of the the Pokemon stops. It determines
			 the order to visit all Pokemon stops (using next_permutation) to
			 minimize the totsl distance traveled. The program returns to the
			 starting location (0,0) after one of each Pokemon is caught. The input
			 contains the list of Pokeman stops(North and East of the starting point)
			 and the order in which to visit the stops to minimize distance traveled.
			 The output displays a single line of integers representing the order of
			 the stops visited (the minimize total distance traveled followed by the
			 total distance traveled in blocks.
*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>

using namespace std;


struct Pokemon {

	int xloc, yloc;
	string pocMon;
};

void strPoc(int maxPoc, vector<Pokemon>& pocStre);
void uniPoke(vector<vector<int>>& indxPoke, const vector<Pokemon>& store);
bool pocNext(vector<int>& index, vector<vector<int>>& pocStre);
int chkDup(int temp, const vector<int>& pokeIndx);
void pocComb(vector<int> index, vector<vector<int>> pocStre, vector<Pokemon> poke);
int pocDist(vector<int> index, vector<int>& bestDist, vector<Pokemon>pocLoc);

int main() {

	int pocStops;
	bool res = false;

	vector<Pokemon> pocLoc;
	vector<vector<int>> pocStre;
	vector<int> index;

	cin >> pocStops;
	if (pocStops > 0) {

		strPoc(pocStops, pocLoc);	  
		uniPoke(pocStre, pocLoc);
		
		for (int i = 0; i < pocStre.size(); i++) {	//stores indexes of pokemon
			index.push_back(pocStre[i][0]);
		}

		pocComb(index, pocStre, pocLoc);	//prints out combinations of indexes
	}
										
	return 0;
}

void strPoc(int maxPoc, vector<Pokemon>& pocStre) {
	/*
		pre : maxPoc > 0
		post: reads in and stores vector xloc, yloc,
			  pokemon names
	*/

	pocStre.resize(maxPoc + 1);
	pocStre[0].xloc = 0;
	pocStre[0].yloc = 0;
	pocStre[0].pocMon = "";

	for (int i = 1; i < maxPoc + 1; i++) {
		cin >> pocStre[i].xloc >> pocStre[i].yloc
			>> pocStre[i].pocMon;
	}
}

void uniPoke(vector<vector<int>>& indxPoke, const vector<Pokemon>& store) {
	/*  
		pre : store contains a vector struct of pokemon names			  
		post: store[indxPoke[i][j]].name ==
			  store[indx[i][k]] for all j, k
			  locations[indxi][j]].name == the ith unique name
			  entered in input 
	*/

	vector <int> temp(1, 0);
	string name;
	bool isThere;

	indxPoke.push_back(temp);
	for (int i = 1; i < store.size(); i++) {
		name = store[i].pocMon;
		isThere = false;
		for (int j = 0; j < indxPoke.size() && !isThere; j++)
			if (name == store[indxPoke[j][0]].pocMon) {
				isThere = true;
				indxPoke[j].push_back(i);
			}
		if (!isThere) {
			temp[0] = i;
			indxPoke.push_back(temp);
		}
	}
}

//pocComb(index, pocStre, pocLoc);
void pocComb(vector<int> index, vector<vector<int>> pocStre, vector<Pokemon> poke) {
	/*
		pre : store and indx contains poke.pocMon indexes
		post: prints out the best path and minimum distance
	*/

	vector <int> finDist, realDist;
	finDist.resize(0);
	realDist.resize(0);

	int bestDist = INT_MAX;
	int x;

	do {
		x = pocDist(index, finDist, poke);
		if (x < bestDist) {
			realDist.resize(0);
			bestDist = x;
			for (int i = 0; i < finDist.size(); i++) {
				realDist.push_back(finDist[i]);
			}
		}
	} while (pocNext(index, pocStre));
	for (int i = 0; i < realDist.size(); i++)
		cout << realDist[i] << " ";
	cout << bestDist << endl;
}

bool pocNext(vector<int>& index, vector<vector<int>>& pocStre) {
	/*
		pre :	store.size() == indx.size() 
				store[i] == indx[i][j] for some j    
		post:	increment store to the next possible set of integers
				such that store[i] == indx[i][j] for some j
				(where next is determined by the order the names appear 
				in input).
	*/

	int pocNum;
	bool done = false;

	for (int i = 1; i < pocStre.size() && !done; i++) { 
		pocNum = chkDup(index[i], pocStre[i]);
		if (pocNum < pocStre[i].size() - 1) {
			index[i] = pocStre[i][++pocNum];
			return true;
		}
		else if (pocNum == pocStre[i].size() - 1) {
			index[i] = pocStre[i][0];
		}
	}
	return done;
}

int chkDup(int temp, const vector<int>& pokeIndx) {
	/*
		pre :	temp>0, pokeIndx contains a vector of ints 
		post:   returns the location where index is found 
				returns -1 if the index is not found. 
	*/

	for (int i = 0; i < pokeIndx.size(); i++)
		if (temp == pokeIndx[i])
			return i;
	return -1;
}


int pocDist(vector<int> index, vector<int>& bestDist, vector<Pokemon>pocLoc) {
	/*
		pre : pocMon and bestDist contains pocLoc.pocMon indexes
			  pocLoc contains xloc and yloc
		post: permutes through index.begin() to index.end(), 
			  calcualtes the best distance and stores the new minimum 
			  path indexes returns the minTotal distance
	*/

	int var1, var2;
	int total = 0, bestTotal = INT_MAX, start = 0;
	vector<int>distance;
	distance.resize(0);
	sort(index.begin(), index.end());

	do {

		for (int i = 1; i < index.size(); i++) {
			var1 = abs(pocLoc[index[i]].xloc - pocLoc[index[i - 1]].xloc);
			var2 = abs(pocLoc[index[i]].yloc - pocLoc[index[i - 1]].yloc);
			total += var1 + var2;
			if (i + 1 == index.size()) {
				var1 = abs(pocLoc[index[i]].xloc - start);
				var2 = abs(pocLoc[index[i]].yloc - start);
				total += var1 + var2;
			}
			distance.push_back(index[i]);
		}

		if (total < bestTotal) {
			bestTotal = total;
			bestDist.resize(0);
			for (int i = 0; i < distance.size(); i++) {
				bestDist.push_back(distance[i]);
			}
		}
		total = start;
		distance.resize(start);
	} while (next_permutation(++index.begin(), index.end()));
		
	return bestTotal;
}


