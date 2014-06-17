#include "rfile.hh"
#include <sys/time.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <random>
using namespace Eigen;
using namespace std;

bool intersect(const Vector3d& lineBasis, const Vector3d& lineDirection, 
	       const Vector3d& planeBasis, const Vector3d& planeNormal, 
	       Vector3d* place)
{
  double denominator = lineDirection.dot(planeNormal);
  if(!denominator)
    return false;
  
  double d = (planeBasis - lineBasis).dot(planeNormal)/denominator;

  *place = lineBasis + d*lineDirection;
  return true;
}

bool inBox(const Vector3d& place, double x, double y, double z)
{
  return 
    place[0]>=0 && place[0] <= x &&
    place[1]>=0 && place[1] <= y &&
    place[2]>=0 && place[2] <= z;
}

// parameters x y z detection-signal noise-level curvature
int main(int argc, char **argv)
{
  if(argc!=7) {
    cerr<<"Syntax: \n";
    cerr<<"gendata x y z signal noise human"<<endl;
    return(EXIT_FAILURE);
  }

  double xsize{atof(argv[1])};
  double ysize{atof(argv[2])};
  double zsize{atof(argv[3])};
  double sigstrength{atof(argv[4])};
  double noise{atof(argv[5])};
  bool human=atoi(argv[6]);

  struct timeval tv;
  gettimeofday(&tv, 0);
  std::default_random_engine generator(tv.tv_sec+tv.tv_usec);

  Vector3d a(std::uniform_real_distribution<double>(0,xsize)(generator), 
	     std::uniform_real_distribution<double>(0,ysize)(generator), 
	     std::uniform_real_distribution<double>(0,zsize)(generator));
  Vector3d b(std::uniform_real_distribution<double>(0,xsize)(generator), 
	     std::uniform_real_distribution<double>(0,ysize)(generator), 
	     std::uniform_real_distribution<double>(0,zsize)(generator));

  Vector3d n = b-a;
  n/=n.norm();

  struct Side 
  {
    Vector3d origin;
    Vector3d normal;
    string name;
  };

  vector<Side> sides{
    {{0,0,0},         { 1, 0, 0}, "left"},
    {{0,0,0},         { 0, 1, 0}, "back"},
    {{0,0,0},         { 0, 0, 1}, "top"},
    {{xsize,0,0},     {-1, 0, 0}, "right"},
    {{0,ysize,0},     { 0,-1, 0}, "front"},
    {{0,0,zsize},     { 0, 0,-1}, "bottom"}   
  };


  for(auto& s: sides) {
    Vector3d place;
    if(intersect(a, n, s.origin, s.normal, &place) && inBox(place, xsize, ysize, zsize)) {
      cerr<<s.name<<" intersection: \n"<<place<<endl;
    }
    else 
      cerr<<"Line does not intersect with "<<s.name<<endl;
  }

  std::poisson_distribution<int> distribution(noise);

  printf("%d %d %d\n", (int)xsize, (int)ysize, (int)zsize);
  uint32_t output;

  for(int z=0; z < zsize; ++z) {
    for(int y=0; y < ysize; ++y) {
      for(int x=0; x< xsize; ++x) {
	Vector3d p(x,y,z);
	double distance = ((a-p)-((a-p).dot(n))*n).norm();	
	
	output=(uint32_t)(distribution(generator)+sigstrength*exp(-8*distance*distance));
	if(human)
	  cout<<output<<'\t';
	else
	  printf("%c", output < 256 ? output : 255);
      }
      if(human)
	cout<<endl;
    }
    if(human)
      cout<<endl;
  }
    
}
