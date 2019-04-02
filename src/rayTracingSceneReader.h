/****************************************
 ***    Project:  GL Raytracer
 ***    File:     rayTracerSceneReader.h
 ***    Author:   Daniele Donghi
 ***           d.donghi AT gmail.com
 ***    Created on November 1, 2011
 ****************************************/

#include <vector>

using namespace std;

class point3D
{
	private:
		// Members
		double u;
		double v;
		double w;

	public:
		// Constructor/Destructor
		point3D(double x, double y, double z) {u = x; v = y; w = z;};
		point3D(point3D *p) {u = p->x(); v = p->y(); w = p->z();};
		point3D() {};
		~point3D() {};

		// Methods
        void setPoint3D(double x, double y, double z) {u = x; v = y; w = z;}
		double x() {return u;}
		double y() {return v;}
		double z() {return w;}
};

class light
{
	private:
		// Members
		point3D *l;
		double col[3];

	public:
		// Constructor/Destructor
		light(point3D *loc) {l = loc;}
		~light() {};

		// Methods
		point3D* location() {return l;}
		double red()   {return col[0];}
		double green() {return col[1];}
		double blue()  {return col[2];}
		void setColor(double r, double g, double b) {col[0] = r; col[1] = g; col[2] = b;}
};

class sphere
{
	private:
		// Members
		point3D *cent;
		double rad;
		double Ka[3];
		double Kd[3];
		double Ks[3];
		double n_exp;
		double refl;		// raytracing only
		double tran;		// raytracing only
		double idx_ref;		// raytracing only

	public:
		// Constructor/Destructor
		sphere() {}
		~sphere() {};

		// Methods
		void setCenter(point3D *c){cent = c;}
		void setRadius(double r){rad=r;}
		void setAmbientProperty(double r, double g, double b)  {Ka[0] = r; Ka[1] = g; Ka[2] = b;}
		void setDiffuseProperty(double r, double g, double b)  {Kd[0] = r; Kd[1] = g; Kd[2] = b;}
		void setSpecularProperty(double r, double g, double b) {Ks[0] = r; Ks[1] = g; Ks[2] = b;}
		void setSpecularExponent(double n) {n_exp = n;}
		void setReflection(double reflection) {refl = reflection;}
		void setTransmission(double transmission) {tran = transmission;}
		void setIndexOfRefraction(double idxRefraction) {idx_ref = idxRefraction;}
		point3D* center() {return cent;}
		double radius() {return rad;}
		double getAmbientProperty(int color)  {return Ka[color];}
		double getDiffuseProperty(int color)  {return Kd[color];}
		double getSpecularProperty(int color) {return Ks[color];}
		double getSpecularExponent() {return n_exp;}
		double getReflection() {return refl;}
		double getTransmission() {return tran;}
		double getIndexOfRefraction() {return idx_ref;}
};

class triangle
{
	private:
		// Members
		point3D *p1;
		point3D *p2;
		point3D *p3;
		double Ka[3];
		double Kd[3];
		double Ks[3];
		double n_exp;
		double refl;		// raytracing only
		double tran;		// raytracing only
		double idx_ref;		// raytracing only

	public:
		// Constructor/Destructor
		triangle() {}
		~triangle() {};

		// Methods
		void setPoint1(point3D *pt1) {p1 = pt1;}
		void setPoint2(point3D *pt2) {p2 = pt2;}
		void setPoint3(point3D *pt3) {p3 = pt3;}
		void setAmbientProperty(double r, double g, double b)  {Ka[0] = r; Ka[1] = g; Ka[2] = b;}
		void setDiffuseProperty(double r, double g, double b)  {Kd[0] = r; Kd[1] = g; Kd[2] = b;}
		void setSpecularProperty(double r, double g, double b) {Ks[0] = r; Ks[1] = g; Ks[2] = b;}
		void setSpecularExponent(double n) {n_exp = n;}
		void setReflection(double reflection) {refl = reflection;}
		void setTransmission(double transmission) {tran = transmission;}
		void setIndexOfRefraction(double idxRefraction) {idx_ref = idxRefraction;}
		point3D* point1() {return p1;}
		point3D* point2() {return p2;}
		point3D* point3() {return p3;}
		double getAmbientProperty(int color)  {return Ka[color];}
		double getDiffuseProperty(int color)  {return Kd[color];}
		double getSpecularProperty(int color) {return Ks[color];}
		double getSpecularExponent() {return n_exp;}
		double getReflection() {return refl;}
		double getTransmission() {return tran;}
		double getIndexOfRefraction() {return idx_ref;}
};

class dbReader
{
   private:
      // Members
      double background[3];
      double ambientLight[3];
      int windowDimensions[2];
      light *lightSource;
      vector<triangle*> triangleList;
      vector<sphere*> sphereList;

   public:
      // Constructor/Destructor
      dbReader() {};
      ~dbReader() {};

      // TODO: Criar métodos
      point3D *PRP, *VRP, *VUP, *VPN;
      double uMin,uMax,vMin,vMax;

      // Public Methods
      double* getBackgroundColor();
      double* getAmbientLight();
      int* getWindowDimensions();
      light* getLight();
      vector<triangle*> getTriangleList();
      vector<sphere*> getSphereList();
      void read(char* fN);
};
