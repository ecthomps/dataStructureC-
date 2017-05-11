/*
	Name	    : Eranus Thompson	
	Description : A program that solves a Kenken puzzle. Each puzzle is solved by populating 
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
#include <iomanip>
#include <cmath>

using namespace std;

const int MAX_SIZE = 9;
const int MAX_SUB_DIV_CELLS = 2;

struct grid {
	
	char let;
	int nums;
};

class kenken {

public:
	kenken() {
		matrix.resize(0);
		targNum.resize(0);
		operSign.resize(0);
		kenCell.resize(0);
		lastCell.resize(0);
		numCells = 0;
		letter = 'a';
	}

	void setBoard(int row, int col, int numCage) {

		numCells = row;
		lastCell.resize(numCage);
		targNum.resize(numCage), operSign.resize(numCage);

		kenCell.resize(row);
		for (int i = 0; i < row; i++)
			kenCell[i].resize(col);

		matrix.resize(row);
		for (int i = 0; i < numCells; i++)
			matrix[i].resize(col);

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				cin >> letter;
				kenCell[i][j] = letter - 'a';
				lastCell[letter - 'a'] = i * numCells + j;
			}
		}

		for (int i = 0; i < numCage; i++)
			cin >> targNum[i] >> operSign[i];
	}

	bool onBoard(int row, int col, int sz) {
		
		return (row >= 0 && row < sz && col >= 0 && col < sz);

		/*if (onBoard(pos, pos, size))	//FIXME
		return true;
		*/
	}

	bool stepBack(int row) {		

		if (row == numCells * numCells) //was row == matrix.size()
			return true;
		//else {
			for (int val = 1, col = 0; val <= numCells && col < numCells; val++, col++) {				
				if (col + 1 > numCells || (row != 0 && row % numCells == 0))
					rowPos = row;

				if (isLegal(row, val, col, (rowPos/numCells))) {
						matrix[(rowPos/numCells)][col] = val;

						if (stepBack(row + 1))
							return true;
					}	
				//else
					//col = 0;
			}
		//}
		return false;
	}

	bool isLegal(int pos, int num, int colPos, int rowPos) {
		
		//int size = matrix.size();

		//check rows
		for (int row = pos - 1; (row + 1) % numCells != 0; row--)	//FIXME 
			if (matrix[row][colPos] == num)
				return false;
		/*for (int row = 0; row < numCells; row++)	//FIXME 
			if (matrix[row][pos] == num)
				return false;*/
		
		//check columns
		for (int col = pos - numCells; col > 0; col -= numCells)	//FIXME
			if (matrix[colPos][col] == num)
				return false;
		/*for (int col = 0; col < numCells; col++)	//FIXME 
			if (matrix[pos][col] == num)
				return false;
				*/

		//last cell in Cage
		if (lastCell[kenCell[rowPos][colPos]] > pos)
			return true;

		if (arithOper(pos, num, colPos, rowPos))
			return true;
		return false;

	}

	bool arithOper(int pos, int num, int colPos, int rowPos) {

		switch (operSign[kenCell[rowPos][colPos]]) {
			case '+': {
				int sum = num;
				for (int i = rowPos; i < numCells; i++) {	//was i = num
					for (int j = colPos; j < numCells; j++)
						if (kenCell[rowPos][colPos] == kenCell[i][j]) //changed kC[pos][num]
							sum += matrix[i][j];				//to kC[rowPos][rowCol]
				}

				if (sum == targNum[kenCell[rowPos][colPos]])
					return true;
				return false;
			}

			case '-': {
				int count = 0;
				for (int i = 0; i < numCells; i++) {
					for (int j = 0; j < numCells /*&& kenCell[rowPos][colPos] != kenCell[i][j]*/; j++)
						if (kenCell[rowPos][colPos] == kenCell[i][j]) {
							count += 1;
							if (abs(num - matrix[i][count]) == targNum[kenCell[rowPos][colPos]])
								return true;
						}
				}
				return false;
			}

			case 'x': {
				int mult = num;
				for (int i = 0; i < numCells; i++) {
					for (int j = 0; j < numCells; j++)
						if (kenCell[rowPos][colPos] == kenCell[i][j] && 
							i - j != rowPos - colPos)
							mult *= matrix[i][j];
				}

				if (mult == targNum[kenCell[rowPos][colPos]])
					return true;
				return false;
			}

			case '/': {
				int count = 0;
				for (int i = 0; i < numCells; i++) {
					for (int j = colPos; j < numCells && kenCell[rowPos][colPos] != kenCell[i][j]; j++)
						if (num / matrix[i][j] == targNum[kenCell[rowPos][colPos]]
							|| matrix[i][j] / num == targNum[kenCell[rowPos][colPos]])
							return true;
				}
				return false;
			}
		}
	}

	void printBoard() {
		
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix.size(); j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}

		/*for (int i = 0; i < kenCell.size(); i++) {
			for (int j = 0; j < kenCell.size(); j++)
				cout << kenCell[i][j] << " ";
			cout << endl;
		}

		for (int i = 0; i < 4; i++)
			cout << targNum[i] << " " << operSign[i] << endl;*/
	}

private:
	vector <vector<int>> kenCell;
	vector <vector<int>> matrix;
	vector<int> lastCell;
	vector <int> targNum;
	vector <char> operSign;

	int numCells, rowPos = 0;
	char letter;
};

int main() {

	kenken kenGrid;
	int numSqr, numCage;
	char letters;

	cin >> numSqr >> numCage;		
	
	kenGrid.setBoard(numSqr, numSqr, numCage);	

	/*kenGrid.printBoard();*/

	if (kenGrid.stepBack(0))
		kenGrid.printBoard();
	else
		cout << "no solution" << endl;		

	return 0;
}