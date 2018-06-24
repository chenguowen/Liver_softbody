#include "particlelib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <GL/freeglut.h>

using namespace std;
/*
Update the forces on each particle
*/
void CalculateForces(PARTICLE *p,int np,PARTICLEPHYS phys,PARTICLESPRING *s,int ns)
{
	//cout<<"CalculateForces"<<endl;
	int i,p1,p2;
	XYZ down = {0.0,0.0,-1.0};
	XYZ zero = {0.0,0.0,0.0};
	XYZ f;
	double len,dx,dy,dz;

	//cout<<"springs[0].frommydisplay2="<<s[0].from<<endl;

	for (i=0;i<np;i++) {
		p[i].f = zero;
		if (p[i].fixed)
			continue;

		/* Gravitation */
		p[i].f.x += phys.gravitational * p[i].m * down.x;
		p[i].f.y += phys.gravitational * p[i].m * down.y;
		p[i].f.z += phys.gravitational * p[i].m * down.z;

		/* Viscous drag */
		p[i].f.x -= phys.viscousdrag * p[i].v.x;
		p[i].f.y -= phys.viscousdrag * p[i].v.y;
		p[i].f.z -= phys.viscousdrag * p[i].v.z;
	}

	/* Handle the spring interaction */
	for (i=0;i<ns;i++) {
		p1 = s[i].from;
		p2 = s[i].to;
		dx = p[p1].p.x - p[p2].p.x;
		dy = p[p1].p.y - p[p2].p.y;
		dz = p[p1].p.z - p[p2].p.z;
		len = sqrt(dx*dx + dy*dy + dz*dz);
		f.x  = s[i].springconstant  * (len - s[i].restlength);
		f.x += s[i].dampingconstant * (p[p1].v.x - p[p2].v.x) * dx / len;
		f.x *= - dx / len;
		f.y  = s[i].springconstant  * (len - s[i].restlength);
		f.y += s[i].dampingconstant * (p[p1].v.y - p[p2].v.y) * dy / len;
		f.y *= - dy / len;
		f.z  = s[i].springconstant  * (len - s[i].restlength);
		f.z += s[i].dampingconstant * (p[p1].v.z - p[p2].v.z) * dz / len;
		f.z *= - dz / len;
		if (!p[p1].fixed) {
			//cout<<"not fixed"<<endl;
			p[p1].f.x += f.x;
			p[p1].f.y += f.y;
			p[p1].f.z += f.z;
		}
		if (!p[p2].fixed) {
			p[p2].f.x -= f.x;
			p[p2].f.y -= f.y;
			p[p2].f.z -= f.z;
		}
	}
}

/*
Perform one step of the solver
*/
void UpdateParticles(PARTICLE *p,int np,PARTICLEPHYS phys,PARTICLESPRING *s,int ns,double dt,int method)
{
	//cout<<"UpdataParticles"<<endl;
	int i;
	PARTICLE *ptmp;
	PARTICLEDERIVATIVES *deriv;

	deriv = (PARTICLEDERIVATIVES *)malloc( np * sizeof(PARTICLEDERIVATIVES)); // 

	switch (method) {
	case 0:                                   /* Euler */
		CalculateForces(p,np,phys,s,ns);
		CalculateDerivatives(p,np,deriv);
		for ( i = 0 ; i < np ; i++ ) 
		{ 
			if(MF_DEBUG==1)
			{
				cout<<"deriv[i].dpdt.x * dt = "<<deriv[i].dpdt.x * dt << endl ;
				cout<<"p[i].p.x = "<<p[i].p.x<<endl ; 
			}
			p[i].p.x += deriv[i].dpdt.x * dt;
			p[i].p.y += deriv[i].dpdt.y * dt;
			p[i].p.z += deriv[i].dpdt.z * dt;
			if(MF_DEBUG==1)
			{
				cout<<p[i].p.x<<endl; // 
				cout<<p[i].p.y<<endl; // 
				cout<<p[i].p.z<<endl; // 
				system("pause");
			}
			p[i].v.x += deriv[i].dvdt.x * dt;
			p[i].v.y += deriv[i].dvdt.y * dt;
			p[i].v.z += deriv[i].dvdt.z * dt;
		}
		break;
	case 1:                                   /* Midpoint */
		CalculateForces(p,np,phys,s,ns);
		CalculateDerivatives(p,np,deriv);
		ptmp = (PARTICLE *)malloc(np * sizeof(PARTICLE));
		for (i=0;i<np;i++) {
			ptmp[i] = p[i];
			ptmp[i].p.x += deriv[i].dpdt.x * dt / 2;
			ptmp[i].p.y += deriv[i].dpdt.y * dt / 2;
			ptmp[i].p.z += deriv[i].dpdt.z * dt / 2;
			ptmp[i].p.x += deriv[i].dvdt.x * dt / 2;
			ptmp[i].p.y += deriv[i].dvdt.y * dt / 2;
			ptmp[i].p.z += deriv[i].dvdt.z * dt / 2;
		}
		CalculateForces(ptmp,np,phys,s,ns);
		CalculateDerivatives(ptmp,np,deriv);
		for (i=0;i<np;i++) {
			p[i].p.x += deriv[i].dpdt.x * dt;
			p[i].p.y += deriv[i].dpdt.y * dt;
			p[i].p.z += deriv[i].dpdt.z * dt;
			p[i].v.x += deriv[i].dvdt.x * dt;
			p[i].v.y += deriv[i].dvdt.y * dt;
			p[i].v.z += deriv[i].dvdt.z * dt;
		}
		free(ptmp);
		break;
	}

	free(deriv);
}

