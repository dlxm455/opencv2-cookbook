//
//  main.cpp
//  Ch3_ProcessingImageswithDesignPatterns
//
//  Created by Shijia Geng on 8/22/16.
//  Copyright © 2016 SG. All rights reserved.
//

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ColorDetector.h"




// 2. Using Controller




// 3. Using Singleton




// 4. Using Model-View-Controller(MVC)




// 5. Coverting color spaces


int main(int argc, const char * argv[]) {
    ColorDetector cdetect;
    cv::Mat image = cv::imread("littleHedgehog.jpg");
    if(!image.data) return 0;
    cdetect.setTargetColor(0, 0, 0);
    cv::namedWindow("result");
    cv::imshow("result", cdetect.process(image));
    //cv::namedWindow("original");
    //cv::imshow("original", image);
    cv::waitKey();
    return 0;
}
