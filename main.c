#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include <time.h>

//#include <mpi.h>
double **get_varh_apo_bmp(char *filename,int dimension);
int euresh_diastashs_txt_varwn(char *filename);
void free_2d_malloced_array(double **array,int dimension);
void display_2d_array(int **array,int dimension);
void grayscale(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH,double **kernel,double coeff,int dimension_varwn);

BMPImage_t* ToMatrixImage(RGB_t** image_kernel);
//void horizontal_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH);
BMPImage_t* horizontal_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH);
BMPImage_t* vertical_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH);

struct timespec start, end;

int main(int argc, char *argv[])
{
    

    if (argc < 2) {
      printf("missing argument\n");
      exit(-1);
    }

    double coeff = 1.0f/9;
    int i,dimension;
    
    char kernel_directory[] = "/zstorage/home/ece01288/ece01288/SUSTHMATA_PARALLILIS_2/0.ERGASIA_EXAMINOU/varh.txt";
//    char kernel_directory[] = "/Users/kostas/Desktop/ergasia_examinou_parallila/ERGASIA_EXAMINOU_PARALLILA/ERGASIA_EXAMINOU_PARALLILA/varh.txt";
    char eikona[]="/zstorage/home/ece01288/ece01288/SUSTHMATA_PARALLILIS_2/0.ERGASIA_EXAMINOU/jounior.bmp";
//    char eikona[]="/Users/kostas/Desktop/ergasia_examinou_parallila/ERGASIA_EXAMINOU_PARALLILA/ERGASIA_EXAMINOU_PARALLILA/jounior.bmp";
//    int metatropi=1;
    char *metatropi_char = argv[1];
    int metatropi = atoi(metatropi_char);
    if(metatropi >= 3){
        printf("Epiloges einai N=0 kamia metatropi\nN=1 vertical metatropi\nN=2 Horizontal metatropi\n");
    }


    dimension = euresh_diastashs_txt_varwn(kernel_directory);
    //dimiourgia 2d metavlitou array
   
    double **kernel = (double**)malloc(dimension*sizeof(double*));
    for(i=0;i<dimension;i++){
            kernel[i] =  (double*)malloc(dimension*sizeof(double*));
    }
    kernel = get_varh_apo_bmp(kernel_directory,dimension);
    
    //elegxos an einai .bmp eikona
    if(!strstr(eikona,".bmp")){
        printf("h trith parametros den einai eikona bmp !!!\n");
        exit(1);
    }
    //==================================================================
    //==================================================================
    //metrisi xronou
    clock_gettime (CLOCK_MONOTONIC, &start);
    //==================================================================
    //==================================================================
    
    BMPImage_t* bmp = ReadBMP(eikona);
    int32_t WIDTH = bmp->header.width_px;
    int32_t HEIGHT = bmp->header.height_px;
    printf("Image size: %d x %d\n", WIDTH, HEIGHT);

    //diadikasia gia grayscaling
    grayscale(bmp,HEIGHT,WIDTH,kernel,coeff,dimension);

    if(metatropi == 0 ){
//        SaveBMP(bmp, "/Users/kostas/Desktop/ergasia_examinou_parallila/ERGASIA_EXAMINOU_PARALLILA/ERGASIA_EXAMINOU_PARALLILA/output_NO_CHANGE.bmp");
        SaveBMP(bmp,"output_NO_CHANGE.bmp");
        DestroyBMP(bmp); // apo thn vivliothiki ths bmp
    }
    //horizontal flip
    else if(metatropi == 1){
        BMPImage_t* gia_allagi =malloc(sizeof(BMPImage_t));
        gia_allagi->header=bmp->header;
//        gia_allagi->data = calloc(bmp->header.image_size_bytes, sizeof(uint8_t));
//        gia_allagi->data = calloc(sizeof(bmp->data), sizeof(uint8_t));
        
        gia_allagi = horizontal_flip_bmp(bmp, HEIGHT, WIDTH);
//        SaveBMP(gia_allagi, "/Users/kostas/Desktop/ergasia_examinou_parallila/ERGASIA_EXAMINOU_PARALLILA/ERGASIA_EXAMINOU_PARALLILA/output_HORIZONTAL.bmp");
        SaveBMP(gia_allagi,"output_HORIZONTAL.bmp");
        DestroyBMP(bmp);
        DestroyBMP(gia_allagi);
    }
    
    //vertical flip
    else if(metatropi == 2){
        BMPImage_t* gia_allagi =malloc(sizeof(BMPImage_t));
        gia_allagi->header=bmp->header;
//        gia_allagi->data = calloc(bmp->header.image_size_bytes, sizeof(uint8_t));
        gia_allagi = vertical_flip_bmp(bmp, HEIGHT, WIDTH);
        
//        SaveBMP(gia_allagi, "/Users/kostas/Desktop/ergasia_examinou_parallila/ERGASIA_EXAMINOU_PARALLILA/ERGASIA_EXAMINOU_PARALLILA/output_VERTICAL.bmp");
        SaveBMP(gia_allagi,"output_VERTICAL.bmp");
        DestroyBMP(gia_allagi);
        DestroyBMP(bmp); // apo thn vivliothiki ths bmp
    }
//=============================================================================
//=============================================================================
//              TELOS EURESIS XRONOU
    clock_gettime(CLOCK_MONOTONIC, &end);
    const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
    long timeElapsed_s = end.tv_sec - start.tv_sec;
    long timeElapsed_n = end.tv_nsec - start.tv_nsec;
    //If we have a negative number in timeElapsed_n , borrow
    if ( timeElapsed_n < 0 ) {timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n; timeElapsed_s--;}
    printf("Time: %ld.%09ld secs \n",timeElapsed_s,timeElapsed_n);
//=============================================================================
//=============================================================================
//  free ton metavlito pinaka me ta varh
    free_2d_malloced_array(kernel,kernel_directory);
    return 0;
}


