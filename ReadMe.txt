The code was written on a MacBookPro running Windows7 on a virtual Machine.  
Dev-cpp version 4.9.9.2
OpenCV version 2.3.1

How to run the code

Method 1 (using Dev c++)
1. Open the file bicubicInterpolation.dev using Dev-cpp.
2. Make sure you have necessary setting for using openCV
3. Click Execute->Parameters. In parameters enter the input_file_path and click OK.
(sample image has been included with the name old.png, havent tried other formats) 
4. compile and run (F9).
5. On successful run, there should be an image window which has the input image. 
	Click on this window and press key '0', or you can close this window. 
	Other Terminal will show the size of Input image.
	It ask for Scale image size which should be >= input image size in both dimensions.
	Floating values may cause abrupt behaviour. 
	Also I have added an extra restriction to limit max size to 2048 in either direction. which can be changed from MAX_IMAGE_DIM macro in main.cpp 		

Method 2 (using terminal)
1. open cmd.exe
2. <path_to bicubicInterpolation.exe> <path to input file>.
3. Follow steps 5 from Method 1. 