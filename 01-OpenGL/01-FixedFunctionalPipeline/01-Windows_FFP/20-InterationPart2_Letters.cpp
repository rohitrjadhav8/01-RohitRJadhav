#include"HeadersAndLetter.h"

#define NumPoints 9000


void VerticalLine(GLfloat fx, GLfloat fy, GLfloat fEnd, int iStatus) {

	static GLfloat add = 0.00f;

	switch (iStatus) {
	case 1:
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx, fy - add);

		add = add + 0.01f;
		if (add >= fEnd)
			add = 0.0f;
		glEnd();
		break;
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx, fy);
		glVertex2f(fx, fy - fEnd);
		glEnd();
		break;
	}
}


void VerticalLine_Small(GLfloat fx, GLfloat fy, GLfloat fEnd, int iStatus) {

	static GLfloat add = 0.00f;
	glLineWidth(3.0f);
	switch (iStatus) {
	case 1:
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx, fy - add);

		add = add + 0.01f;
		if (add >= fEnd)
			add = 0.0f;
		glEnd();
		break;
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx, fy);
		glVertex2f(fx, fy - fEnd);
		glEnd();
		break;
	}
}


void Three_HorizontalLine(GLfloat x, GLfloat y, GLfloat fEnd, int iStatus) {

	static GLfloat add = 0.00f;
	glLineWidth(3.0f);

	switch (iStatus) {
	case 1:
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(x + add, y);
		glVertex2f(x + add, 0.0f);
		glVertex2f(x + add, -y);

		add = add + 0.01f;
		if (add >= fEnd)
			add = 0.0f;
		glEnd();
		break;
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(x, y);
		glVertex2f(x + fEnd, y);

		glVertex2f(x, 0.0f);
		glVertex2f(x + fEnd, 0.0f);

		glVertex2f(x, -y);
		glVertex2f(x + fEnd, -y);

		glEnd();
		break;
	}
}

void Curve(GLfloat fRadius, int iStatus, int iOrientation) {

	static GLfloat angle = 92.68f;
	GLfloat x, y;

	glPointSize(3.0f);
	switch (iStatus) {
	case 1:

		if (iOrientation == 1) {
			
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);

			if (angle <= 95.800f) {
				x = (GLfloat)(fRadius * cos(angle));
				y = (GLfloat)(fRadius * sin(angle));
				//fprintf(gbFile, "angle: %f\tx: %f\ty:%f\n", angle,x,y);
				glVertex2f(x, y);
				angle = angle + 0.01f;
			}
			else
				angle = 92.680f;

			glEnd();
		}
		else if (iOrientation == 2) {
			
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);

			if (angle >= 89.5600f) {
				x = (GLfloat)(fRadius * cos(angle));
				y = (GLfloat)(fRadius * sin(angle));
				//fprintf(gbFile, "angle: %f\tx: %f\ty:%f\n", angle,x,y);
				glVertex2f(x, y);
				angle = angle - 0.01f;
			}
			else
				angle = 92.680f;

			glEnd();

		}
		break;
	case 2:
		if (iOrientation == 1) {
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (angle = 92.68f; angle <= 95.8f; angle = angle + 0.002f) {
				x = (GLfloat)(fRadius * cos(angle));
				y = (GLfloat)(fRadius * sin(angle));
				//fprintf(gbFile, "angle: %f\tx: %f\ty:%f\n", angle,x,y);
				glVertex2f(x, y);
			}
			glEnd();
		}
		else if (iOrientation == 2) {
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (angle = 92.68f; angle >= 89.56f; angle = angle - 0.002f) {
				x = (GLfloat)(fRadius * cos(angle));
				y = (GLfloat)(fRadius * sin(angle));
				//fprintf(gbFile, "angle: %f\tx: %f\ty:%f\n", angle,x,y);
				glVertex2f(x, y);
			}
			glEnd();

		}

		break;
	}
}

void SlantLine(GLfloat fx, GLfloat fy, GLfloat fXEnd, GLfloat fYEnd, int iStatus, int iOrientation) {

	static GLfloat x = 0.00f, y = 0.00f;
	glLineWidth(3.0f);

	switch (iStatus) {
	case 1:
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		if (x <= fXEnd && y <= fYEnd) {
			glVertex2f(fx + x, fy - y);
			x = x + 0.01f;
			y = y + 0.01f;
		}
		else {
			x = 0.0f;
			y = 0.0f;
		}
		glEnd();
		break;
	case 2:
		if (iOrientation == 1) {
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);

			glVertex2f(fx, fy);
			glVertex2f(fx + fXEnd, fy - fYEnd);

			glEnd();
		}
		else if (iOrientation == 2) {
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);

			glVertex2f(fx, fy);
			glVertex2f(fx - fXEnd, fy - fYEnd);

			glEnd();
		}
		break;
	}
}

