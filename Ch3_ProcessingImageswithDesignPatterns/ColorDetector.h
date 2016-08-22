//
//  ColorDetector.h
//  Ch3_ProcessingImageswithDesignPatterns
//  1. Using Strategy pattern
//  Identify all pixels wiht a color
//
//  Created by Shijia Geng on 8/22/16.
//  Copyright © 2016 SG. All rights reserved.
//

#ifndef ColorDetector_h
#define ColorDetector_h
class ColorDetector {
    int minDist; // minimum acceptable distance
    cv::Vec3b target; // target color
    cv::Mat result;
    int getDistance (const cv::Vec3b& color) const;
    
public:
    ColorDetector() : minDist(100) {
        target[0] = target[1] = target[2] = 0;
    }
    cv::Mat process(const cv::Mat &image);
    void setColorDistanceThreshold(int distance);
    int getColorDistanceThreshold() const;
    void setTargetColor(unsigned char red, unsigned char green, unsigned char blue);
    void setTargetColor(cv::Vec3b color);
    cv::Vec3b getTargetColor() const;
    
};


#endif /* ColorDetector_h */
