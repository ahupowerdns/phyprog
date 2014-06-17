#include "rfile.hh"
#include <sys/time.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <random>
#include <stdexcept>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
using namespace Eigen;
using namespace std;

void unixDie(const string& reason)
{
  throw runtime_error("Fatal error "+reason+": "+strerror(errno));
}

off_t filesize(int fd)
{
  struct stat buf;
  if(fstat(fd, &buf) < 0)
    unixDie("getting filesize");
  return buf.st_size;
}

class SensorReader
{
public:
  SensorReader(const std::string& fname);
  uint8_t get(int x, int y, int z);
  int d_xsize, d_ysize, d_zsize;
private:
  RFile d_in;
  const uint8_t* d_ptr;
};

SensorReader::SensorReader(const std::string& fname) : d_in(fname.c_str(), "r")
{
  char line[80];
  if(!fgets(line, sizeof(line)-1, d_in.d_fp) || sscanf(line, "%d %d %d\n", &d_xsize, &d_ysize, &d_zsize)!=3) 
    throw runtime_error("Input format wrong");

  auto offset=ftell(d_in.d_fp);

  cout<<"Dimensions: "<<d_xsize<<", "<<d_ysize<<", "<<d_zsize<<endl;
  auto fsize=filesize(d_in.d_fd);

  if(fsize != offset+d_xsize*d_ysize*d_zsize) {
    cerr<<"Wrong filesize: "<<filesize(d_in.d_fd)<< " != "<< offset+d_xsize*d_ysize*d_zsize << endl;
    exit(EXIT_FAILURE);
  }

  void* addr=0;
  if((addr=mmap(addr, fsize, PROT_READ, MAP_SHARED, d_in.d_fd, 0)) == MAP_FAILED)
    unixDie("Setting up mmap");

  d_ptr = (uint8_t*)addr + offset;
}

uint8_t SensorReader::get(int x, int y, int z)
{
  return d_ptr[z*d_xsize*d_ysize + y*d_xsize + x];
}

int main(int argc, char**argv)
try
{
  SensorReader sr(argv[1]);

  Vector3d a(atof(argv[2]), atof(argv[3]), atof(argv[4]));
  Vector3d b(atof(argv[5]), atof(argv[6]), atof(argv[7]));
  Vector3d n=(b-a);
  n/=n.norm();

  uint8_t val;
  double sum=0;
  for(int z=0; z < sr.d_zsize; ++z) {
    for(int y=0; y < sr.d_ysize; ++y) {
      for(int x=0; x< sr.d_xsize; ++x) {
	Vector3d p(x,y,z);
	double distance = ((a-p)-((a-p).dot(n))*n).norm();	
	val=sr.get(x,y,z);

	if(distance < 1) {
	  //	  cout<<"*";
	  sum+=val;
	}

	//	cout<<(int)val<<'\t';
      }
      //      cout<<'\n';
    }
    //    cout<<'\n';
  }
  cout<<"sum: "<<sum<<endl;

}
catch(exception& e)
{
  cerr<<"Error: "<<e.what()<<endl;
}
