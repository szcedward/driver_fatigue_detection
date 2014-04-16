#pragma once


/******
 * Simple rectangle class. Note that we are
 *
 *
 * Sam Epstein
 ******/
class Rect
{
public:
	Rect(){}
	Rect(int top, int bottom, int left, int right) 
	{
		this->top=top;
		this->left=left;
		this->right=right;
		this->bottom=bottom;
	}

	int top;
	int left;
	int right;
	int bottom;
};