//BMPImage_t_* ToMatrixImage(RGB_t** image_kernel) {
//  
//   BMPImage_t_* eikona_gia_matrix = =malloc(sizeof(BMPImage_t));
//    
//    
//
//  RGB_t** image = calloc(bmp->header.height_px, sizeof(RGB_t*));
//    int j,i;
//  for ( j = 0; j < bmp->header.height_px; j++) {
//    image[j] = calloc(bmp->header.width_px, sizeof(RGB_t));
//    for ( i = 0; i < bmp->header.width_px; i++) {
//      RGB_t pixel = GetPixel(bmp, i, j);
//      image[j][i].r = pixel.r;
//      image[j][i].g = pixel.g;
//      image[j][i].b = pixel.b;
//    }
//  }
//
//  return image;
//}

//=====================================================================
//sunartisi gia grayscaling eikonas
//=====================================================================
BMPImage_t* vertical_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH){
    BMPImage_t* eikona2 =malloc(sizeof(BMPImage_t));
    eikona2->header=input_eikona->header;
    eikona2->data = calloc(input_eikona->header.image_size_bytes, sizeof(uint8_t));
    
    int w,h;
    for(h=0; h<HEIGHT; h++)
    {
        for(w=0; w<WIDTH; w++)
        {
            RGB_t pixel = GetPixel(input_eikona,w,HEIGHT-h-1);
            SetPixel(eikona2, w, h, pixel);
        }
    }
    
    input_eikona=eikona2;
    return input_eikona;
}



//sunartisi gia flip eikonas se horizontal
BMPImage_t* horizontal_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH){
//void horizontal_flip_bmp(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH){
    BMPImage_t* eikona2 =malloc(sizeof(BMPImage_t));
    eikona2->header=input_eikona->header;
    eikona2->data = calloc(input_eikona->header.image_size_bytes, sizeof(uint8_t));

    int h,w;
    for(h=0; h<HEIGHT; h++)
    {
        for(w=0; w<WIDTH; w++)
        {
            RGB_t pixel = GetPixel(input_eikona,WIDTH-w-1,h);
            SetPixel(eikona2, w, h, pixel);
        }
    }
    input_eikona=eikona2;
    return input_eikona;
}


