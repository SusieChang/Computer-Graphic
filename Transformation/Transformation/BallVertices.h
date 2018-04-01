/*
*Ëã·¨²Î¿¼£ºhttps://blog.csdn.net/a358333644/article/details/50780130
*/
#include "imgui/imgui.h"
#include <iostream>
#include <algorithm>  
#include <glew/glfw3.h>
#include <math.h>
#include <vector>
#define PI 3.1415926  
using namespace std;
int genBallVertices(float* &ballVertices, float radius, int stack, int slice) {
	float stackStep = (float)(PI / stack);
	float sliceStep = (float)(PI / slice);
	float r0, r1, x0, x1, y0, y1, z0, z1;
	float alpha0 = 0, alpha1 = 0;
	float beta = 0;
	int index = 0;
	vector<float> coodlist;
	for (int i = 0; i < stack; i++) {
		alpha0 = (float)(-PI / 2 + (i*stackStep));
		alpha1 = (float)(-PI / 2 + ((i + 1)*stackStep));
		y0 = (float)(radius * sin(alpha0));
		r0 = (float)(radius * cos(alpha0));
		y1 = (float)(radius * sin(alpha1));
		r1 = (float)(radius * cos(alpha1));
		
		for (int j = 0; j <= (slice*2); j++) {
			beta = j * sliceStep;
			x0 = (float)(r0 * cos(beta));
			z0 = -(float)(r0 * sin(beta));
			x1 = (float)(r1 * cos(beta));
			z1 = -(float)(r1 * sin(beta));
			coodlist.push_back(x0);
			coodlist.push_back(y0);
			coodlist.push_back(z0);
			coodlist.push_back(x1);
			coodlist.push_back(y1);
			coodlist.push_back(z1);
		}
	}
	int size = coodlist.size();
	ballVertices = new float[size];
	for (int i = 0; i < size; i++) {
		ballVertices[i] = coodlist.at(i);
	}
	return size;
};
