#include <iostream>
#include <opencv2/opencv.hpp>

void SimpleFilter(cv::Mat frame){
    cv::namedWindow("SimpleFilter",cv::WINDOW_NORMAL);
    int Kernelsizeinit = 0;
    cv::createTrackbar("KernelSize", "SimpleFilter",&Kernelsizeinit, 100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSize = cv::getTrackbarPos("KernelSize","SimpleFilter");
        if(!(KernelSize & 1)){
            KernelSize++;
        }
        cv::blur(frame,changed,cv::Size(KernelSize,KernelSize));
        cv::imshow("SimpleFilter",changed);
    }
}
void MedianFilter(cv::Mat frame){
    cv::namedWindow("SimpleFilter",cv::WINDOW_NORMAL);
    int Kernelsizeinit = 0;
    cv::createTrackbar("KernelSize", "SimpleFilter",&Kernelsizeinit, 100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSize = cv::getTrackbarPos("KernelSize","SimpleFilter");
        if(!(KernelSize & 1)){
            KernelSize++;
        }
        cv::blur(frame,changed,cv::Size(KernelSize,KernelSize));
        cv::imshow("SimpleFilter",changed);
    }
}

int main() {

    cv::String path = "C:\\Users\\jc\\Desktop\\pycharmprojects\\Assignments_DigitalMediaComputing\\img.jpg";
    cv::Mat origin_img = cv::imread(path);
    //SimpleFilter(origin_img);
    MedianFilter(origin_img);
    return 0;
}
