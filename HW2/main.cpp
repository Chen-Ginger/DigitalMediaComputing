#include <iostream>
#include <opencv2/opencv.hpp>

//void BoxFilter(cv::Mat frame){
//    cv::namedWindow("BoxFilter",cv::WINDOW_NORMAL);
//    cv::resizeWindow("BoxFilter",cv::Size(frame.rows, frame.cols));
//    int Kernelsizeinit = 0;
//    cv::createTrackbar("KernelSize", "BoxFilter",&Kernelsizeinit, 100);
//    cv::Mat changed;
//    while(true){
//        if(cv::waitKey(1) == 'q'){
//            break;
//        }
//        int KernelSize = cv::getTrackbarPos("KernelSize","BoxFilter");
//        if(!(KernelSize & 1)){
//            KernelSize++;
//        }
//        cv::blur(frame,changed,cv::Size(KernelSize,KernelSize));
//        cv::imshow("BoxFilter",changed);
//    }
//}
//void MedianFilter(cv::Mat frame){
//    cv::namedWindow("MedianFilter",cv::WINDOW_NORMAL);
//    int Kernelsizeinit = 0;
//    cv::createTrackbar("KernelSize", "MedianFilter",&Kernelsizeinit, 100);
//    cv::Mat changed;
//    while(true){
//        if(cv::waitKey(1) == 'q'){
//            break;
//        }
//        int KernelSize = cv::getTrackbarPos("KernelSize","MedianFilter");
//        if(!(KernelSize & 1)){
//            KernelSize++;
//        }
//        cv::medianBlur(frame,changed,KernelSize);
//        cv::imshow("MedianFilter",changed);
//    }
//}

int main() {

    cv::String path = "C:\\Users\\jc\\Desktop\\pycharmprojects\\Assignments_DigitalMediaComputing\\img.jpg";
    cv::Mat origin_img = cv::imread(path);
    cv::Mat frame = origin_img.clone();
    cv::namedWindow("BoxFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("BoxFilter",cv::Size(frame.rows, frame.cols));
    cv::namedWindow("MedianFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("MedianFilter",cv::Size(frame.rows, frame.cols));
    cv::namedWindow("GuassianFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("GuassianFilter",cv::Size(frame.rows, frame.cols+50));
    cv::namedWindow("BilateralFilter",cv::WINDOW_GUI_NORMAL);
    cv::resizeWindow("BilateralFilter",cv::Size(frame.rows, frame.cols+100));
    int Kernelsizeinit = 1;
    cv::createTrackbar("KernelSize", "BoxFilter",&Kernelsizeinit, 100);
    cv::createTrackbar("KernelSize", "MedianFilter",&Kernelsizeinit, 100);
    cv::createTrackbar("KernelSize", "GuassianFilter",&Kernelsizeinit, 100);
    cv::createTrackbar("Sigma", "GuassianFilter",&Kernelsizeinit, 100);
    cv::createTrackbar("SigmaColor","BilateralFilter",&Kernelsizeinit,100);
    cv::createTrackbar("SigmaSpace","BilateralFilter",&Kernelsizeinit,100);
    cv::createTrackbar("Diameter","BilateralFilter",&Kernelsizeinit,100);

    cv::Mat changedBox;
    cv::Mat changedMed;
    cv::Mat changedGus;
    cv::Mat changedBil;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSize = cv::getTrackbarPos("KernelSize","BoxFilter");
        if(!(KernelSize & 1)){
            KernelSize++;
        }
        int KernelSizeMedian = cv::getTrackbarPos("KernelSize","MedianFilter");
        if(!(KernelSizeMedian & 1)){
            KernelSizeMedian++;
        }
        int KernelSizeGaussian = cv::getTrackbarPos("KernelSize","GuassianFilter");
        if(!(KernelSizeGaussian & 1)){
            KernelSizeGaussian++;
        }

        int sigma = cv::getTrackbarPos("Sigma","GuassianFilter");


        int sigmaColor = cv::getTrackbarPos("SigmaColor", "BilateralFilter");
        int sigmaSpace = cv::getTrackbarPos("SigmaSpace", "BilateralFilter");
        int diameter = cv::getTrackbarPos("Diameter", "BilateralFilter");

        cv::blur(frame,changedBox,cv::Size(KernelSize,KernelSize));
        cv::imshow("BoxFilter",changedBox);

        cv::medianBlur(frame,changedMed,KernelSizeMedian);
        cv::imshow("MedianFilter",changedMed);

        cv::GaussianBlur(frame,changedGus,cv::Size(KernelSizeGaussian,KernelSizeGaussian),sigma,sigma);
        cv::imshow("GuassianFilter",changedGus);

        cv::bilateralFilter(frame,changedBil,diameter,sigmaColor,sigmaSpace);
        cv::imshow("BilateralFilter",changedBil);
    }


    return 0;
//    cv::Mat kernal = (cv::Mat_<uchar>(3,3) << 0,0,0,0,1,0,0,0,0 );
//    cv::Mat in = (cv::Mat_<uchar>(3,3) << 1,2,3,4,5,6,7,8,9 );
//    cv::Mat out;
//    cv::filter2D(in,out,in.depth(),kernal,cv::Point(0,0));
//    std::cout<<out<<std::endl;
}
