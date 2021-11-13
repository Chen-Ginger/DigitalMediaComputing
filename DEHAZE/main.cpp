#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>

// get dark_channel
void Dark_Channel(cv::Mat frame,cv::Mat *changed, int KernelSize){

    KernelSize /= 2;
    std::vector<cv::Mat> BGRchannel(3);
    cv::Mat minBGR(frame.size(),CV_8UC1,cv::Scalar(0));
    //get min of BGR
    cv::split(frame,BGRchannel);
    for(int i = 0; i < frame.rows; i ++){
        for(int j = 0; j < frame.cols; j++){
            minBGR.at<uchar>(i,j) = std::min({BGRchannel[0].at<uchar>(i,j),BGRchannel[1].at<uchar>(i,j),BGRchannel[2].at<uchar>(i,j)});
        }
    }

    //min filter

    cv::Mat dark_channel = minBGR.clone();
    for(int i = 0; i < frame.rows; i++){
        for(int j = 0; j < frame.cols; j++){

            uchar mn = minBGR.at<uchar>(i,j);
            for(int i_kernel = i - KernelSize; i_kernel <= i + KernelSize; i_kernel++){
                for(int j_kernel = j - KernelSize; j_kernel <= j + KernelSize; j_kernel++){
                    if(i_kernel < 0 || j_kernel < 0 || i_kernel >= frame.rows || j_kernel >= frame.cols ){
                        continue;
                    }
                    else{
                            mn = std::min(mn,minBGR.at<uchar>(i_kernel,j_kernel));
                    }
                }
            }
            dark_channel.at<uchar>(i,j) = mn;

        }
    }
    *changed = dark_channel.clone();


}

void get_t(cv::Mat frame,cv::Mat &changed, int A){
    double ratio = A / 255.0 ;
    cv::Mat dark ;
    frame.convertTo(dark,CV_32FC1, 1/255.0);
    dark = 1 - dark/ratio;

    changed = dark;
}

void DeHaze(cv::Mat frame,cv::Mat t, cv::Mat &changed, int A ){
    cv::Mat I,T;
    frame.convertTo(I,CV_32FC3,1/255.0);
    t.convertTo(T,CV_32FC1,1/255.0);
    double ratio = A / 255.0;
    std::vector<cv::Mat> divI;
    cv::split(I,divI);
    for(int i = 0; i < T.rows; i++){
        for(int j = 0; j < T.cols; j++){
            if(T.at<float>(i,j) <= 0.2){
                T.at<float>(i,j) = 0.2;
            }
        }
    }
    for(int i = 0; i < 3; i++)
        divI[i] = (divI[i] - ratio) / (T) + ratio;
    cv::merge(divI,changed);

}

void guidedFilter_oneChannel(cv::Mat &srcImg, cv::Mat &guideImg, cv::Mat &dstImg,  int rad = 9, const double eps = 0.01)
{
    if(!(rad&1)) rad++;


//    srcImg.convertTo(srcImg, CV_32FC1, 1.0 / 255.0);
    guideImg.convertTo(guideImg, CV_32FC1, 1.0 / 255.0);


    cv::Mat mean_srcImg, mean_guideImg;
    boxFilter(srcImg, mean_srcImg, CV_32FC1, cv::Size(rad, rad));
    boxFilter(guideImg, mean_guideImg, CV_32FC1, cv::Size(rad, rad));

    cv::Mat mean_guideImg_square, mean_guideImg_srcImg;
    boxFilter(guideImg.mul(guideImg), mean_guideImg_square, CV_32FC1, cv::Size(rad, rad));
    boxFilter(guideImg.mul(srcImg), mean_guideImg_srcImg, CV_32FC1, cv::Size(rad, rad));

    cv::Mat var_guideImg = mean_guideImg_square - mean_guideImg.mul(mean_guideImg);
    cv::Mat cov_guideImg_srcImg = mean_guideImg_srcImg - mean_guideImg.mul(mean_srcImg);

    cv::Mat aImg = cov_guideImg_srcImg / (var_guideImg + eps);
    cv::Mat bImg = mean_srcImg - aImg.mul(mean_guideImg);

    cv::Mat mean_aImg, mean_bImg;
    boxFilter(aImg, mean_aImg, CV_32FC1, cv::Size(rad, rad));
    boxFilter(bImg, mean_bImg, CV_32FC1, cv::Size(rad, rad));

    dstImg = (mean_aImg.mul(guideImg) + mean_bImg);

    dstImg.convertTo(dstImg, CV_8UC1, 255);

}

int main() {


    cv::String path = "C:\\Users\\jc\\Desktop\\Assignments_DigitalMediaComputing\\DEHAZE\\an.png";
    cv::Mat origin_img = cv::imread(path);
    cv::Mat frame = origin_img.clone();
    cv::Mat gray;
    cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
    for(int i = 0; i < gray.rows;i++){
        for(int j = 0; j < gray.cols; j++){
            gray.at<uchar>(i,j) = 255 - gray.at<uchar>(i,j);
        }
    }
//    cv::imshow("gray",gray);
//    cv::waitKey(0);
    cv::Mat dark_channel(frame.size(),CV_8UC1,cv::Scalar(0));
    Dark_Channel(frame,&dark_channel,23);

    cv::Mat t(frame.size(),CV_32FC3,cv::Scalar(0));


    cv::Mat Dehazed;

    cv::namedWindow("control",cv::WINDOW_NORMAL);
    cv::resizeWindow("control",cv::Size(800,200));
    int init = 50;
    cv::createTrackbar("Natural light(A)","control",&init,100);
    int r = 30;
    cv::createTrackbar("guidedfilter_Radius","control",&r,200);
    int eps = 50;
    cv::createTrackbar("regularation","control",&eps,200);
//    get_t(dark_channel,t,150);
//    cv::Mat g(frame.size(),CV_8UC1,cv::Scalar(212));
//    guidedFilter_oneChannel(t,gray,g,55,0.0001);
//    cv::imshow("da",gray);
//    cv::imshow("dsa",g);
//    cv::imshow("ds",t);
//    cv::waitKey(0);
    while(true) {
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int A = cv::getTrackbarPos("Natural light(A)","control");
        A += 155;
        int R = cv::getTrackbarPos("guidedfilter_Radius","control");
        int Re = cv::getTrackbarPos("regularation","control");
        Re *= 10;
        double eps = 1.0 / Re;

        cv::Mat Gray = gray.clone();
        get_t(dark_channel,t,A);
        cv::Mat T(frame.size(),CV_8UC1,cv::Scalar(0));
        guidedFilter_oneChannel(t,Gray,T,R,eps);
//        std::cout<<t.type()<<std::endl;
//        std::cout<<T.type()<<std::endl;
        DeHaze(frame,T,Dehazed,A);
        cv::imshow("DeHazed", Dehazed);
        cv::imshow("origin",frame);
        cv::imshow("darkchannel_with_guidedfilter",T);
        cv::imshow("darkchannel",t);

    }
    return 0;
}

