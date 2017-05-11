#include "quadT.h"

QuadTree::QuadTree()
{
	root = nullptr;
}

void QuadTree::frmStri(string encodeStr, int &inputPosition, int level)
{
	if (root == nullptr) {
		root = new QuadNode(encodeStr[inputPosition], level++);
		root->type = encodeStr[inputPosition++];
	}
	else if (encodeStr.size() <= inputPosition)
		return;
	else {	
		
		if (encodeStr[inputPosition] == 'i') {			
			root = new QuadNode(encodeStr[inputPosition], level++);
			for (int i = 0; i < 4; i++)				
				root->quads[i] = new QuadNode(encodeStr[inputPosition++], level);			
		}
		else {			
			frmStri(encodeStr, ++inputPosition, level);
		}
	}
				
			
}


void QuadTree::printTree()
{
	if (root != nullptr) {
		cout << root->quads << "    " << root->level;

		for (int i = 0; i < 4; i++) {
			cout << root->quads[i] << "    " << root->level
			     << endl;
		}

	}
}
