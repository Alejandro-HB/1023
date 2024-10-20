#include <iostream>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

void hbSplitChannelsHSV(const Mat &src, Mat &dst);
Mat hbImageProcessing(Mat &grayImage, int th);

int main(){
    //Reading image
    string filename="./cvImagesCell/113.bmp";
    //string filename="./cvImagesCell/102.bmp";
    //string filename="./cvImagesCell/110.bmp";

    Mat src=imread(filename, IMREAD_COLOR);
    Mat grayImage, hsvImage, res1, res2, finalResult;

    
    //Verify if there are data in the image
    if(!src.data){ 
        cout<<"\nUnable to read input image";
        return -1;
    }


    cvtColor(src,  hsvImage, COLOR_BGR2HSV_FULL);
    hbSplitChannelsHSV(hsvImage, grayImage);
    res1=hbImageProcessing(grayImage,90);
    res2=hbImageProcessing(grayImage, 130);
    
    //
    res1=res1-100;
    //Showing res1 and res2
    namedWindow("res1", WINDOW_NORMAL);
    imshow("res1", res1);
    namedWindow("res2", WINDOW_NORMAL);
    imshow("res2", res2);
    //res2=Scalar::all(255)-res2;
    finalResult=res1+res2;

    //Showing final result
    namedWindow("Final result", WINDOW_NORMAL);
    imshow("Final result", finalResult);
    imwrite("./cellDetector.bmp", finalResult);

    waitKey(0);
    return 0;
}

void hbSplitChannelsHSV(const Mat &src, Mat &dst){
    //Verify if src is 3 channel image
    if(src.channels()!=3){
        cout<<"A 3 channel image is needed to work";
        return;
    }
    cout<<"Image is a 3 channel image\n";
    //imshow("Source image",src);
    vector<Mat>ch, hsvCh;
    Mat roiImage, blueMask, hsvBlurredImageColor;
    int kernelSize=71;

    //Split image into its 3 channels
    split(src, ch);
    split(src,  hsvCh);

    //Showing HSV channel
    namedWindow("H channel", WINDOW_NORMAL);
    namedWindow("S channel",  WINDOW_NORMAL);
    namedWindow("V channel",  WINDOW_NORMAL);
    imshow("H channel", ch[0]);
    imshow("S channel", ch[1]);
    imshow("V channel", ch[2]);

    //Median blur to each channel
    cout<<"medianblur to channel S applied\n";
    medianBlur(ch[0], hsvCh[0], kernelSize);
    medianBlur(ch[1], hsvCh[1], kernelSize);
    medianBlur(ch[2], hsvCh[2], kernelSize);

    //Show HSV blurred channels
    namedWindow("Blurred H channel",  WINDOW_NORMAL);
    namedWindow("Blurred S channel",  WINDOW_NORMAL);
    namedWindow("Blurred V channel",  WINDOW_NORMAL);
    imshow("Blurred H channel", hsvCh[0]);
    imshow("Blurred S channel", hsvCh[1]);
    imshow("Blurred V channel", hsvCh[2]);

    //Using S channel for  thresholding
    dst=hsvCh[1];
    
    //Merge blurredImage  channels for image showing
    merge(hsvCh, hsvBlurredImageColor);

    //Switch  to BGR color space
    cvtColor(hsvBlurredImageColor, hsvBlurredImageColor, COLOR_HSV2BGR);
    vector<Mat> ch2;
    split(hsvBlurredImageColor, ch2);

    //Showing blurred image
    namedWindow("hsvBlurredImageColor", WINDOW_NORMAL);
    imshow("hsvBlurredImageColor", hsvBlurredImageColor);

    
    //Trying Color mask (not good results)
    //Mat red_mask = Mat::zeros(src.size(), CV_8UC1);  // Initialize mask
    //Mat brown_mask = Mat::zeros(src.size(), CV_8UC1);  // Initialize mask
    
    //red_mask
    //for (int i = 0; i < src.rows; ++i) {
    //    for (int j = 0; j < src.cols; ++j) {
    //        int b = ch2[0].at<uchar>(i, j);  // H channel
    //        int g = ch2[1].at<uchar>(i, j);  // S channel
    //        int r = ch2[2].at<uchar>(i, j);  // V channel
//
    //        // Check for blue hue range (tweak for your case)
    //        if (r >= 168/*168*/ && r <= 180 && g >= 80 && g <=130 && b >= 100 && b<=130) {
    //            //cout<<"Pixel blanco\n";
    //            red_mask.at<uchar>(i, j) = 255;  // Mark pixel as blue
    //        }
    //    }
    //    }


    //brown_mask
    //for (int i = 0; i < src.rows; ++i) {
    //    for (int j = 0; j < src.cols; ++j) {
    //        int b = ch2[0].at<uchar>(i, j);  // H channel
    //        int g = ch2[1].at<uchar>(i, j);  // S channel
    //        int r = ch2[2].at<uchar>(i, j);  // V channel
//
    //        // Check for blue hue range (tweak for your case)
    //        if (r >= 135/*168*/ && r <= 160 && g >= 76 && g <=101 && b >= 44 && b<=64) {
    //            //cout<<"Pixel blanco\n";
    //            brown_mask.at<uchar>(i, j) = 255;  // Mark pixel as blue
    //        }
    //    }
    //    }

    //namedWindow("redMask", WINDOW_NORMAL);
    //imshow("redMask", red_mask);


    //namedWindow("brownMask", WINDOW_NORMAL);
    //imshow("brownMask", brown_mask);
    
    waitKey(0);
    destroyAllWindows();
}

Mat hbImageProcessing(Mat &grayImage, int th){
    Mat blurredImage, thresholdImage;

    //Aplying thresholding
                //in, out, th, maxvalue, thmode
    threshold(grayImage, thresholdImage, th, 255, THRESH_BINARY);
    return  thresholdImage;

}

