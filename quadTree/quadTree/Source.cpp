#include "quadT.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	string quadStr;
	QuadTree tree;

	cin >> quadStr;

	//tree.frmStr(quadStr);
	tree.frmStr(quadStr, 0, 0);
	return 0;
}