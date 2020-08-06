#include <cmath>
//deklaracija funkcija
void cross(double r[3],double a[3],double b[3]);
double magnitute(double r[3]);
double magnituteSquared(double r[3]);
void normalize(double r[3]);
double dot(double a[3], double b[3]);
void transformVector(double vec[3],const double m[16]);
void transformVector2(double vec[3],const double m[9]);
void invert(double inv[16],const double matrix[16]);
double clamp(double x,double min,double max);

void transformVector(double vec[3],const double m[16])
{
	double 	tx=vec[0]*m[0]+vec[1]*m[4]+vec[2]*m[8]+m[12],
			ty=vec[0]*m[1]+vec[1]*m[5]+vec[2]*m[9]+m[13],
			tz=vec[0]*m[2]+vec[1]*m[6]+vec[2]*m[10]+m[14];
	vec[0]=tx;
	vec[1]=ty;
	vec[2]=tz;
}

void invert(double inv[16],const double matrix[16])
{
	double det;
    int i;

    inv[0] = matrix[5]  * matrix[10] * matrix[15] - 
             matrix[5]  * matrix[11] * matrix[14] - 
             matrix[9]  * matrix[6]  * matrix[15] + 
             matrix[9]  * matrix[7]  * matrix[14] +
             matrix[13] * matrix[6]  * matrix[11] - 
             matrix[13] * matrix[7]  * matrix[10];

    inv[4] = -matrix[4]  * matrix[10] * matrix[15] + 
              matrix[4]  * matrix[11] * matrix[14] + 
              matrix[8]  * matrix[6]  * matrix[15] - 
              matrix[8]  * matrix[7]  * matrix[14] - 
              matrix[12] * matrix[6]  * matrix[11] + 
              matrix[12] * matrix[7]  * matrix[10];

    inv[8] = matrix[4]  * matrix[9] * matrix[15] - 
             matrix[4]  * matrix[11] * matrix[13] - 
             matrix[8]  * matrix[5] * matrix[15] + 
             matrix[8]  * matrix[7] * matrix[13] + 
             matrix[12] * matrix[5] * matrix[11] - 
             matrix[12] * matrix[7] * matrix[9];

    inv[12] = -matrix[4]  * matrix[9] * matrix[14] + 
               matrix[4]  * matrix[10] * matrix[13] +
               matrix[8]  * matrix[5] * matrix[14] - 
               matrix[8]  * matrix[6] * matrix[13] - 
               matrix[12] * matrix[5] * matrix[10] + 
               matrix[12] * matrix[6] * matrix[9];

    inv[1] = -matrix[1]  * matrix[10] * matrix[15] + 
              matrix[1]  * matrix[11] * matrix[14] + 
              matrix[9]  * matrix[2] * matrix[15] - 
              matrix[9]  * matrix[3] * matrix[14] - 
              matrix[13] * matrix[2] * matrix[11] + 
              matrix[13] * matrix[3] * matrix[10];

    inv[5] = matrix[0]  * matrix[10] * matrix[15] - 
             matrix[0]  * matrix[11] * matrix[14] - 
             matrix[8]  * matrix[2] * matrix[15] + 
             matrix[8]  * matrix[3] * matrix[14] + 
             matrix[12] * matrix[2] * matrix[11] - 
             matrix[12] * matrix[3] * matrix[10];

    inv[9] = -matrix[0]  * matrix[9] * matrix[15] + 
              matrix[0]  * matrix[11] * matrix[13] + 
              matrix[8]  * matrix[1] * matrix[15] - 
              matrix[8]  * matrix[3] * matrix[13] - 
              matrix[12] * matrix[1] * matrix[11] + 
              matrix[12] * matrix[3] * matrix[9];

    inv[13] = matrix[0]  * matrix[9] * matrix[14] - 
              matrix[0]  * matrix[10] * matrix[13] - 
              matrix[8]  * matrix[1] * matrix[14] + 
              matrix[8]  * matrix[2] * matrix[13] + 
              matrix[12] * matrix[1] * matrix[10] - 
              matrix[12] * matrix[2] * matrix[9];

    inv[2] = matrix[1]  * matrix[6] * matrix[15] - 
             matrix[1]  * matrix[7] * matrix[14] - 
             matrix[5]  * matrix[2] * matrix[15] + 
             matrix[5]  * matrix[3] * matrix[14] + 
             matrix[13] * matrix[2] * matrix[7] - 
             matrix[13] * matrix[3] * matrix[6];

    inv[6] = -matrix[0]  * matrix[6] * matrix[15] + 
              matrix[0]  * matrix[7] * matrix[14] + 
              matrix[4]  * matrix[2] * matrix[15] - 
              matrix[4]  * matrix[3] * matrix[14] - 
              matrix[12] * matrix[2] * matrix[7] + 
              matrix[12] * matrix[3] * matrix[6];

    inv[10] = matrix[0]  * matrix[5] * matrix[15] - 
              matrix[0]  * matrix[7] * matrix[13] - 
              matrix[4]  * matrix[1] * matrix[15] + 
              matrix[4]  * matrix[3] * matrix[13] + 
              matrix[12] * matrix[1] * matrix[7] - 
              matrix[12] * matrix[3] * matrix[5];

    inv[14] = -matrix[0]  * matrix[5] * matrix[14] + 
               matrix[0]  * matrix[6] * matrix[13] + 
               matrix[4]  * matrix[1] * matrix[14] - 
               matrix[4]  * matrix[2] * matrix[13] - 
               matrix[12] * matrix[1] * matrix[6] + 
               matrix[12] * matrix[2] * matrix[5];

    inv[3] = -matrix[1] * matrix[6] * matrix[11] + 
              matrix[1] * matrix[7] * matrix[10] + 
              matrix[5] * matrix[2] * matrix[11] - 
              matrix[5] * matrix[3] * matrix[10] - 
              matrix[9] * matrix[2] * matrix[7] + 
              matrix[9] * matrix[3] * matrix[6];

    inv[7] = matrix[0] * matrix[6] * matrix[11] - 
             matrix[0] * matrix[7] * matrix[10] - 
             matrix[4] * matrix[2] * matrix[11] + 
             matrix[4] * matrix[3] * matrix[10] + 
             matrix[8] * matrix[2] * matrix[7] - 
             matrix[8] * matrix[3] * matrix[6];

    inv[11] = -matrix[0] * matrix[5] * matrix[11] + 
               matrix[0] * matrix[7] * matrix[9] + 
               matrix[4] * matrix[1] * matrix[11] - 
               matrix[4] * matrix[3] * matrix[9] - 
               matrix[8] * matrix[1] * matrix[7] + 
               matrix[8] * matrix[3] * matrix[5];

    inv[15] = matrix[0] * matrix[5] * matrix[10] - 
              matrix[0] * matrix[6] * matrix[9] - 
              matrix[4] * matrix[1] * matrix[10] + 
              matrix[4] * matrix[2] * matrix[9] + 
              matrix[8] * matrix[1] * matrix[6] - 
              matrix[8] * matrix[2] * matrix[5];

    det = matrix[0] * inv[0] + matrix[1] * inv[4] + matrix[2] * inv[8] + matrix[3] * inv[12];

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        inv[i] *= det;
}