void grayscale(BMPImage_t* input_eikona,int32_t HEIGHT,int32_t WIDTH,double **kernel,double coeff,int dimension_varwn){
//    display_2d_array(kernel, dimension_varwn);
    int h=0,w=0,kx=0,ky=0;
    
    for(h=0; h<HEIGHT; h++)
    {
        for(w=0; w<WIDTH; w++)
        {
            int color_r=0;
            int color_g=0;
            int color_b=0;

            for(kx=0; kx<dimension_varwn; kx++)
            {
                for(ky=0; ky<dimension_varwn;ky++)
                {
                    int a=w+kx-1;
                    int b=h+ky+1;
                    if( a<0 ) { a=0 ;}
                    if( a>= WIDTH) { a=WIDTH-1;}
                    if( b< 0 ) {b=0;}
                    if( b>= HEIGHT) { b=HEIGHT-1;}


//h for  sthn kx kai ky kanonika prepei na paei apo -1 ws 1 gia na ksekianw apo to mesaio stoixeio
                    RGB_t color_pix = GetPixel(input_eikona,a, b);
//                    printf("KERNEL TOU KX KY : %f\n",kernel[kx][ky]);
                    color_r += color_pix.r * kernel[kx][ky] * coeff;
                    color_g += color_pix.g * kernel[kx][ky] * coeff;
                    color_b += color_pix.b * kernel[kx][ky] * coeff;
                }
            }
//prepei na ginei douleia stis for gia na apofygw ta errors oson afora tous ypologismous gia th maska, na mpoun oria gia th maska

            RGB_t color;
            color.r = color_r;
            color.g = color_g;
            color.b = color_b;

            SetPixel(input_eikona, w, h, color);
        }
    }

}


//=====================================================================
//extra sunartiseis
//=====================================================================

//euresh column kai row
int euresh_diastashs_txt_varwn(char *filename){
    char buff[BUFSIZ]={0},temp_char;
    int cols_num=0,rows_num=0;
    char *p;
    
    FILE *file = fopen(filename, "r");
    if(file==0){
        fprintf(stderr,"failed to open txt\n");
        exit(-1);
     }

    //euresh arithmwn column sto txt
    //====================================
    if(NULL==fgets(buff, BUFSIZ, file))
          exit(-1);
       rewind(file);
       for(p=buff;NULL!=strtok(p, " \t\n");p=NULL)
           ++cols_num;
    //====================================
    //euresh arithmwn rows sto txt
    temp_char = getc(file);
  
    while (temp_char != EOF)
    {
            if (temp_char== '\n')
             rows_num += 1;
           temp_char = getc(file);
    }
    //printf("rows: %d\n",rows_num);
    
    if(cols_num!=rows_num){
        printf("O ARITHMOS DIASTASEWN TOU PINAKA VARWN DEN EINAI MORFHS NxN !\n");
        exit(-1);
    }
    
    fclose(file);
    return cols_num;
}

//euresh varwn me xrisi tis sunartiseis pou vriskei tis diastaseis
double **get_varh_apo_bmp(char *filename,int dimension){
    int i,j;
 
    
    FILE *file = fopen(filename, "r");
    if(file==0){
        fprintf(stderr,"failed to open txt\n");
        exit(-1);
     }

    double **varh_bmp_eikonas_fun = (double**)malloc(dimension*sizeof(double*));
    for(i=0;i<dimension;i++){
//        varh_bmp_eikonas_fun[i] =  (double*)malloc(dimension*sizeof(double*));
        varh_bmp_eikonas_fun[i] =  (double*)malloc(dimension*sizeof(double));
    }
    
    for(i = 0; i < dimension; i++) {
        for(j = 0; j < dimension; j++) {
              char s[256] = "0";
              fscanf(file, " %s", s);
              varh_bmp_eikonas_fun[i][j] = atof(s);
          }
      }
    fclose(file);
    
    printf("PINAKAS VARWN :\n");
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            printf("%.2f   ",varh_bmp_eikonas_fun[i][j]);
        }
        printf("\n");
    }
    
    return varh_bmp_eikonas_fun;
}

//free ton 2d malloc pinaka
void free_2d_malloced_array(double **array,int dimension){
    int i;
    
    for(i=0;i<dimension;i++){
            free(array[i]);
    }
    free(array);
}

//ektupwsh 2d pinaka (gia ta varh)
void display_2d_array(int **array,int dimension){
        int i,j;
    for( i=0;i<dimension;i++){
            for(j=0;j<dimension;j++){
                printf("[%d]",array[i][j]);
            }
        printf("\n");
    }
}







