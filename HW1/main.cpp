#include<opencv2/opencv.hpp>
#include<iostream>
#include <string>

void SetValue(cv::Mat &frame, int degree){
    degree -= 50;


    cv::Mat changed;

    if(degree < 0) {
        degree *= 5;
        cv::cvtColor(frame,changed,cv::COLOR_BGR2HSV);
        for (int i = 0; i < changed.rows; i++) {
            for (int j = 0; j < changed.cols; j++) {
                changed.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(changed.at<cv::Vec3b>(i, j)[2] + degree);
            }
        }
        cv::cvtColor(changed,frame,cv::COLOR_HSV2BGR);
    }
    else{
        degree *= 5;
        cv::cvtColor(frame,changed,cv::COLOR_BGR2HLS);
        for (int i = 0; i < changed.rows; i++) {
            for (int j = 0; j < changed.cols; j++) {
                changed.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(changed.at<cv::Vec3b>(i, j)[1] + degree);
            }
        }
        cv::cvtColor(changed,frame,cv::COLOR_HLS2BGR);
    }


}
void SetBrightness(cv::Mat &frame, int degree){
    degree -= 50;
    degree *= 5;
    for(int i = 0; i < frame.rows; i++){
        for(int j = 0; j < frame.rows; j++){
            for(int k = 0; k < 3; k++) {
                frame.at<cv::Vec3b>(i, j)[k] = cv::saturate_cast<uchar>(frame.at<cv::Vec3b>(i, j)[k]+degree);
            }
        }
    }
}
int main() {
    cv::String path = "C:\\Users\\jc\\Desktop\\pycharmprojects\\Assignments_DigitalMediaComputing\\img.jpg";
    cv::Mat origin_img = cv::imread(path);

    cv::Mat img = origin_img.clone();
    cv::namedWindow("control",cv::WINDOW_AUTOSIZE);
    int mid = 50;
    cv::createTrackbar("value","control", &mid,100);
    //cv::createTrackbar("brightness","control", &mid,100);
    while(true) {
        int value = cv::getTrackbarPos("value", "control");
        //int brightness = cv::getTrackbarPos("brightness", "control");
        img = origin_img.clone();
        SetValue(img,value);
        //SetBrightness(img, brightness);
        cv::imshow("value_changed", img);
        if(cv::waitKey(1) == 'q'){
            break;
        }

    }
    return 0;
}
