#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#define MAX_IMAGE_DIM 2048

bool IsDebugMode = false;

float BSpline( float x )
{
	float f = x;
	if( f < 0.0 )
		f = -f;

	if( f >= 0.0 && f <= 1.0 )
		return ( 2.0 / 3.0 ) + ( 0.5 ) * ( f* f * f ) - (f*f);
	else if( f > 1.0 && f <= 2.0 )
		return 1.0 / 6.0 * pow( ( 2.0 - f  ), 3.0 );
	return 1.0;
}  

IplImage * createImage(int w, int h)
{
	IplImage * img = NULL;
	img = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3); 
	return img;
}

int pos (int num)
{
	return num > 0 ? num : 0;
}

IplImage * bicubic_algo(IplImage *origImg, int newWidth, int newHeight)
{      
    IplImage *scaledImage = createImage(newWidth,newHeight);         //Final image to be returned 
    uchar * inputData = (uchar*) origImg->imageData;                 //converting original image to usigned char format         
    uchar * scaledData = (uchar*) scaledImage->imageData;
    int i,j, m, n;         // variables for loops 
    int x,y;               // relative position in old image   
    float dx,dy;           // delta_x and delta_y   
    float tx,ty;           // scaling ratio (old/new)   
    float Bmdx;            // Bspline m-dx 
    float Bndy;            // Bspline dy-n   

    int scaledWidthStep = scaledImage->widthStep;
    int scaledImageNChannels = scaledImage->nChannels;
    int origWidthStep = origImg->widthStep;
    int origNChannels = origImg->nChannels;
        
    tx = (float)origImg->width /newWidth ;
    ty =  (float)origImg->height / newHeight;
        
    for(i=0; i<newHeight; i++)
	{
        for(j=0; j<newWidth; j++)
        {
           x = (int)(tx*j);
           y =(int)(ty*i);
           
           dx= tx*j-x;
           dy=ty*i -y;
           
           // these two loops calculate the summation of F(i+m,j+n)*R(m-dx)*R(n-dy) where m and n are varying from -1 to 2. R is the Bspline function      
           for(m=-1;m<=2;m++)
           {
              Bmdx = BSpline(m-dx);               
              for(n=-1;n<=2;n++)
              {
                 Bndy = BSpline(dy-n);               
                 scaledData[i*scaledWidthStep +j*scaledImageNChannels+0] += (uchar) (inputData[pos((y+n)*origImg->widthStep + (x+m)*origImg->nChannels + 0)] * Bmdx * Bndy); 
                 scaledData[i*scaledWidthStep +j*scaledImageNChannels+1] += (uchar) (inputData[pos((y+n)*origImg->widthStep + (x+m)*origImg->nChannels + 1)] * Bmdx * Bndy);
                 scaledData[i*scaledWidthStep +j*scaledImageNChannels+2] += (uchar) (inputData[pos((y+n)*origImg->widthStep + (x+m)*origImg->nChannels + 2)] * Bmdx * Bndy);
              }
           }
        }
	}	
    return scaledImage;   
}

int main(int argc, char* argv[])
{
	IplImage* inputImage = cvLoadImage(argv[1]);
    IplImage* outputImage;
    int desiredSizeX=0;
    int desiredSizeY=0;
           
    if (!inputImage)
    {
		printf("Image not found. Please check if path is correct\n");
        exit(0);
    }   
    cvNamedWindow( "Input Image", CV_WINDOW_AUTOSIZE );
    cvShowImage("Input Image", inputImage);
    printf("Image size is %d, %d\n", inputImage->width, inputImage->height);
    cvWaitKey(0);
   
    printf("\nEnter the size of Upscaled Image.\nBoth dimenstions should be greator than(or equal to) original image\nDo not use floating values\n");
    scanf("%d %d", &desiredSizeX, &desiredSizeY);          
    while (desiredSizeX < inputImage->width || desiredSizeY < inputImage->height || desiredSizeX > MAX_IMAGE_DIM || desiredSizeY > MAX_IMAGE_DIM)
    {
        printf("\nRe-enter the correct size\n");
        scanf("%d %d", &desiredSizeX, &desiredSizeY);          
    } 

    printf("\nRequested upscale Image size is: %d , %d\n", desiredSizeX, desiredSizeY);
    outputImage = bicubic_algo(inputImage, desiredSizeX, desiredSizeY);
    cvNamedWindow( "OUTPUT Image", CV_WINDOW_AUTOSIZE );
    cvShowImage("OUTPUT Image", outputImage);
    cvWaitKey(0);
    cvReleaseImage(&inputImage);
    cvReleaseImage(&outputImage);
}
