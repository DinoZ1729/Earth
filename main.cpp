#include <iostream>
#include <cstring>
#include <unistd.h>
#include <windows.h>
#include <fstream>
#include <windows.h>
#include "functions.h"
#define PI 3.14159265358979323846
using namespace std;

//screen dimensions

#define WIDTH 800
#define HEIGHT 608

//WIDTH and height of each character in pixels
const int dW=4,dH=8;

//set cursor at start to avoid flickering
void gotoxy ( short x, short y )
{
COORD coord = {x, y};
SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), coord );
}
char palette[]=" .:;',wiogOLXHWYV@";
int findIndex(char c,char s[])
{
	for(int i=0;i<strlen(s);i++)
	{
		if(c==s[i])	return i;
	}
	return -1;
}

void drawPoint(char canvas[HEIGHT/dH][WIDTH/dW],int A,int B,char c)
{
	if(A<0||B<0||A>=WIDTH/dW||B>=HEIGHT/dH)	return;
	canvas[B][A]=c;
}



class camera
{
public:
	double x,y,z;
	double matrix[16],inv[16];
	camera(double r,double alfa,double beta)
	{
		//alfa is camera's angle along the xy plane.
		//beta is camera's angle along z axis
		//r is the distance from the camera to the origin
		double a=sin(alfa),b=cos(alfa),c=sin(beta),d=cos(beta);
		x=r*b*d;
		y=r*a*d;
		z=r*c;
		
		//matrix
		matrix[3]=matrix[7]=matrix[11]=0;
		matrix[15]=1;
		//x
		matrix[0]=-a;
		matrix[1]=b;
		matrix[2]=0;
		//y
		matrix[4]=b*c;
		matrix[5]=a*c;
		matrix[6]=-d;
		//z
		matrix[8]=b*d;
		matrix[9]=a*d;
		matrix[10]=c;
		
		matrix[12]=x;
		matrix[13]=y;
		matrix[14]=z;
		
		//invert
		invert(inv,matrix);
	}
	int convert(int cooInt[2],double tx,double ty,double tz)
	{
		//converts from world to pixel coordinates
		//returns 0 if the point is invisible to the camera
		double vec[3]={tx,ty,tz};
		transformVector(vec,inv);
		if(vec[2]>0)	return 0;
		double xI=-vec[0]/vec[2];
		double yI=-vec[1]/vec[2];
		xI*=WIDTH/dW/2;
		yI*=WIDTH/dH/2;
		xI+=WIDTH/dW/2;
		yI+=HEIGHT/dH/2;
		int A=(int)xI,B=(int)yI;
		cooInt[0]=A;
		cooInt[1]=B;
		return 1;
	}
	//rendering
	
	void renderPoint(char canvas[HEIGHT/dH][WIDTH/dW],double tx,double ty,double tz,char c)
	{
		int vec[2];
		convert(vec,tx,ty,tz);
		drawPoint(canvas,vec[0],vec[1],c);
	}
	
	void renderSphere(char canvas[HEIGHT/dH][WIDTH/dW],double radius,double angle_offset,char earth[80][202],char earth_night[80][202])
	{
		double light[3]={0,999999,0};	//Sun
		//shoot the ray through every pixel
		for(int yi=0;yi<HEIGHT/dH;yi++){
		for(int xi=0;xi<WIDTH/dW;xi++){
			double o[3]={x,y,z};	//coordinates of the camera, origin of the ray
			double u[3]=	//u is unit vector, direction of the ray
			{
			-((double)(xi-WIDTH/dW/2)+0.5)/(double)(WIDTH/dW/2)*1.2,
			((double)(yi-HEIGHT/dH/2)+0.5)/(double)(WIDTH/dH/2),
			-1
			};
			transformVector(u,matrix);
			u[0]-=x;
			u[1]-=y;
			u[2]-=z;
			normalize(u);
			double discriminant=dot(u,o)*dot(u,o)-dot(o,o)+radius*radius;
			if(discriminant<0)	continue;		//ray doesn't hit the sphere
			double distance=-sqrt(discriminant)-dot(u,o);
			double inter[3]=		//intersection point
			{
				o[0]+distance*u[0],
				o[1]+distance*u[1],
				o[2]+distance*u[2]
			};
			
			double n[3]=		//surface normal
			{
				o[0]+distance*u[0],
				o[1]+distance*u[1],
				o[2]+distance*u[2]
			};
			normalize(n);	
			double l[3];			//unit vector pointing from intersection to light source
			vector(l,inter,light);
			normalize(l);
			double luminance=clamp(5*(dot(n,l))+0.5,0,1);
			double temp[3]={inter[0],inter[1],inter[2]};
			rotateX(temp,-PI*2*26/360);
			//computing coordinates for the sphere
			double phi=-temp[2]/radius/2+0.5, theta=atan(temp[1]/temp[0])/PI+0.5+angle_offset/2/PI;
			theta-=floor(theta);
			int earthX=(int)(theta*202),earthY=(int)(phi*80);
			int 
			day=findIndex(earth[earthY][earthX],palette),
			night=findIndex(earth_night[earthY][earthX],palette);
			int index=(int)((1.0-luminance)*night+luminance*day);
			drawPoint(canvas,xi,yi,palette[index]);
		}}
	}

};

int main()
{
	char earth[80][202];
	ifstream file1("earth.txt");
	for(int i=0;i<80;i++)
	{
		string c;
		getline(file1, c);
		for(int j=0;j<202;j++)	
		{
			earth[i][j]=c[j];
		}
	}
	char earth_night[80][202];
	ifstream file2("earth_night.txt");
	for(int i=0;i<80;i++)
	{
		string c;
		getline(file2, c);
		for(int j=0;j<202;j++)	
		{
			earth_night[i][j]=c[j];
		}
	}
	getchar();
	system("cls");
	double angle_offset=0;
	while(1)
	{
		camera cam(2,0,0);
		char canvas[HEIGHT/dH][WIDTH/dW];
		
		for(int i=0;i<HEIGHT/dH;i++){
		for(int j=0;j<WIDTH/dW;j++){
			canvas[i][j]=0;
		}}
		
		cam.renderSphere(canvas,1,angle_offset,earth,earth_night);
		//display:
		for(int i=0;i<HEIGHT/dH;i++){
		for(int j=0;j<WIDTH/dW;j++){
			printf("%c",canvas[i][j]);
		}
		printf("\n");
		}
		
		gotoxy(0,0);
		
		//update camera position
		//sleep(2);
		angle_offset+=2*PI/18;
	}
	return 0;
}
