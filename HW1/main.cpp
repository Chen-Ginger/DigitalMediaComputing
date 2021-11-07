#include<opencv2/opencv.hpp>
#include<iostream>
#include <string>
#include<vector>
void SetValue(cv::Mat &frame, int degree){
    degree -= 50;
    cv::Mat changed;
    degree *= 5;
    cv::cvtColor(frame,changed,cv::COLOR_BGR2HSV);
    for (int i = 0; i < changed.rows; i++) {
        for (int j = 0; j < changed.cols; j++) {
            changed.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(changed.at<cv::Vec3b>(i, j)[2] + degree);
        }
    }
    cv::cvtColor(changed,frame,cv::COLOR_HSV2BGR);
}

void SetSaturation(cv::Mat frame, int degree){
    degree -= 50;
    degree *= 5;
    cv::Mat changed;
    cv::cvtColor(frame,changed,cv::COLOR_BGR2HSV);
    for(int i = 0; i < changed.rows; i++){
        for(int j = 0; j < changed.cols;j++){
            changed.at<cv::Vec3b>(i,j)[1] = cv::saturate_cast<uchar>(changed.at<cv::Vec3b>(i,j)[1] + degree);
        }
    }
    cv::cvtColor(changed,frame,cv::COLOR_HSV2BGR);
}
void SetContract(cv::Mat frame, int degree){
    degree -= 50;
    degree *= 2;
    double ratio = degree / 100.0;

    cv::Scalar mean  = cv::mean(frame);

    for(int i = 0; i < frame.rows; i++){
        for(int j = 0; j < frame.cols; j++){
            for(int k = 0; k < 3; k++){
                frame.at<cv::Vec3b>(i,j)[k] = cv::saturate_cast<uchar>(
                        frame.at<cv::Vec3b>(i,j)[k]+int(ratio*(frame.at<cv::Vec3b>(i,j)[k]-mean[k])));
            }
        }
    }

}
void SetHue(cv::Mat frame, int degree){
    degree -= 50;
    degree *= 4;
    cv::Mat changed;
    cv::cvtColor(frame,changed,cv::COLOR_BGR2HSV);
    for(int i = 0; i < changed.rows; i++){
        for(int j = 0; j < changed.cols; j++){
            changed.at<cv::Vec3b>(i,j)[0] = cv::saturate_cast<uchar>(changed.at<cv::Vec3b>(i,j)[0] + degree);
            if(changed.at<cv::Vec3b>(i,j)[0] > 180) {
                changed.at<cv::Vec3b>(i,j)[0] = 180;
            }
        }
    }
    cv::cvtColor(changed,frame, cv::COLOR_HSV2BGR);
}
int main() {
    cv::String path = "C:\\Users\\jc\\Desktop\\pycharmprojects\\Assignments_DigitalMediaComputing\\img.jpg";
    cv::Mat origin_img = cv::imread(path);

    cv::Mat img = origin_img.clone();

    cv::namedWindow("control");
    cv::namedWindow("changed");
    cv::resizeWindow("control",cv::Size(1000,200));
    int mid = 50;
    cv::createTrackbar("value","control", &mid,100);
    cv::createTrackbar("saturation","control", &mid,100);
    cv::createTrackbar("contract","control", &mid,100);
    cv::createTrackbar("hue","control", &mid,100);
    while(true) {
        int value = cv::getTrackbarPos("value", "control");
        int saturation = cv::getTrackbarPos("saturation", "control");
        int contrcat = cv::getTrackbarPos("contract","control");
        int hue = cv::getTrackbarPos("hue","control");
        img = origin_img.clone();
        SetValue(img,value);
        SetSaturation(img, saturation);
        SetHue(img,hue);
        SetContract(img,contrcat);

        cv::imshow("changed", img);
        if(cv::waitKey(1) == 'q'){
            break;
        }

    }
    return 0;
}
