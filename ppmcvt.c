#include "pbm.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Transformation Functions
PBMImage * toPBM(PPMImage * inputImage); //-b
PGMImage * toPGM(PPMImage * inputImage, int pgmMax); //-g max pixel
PPMImage* isolate(PPMImage * inputImage, char * color); // -i color isolated
PPMImage* sepia(PPMImage* inputImage); //-s 
PPMImage* vMirror(PPMImage * inputImage); // -m
PPMImage* removeColor(PPMImage * inputImage, char * color);//-r
PPMImage* thumbnail(PPMImage* inputImage, int scale);//-t
PPMImage* tile(PPMImage * inputImage, int scalet); //-n 
int main( int argc, char *argv[] )
{
	int b, g, i, r, s, m, t, n, maxPGM, scale, scalet;
	int o;
	char * colori;
	char * colorr;
	char * inputFile;
	char * outputFile;
	//char * outputFile
	//optind index for argv[] once it's at the end 
	while((o = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1){
		switch(o){
			case 'b': //convert input file to Portable Bitmap PBM file (default)
				b = 1;
				break;
			case 'g': // convert IF to PGM file using max grayscale pixel value (0-255)
				g = 1;
				maxPGM = atoi(optarg);
				
				break;
			case 'i': //isolate specific RGB channel.
				i = 1; 
				colori = optarg;
				//printf("\tcolor = %s\n", colori);
				break;
			case 'r': //remove specified RGB channel
				r = 1;
				colorr = optarg;
				break;
			case 's': //apply sepia transformation
				s = 1;
				break;
			case 'm':
				m = 1;
				break;
			case 't':
				t = 1;
				scale = atoi(optarg);
				break;
			case 'n':
				n = 1;
				scalet = atoi(optarg);
				break;
			case 'o':
				inputFile = argv[optind];
				if(inputFile == NULL){
					fprintf(stderr,"Error: No input file specified\n");
					exit(1);
				}
				outputFile = optarg;
				if(outputFile == NULL){
					fprintf(stderr,"Error: No output file specified\n");
					exit(1);
				}
				break;
			default:
				fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
				exit(1);
				break;
		}
	}
		if(inputFile == NULL){
			fprintf(stderr,"Error: No input file specified\n");
			exit(1);
		}
		if(outputFile == NULL){
			fprintf(stderr,"Error: No output file specified\n");
			exit(1);
		}
	//printf("inputFile = %s \n",inputFile);
	//printf("outputFile = %s \n",outputFile);
	//PPMImage * imagein = read_ppmfile(inputFile);
	//PGMImage* imagepgm; 
	//PBMImage* imagepbm;
	PPMImage * imagein = read_ppmfile(inputFile);
	if(g == 1){
		PGMImage* imagepgm = toPGM(imagein, maxPGM);
		write_pgmfile(imagepgm,outputFile);
		del_pgmimage(imagepgm);
	}
	if(b == 1){
		PBMImage* imagepbm = toPBM(imagein);
		write_pbmfile(imagepbm, outputFile);
		del_pbmimage(imagepbm);
	}
	if(i == 1){
		imagein = isolate(imagein,colori);
		write_ppmfile(imagein,outputFile);
	}
	if(r == 1){
		
		imagein = removeColor(imagein,colorr);
		write_ppmfile(imagein,outputFile);
	}
	if(s == 1){
		imagein = sepia(imagein);
		write_ppmfile(imagein, outputFile);	
	}if(m == 1){
		imagein = vMirror(imagein);
		write_ppmfile(imagein, outputFile);
	}
	if(t == 1){
		imagein = thumbnail(imagein,scale);
		write_ppmfile(imagein, outputFile);
	}
	if(n == 1){
		imagein = tile(imagein,scalet);
		write_ppmfile(imagein, outputFile);
	}

	del_ppmimage(imagein);
	
    return 0;
}