void HorizontalLine(GLfloat fx, GLfloat fy, GLfloat fEnd, int iStatus) {
	static GLfloat add = 0.00f;
	glLineWidth(3.0f);

	switch (iStatus) {
	case 1:
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx + add, fy);

		add = add + 0.01f;
		if (add >= fEnd)
			add = 0.0f;
		glEnd();
		break;
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex2f(fx, fy);
		glVertex2f(fx + fEnd, fy);
		glEnd();
		break;
	}

}

void My_O(GLfloat fRadius, int iFlag) {

	static GLfloat angle = 0.0f;
	glPointSize(3.0f);
	switch (iFlag) {
	case 1:
		if (angle <= 2.0f * M_PI) {
			
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2f((GLfloat)(fRadius * cos(angle)), (GLfloat)(fRadius * sin(angle)));
			angle = angle + 0.01f;
			glEnd();
		}
		break;
	case 2:
		glBegin(GL_POINTS);
		for (angle = 0.0f; angle <= 2.0f * M_PI; angle = angle + 0.01f) {
			glVertex2f((GLfloat)(fRadius * cos(angle)), (GLfloat)(fRadius * sin(angle)));
		}
		glEnd();
		break;
	}
}

void My_V(GLfloat x, GLfloat y, GLfloat fWidth, GLfloat fDepth, int iFlag) {

	GLfloat x1, y1;
	GLfloat x2, y2;

	switch (iFlag) {
	case 1:
		x1 = x - fWidth / 2.0f;
		y1 = y - fDepth;
		x2 = x + fWidth / 2.0f;
		y2 = y - fDepth;
		break;

	case 2:
		x1 = x - fWidth / 2.0f;
		y1 = y + fDepth;
		x2 = x + fWidth / 2.0f;
		y2 = y + fDepth;
		break;
	case 3:
		x1 = x + fDepth;
		y1 = y + fWidth / 2.0f;
		x2 = x + fDepth;
		y2 = y - fWidth / 2.0f;
		break;
	}
	glLineWidth(3.0f);

	glBegin(GL_LINES);


	glVertex2f(x1, y1);
	glVertex2f(x, y);

	glVertex2f(x, y);
	glVertex2f(x2, y2);
	glEnd();
}


void My_I(void) {
	//glLineWidth(2.0f);
	HorizontalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.0f, 0.25, 0.50f, 2);
	HorizontalLine(-0.2f, -0.2f, 0.40f, 2);
}

void My_N(void) {
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.2f, 0.2f, 0.40f, 2);
	SlantLine(-0.2f, 0.2f, 0.3f, 0.3f, 2, 1);
}

void My_T(void) {
	HorizontalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.0f, 0.25, 0.50f, 2);
}

void My_E(void) {
	VerticalLine(-0.20f, 0.2f, 0.40f, 2);
	Three_HorizontalLine(-0.20f, 0.2f, 0.2f, 2);
}

void My_R(void) {
	VerticalLine(-0.20f, 0.26f, 0.52f, 2);
	SlantLine(-0.20f, 0.0f, 0.2f, 0.2f, 2, 1);
	//glPointSize(1.0f);
	glTranslatef(-0.16f, 0.130f, 0.0f);
	Curve(0.13f, 2, 1);
}

void My_A(void) {

	SlantLine(0.0f, 0.2f, 0.2f, 0.4f, 2, 1);
	SlantLine(0.0f, 0.2f, 0.2f, 0.4f, 2, 2);
	HorizontalLine(-0.20f, 0.0f, 0.42f, 2);
}

void My_C(void) {
	Curve(0.2f, 2, 2);
}

void My_W(void) {
	My_V(0.0f, 0.0f, 0.4f, 0.2f, 1);
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.2f, 0.2f, 0.40f, 2);
}

void My_D(void) {
	VerticalLine(0.0f, 0.2f, 0.40f, 2);
	Curve(0.2f, 2, 1);
}

void My_S(void) {
	Curve(0.2f, 2, 2);
	glTranslatef(-0.250f, -0.2f, 0.0f);
	Curve(0.2f, 2, 1);
}

void My_M(void) {
	My_V(0.0f, 0.0f, 0.4f, 0.2f, 2);
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.2f, 0.2f, 0.40f, 2);
}


void My_B(void) {
	VerticalLine(0.0f, 0.25f, 0.5f, 2);
	glTranslatef(0.050f, 0.12f, 0.0f);
	Curve(0.12f, 2, 1);
	glTranslatef(0.0f, -0.24f, 0.0f);
	Curve(0.12f, 2, 1);
}

void My_H(void) {
	VerticalLine(-0.15f, 0.2f, 0.40f, 2);
	HorizontalLine(-0.21f, 0.0f, 0.42f, 2);
	VerticalLine(0.15f, 0.2f, 0.40f, 2);
}

