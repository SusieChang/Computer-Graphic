//
//#include<glut\glut.h>
//#include<iostream>
//#include<math.h>
//using namespace std;
//
//GLfloat pointsize = 1.0f;
//
//void drawOneLine(GLint x, GLint y, GLint x1, GLint y1) {
//	GLint a = x;
//	GLfloat m = (y1 - y)*1.0 / (x1 - x);       //斜率
//	GLfloat b = y - m*x;
//	GLfloat thethay = m*a + b - y;             //thetha y
//	GLfloat d0 = 2 * thethay - 1;              //初始化d0
//	glPointSize(pointsize);
//	GLint cx = x, cy = y;
//	glVertex2i(x, y);                         //画第一个点
//	while (a <= x1) {
//		a++;
//		thethay = m*a + b - cy;                //更新thetha y
//		if (d0 <= 0) {                          //更新d0
//			d0 += 2 * thethay;
//			cy = cy;
//		}
//		else {
//			d0 += 2 * thethay - 2;
//			cy = cy + 1;
//		}
//		glVertex2i(a, cy);
//	}
//
//}
//void drawbresenhamcycle(GLint R) {    //默认原点是0,0 bresenham算法画圆
//	GLint a = 0;
//	GLint y = (int)(R*1.0 / (sqrt(2)));
//	GLfloat d0 = 1.25 - R;
//	GLfloat d;
//	glPointSize(pointsize);
//	GLint cx = 0, cy = R;
//	glVertex2i(0, 0);
//	while (a <= y) {
//		glVertex2i(a, cy);         //八分画圆
//		glVertex2i(-a, -cy);
//		glVertex2i(-a, cy);
//		glVertex2i(a, -cy);
//		glVertex2i(cy, -a);
//		glVertex2i(-cy, -a);
//		glVertex2i(-cy, a);
//		glVertex2i(cy, a);
//
//		a++;
//		if (d0 <= 0) {
//			d0 = d0 + 2 * a + 3;
//			cy = cy;
//		}
//		else {
//			d0 = d0 + 2 * (a - cy) + 5;
//			cy = cy - 1;
//		}
//	}
//
//
//}
//void drawsigncycle(GLint R) {         //正负法画圆
//	GLint re = 0;
//	GLint x = 0;
//	GLint y = R;
//
//	glVertex2i(0, 0);
//	while (x < (int)(R*1.0 / sqrt(2))) {
//
//		glVertex2i(x, y);
//		glVertex2i(-x, -y);
//		glVertex2i(-x, y);
//		glVertex2i(x, -y);
//		glVertex2i(y, -x);
//		glVertex2i(-y, -x);
//		glVertex2i(-y, x);
//		glVertex2i(y, x);
//		if (re <= 0) {
//			re = re + 2 * x + 1;
//			x = x + 1;
//		}
//		else {
//			re = re - 2 * y + 1;
//			y = y - 1;
//		}
//	}
//
//
//}
//
//void display(void) {          //设置园的半径
//	glClear(GL_COLOR_BUFFER_BIT);
//	glColor3f(0.0, 1.0f, 0.0f);
//	glBegin(GL_POINTS);
//
//	drawOneLine(2, 2, 100, 200);
//	drawbresenhamcycle(400);
//	drawsigncycle(200);
//
//	glEnd();
//	glFlush();
//}
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("test");
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(-400.0, 600.0, -400.0, 600.0);
//	glutDisplayFunc(display);
//	glutMainLoop();
//	return 0;
//}
//
