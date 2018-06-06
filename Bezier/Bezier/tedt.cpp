//#include<GL/GLUT.H>  
//#include <windows.h>      
//#include <math.h>      
//#include <gl/GL.h>  
//#include <iostream>
//#include<time.h>
//using namespace std;
//int num = 1;
//int flag = 0;//用来判断移动哪个控制点的标志
//			 //初始化控制点的位置
//GLfloat C[4][2] =
//{
//	{ -1.0f, 0.1f },{ -0.33f, 0.8f },{ 0.6f, -0.3f },{ 0.9f,0.7f }
//};
//
//GLfloat control[4][2] =
//{
//	{ -0.9f, 0.1f },{ -0.8f, 0.6f },{ 0.2f, 0.8f },{ 1.0f, 0.2f }
//};
//
////检测鼠标移动消息
//void motion(int x, int y)
//{
//	num = 0;
//	//将屏幕坐标转化为画图时的坐标
//	float xx = x / 400.0f - 1.0f;
//	float yy = -(y / 400.0f - 1.0f);
//	//控制台输出位置信息
//	cout << "X: " << xx << "Y: " << yy << endl;
//	if (flag == 0)
//	{
//		control[0][0] = xx;
//		control[0][1] = yy;
//
//		C[0][0] = xx;
//		C[0][1] = yy;
//	}
//	else if (flag == 1)
//	{
//		control[1][0] = xx;
//		control[1][1] = yy;
//
//		C[1][0] = xx;
//		C[1][1] = yy;
//	}
//	else if (flag == 2)
//	{
//		control[2][0] = xx;
//		control[2][1] = yy;
//
//
//		C[2][0] = xx;
//		C[2][1] = yy;
//	}
//	else if (flag == 3)
//	{
//		control[3][0] = xx;
//		control[3][1] = yy;
//
//
//		C[3][0] = xx;
//		C[3][1] = yy;
//	}
//	//重绘窗口
//	glutPostRedisplay();
//}
//
////画贝塞尔曲线
//void myDisplay(void) {
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	//绘制出四个控制点
//	glPointSize(4);//控制点大小设为4
//	glColor3f(1.0, 0.0, 0.0);//颜色设为红色
//	for (int i = 0; i < 4; i++) {
//		glBegin(GL_POINTS);
//		glVertex2fv(&control[i][0]);
//		glEnd();
//	}
//
//	//画出控制点之间的连线
//	glColor3f(0.0, 1.0, 1.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 4; i++) {
//		glVertex2fv(&control[i][0]);
//	}
//	glEnd();
//
//
//	GLfloat result[100][2];
//
//
//	//计算坐标并存在result中
//	//迭代100次
//	int u = 0;
//	for (double t = 0.0; t <= 1; t += 0.01)
//	{
//		//de Casteljau算法三阶递推公式
//		result[u][0] = control[0][0] * (1 - t)*(1 - t)*(1 - t) + control[1][0] * 3 * t*(1 - t)*(1 - t) +
//			control[2][0] * 3 * t*t*(1 - t) + control[3][0] * t*t*t;
//		result[u][1] = control[0][1] * (1 - t)*(1 - t)*(1 - t) + control[1][1] * 3 * t*(1 - t)*(1 - t) +
//			control[2][1] * 3 * t*t*(1 - t) + control[3][1] * t*t*t;
//		u++;
//	}
//
//
//	glColor3f(1.0, 1.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 100; i++)
//	{
//		glVertex2fv(result[i]);
//	}
//	glEnd();
//
//
//	glFlush();
//}
//
////键盘事件响应
//void keyboard(unsigned char key, int i, int j)
//{
//	if (key == '0')
//	{
//		flag = 0;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '1')
//	{
//		flag = 1;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '2')
//	{
//		flag = 2;
//		cout << "flag: " << flag << endl;
//	}
//	else if (key == '3')
//	{
//		flag = 3;
//		cout << "flag: " << flag << endl;
//	}
//}
//
//void motion_bezier()
//{
//
//	GLfloat C[3][2] =
//	{
//		{ -1.0f, 0.1f },{ 0.5f, 0.8f },{ 1.0f, -0.3f }
//	};
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	glLineWidth(3);
//	//绘制出控制点
//	glPointSize(6);//控制点大小设为4
//	glColor3f(1.0, 0.0, 0.0);//颜色设为红色
//	for (int i = 0; i < 3; i++) {
//		glBegin(GL_POINTS);
//		glVertex2fv(&C[i][0]);
//		glEnd();
//	}
//
//	//画出控制点之间的连线
//	glColor3f(0.0, 1.0, 1.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 3; i++) {
//		glVertex2fv(&C[i][0]);
//	}
//	glEnd();
//
//
//
//	GLfloat result[10001][2];
//	GLfloat result2[10001][2];
//	//计算坐标并存在result中
//	//迭代10000次
//	int u = 0;
//	for (double t = 0.0; t <= 1.0; t += 0.0001)
//	{
//		//de Casteljau算法二阶递推公式
//		result[u][0] = C[0][0] * (1 - t) + C[1][0] * t;
//		result[u][1] = C[0][1] * (1 - t) + C[1][1] * t;
//
//		result2[u][0] = C[1][0] * (1 - t) + C[2][0] * t;
//		result2[u][1] = C[1][1] * (1 - t) + C[2][1] * t;
//		u++;
//	}
//
//
//	glColor3f(1.0, 1.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < num; i++)
//	{
//		glVertex2fv(result[i]);
//	}
//	glEnd();
//	glColor3f(0.0, 0.0, 1.0);
//	glBegin(GL_POINTS);
//	glVertex2fv(result[num]);
//	glEnd();
//
//
//	glColor3f(1.0, 1.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < num; i++)
//	{
//		glVertex2fv(result2[i]);
//	}
//	glEnd();
//	glColor3f(0.0, 0.0, 1.0);
//	glBegin(GL_POINTS);
//	glVertex2fv(result2[num]);
//	glEnd();
//
//
//	GLfloat result3[10001][2];
//	//计算坐标并存在result2中
//	//迭代100次
//	u = 0;
//	for (double t = 0.0; t <= 1.0; t += 0.0001)
//	{
//		//de Casteljau算法二阶递推公式
//		result3[u][0] = result[u][0] * (1 - t) + result2[u][0] * t;
//		result3[u][1] = result[u][1] * (1 - t) + result2[u][1] * t;
//		u++;
//	}
//
//
//	glColor3f(1.0, 1.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < num; i++)
//	{
//		glVertex2fv(result3[i]);
//	}
//	glEnd();
//	glColor3f(0.0, 0.0, 1.0);
//	glBegin(GL_POINTS);
//	glVertex2fv(result3[num]);
//	glEnd();
//
//	glColor3f(1.0, 0.0, 0.0);
//	glBegin(GL_LINE_STRIP);
//	glVertex2fv(result[num]);
//	glVertex2fv(result2[num]);
//	glEnd();
//
//	glFlush();
//	glutSwapBuffers();
//}
//
//
//void motion_three_bezier()
//{
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	glLineWidth(3);
//	//绘制出控制点
//	glPointSize(6);//控制点大小设为4
//	glColor3f(1.0, 0.0, 0.0);//颜色设为红色
//	for (int i = 0; i < 4; i++) {
//		glBegin(GL_POINTS);
//		glVertex2fv(&C[i][0]);
//		glEnd();
//	}
//
//	//画出控制点之间的连线
//	glColor3f(0.0, 1.0, 1.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i < 4; i++) {
//		glVertex2fv(&C[i][0]);
//	}
//	glEnd();
//
//
//	//计算三阶
//	GLfloat three_order[3][10001][2];
//	int u = 0;
//	for (double t = 0.0; t <= 1.0; t += 0.0001)
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			three_order[i][u][0] = C[i][0] * (1 - t) + C[i + 1][0] * t;
//			three_order[i][u][1] = C[i][1] * (1 - t) + C[i + 1][1] * t;
//
//		}
//		u++;
//	}
//	u = 0;
//	GLfloat two_order[2][10001][2];
//	for (double t = 0.0; t <= 1.0; t += 0.0001)
//	{
//		for (int i = 0; i < 2; i++)
//		{
//			two_order[i][u][0] = three_order[i][u][0] * (1 - t) + three_order[i + 1][u][0] * t;
//			two_order[i][u][1] = three_order[i][u][1] * (1 - t) + three_order[i + 1][u][1] * t;
//		}
//		u++;
//	}
//
//	u = 0;
//	GLfloat result[10001][2];
//	for (double t = 0.0; t <= 1.0; t += 0.0001)
//	{
//		result[u][0] = two_order[0][u][0] * (1 - t) + two_order[1][u][0] * t;
//		result[u][1] = two_order[0][u][1] * (1 - t) + two_order[1][u][1] * t;
//		u++;
//	}
//
//	glColor3f(0.0, 1.0, 1.0);
//	for (int i = 0; i < 2; i++)
//	{
//		glColor3f(1.0, 1.0, 0.0);//颜色设为黄色
//		glBegin(GL_LINE_STRIP);
//		glVertex2fv(three_order[i][num]);
//		glVertex2fv(three_order[i + 1][num]);
//		glEnd();
//
//		glColor3f(1.0, 0.0, 0.0);//颜色设为红色
//		glBegin(GL_POINTS);
//		for (int i = 0; i < 3; i++)
//			glVertex2fv(three_order[i][num]);
//		glEnd();
//	}
//
//	glColor3f(0.0, 0.0, 1.0);//颜色设为蓝色
//	glBegin(GL_LINE_STRIP);
//	glVertex2fv(two_order[0][num]);
//	glVertex2fv(two_order[1][num]);
//	glEnd();
//	glColor3f(1.0, 0.0, 0.0);//颜色设为红色
//	glBegin(GL_POINTS);
//	for (int i = 0; i < 2; i++)
//		glVertex2fv(two_order[i][num]);
//	glEnd();
//
//	glColor3f(99.0, 18.0, 200.0);
//	glBegin(GL_LINE_STRIP);
//	for (int i = 0; i<num; i++)
//		glVertex2fv(result[i]);
//	glEnd();
//
//	glFlush();
//	glutSwapBuffers();
//
//}
//
//
//void myIdle(void)
//{
//
//	if (num <= 9999)
//		++num;
//	//motion_bezier();
//	motion_three_bezier();
//}
//
//
//
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 800);
//	glutCreateWindow("Bezier");
//	glutKeyboardFunc(keyboard);//响应键盘事件
//							  // glutDisplayFunc(myDisplay);//画图
//							 //  glutDisplayFunc(motion_bezier);
//	glutDisplayFunc(motion_three_bezier);
//	glutMotionFunc(motion);//响应鼠标移动事件
//	glutIdleFunc(&myIdle);
//	glutMainLoop();
//
//	return 0;
//}