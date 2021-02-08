#include "pbm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
  PPMImage* a = malloc(sizeof(PPMImage));
  a->width = w;
  a->height = h;
  a->max = m;

  for(int i = 0; i < 3; i++){ 
    a->pixmap[i] = malloc(h * sizeof(int*));
    for(int j = 0; j < h; j++){
      a->pixmap[i][j] = malloc(w * sizeof(int));
       }
    }
  return a;
}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
  PBMImage* a = malloc(sizeof(PBMImage));
  a->width = w;
  a->height = h;

  a->pixmap = malloc(h * sizeof(int*));
  for(int i = 0; i < h; i++){
      a->pixmap[i] = malloc(w * sizeof(int));
  }
  return a;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
  
  PGMImage* a = malloc(sizeof(PGMImage));
  a->width = w;
  a->height = h;
  a->max = m;

  a->pixmap = malloc(h * sizeof(int*));
  for(int i = 0; i < h; i++){
      a->pixmap[i] = malloc(w * sizeof(int));
  }
  return a;
}

void del_ppmimage( PPMImage * p )
{
  for(int i = 0; i < 3; i++){ 
    for(int j = 0; j < p->height; j++){
      free(p->pixmap[i][j]);
    }
    free(p->pixmap[i]);
  }
  //free(p->pixmap);
  free(p);
}

void del_pgmimage( PGMImage * p )
{
  for(int h = 0; h <p->height; h++){
    free(p->pixmap[h]);
  }
  free(p->pixmap);
  free(p);
}

void del_pbmimage( PBMImage * p )
{
  for(int h = 0; h <p->height; h++){
    free(p->pixmap[h]);
  }
  free(p->pixmap);
  free(p);
}

//Transformation Functions
//PPM to PBM for -b option
PBMImage * toPBM(PPMImage * inputImage){

  PBMImage *pbm = new_pbmimage(inputImage->width,inputImage->height);

  for(int h = 0;h < pbm->height; h++){
    for(int w = 0; w < pbm->width; w++){
      int r = inputImage->pixmap[0][h][w]; // red pixel value
      int g = inputImage->pixmap[1][h][w]; // green pixel value
      int b = inputImage->pixmap[2][h][w]; // blue pixel value

      double ppmMax = (double)(inputImage->max)/2;
      double avg = ((double)(r + b + g)/ 3);
      int pixel = avg < ppmMax;
      pbm->pixmap[h][w] = pixel;
    }
  }
  return pbm;
}

PGMImage * toPGM(PPMImage * inputImage,int pgmMax){
  

  PGMImage * pgm = new_pgmimage(inputImage->width, inputImage->height, inputImage->max);
  if(pgmMax > 65536){
				fprintf(stderr,"Error: Invalid max grayscale pixel value: %d; must be less than 65,536\n",pgmMax);
        exit(1);
				}
  for(int h = 0;h < pgm->height; h++){
    for(int w = 0; w < pgm->width; w++){
      int r = inputImage->pixmap[0][h][w]; // red pixel value
      int g = inputImage->pixmap[1][h][w]; // green pixel value
      int b = inputImage->pixmap[2][h][w]; // blue pixel value
      int ppmMax = inputImage->max;
      double sum = (double) (r + b + g);
      double avg = (sum / (double) 3);
      if(ppmMax>pgmMax){
        ppmMax = pgmMax;
      }if(pgmMax>ppmMax){
        pgmMax = ppmMax;
      }
      avg = avg / (double) ppmMax; 
      avg = avg * (double) pgmMax;
      
      pgm->pixmap[h][w] = avg;
    }
  }
  //deallocate PPM Image!!!
  return pgm;
}

PPMImage* isolate(PPMImage * inputImage, char * color){

  PPMImage* ppm = inputImage;

  if(strcmp(color,"red") == 0){
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[1][h][w] = 0;
        ppm->pixmap[2][h][w] = 0;
      }
    }
    return ppm;
  }else if(strcmp(color,"green") == 0){
    printf("\tblue\n");
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[0][h][w] = 0;
        ppm->pixmap[2][h][w] = 0;

      }
    }
    return ppm;
  }else if( strcmp(color,"blue") == 0){
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[0][h][w] = 0;
        ppm->pixmap[1][h][w] = 0;
      }
    }
    return ppm;
  }else{
    fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n",color);
    exit(1);
  }
  return ppm;
}

PPMImage* removeColor(PPMImage * inputImage, char * color){
  PPMImage* ppm = inputImage;

  if(strcmp(color,"red") == 0){
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[0][h][w] = 0;
      }
    }
    return ppm;
  }else if(strcmp(color,"green") == 0){
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[1][h][w] = 0;
      }
    }
    return ppm;
  }else if( strcmp(color,"blue") == 0){
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        ppm->pixmap[2][h][w] = 0;
      }
    }
    return ppm;
  }else{
    fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n",color);
    exit(1);
  }
  return ppm;
}

