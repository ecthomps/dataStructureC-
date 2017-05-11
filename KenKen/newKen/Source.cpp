/*
	Name : Eranus Thompson

	Descriiption : A program that solves a Kenken puzzle. Each puzzle is solved by populating 
				   a grid with digits 1-3 for a 3*3 grid, 1-4 for a 4*4 grid, etc so that a single 
				   digit appears once in each row and column. Furthermore, the KenKen grid is divided
				   into outlined groups of cells called cages. Each digit in the cells of each cage are
				   grouped using an arithmetic operation (+, -, *, /) to produce a certain target number.
				   The input begins with two integers (n and c) separated by a single space, followed by 
				   n lines representing n letters (a - z) each. Every cell containing the same letter is 
				   represented in the same cage, and every letter denotes a cage which always starts with "a"
				   and run consecutively. The remaining c lines contains a number with a symbol separated by a 
				   space. The output will be an n * n matrix with values that solve the puzzle. If there is no 
				   solution then the output is "no solution".
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class kenken {

public:
	kenken() {
		/*
			post : resizes kenCell, lastCageVal, targNum
				   operSign to 0
				   initialize startCageLetter to 'a'
				   initialize numCells to 0
		*/

		kenCell.resize(0), lastCageVal.resize(0);
		targNum.resize(0), operSign.resize(0);

		startCageLetter = 'a';
		numCells = 0;
	}

	void setBoard(int numSqr, int numCage) {
		/*
			pre  :  3 <= numSqr <= 9
				    3 <= numCage <= 9
			post :  creates a numSqr * numSqr board
		*/

		int num;
		char symb;

		kenCell.resize(numSqr*numSqr);		
		numCells = numSqr;

		lastCageVal.resize(numCage);
		targNum.resize(numCage);
		operSign.resize(numCage);
						
		for (int i = 0; i < numSqr; i++) {
			for (int j = 0; j < numSqr; j++) {
				cin >> cageLetter;
				kenCell[i*numSqr + j] = cageLetter - startCageLetter;
				lastCageVal[cageLetter - startCageLetter] = i*numSqr + j;
			}
		}

		for (int i = 0; i < numCage; i++) 			
			cin >> targNum[i] >> operSign[i];		
	}

	bool stepBack(int num, vector<int>& matrix) {
		/*
			pre  : num >= 0 matrix.size() > 0
			post : returns true if num == numCells*numCells
				   returns true if isLegal() == true && 
				   stepBack() == true, else returns false
		*/

		if (num == kenCell.size())
			return true;
		else {
			for (int pos = 1; pos <= numCells; pos++) {

				if (isLegal(num, pos, matrix)) {
					matrix[num] = pos;

					if (stepBack(num + 1, matrix))
						return true;
				}
			}
			return false;
		}
	}

	bool isLegalRow(int num, int posVal, const vector<int>& matrix) {
		/*
			pre  : num >= 0, n > 0, matrix.size() > 0
			post : returns false if num appears
				   matrix[row] == posVal
				   else returns true
		*/

		int row = num - 1;
		while ((row + 1) % numCells != 0) {
			if (matrix[row] == posVal)
				return false;
			row--;
		}
		return true;
	}

	bool isLegalCol(int num, int posVal, const vector<int>& matrix) {
		/*
			pre  : num >= 0, n > 0, matrix.size() > 0
			post : returns false if num appears 
			       matrix[col] == posVal
				   else returns true
		*/

		for (int col = num - numCells; col > 0; col -= numCells) {
			if (matrix[col] == posVal)
				return false;
		}
		return true;
	}

	bool isLegal(int num, int posVal, const vector<int>& matrix) {
		/*
			pre  : num >= 0, n > 0, matrix.size() > 0
			post : returns false if num appears in the 
				   same row || col
				   returns false if checkLastCageVal() == false
				   returns false if arithOper() == false
				   else returns true
		*/
		
		if (!isLegalRow(num, posVal, matrix))
			return false;

		if (!isLegalCol(num, posVal, matrix))
			return false;
		
		if (lastCageVal[kenCell[num]] > num)
			return true;		

		if (arithOper(num, posVal, matrix))
			return true;
		return false;				
	}

	bool arithOper(int num, int posVal, const vector<int>& matrix) {
		/*
			pre  : num >= 0, n > 0, matrix.size() > 0
			post : returns true is sum || sub || mult || 
				   div == true
				   else returns falsse
					
		*/

		switch (operSign[kenCell[num]]) {

			case '+': {
				int sum = posVal;
				for (int i = 0; i < num; i++)
					if (kenCell[i] == kenCell[num])
						sum += matrix[i];

				if (sum == targNum[kenCell[num]])
					return true;
				return false;
			}

			case '-': {	
				int pos = 0;					
				while (kenCell[pos] != kenCell[num])
					pos++;

				if (abs(matrix[pos] - posVal) == targNum[kenCell[num]])
						return true;
					return false;				
			}

			case 'x': {
				int mult = posVal;
				for (int i = 0; i < num; i++)
					if (kenCell[i] == kenCell[num])
						mult *= matrix[i];

				if (mult == targNum[kenCell[num]])
					return true;
				return false;
			}

			case '/': {	
				int pos = 0;				
				while (kenCell[pos] != kenCell[num])
					pos++;

				if ((matrix[pos] / posVal == targNum[kenCell[num]]) ||
					(posVal / matrix[pos] == targNum[kenCell[num]]))
						return true;				
					return false;
			}			
		}
		return true;
	}

	void printBoard(const vector<int>& matrix) {
		/*
			pre  : matrix.size() > 0
			post : prints out a numCells by numCells
				   board 			
		*/

		for (int i = 0; i < numCells; i++) {
			for (int j = 0; j < numCells; j++)
				cout << matrix[i * numCells + j] << " ";
			cout << endl;
		}
	}

private:
	vector<int> kenCell, lastCageVal;
	vector<int> targNum;
	vector<char> operSign;

	char startCageLetter, cageLetter;
	int numCells;
};

int main() {

	int numSqr, numCage;
	kenken kenGrid;
	vector<int> kenPuz;	

	cin >> numSqr >> numCage;
	kenGrid.setBoard(numSqr, numCage);
	kenPuz.resize(numSqr*numSqr);

	if (kenGrid.stepBack(0, kenPuz))
		kenGrid.printBoard(kenPuz);
	else
		cout << "no solution" << endl;

	return 0;
}