void My_J(void) {
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	HorizontalLine(-0.35f, 0.2f, 0.3f, 2);
	SlantLine(-0.2f, -0.2f, 0.15f, 0.15f, 2, 2);
	VerticalLine(-0.4f, -0.20f, 0.2f, 2);
}

void My_K(void) {
	My_V(-0.20f, 0.0f, 0.4f, 0.2f, 3);
	VerticalLine(-0.2f, 0.25f, 0.50f, 2);
}

void My_L(void) {
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	HorizontalLine(-0.2f, -0.2f, 0.2f, 2);
}

void My_Interact(GLfloat x, GLfloat y) {
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(x + 0.0f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_I();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.020f, 0.0f - y, -0.200f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_N();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.040f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.060f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_E();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0725f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_R();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0775f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_A();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.095f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_C();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.10f, 0.0f - y, -0.20f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();
	HorizontalLine(-2.2f, 0.0f, 2.50f, 2);
}


void My_Rotate(GLfloat x, GLfloat y) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_R();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.008f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_O(0.15f, 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0180f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0300f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_A();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0420f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.05500f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_E();
}

void My_Move(GLfloat x, GLfloat y) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x - 0.030f, 0.0f - y, -0.250f);

	glScalef(0.05f, 0.05f, 0.0f);
	My_V(0.0f, 0.0f, 0.4f, 0.2f, 2);
	VerticalLine(-0.2f, 0.2f, 0.40f, 2);
	VerticalLine(0.2f, 0.2f, 0.40f, 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x - 0.020f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_O(0.2f, 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x - 0.0100f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_V(0.0f, -0.20f, 0.4f, 0.4f, 2);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0100f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_E();
}


void My_Key(GLfloat x, GLfloat y) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_K();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.010f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_E();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.01f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_V(0.0f, 0.0f, 0.3f, 0.2f, 2);
	VerticalLine(0.0f, 0.0f, 0.2f, 2);

}

void My_To(GLfloat x, GLfloat y) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_O(0.2f, 2);
}

void My_RohitJadhav(GLfloat x, GLfloat y) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.0f, 0.0f - y, -5.0f);
	My_R();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (0.10f), 0.0f - y, -5.0f);
	My_O(0.15f, 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.250f, 0.0f - y, -5.0f);
	My_H();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.450f, 0.0f - y, -5.0f);
	My_I();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.60f, -0.10f - y, -5.0f);
	My_T();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.20f, 0.0f - y, -5.0f);
	My_R();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.80f, 0.0f - y, -5.0f);
	My_J();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.70f, 0.0f - y, -5.0f);
	My_A();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.90f, 0.0f - y, -5.0f);
	My_D();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.20f, 0.0f - y, -5.0f);
	My_H();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.50f, 0.0f - y, -5.0f);
	My_A();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.80f, 0.0f - y, -5.0f);
	My_V(0.0f, -0.20f, 0.4f, 0.4f, 2);
	//HorizontalLine(-3.20f, 0.0f, 3.50f, 2);

}

void My_VikasKamble(GLfloat x, GLfloat y) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.00f, 0.0f - y, -5.0f);
	My_V(0.0f, -0.20f, 0.4f, 0.4f, 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.20f, 0.0f - y, -5.0f);
	My_I();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.50f, 0.0f - y, -5.0f);
	My_K();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.60f, 0.0f - y, -5.0f);
	My_A();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.0f, 0.0f - y, -5.0f);
	My_S();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.50f, 0.0f - y, -5.0f);
	My_K();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.550f, 0.0f - y, -5.0f);
	My_A();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.850f, 0.0f - y, -5.0f);
	My_M();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.00f, 0.0f - y, -5.0f);
	My_B();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.40f, 0.0f - y, -5.0f);
	My_L();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 2.50f, 0.0f - y, -5.0f);
	My_E();
	//HorizontalLine(-3.20f, 0.0f, 3.50f, 2);

}

void My_Render(GLfloat x, GLfloat y) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 1.50f, 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_R();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (1.250f), 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_E();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (1.0f), 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_N();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (0.7f), 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_D();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (0.250f), 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_E();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + (-0.050f), 0.0f - y, -5.0f);
	glRotatef(180.0f, 0.0f, 1.0, 0.0f);
	My_R();

}


void My_Continue(GLfloat x, GLfloat y) {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.00f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_C();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.010f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_O(0.2f, 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.025f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_N();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.035f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_T();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.043f, -0.0050f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_I();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.060f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_N();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.070f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_L();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + 0.080f, 0.0f - y, -0.250f);
	glScalef(0.05f, 0.05f, 0.0f);
	My_E();


}