PPMImage* sepia(PPMImage * inputImage){

  PPMImage * ppm = inputImage;
    for(int h = 0;h < ppm->height; h++){
      for(int w = 0; w < ppm->width; w++){
        int r = inputImage->pixmap[0][h][w]; // red pixel value
        int g = inputImage->pixmap[1][h][w]; // green pixel value
        int b = inputImage->pixmap[2][h][w]; // blue pixel value
        if(r > 255)
          r = 255;
        if(g > 255)
          g = 255;
        if(b > 255)
          b= 255;
        int r1 =(0.393 * r + 0.769 * g + 0.189 * b);
        int g1 = (0.349 * r + 0.686 * g + 0.169 * b);
        int b1 =( 0.272 * r + 0.534 * g + 0.131 * b);
        //turns out values over 255 on sepia turn bright colors. Cap them at 255.
        if(r1 > 255)
          r1 = 255;
        if(g1 > 255)
          g1 = 255;
        if(b1 > 255)
          b1= 255;

        inputImage->pixmap[0][h][w] = r1; // sepia red pixel value
        inputImage->pixmap[1][h][w] = g1; // sepia green pixel value
        inputImage->pixmap[2][h][w] = b1; // sepia blue pixel value
      }
    }
    return ppm;
}

PPMImage* vMirror(PPMImage * inputImage){
  PPMImage * ppm = inputImage;

  for(int h = 0;h < ppm->height; h++){
    for(int w = 0; w < ppm->width; w++){
      int half1 = ppm->width - w - 1;
      int r1 = inputImage->pixmap[0][h][w]; // red pixel value
      int g1 = inputImage->pixmap[1][h][w]; // green pixel value
      int b1 = inputImage->pixmap[2][h][w]; // blue pixel value*/

      inputImage->pixmap[0][h][half1] = r1;// red pixel value
      inputImage->pixmap[1][h][half1] = g1; // green pixel value
      inputImage->pixmap[2][h][half1] = b1; // blue pixel value
    }
  }
  return ppm;
}
//finish
PPMImage* thumbnail(PPMImage* inputImage, int scale){

 


  PPMImage * ppm = inputImage;
  if(scale == 1){
     return ppm;
  }
   if(scale < 1 || scale > 8|| ppm->height < scale || ppm->width < scale){
    fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", scale);
    exit(1);
  }
  
  int h2 = (ppm->height)/scale;
  int w2 = (ppm->width)/scale;
  if (scale > ppm->height){
     h2 = 1;
  }
  if(scale > ppm->width) {
     w2 = 1;
  }
  PPMImage * output = new_ppmimage((inputImage->width)/scale,(inputImage->height)/scale,inputImage->max);
  for(int h = 0;h < h2; h++){
    for(int w = 0; w < w2; w++){
      int r1 = inputImage->pixmap[0][h*scale][w*scale]; // red pixel value
      int g1 = inputImage->pixmap[1][h*scale][w *scale]; // green pixel value
      int b1 = inputImage->pixmap[2][h*scale][w*scale]; // blue pixel value*/
      //printf("\t w = %i \n h = % i", w, h);
      output->pixmap[0][h][w] = r1;// red pixel value
      output->pixmap[1][h][w] = g1; // green pixel value
      output->pixmap[2][h][w] = b1; // blue pixel value
    }
  }
  return output;
}  

PPMImage* tile(PPMImage * inputImage, int scalet){

  PPMImage * ppm = new_ppmimage(inputImage->width, inputImage->height, inputImage->max);

    if(scalet < 1 || scalet > 8 || ppm->height < scalet || ppm->width < scalet){
      fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", scalet);
      exit(1);
    }
    if(scalet == 1){
      return ppm;
    }
    int h2 = (ppm->height)/scalet;
    int w2 = (ppm->width)/scalet;
    if (scalet > ppm->height){
      h2 = 1;
    }
    if(scalet > ppm->width) {
      w2 = 1;
    }
    PPMImage * output = new_ppmimage((inputImage->width)/scalet,(inputImage->height)/scalet,inputImage->max);
    for(int h = 0;h < h2; h++){
      for(int w = 0; w < w2; w++){
        int r1 = inputImage->pixmap[0][h*scalet][w*scalet]; // red pixel value
        int g1 = inputImage->pixmap[1][h*scalet][w *scalet]; // green pixel value
        int b1 = inputImage->pixmap[2][h*scalet][w*scalet]; // blue pixel value
        //printf("\t w = %i \n h = % i", w, h);
        output->pixmap[0][h][w] = r1;// red pixel value
        output->pixmap[1][h][w] = g1; // green pixel value
        output->pixmap[2][h][w] = b1; // blue pixel value
      }
    }
    for(int i = 0; i < scalet; i++){
      int h1 = 0;
      int w1 = 0;
      int x = 0; 
      int y = 1;
      int j = 0;
      for (int x = 0; x < scalet; x++) {
      for(int h = 0;h < h2; h++){
        for(int w = 0; w < w2; w++){
        ppm->pixmap[0][h+x*h2][i*w2+w] = output->pixmap[0][h][w] ;// red pixel value
        ppm->pixmap[1][h+x*h2][i*w2+w] = output->pixmap[1][h][w] ; // green pixel value
        ppm->pixmap[2][h+x*h2][i*w2+w] = output->pixmap[2][h][w] ; // blue pixel value
        }
      } 
      }
    }
        return ppm;
}

