//#include <GL/glut.h>
//#include <gl/GL.h>
//#include <math.h>
//#include <windows.h>
//#include <iostream>
//#include <time.h>
//
//using namespace std;
//
//unsigned int SCR_WIDTH = 800;
//unsigned int  SCR_HEIGHT  = 800;
//
//int num = 1;
//int flag = 0;
//
//void mousemovement(int x, int y);
//void mousepress(int button, int state, int _x, int _y);
//void keyboard_callback(unsigned char key, int i, int j);
//void bezier_curve();
//void myIdle(void);
//
//GLfloat C[4][2] = {
//	{ -1.0f, 0.1f },{ -0.33f, 0.8f },{ 0.6f, -0.3f },{ 0.9f,0.7f }
//};
//GLfloat Control[4][2] = {
//	{ -0.9f, 0.1f },{ -0.8f, 0.6f },{ 0.2f, 0.8f },{ 1.0f, 0.2f }
//};
//
//void mousemovement(int xx, int yy) {
//	num = 0;
//	float x = xx / ((float)SCR_WIDTH/2) - 1.0f;
//	float y = - (yy / ((float)SCR_HEIGHT/2)- 1.0f);
//	switch (flag)
//	{
//	case 0:
//		Control[0][0] = x;
//		Control[0][1] = y;
//		C[0][0] = x;
//		C[0][1] = y;
//		break;
//	case 1:
//		Control[1][0] = x;
//		Control[1][1] = y;
//		C[1][0] = x;
//		C[1][1] = y;
//		break;
//	case 2:
//		Control[2][0] = x;
//		Control[2][1] = y;
//		C[2][0] = x;
//		C[2][1] = y;
//		break;
//	case 3:
//		Control[3][0] = x;
//		Control[3][1] = y;
//		C[3][0] = x;
//		C[3][1] = y;
//		break;
//	default:
//		break;
//	}
//	glutPostRedisplay();
//}
//
//void mousepress(int button, int state, int _x, int _y) {
//	if (GLUT_DOWN == state && button == GLUT_RIGHT_BUTTON) {
//		cout << "press" << endl;
//	}
//}
//
//void keyboard_callback(unsigned char key, int i, int j)
//{
//	switch (key)
//	{
//	case '0':
//		flag = 0;
//	case '1':
//		flag = 1;
//	case '2':
//		flag = 2;
//	case '3':
//		flag = 3;
//	default:
//		break;
//	}
//}
//
//void bezier_curve() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	glLineWidth(3);
//	//draw control point
//	glPointSize(4);
//	glColor3f(1.0, 0.0, 1.0);
//	for (int i = 0; i < 4; i++) {
//		glBegin(GL_POINTS);
//		glVertex2fv(&C[i][0]);
//		glEnd();
//	}
//
//	//draw control line
//	glColor3f(0.0, 1.0, 1.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 4; i++) {
//		glVertex2fv(&C[i][0]);
//	}
//	glEnd();
//
//	//计算三阶
//	GLfloat three_order[3][10001][2];
//	int u = 0;
//	for (double t = 0.0; t <= 1.0; t += 0.0001) {
//		for (int i = 0; i < 3; i++) {
//			three_order[i][u][0] = C[i][0] * (1 - t) + C[i + 1][0] * t;
//			three_order[i][u][1] = C[i][1] * (1 - t) + C[i + 1][1] * t;
//		}
//		u++;
//	}
//	u = 0;
//	GLfloat two_order[2][10001][2];
//	for (double t = 0.0; t <= 1.0; t += 0.0001) {
//		for (int i = 0; i < 2; i++) {
//			two_order[i][u][0] = three_order[i][u][0] * (1 - t) + three_order[i + 1][u][0] * t;
//			two_order[i][u][1] = three_order[i][u][1] * (1 - t) + three_order[i + 1][u][1] * t;
//		}
//		u++;
//	}
//	u = 0;
//	GLfloat result[10001][2];
//	for (double t = 0.0; t <= 1.0; t += 0.0001) {
//		result[u][0] = two_order[0][u][0] * (1 - t) + two_order[1][u][0] * t;
//		result[u][1] = two_order[0][u][1] * (1 - t) + two_order[1][u][1] * t;
//		u++;
//	}
//
//	glColor3f(0.0, 1.0, 1.0);
//	for (int i = 0; i < 2; i++) {
//		glColor3f(1.0, 1.0, 0.0);
//		glBegin(GL_LINE_STRIP);
//		glVertex2fv(three_order[i][num]);
//		glVertex2fv(three_order[i + 1][num]);
//		glEnd();
//
//		glColor3f(1.0, 0.0, 1.0);
//		glBegin(GL_POINTS);
//		for (int i = 0; i < 3; i++)
//			glVertex2fv(three_order[i][num]);
//		glEnd();
//	}
//
//	glColor3f(0.0, 0.0, 1.0);
//	glBegin(GL_LINE_STRIP);
//	glVertex2fv(two_order[0][num]);
//	glVertex2fv(two_order[1][num]);
//	glEnd();
//	glColor3f(1.0, 0.0, 1.0);
//	glBegin(GL_POINTS);
//	for (int i = 0; i < 2; i++)
//		glVertex2fv(two_order[i][num]);
//	glEnd();
//
//	glColor3f(0.0, 1.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i<num; i++)
//		glVertex2fv(result[i]);
//	glEnd();
//
//	glFlush();
//	glutSwapBuffers();
//}
// void myIdle(void) {
//
//	if (num <= 9999)
//		++num;
//	bezier_curve();
//}
//
// int main(int argc, char *argv[])
// {
//	 glutInit(&argc, argv);
//	 glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	 glutInitWindowPosition(100, 100);
//	 glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
//	 glutCreateWindow("Bezier");
//	 glutKeyboardFunc(keyboard_callback);
//	 glutDisplayFunc(bezier_curve);
//	 glutMotionFunc(mousemovement);//响应鼠标移动事件
//	 glutIdleFunc(&myIdle);
//	 glutMainLoop();
//
//	 return 0;
// }