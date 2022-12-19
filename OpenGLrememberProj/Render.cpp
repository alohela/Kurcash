#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"
static double t = 0;
static int k = 0;
static double g = 0.001;
bool a = true;
bool a1 = true;
bool textureMode = true;
bool lightMode = true;
class draw
{
public:
	float _height = 3;
	float dots[8][3] =
	{
	0, 1, _height,
	-2, 8, _height,
	-6, 4, _height,
	-2, 0, _height,
	-6, -4, _height,
	-2,-2, _height,
	2, -7, _height,
	5, 0, _height
	};
public:
	float* cross_product(float* a, float* b)
	{
		float result[] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0] };
		normalize(result);

		return result;
	}
public:
	float* calculate_normal(float* a, float* b, float* c)
	{
		float x[] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
		float y[] = { c[0] - a[0], c[1] - a[1], c[2] - a[2] };

		float* result = cross_product(x, y);

		return result;
	}

	void normalize(float* v)
	{
		float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		for (int i = 0; i < 3; i++)
			v[i] = v[i] / length;
	}

	void draw_the_thing(float height, GLuint texId)
	{
		for (int i = 0; i < 8; i++)
		{
			dots[i][2] = height;
		}
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		if (height <= 0)
			glNormal3f(0, 0, -1);
		else
			glNormal3f(0, 0, 1);

		for (int i = 0; i < 8; i++)
		{
			glVertex3fv(dots[i]);
		}
		glEnd();
	}

	void connect_sides(float height_bottom, float height_upper)
	{
		float side1[4][3] =
		{
			0, 1, height_bottom,
			-2, 8, height_bottom,
			-2, 8, height_upper,
			0, 1, height_upper
		};
			glBegin(GL_QUADS);

		glNormal3fv(calculate_normal(side1[0], side1[1], side1[2]));
		glVertex3fv(side1[0]);
		glVertex3fv(side1[1]);
		glVertex3fv(side1[2]);
		glVertex3fv(side1[3]);

		float side9[4][3] =
		{
			0, 1, height_bottom,
			-2, 4, height_bottom,
			-2, 4, height_upper,
			0, 1, height_upper
		};
		glNormal3fv(calculate_normal(side1[0], side1[1], side1[2]));
		glVertex3fv(side9[0]);
		glVertex3fv(side9[1]);
		glVertex3fv(side9[2]);
		glVertex3fv(side9[3]);
		float side2[4][3] =
		{
			-2, 8, height_bottom,
			-6, 4, height_bottom,
			-6, 4, height_upper,
			-2, 8, height_upper
		};
		glNormal3fv(calculate_normal(side2[0], side2[1], side2[2]));
		glVertex3fv(side2[0]);
		glVertex3fv(side2[1]);
		glVertex3fv(side2[2]);
		glVertex3fv(side2[3]);

		float side3[4][3] =
		{
			-6, 4, height_bottom,
			-2, 0, height_bottom,
			-2, 0, height_upper,
			-6, 4, height_upper
		};
		glNormal3fv(calculate_normal(side3[0], side3[1], side3[2]));
		glVertex3fv(side3[0]);
		glVertex3fv(side3[1]);
		glVertex3fv(side3[2]);
		glVertex3fv(side3[3]);

		float side4[4][3] =
		{
			-2, 0, height_bottom,
			-6, -4, height_bottom,
			-6, -4, height_upper,
			-2, 0, height_upper
		};
		glNormal3fv(calculate_normal(side4[0], side4[1], side4[2]));
		glVertex3fv(side4[0]);
		glVertex3fv(side4[1]);
		glVertex3fv(side4[2]);
		glVertex3fv(side4[3]);

		float side5[4][3] =
		{
			-6, -4, height_bottom,
			-2, -2, height_bottom,
			-2, -2, height_upper,
			-6, -4, height_upper
		};
		glNormal3fv(calculate_normal(side5[0], side5[1], side5[2]));
		glVertex3fv(side5[0]);
		glVertex3fv(side5[1]);
		glVertex3fv(side5[2]);
		glVertex3fv(side5[3]);

		float side6[4][3] =
		{
			-2, -2, height_bottom,
			2, -7, height_bottom,
			2, -7, height_upper,
			-2, -2, height_upper
		};
		glNormal3fv(calculate_normal(side6[0], side6[1], side6[2]));
		glVertex3fv(side6[0]);
		glVertex3fv(side6[1]);
		glVertex3fv(side6[2]);
		glVertex3fv(side6[3]);

		float side7[4][3] =
		{
			2, -7, height_bottom,
			5, 0, height_bottom,
			5, 0, height_upper,
			2, -7, height_upper
		};

		glNormal3fv(calculate_normal(side7[0], side7[1], side7[2]));
		glVertex3fv(side7[0]);
		glVertex3fv(side7[1]);
		glVertex3fv(side7[2]);
		glVertex3fv(side7[3]);

		glEnd();
	}

	void draw_in3d(float height_bottom, float height_upper, GLuint texId)
	{
		draw_the_thing(height_bottom, texId);
		draw_the_thing(height_upper, texId);
		connect_sides(height_bottom, height_upper);
	}
};

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;


	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
			camDist * cos(fi2) * sin(fi1),
			camDist * sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}


	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);


		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale * 0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale * 1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
	}


	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}


}

void mouseWheelEvent(OpenGL* ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;

}