void cross(double r[3],double a[3],double b[3])
{
	r[0]=a[1]*b[2]-a[2]*b[1];
	r[1]=a[2]*b[0]-a[0]*b[2];
	r[2]=a[0]*b[1]-a[1]*b[0];
}

double magnitute(double r[3])
{
	return sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
}

void normalize(double r[3])
{
	double len=magnitute(r);
	r[0]/=len;
	r[1]/=len;
	r[2]/=len;
}

double dot(double a[3], double b[3])
{
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
void vector(double a[3],double b[3],double c[3])
{
	a[0]=b[0]-c[0];
	a[1]=b[1]-c[1];
	a[2]=b[2]-c[2];
}

void transformVector2(double vec[3],const double m[9])
{
	double 
	x=m[0]*vec[0]+m[1]*vec[1]+m[2]*vec[2],
	y=m[3]*vec[0]+m[4]*vec[1]+m[5]*vec[2],
	z=m[6]*vec[0]+m[7]*vec[1]+m[8]*vec[2];
	vec[0]=x;
	vec[1]=y;
	vec[2]=z;
}

void rotateX(double vec[3],double theta)
{
	double a=sin(theta),b=cos(theta);
	double m[9]=
	{
		1,0,0,
		0,b,-a,
		0,a,b
	};
	transformVector2(vec,m);
}
void rotateY(double vec[3],double theta)
{
	double a=sin(theta),b=cos(theta);
	double m[9]=
	{
		b,0,a,
		0,1,0,
		-a,0,b
	};
	transformVector2(vec,m);
}
void rotateZ(double vec[3],double theta)
{
	double a=sin(theta),b=cos(theta);
	double m[9]=
	{
		b,-a,0,
		a,b,0,
		0,0,1
	};
	transformVector2(vec,m);
}

double clamp(double x,double min,double max)
{
    if (x < min)
        x = min;
    else if (x > max)
        x = max;
    return x;
}
