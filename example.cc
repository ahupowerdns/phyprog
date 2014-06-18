#include <stdio.h>
#include <stdint.h>
#include <iostream>
using namespace std;

int main(int argc, char**argv)
{
	if(argc != 2){
		cout<<"Usage: example.exe FILENAME";
		return 0;
	}
	// Open file
	FILE *file;
	file = fopen(argv[1],"r");
	if(file==NULL){
		cout<<"Error loading file";
		return 0;
	}

	// Get dimensions
	char line[80];
	fgets(line, sizeof(line)-1, file);
	int x_dim, y_dim, z_dim;
	sscanf(line, "%d %d %d\n", &x_dim, &y_dim, &z_dim);

	int linePos = ftell(file);

	// Iterate through data
	for(int z=0;z<z_dim;z++){
		for(int y=0;y<y_dim;y++){
			for(int x=0;x<x_dim;x++){
				int pos = z*(z_dim*y_dim) + y*y_dim + x;
				fseek(file,linePos + pos,SEEK_SET);
				char val = '0';
				fread(&val,sizeof(val),1,file);
				printf("%d  (%d,%d,%d): %d\n",linePos + pos,x,y,z,val);
			}
		}
	}
}