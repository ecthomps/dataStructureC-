/*
		Name        : Eranus Thompson
		Description : Given a gaph that has a complete set of pipes (edges) that connect
					  all junctions (vertices), the program uses minimum spanning tree,
					  find, or union to pick the pipes (edges) which most closely have the
					  same capacity to be replaced. The input (read from a keyboard) consists
					  of multiple datasets followed by a line containing two zeros separated
					  by a space (which indicates the end of the input datasets). The junctions
					  (vertices) >= 2 and the pipes (edges) <= 0 && >= ( vertices(vertices - 1)/2).
					  For a complete set of pipes that connect all junctions, the program outputs
					  the set with the smallest range of capacities. Otherwise, it outputs -1.
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int maxWeightCap = 10000;

struct mstEdge {

	int src;
	int dest;
	int weight;

	mstEdge(int v = 0, int e = 0, int w = 0) {
		/*
			pre  : v > 0, e > 0, w > 0
			post : src = v, dest = e, weight = w
		*/

		src = v;
		dest = e;
		weight = w;
	}

	bool operator < (const mstEdge& rhs) {
		/*
			pre  : rhs != null
			post : returns true if weight < rhs.weight
		*/

		return weight < rhs.weight;
	}
};

struct makeSubSet {

	int index;
	int parent;
};

class mstGraph {

public:

	void createGraph(int numVer, int numEdge) { 
		/*
			pre  : numVer >= 0 && numEdge >= 0
			post : creates an undirected graph
		*/

		int ver1, ver2, wght, count = 0;

		while (!eofDataSet(numVer, numEdge)) { 
			edgeVec.resize(numEdge);

			for (int i = 0; i < numEdge; i++) { 
				cin >> ver1 >> ver2 >> wght; 
				edgeVec[i].src = ver1;
				edgeVec[i].dest = ver2;
				edgeVec[i].weight = wght;
			}
			sort(edgeVec.begin(), edgeVec.end());	
													
			kruskMST(numVer, numEdge); 
			cin >> numVer >> numEdge;
		}
		printMinWeight();
	}
		
	void unionSet(vector<makeSubSet>& subSet, int junc1, int junc2) {
		/*
			pre  : subSet.size() > 0, junc1 > 0, junc2 > 0
			post : forms a union on junc1 and junc2 
		*/
		
		if (subSet[junc1].parent < subSet[junc2].parent)
			subSet[junc2].parent = junc1;
		else if (subSet[junc1].parent > subSet[junc2].parent)
			subSet[junc2].parent = junc1;				
		else
			subSet[junc2].parent = junc1;		
	}
		
	int findSet(vector<makeSubSet>& subSet, int indx) {
		/*
			pre  : subSet.size() > 0, indx > 0
			post : returns subSet[indx].parent
		*/
		
		if (subSet[indx].parent > 0) {	
			subSet[indx].parent = findSet(subSet, subSet[indx].parent);
			return subSet[indx].parent;			
		}		
		return indx;	
	}

	void kruskMST(int numVer, int numEdge) {
		/*
			pre  : numVer > 0, numEdge > 0
			post : creates a minimum spanning tree
		*/

		int count = 0, minWeight = 0, minCap = INT_MAX;
		int minWRange, maxWRange, numPar = 0;
		int xPar, yPar;
		bool isCycle, inComplete = false;		
		groupSet.resize(numVer + 1);

		for (int i = 0; (i < numVer && ((numEdge - i) >= (numVer - 1))) && (minWeight >= 0); i++) { 
			initializeSubSet(numVer);
			minWRange = INT_MAX, maxWRange = 0;
			isCycle = false;
			count = i;
						 
			while (count < (numVer - 1) + i){	
				int x = findSet(groupSet, edgeVec[count].src); 
				int y = findSet(groupSet, edgeVec[count].dest); 				

				if (x != y) {		
					unionSet(groupSet, x, y); 
					findMinMax(minWRange, maxWRange, count, isCycle); 					
				}
				else {
					isCycle = true;
					if(count < edgeVec.size())
						findMinMax(minWRange, maxWRange, count+1, isCycle);
				}
				count++;
			}

			if (!isCycle)
				minWeight = findMinWeight(minWRange, maxWRange, minCap);  

			inComplete = isComplete(groupSet, numPar); 
		}
		if ((!inComplete || isCycle) && (minCap == INT_MAX))
			strMinWeight.push_back(-1); 
		else
			strMinWeight.push_back(minWeight); 
	};

	void initializeSubSet(int numVer) {
		/*
			pre: numVer > 0
			post: initalizes groupSets index &&
			      parent
		*/

		for (int i = 1; i <= numVer; i++) {
			groupSet[i].index = i;
			groupSet[i].parent = -1;
		}
	}

	void findMinMax(int& minRange, int& maxRange, int count, bool& isCycle) {
		/*
			pre  : 0 < minRange <= INT_MAX, maxRange >= 0
			       count > 0
			post : sets minRange = edgeVec[0].weight
				   sets maxRange = edgeVec[edgeVec.size()-1]
		*/

		if (count < edgeVec.size()) {
			isCycle = false;
			if (edgeVec[count].weight < minRange)
				minRange = edgeVec[count].weight;
			if (edgeVec[count].weight > maxRange)
				maxRange = edgeVec[count].weight;
		}
	}

	int findMinWeight(int minRange, int maxRange, int& minCap) {
		/*
			pre  : 0 < minRange <= INT_MAX, maxRange >= 0
			       count > 0
			post : sets minCap = maxRange - minRange
		*/

		if ((maxRange - minRange < minCap) && (maxRange - minRange) <= maxWeightCap)
			minCap = maxRange - minRange;
		return minCap;
	}

	bool isComplete(vector<makeSubSet> subSets, int numPar) {
		/*
			pre  : subSets.size > 0, numPar > 0
			post : returns true if numPar > 1
				   else returns false
		*/

		for (int i = 1; i < subSets.size(); i++)
			if (subSets[i].parent < 0)
				numPar++;
		if (numPar > 1)
			return true;
		return false;
	}

	bool eofDataSet(int node, int pipe) {
		/*
			pre  : node >= 0, pipe >= 0
			post : returns true if node == 0 && pipe == 0
			       else returns false
		*/

		if (node == 0 && pipe == 0)
			return true;
		return false;
	}

	void printMinWeight() {
		/*
			pre  :
			post : prints out strMinWeight
		*/

		for (int i = 0; i < strMinWeight.size(); i++)
			cout << strMinWeight[i] << endl;
	}

private:
	vector <mstEdge> edgeVec;
	vector <makeSubSet> groupSet;
	vector <int> strMinWeight;
};

int main() {

	mstGraph graph;
	mstEdge edge;
	int numVer, numEdge;

	cin >> numVer >> numEdge;
	graph.createGraph(numVer, numEdge);
	
	return 0;
}