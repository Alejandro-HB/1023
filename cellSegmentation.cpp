#include <iostream>
using namespace std;

//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
//En core viene el contenedor mat, usado para contener una imagen
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;

void splitChannels(const Mat &src);
void splitChannelsHSV(const Mat &src, Mat &dst);
Mat imageProcessing(Mat &grayImage, int th);

int main(){
    //Reading image
    string filename="./cvImagesCell/113.bmp";
    //string filename="../../../cvImagesMed/retine.jpg";
    //string filename="../../../imagesMed/chameleon.jpg";
    Mat src=imread(filename, IMREAD_COLOR);
    Mat grayImage, grayImage2, hsvImage, res1, res2;

    
    //Verify if there are data in the image
    if(!src.data){ 
        cout<<"\nUnable to read input image";
        return -1;
    }

    cout<<src.size;
    //Gray image
    //cvtColor(src, grayImage, COLOR_BGR2GRAY);
    //imageProcessing(grayImage);
    //splitChannels(src);

    cvtColor(src,  hsvImage, COLOR_BGR2HSV_FULL);
    splitChannelsHSV(hsvImage, grayImage);
    res1=imageProcessing(grayImage, 245);
    res2=imageProcessing(grayImage, 205);
    //Aplying negative
    res2=Scalar::all(255)-res2;
    Mat res=res1+res2;
    //Aplying negative
    res=Scalar::all(255)-res;
    namedWindow("result", WINDOW_NORMAL);
    imshow("result", res);

    //namedWindow("Input image",  WINDOW_NORMAL);
    //imshow("Input image", grayImage);
    

    

    waitKey(0);
    return 0;
}


void splitChannels(const Mat &src){
    //Verify if src is 3 channel image
    if(src.channels()!=3){
        cout<<"A 3 channel image is needed to work";
        return;
    }
    cout<<"Image is a 3 channel image\n";
    //imshow("Source image",src);

    vector<Mat>ch;
    Mat blueImage;
    //Split image into its 3 channels
    split(src, ch);

    //Showing channel
    imshow("Blue channel", ch[0]);
    imshow("Green channel", ch[1]);
    imshow("Red channel", ch[2]);

    //Merge channels to see blue color
    ch[1]=0;
    ch[2]=0;
    merge(ch, blueImage);

    namedWindow("Blue image",  WINDOW_NORMAL);
    imshow("Blue image", blueImage);

    
    waitKey(0);
    destroyAllWindows();
}

void splitChannelsHSV(const Mat &src, Mat &dst){
    //Verify if src is 3 channel image
    if(src.channels()!=3){
        cout<<"A 3 channel image is needed to work";
        return;
    }
    cout<<"Image is a 3 channel image\n";
    //imshow("Source image",src);
    vector<Mat>ch;
    Mat roiImage, blueMask;
    //Split image into its 3 channels
    split(src, ch);

    //Showing channel
    namedWindow("H channel", WINDOW_NORMAL);
    namedWindow("S channel",  WINDOW_NORMAL);
    namedWindow("V channel",  WINDOW_NORMAL);
    imshow("H channel", ch[0]);
    imshow("S channel", ch[1]);
    imshow("V channel", ch[2]);

    dst=ch[1];
    //normalize(dst, dst, 0, 255, NORM_MINMAX, CV_8UC1);
    equalizeHist(dst, dst);
    namedWindow("normalized dst", WINDOW_NORMAL);
    imshow("normalized dst", dst);
    
    Mat blue_mask = Mat::zeros(src.size(), CV_8UC1);  // Initialize mask
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            int hue = ch[0].at<uchar>(i, j);  // H channel
            int sat = ch[1].at<uchar>(i, j);  // S channel
            int val = ch[2].at<uchar>(i, j);  // V channel

            // Check for blue hue range (tweak for your case)
            if (hue >= 230 && hue <= 240 && sat >= 50 && val >= 50) {
                blue_mask.at<uchar>(i, j) = 255;  // Mark pixel as blue
            }
        }
        }
    //Scalar lower_blue(240, 60, 70);  // Darker blue range
    //Scalar upper_blue(240, 30, 70);
    //inRange(src, lower_blue, upper_blue, blueMask);
    namedWindow("blueMask", WINDOW_NORMAL);
    imshow("blueMask", blue_mask);




    //Merge channels to see blue color
    //ch[1]=0;
    //ch[2]=0;
    //merge(ch, blueImage);

    //namedWindow("Blue image",  WINDOW_NORMAL);
    //imshow("Blue image", blueImage);

    
    waitKey(0);
    destroyAllWindows();
}

Mat imageProcessing(Mat &grayImage, int th){
    //Aplying median blur
    unsigned int kernelSize=151;
    Mat blurredImage, srcF;
    //Meadian blur means that uses median to compute the  value of the pixel
    //medianBlur(grayImage, blurredImage, kernelSize);
    GaussianBlur(grayImage, blurredImage, Size(kernelSize, kernelSize), 0);
    namedWindow("Blurred image", WINDOW_NORMAL);
    imshow("Blurred image", blurredImage);
    //imwrite("./blurred113");

    //namedWindow("Median blur image",   WINDOW_NORMAL);
    //imshow("Median blur image", dst2);

    //Aplying thresholding
    Mat thresholdImage;
    //int th=245;
    //Scalar ave=mean(grayImage);
    //int th=(int)ave.val[0];
                //in, out, th, maxvalue, thmode
    threshold(blurredImage, thresholdImage, th, 255, THRESH_BINARY);
    namedWindow("Thresholded image", WINDOW_NORMAL);
    imshow("Thresholded image", thresholdImage);
    waitKey(0);
    return  thresholdImage;
    //thresholdImage.convertTo(srcF, CV_32F);
////
    //Mat kernel4=(Mat_<double>(3,3)<<    -1, -1, -1
    //                                    -1,  8, -1
    //                                    -1, -1, -1);
//
    //Mat dst4;
    //filter2D(srcF, dst4, -1, kernel4, Point(-1, -1), 0, BORDER_DEFAULT);
//
    //normalize(dst4, dst4, 0, 255, NORM_MINMAX,  CV_8UC1);
    //equalizeHist(dst4, dst4);
    //namedWindow("High-pass filter 4 image",   WINDOW_NORMAL);
    //imshow("High-pass filter 4 image", dst4);
}

