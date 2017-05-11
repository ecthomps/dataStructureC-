#ifndef quadT_H
#define quadT_H

#include <iostream>
#include <string>

using namespace std;

const int IMG_SIZE = 32;

struct QuadNode {
	char type;
	int level;
	QuadNode* quads[4];

	QuadNode(char t = ' ', int lev = 0) {
		type = t;
		level = lev;
	}
};

class QuadTree {
public:
	QuadTree();
	void frmStr(string encodeStr);
	void frmStri(string encodeStr, int &inputPosition, int level);
	void printTree();
	void printImage();
	char querryTree(int row, int col);
	bool sameTree(QuadNode *ptr1, QuadNode *ptr2);
	int numBlack();
	int largestRepeat();
private:
	QuadNode *root;
};

#endif
