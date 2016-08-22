//
//  main.cpp
//  Ch2_ManipulatingPixels
//
//  Created by Shijia Geng on 8/21/16.
//  Copyright © 2016 SG. All rights reserved.
//

#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 1. Accessing pixel values
// Adding salt-and-pepper noise
void salt(cv::Mat &image, int n) {
    for (int k = 0; k < n; k++) {
        int i = rand()%image.rows;
        int j = rand()%image.cols;
        
        if (image.channels() == 1) {
            image.at<uchar>(i,j) = 255;
        } else if (image.channels() == 3) {
            image.at<cv::Vec3b>(i,j)[0] = 255;
            image.at<cv::Vec3b>(i,j)[1] = 255;
            image.at<cv::Vec3b>(i,j)[2] = 255;
        }
        
    }
}



// 2. Scanning an image (all pixels) with pointers
// Reducing the number of colors in an image.
void colorReduce(cv::Mat &image, int div = 64) {
    int n_row = image.rows;
    int n_col = image.cols * image.channels(); // times channel number !!! important
    
    for (int i = 0; i < n_row; i++) {
        uchar* data = image.ptr<uchar>(i); // address of row i -> channel 0 1 2 0 1 2 0 1 2 ...
        for (int j = 0; j < n_col; j++) {
            data[j] = data[j] / div * div + div / 2;
            // all points in range [(n-1)*div, n*div] map to n*div/2
            // another way is using bit masking
        }
    }
}



// 3. Scanning an image with iterators
void colorReduce2(cv::Mat &image, int div = 64) {
    auto it = image.begin<cv::Vec3b>(); // cv::Mat_<cv::Vec3b>::iterator it
    auto itend = image.end<cv::Vec3b>();
    for ( ; it != itend; ++it) {
        (*it)[0] = (*it)[0] / div * div + div / 2;
        (*it)[1] = (*it)[1] / div * div + div / 2;
        (*it)[2] = (*it)[2] / div * div + div / 2;
    }
}



// 4. Efficient image scanning loops
void colorReduce3(cv:: Mat &image, int div = 64) {
    int n_row = image.rows;
    int n_col = image.cols;
    
    if (image.isContinuous()) { // no padded pixels
        n_col = n_col*n_row;
        n_row = 1;
    }
    int n = static_cast<int> (log(static_cast<double>(div))/log(2.0));
    uchar mask = 0xFF<<n;
    for (int i = 0; i < n_row; i++) {
        uchar *data = image.ptr<uchar>(i);
        for (int j = 0; j < n_col; j++) {
            *data++ = *data & mask + div/2;
            *data++ = *data & mask + div/2;
            *data++ = *data & mask + div/2;
        }
    }
}



// 5. Scanning image with neighbor access
// Sharpenning an image
void sharpen(const cv::Mat &image, cv::Mat &result) { // input image has to be grey scale
    result.create(image.size(), image.type());
    for (int i = 1; i < image.rows-1; i++) {
        const uchar* previous = image.ptr<const uchar>(i-1);
        const uchar* current = image.ptr<const uchar>(i);
        const uchar* next = image.ptr<const uchar>(i+1);
        
        uchar* output = result.ptr<uchar>(i);
        for (int j = 1; j < image.cols-1; j++) {
            *output++ =
            cv::saturate_cast<uchar>(5*current[j] - current[j-1] - current[j+1] - previous[j] - next[j]);
            // cv::saturate_cast keeps value in range [0, 255]
        }
    
    }
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));
     
}

// using cv::filter2D function
void sharpen2D(const cv::Mat &image, cv::Mat &result) {
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    kernel.at<float>(1,1) = 5.0;
    kernel.at<float>(0,1) = -1.0;
    kernel.at<float>(2,1) = -1.0;
    kernel.at<float>(1,0) = -1.0;
    kernel.at<float>(1,2) = -1.0;
    
    cv::filter2D(image, result, image.depth(), kernel); // input image can be any type
}









int main(int argc, const char * argv[]) {
    cv::Mat image = cv::imread("littleHedgehog.jpg");
    //cv::namedWindow("OriginalImage");
    //cv::imshow("OriginalImage", image);
    cv::Mat imageClone = image.clone();
    cv::Mat imageGray;
    cv::cvtColor(image, imageGray, CV_BGR2GRAY); // imgproc.hpp
    
    /*
    // 1.
    salt(image, 3000);
    cv::namedWindow("SaltedImage");
    cv::imshow("SaltedIm:age", imageClone);
    
    // 4.
    colorReduce3(imageClone);
    cv::namedWindow("ColorReducedImage");
    cv::imshow("ColorReducedImage", imageClone);
    
    // 5.
    cv::Mat imageSharp;
    sharpen2D(imageGray, imageSharp);
    cv::namedWindow("SharpenedImage");
    cv::imshow("SharpenedImage", imageSharp);
    */
    // 6. Performing simple image arthmetic
    // Combining two images
    cv::Mat image2 = cv::imread("sandArt.jpg");
    cv::Mat image2Resize;
    cv::resize(image2, image2Resize, image.size());
    cv::Mat imageSum;
    cv::addWeighted(image, 0.7, image2Resize, 0.3, 0., imageSum);
    cv::namedWindow("SumImage");
    //cv::imshow("SumImage", imageSum);
     
    
    //TODO: Splitting channels
    /*
    cv::Mat result;
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    planes[0] += image2Resize; // ??? it doesn't work
    cv::merge(planes, result);
    */
    
    // 7. Defining regions of interest
    cv::Mat imageROI;
    imageROI = image(cv::Rect(0, 0, image2.cols, image2.rows));
    //imageROI points to the same data buffer as image
    cv::addWeighted(imageROI, 0.7, image2, 0.3, 0., imageROI);
    //cv::Mat image2Gray;
    //cv::cvtColor(image2, image2Gray, CV_BGR2GRAY);
    //image2.copyTo(imageROI,image2Gray); //mask
    cv::namedWindow("ROI");
    cv::imshow("ROI", image);
    
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}