void keyDownEvent(OpenGL* ogl, int key)
{
	if (key == 'S') {
		a = !a;
	}
	if (key == 'A') {
		a1 = !a1;
	}
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL* ogl, int key)
{

}



GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL* ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);


	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE* texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture1.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);



	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}

inline double f(double p1, double p2, double p3, double p4, double t)
{
	return p1 * (1 - t) * (1 - t) * (1 - t) + 3 * p2 * t * (1 - t) * (1 - t) + 3 * p3 * t * t * (1 - t) + p4 * t * t * t; //Кривая Безье
}

void normalize(float* v)
{
	float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	for (int i = 0; i < 3; i++)
		v[i] = v[i] / length;
}

float* cross_product(float* a, float* b)
{
	float result[] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0] };
	normalize(result);

	return result;
}
	float* calculate_normal(float* a, float* b, float* c)
	{
		float x[] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
		float y[] = { c[0] - a[0], c[1] - a[1], c[2] - a[2] };

		float* result = cross_product(x, y);

		return result;
	}
	void function(double s, double s1) {
		for (s = 0; s < 1; s += 0.01) {
			glLineWidth(3);
			glBegin(GL_POLYGON);
			if (s > 0 && s < 1) {
				float side8[4][3] =
				{
					5, 0, 0,
					0, 1, 0,
					0, 1, 1,
					5, 0, 1
				};

				glNormal3fv(calculate_normal(side8[0], side8[1], side8[2]));
				glVertex3fv(side8[0]);
				glVertex3fv(side8[1]);
				glVertex3fv(side8[2]);
				glVertex3fv(side8[3]);

			}
			double P1[] = { 0,1,s };
			double P2[] = { 2,3,s };
			double P3[] = { 4,3,s };
			double P4[] = { 5,0,s };
			double P[3];
			for (double t = 0; t <= 1.0001; t += 0.01)
			{
				P[0] = f(P1[0], P2[0], P3[0], P4[0], t);
				P[1] = f(P1[1], P2[1], P3[1], P4[1], t);
				P[2] = f(P1[2], P2[2], P3[2], P4[2], t);

				glVertex3dv(P);
			}
			glEnd();

		}
	}
	void function2(double s) {
		glBegin(GL_POLYGON);
		if (s == 1)
			glNormal3d(0, 0, 1);
		else
			glNormal3d(0, 0, -1);
		double P1[] = { 0,1,s };
		double P2[] = { 2,3,s };
		double P3[] = { 4,3,s };
		double P4[] = { 5,0,s };
		double P[3];
		for (double t = 0; t <= 1.0001; t += 0.01)
		{
			P[0] = f(P1[0], P2[0], P3[0], P4[0], t);
			P[1] = f(P1[1], P2[1], P3[1], P4[1], t);
			P[2] = f(P1[2], P2[2], P3[2], P4[2], t);

			glVertex3dv(P);
		}
		glEnd();
	}


void Render(OpenGL* ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  
	

	//Начало рисования квадратика станкина
	double A[2] = { -2, -2 };
	double B[2] = { 2, -7 };
	double C[2] = { 5, 0 };
	double D[2] = { 0, 1 };
	double P1[] = { 0,0,0 };
	double P2[] = { 4,0,-3 };
	double P3[] = { 8,0,-3 };
	double P4[] = { 12,0,0 };
	double P[3];
	static double time = 0;
	P[0] = f(P1[0], P2[0], P3[0], P4[0], t);
	P[1] = f(P1[1], P2[1], P3[1], P4[1], t);
	P[2] = f(P1[2], P2[2], P3[2], P4[2], t);

	glLineWidth(3);
	glBegin(GL_LINE_STRIP);

	for (double k = 0; k <= 1.0001; k += 0.01)
	{
		double G[3];
		G[0] = f(P1[0], P2[0], P3[0], P4[0], k);
		G[1] = f(P1[1], P2[1], P3[1], P4[1], k);
		G[2] = f(P1[2], P2[2], P3[2], P4[2], k);


		glVertex3dv(G);
	}
	glEnd();
	glPushMatrix();
	glTranslated(P[0], P[1], P[2]);
	glRotated(time/15.0, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.6, 0.6, 0.);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, -1);
	glTexCoord2d(1, 1);
	glVertex2dv(A);
	glTexCoord2d(2, 1);
	glVertex2dv(B);
	glTexCoord2d(2, 2);
	glVertex2dv(C);
	glTexCoord2d(1, 2);
	glVertex2dv(D);

	glEnd();
	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);
	glTexCoord2d(1, 1);
	glVertex3d(-2, -2, 1);
	glTexCoord2d(2, 1);
	glVertex3d(2, -7, 1);
	glTexCoord2d(2, 2);
	glVertex3d(5, 0, 1);
	glTexCoord2d(1, 2);
	glVertex3d(0, 1, 1);

	glEnd();
	double s = 0;






	glLineWidth(3);

	//конец рисования квадратика станкина
	function(0, 1);
	function2(0);
	function2(1);
	draw figure;
	figure.draw_in3d(0,1,texId);
	glPopMatrix();
	if (a1==true)
	time += 2;
	if (a == true) {
		t += g;
		if (t >= 1)
			g = -0.001;
		if (t <= 0) {
			g = 0.001;
		}
	}
	
   //Сообщение вверху экрана


	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
									//(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;

	ss << "A - вкл/выкл Rotated S - вкл/выкл Translated" << std::endl;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}