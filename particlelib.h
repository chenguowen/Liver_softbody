#define MF_DEBUG 0 
typedef struct {
	float x;
	float y;
	float z;
} XYZ;
	
typedef struct {
   double m;   /* Mass                          */
   XYZ p;      /* Position                      */
   XYZ v;      /* Velocity                      */
   XYZ f;      /* Force                         */
   int fixed;  /* Fixed point or free to move   */
} PARTICLE;

typedef struct {
   XYZ dpdt;
   XYZ dvdt;
} PARTICLEDERIVATIVES;

typedef struct {
   double gravitational;
   double viscousdrag;
} PARTICLEPHYS;

typedef struct {
   int from;
   int to;
   double springconstant;
   double dampingconstant;
   double restlength;
} PARTICLESPRING;

void CalculateForces(PARTICLE *,int,PARTICLEPHYS,PARTICLESPRING *,int);
void UpdateParticles(PARTICLE *,int,PARTICLEPHYS,PARTICLESPRING *,int,double,int);
void CalculateDerivatives(PARTICLE *,int,PARTICLEDERIVATIVES *);
double Solver1D(double,double,int,double (*)(double));