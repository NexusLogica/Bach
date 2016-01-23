//
//  main.cpp
//  retina
//
//  Created by Lawrence Gunn on 6/15/15.
//  Copyright (c) 2015 Lawrence Gunn. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

uint8_t getRed(uint8_t* data, int x, int y, Size sz, int numChannels) {
    return data[x*sz.width*numChannels + y*numChannels + 2];
}

uint8_t getGreen(uint8_t* data, int x, int y, Size sz, int numChannels) {
    return data[x*sz.width*numChannels + y*numChannels + 1];
}

uint8_t getBlue(uint8_t* data, int x, int y, Size sz, int numChannels) {
    return data[x*sz.width*numChannels + y*numChannels + 0];
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    Mat image;
    image = imread("/Users/lgunn/Downloads/camera-mount-low-cable-v1_3dm.png", CV_LOAD_IMAGE_COLOR);   // Read the file
    Size sz = image.size();
    int numChannels = image.channels();
    std::cout << "Size is " << sz.width << " by " << sz.height << endl;
    std::cout << "Num channels " << numChannels << endl;
    
    
    uint8_t* imgData = (uint8_t*)image.data;
    uint8_t red = getRed(imgData, 0, 0, sz, numChannels);
    uint8_t green = getGreen(imgData, 0, 0, sz, numChannels);
    uint8_t blue = getBlue(imgData, 0, 0, sz, numChannels);
    std::cout << "rgb  #" << std::hex << (int) red << std::hex << (int) green << std::hex << (int) blue << endl;

    return 0;
}
