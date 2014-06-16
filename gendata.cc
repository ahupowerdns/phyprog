#include "rfile.hh"
#include <sys/time.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <random>
using namespace Eigen;
using namespace std;

// parameters x y z detection-signal noise-level curvature
int main(int argc, char **argv)
{
  if(argc!=7) {
    cerr<<"Syntax: \n";
    cerr<<"gendata x y z signal noise human"<<endl;
    return(EXIT_FAILURE);
  }

  int xsize{atoi(argv[1])};
  int ysize{atoi(argv[2])};
  int zsize{atoi(argv[3])};
  double sigstrength{atof(argv[4])};
  double noise{atof(argv[5])};
  bool human=atoi(argv[6]);

  struct timeval tv;
  gettimeofday(&tv, 0);
  std::default_random_engine generator(tv.tv_sec+tv.tv_usec);

  Vector3d a(std::uniform_real_distribution<double>(0,xsize-1)(generator), 
	     std::uniform_real_distribution<double>(0,ysize-1)(generator), 
	     std::uniform_real_distribution<double>(0,zsize-1)(generator));
  Vector3d b(std::uniform_real_distribution<double>(0,xsize-1)(generator), 
	     std::uniform_real_distribution<double>(0,ysize-1)(generator), 
	     std::uniform_real_distribution<double>(0,zsize-1)(generator));


  Vector3d n = b-a;
  n/=n.norm();

  std::poisson_distribution<int> distribution(noise);

  printf("%d %d %d\n", xsize, ysize, zsize);
  uint32_t output;

  for(int z=0; z < zsize; ++z) {
    for(int y=0; y < ysize; ++y) {
      for(int x=0; x< xsize; ++x) {
	Vector3d p(x,y,z);
	double distance = ((a-p)-((a-p).dot(n))*n).norm();	
	
	output=(uint32_t)(distribution(generator)+sigstrength*exp(-10*distance*distance));
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
