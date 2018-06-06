#include <GL/glut.h>
#include <gl/GL.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

void drawLine();
void drawCurve();
void myDisplay();
void mousePress(int button, int state, int x, int y);
void mouseMove(int x, int y);
void keyboard_callback(unsigned char key, int i, int j);

vector<pair<GLfloat, GLfloat> > points;
bool point = false;
unsigned int SCR_WIDTH = 600;
int flag = 0;

//draw line
void drawLine() {
	glColor3f(1.0f, 1.0f, 0);
	glPointSize(3.0);
	for (int i = 0; i <= 2; i++) {
		glBegin(GL_LINES);
		glVertex2f(points[i].first, points[i].second);
		glVertex2f(points[i + 1].first, points[i + 1].second);
		glEnd();
	}
}

//draw curve
void drawCurve() {
	glColor3f(0, 1.0f, 0);
	glPointSize(2.0);
	for (GLfloat t = 0; t <= 1.0; t += 0.001) {
		GLfloat x = points[0].first*pow(1.0f - t, 3) + 3 * points[1].first*t*pow(1.0f - t, 2) + 3 * points[2].first*t*t*(1.0f - t) + points[3].first*pow(t, 3);
		GLfloat y = points[0].second*pow(1.0f - t, 3) + 3 * points[1].second*t*pow(1.0f - t, 2) + 3 * points[2].second*t*t*(1.0f - t) + points[3].second*pow(t, 3);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(6.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, SCR_WIDTH, 0.0, SCR_WIDTH);
	glFlush();
}

//mouse press
void mousePress(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && point<=4)
	{
		if (points.size() < 4) {
			glBegin(GL_POINTS);
			glVertex2i(x, SCR_WIDTH - y);
			glEnd();
			points.push_back(make_pair(GLfloat(x), GLfloat(SCR_WIDTH - y)));
		}
		else if (points.size() == 4) {
			drawLine();
			drawCurve();
			point = true;
		}
		glFlush();
	}
}
void mouseMove(int x, int y) {
	if (point == false) return;
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		pair<GLfloat, GLfloat> val(GLfloat(x), GLfloat(SCR_WIDTH - y));
		points.at(flag) = val;
		drawLine();
		drawCurve();
	}
}

void keyboard_callback(unsigned char key, int i, int j) {
	switch (key)
	{
	case '0':
		flag = 0;
		cout << "point: " << flag << endl;
		break;
	case '1':
		flag = 1;
		cout << "point: " << flag << endl;
		break;
	case '2':
		flag = 2;
		cout << "point: " << flag << endl;
		break;
	case '3':
		flag = 3;
		cout << "point: " << flag << endl;
		break;
	case 'C':
		flag = 4;
		cout <<"delete point"<<endl;
		points.pop_back();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCR_WIDTH, SCR_WIDTH);
	glutCreateWindow("BezierCurve");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(keyboard_callback);
	glutMouseFunc(mousePress);
	glutPassiveMotionFunc(mouseMove);
	glutMainLoop();
	return 0;
}