/*
Calculate the derivatives
dp/dt = v
dv/dt = f / m
*/
void CalculateDerivatives(PARTICLE *p,int np,PARTICLEDERIVATIVES *deriv)
{
	int i;
	//cout<<"CalculateDerivatives"<<endl;
	for (i=0;i<np;i++) {
		deriv[i].dpdt.x = p[i].v.x;
		deriv[i].dpdt.y = p[i].v.y;
		deriv[i].dpdt.z = p[i].v.z;
		deriv[i].dvdt.x = p[i].f.x / p[i].m;
		deriv[i].dvdt.y = p[i].f.y / p[i].m;
		deriv[i].dvdt.z = p[i].f.z / p[i].m;
	}
}

/*
A 1st order 1D DE solver.
Assumes the function is not time dependent.
Parameters
h      - step size
y0     - last value
method - algorithm to use [0,5]
fcn    - evaluate the derivative of the field
*/
double Solver1D(double h,double y0,int method,double (*fcn)(double))
{
	double ynew;
	double k1,k2,k3,k4,k5,k6;
	//cout<<"Solver1D"<<endl;
	switch (method) {
	case 0:                          /* Euler method */
		k1 = h * (*fcn)(y0);
		ynew = y0 + k1;
		break;
	case 1:                          /* Modified Euler */
		k1 = h * (*fcn)(y0);
		k2 = h * (*fcn)(y0 + k1);
		ynew = y0 + (k1 + k2) / 2;
		break;
	case 2:                          /* Heuns method */
		k1 = h * (*fcn)(y0);
		k2 = h * (*fcn)(y0 + 2 * k1 / 3);
		ynew = y0 + k1 / 4 + 3 * k2 / 4;
		break;
	case 3:                          /* Midpoint */
		k1 = h * (*fcn)(y0);
		k2 = h * (*fcn)(y0 + k1 / 2);
		ynew = y0 + k2;
		break;
	case 4:                          /* 4'th order Runge-kutta */
		k1 = h * (*fcn)(y0);
		k2 = h * (*fcn)(y0 + k1/2);
		k3 = h * (*fcn)(y0 + k2/2);
		k4 = h * (*fcn)(y0 + k3);
		ynew = y0 + k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;
		break;
	case 5:                          /* England 4'th order, six stage */
		k1 = h * (*fcn)(y0);
		k2 = h * (*fcn)(y0 + k1 / 2);
		k3 = h * (*fcn)(y0 + (k1 + k2) / 4);
		k4 = h * (*fcn)(y0 - k2 + 2 * k3);
		k5 = h * (*fcn)(y0 + (7 * k1 + 10 * k2 + k4) / 27);
		k6 = h * (*fcn)(y0 + (28*k1 - 125*k2 + 546*k3 + 54*k4 - 378*k5) / 625);
		ynew = y0 + k1 / 6 + 4 * k3 / 6 + k4 / 6;
		break;
	}

	return(ynew);
}
