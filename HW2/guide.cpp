//
// Created by jc on 2021/11/11.
//
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
bool my_guidedFilter_oneChannel(Mat &srcImg, Mat &guideImg, Mat &dstImg,  int rad = 9,  double eps = 0.01)
{


    //--------------转换数值类型，并归一化-------------
    srcImg.convertTo(srcImg, CV_32FC1, 1.0 / 255.0);
    guideImg.convertTo(guideImg, CV_32FC1, 1.0 / 255.0);

    //--------------求引导图像和输入图像的均值图
    Mat mean_srcImg, mean_guideImg;
    boxFilter(srcImg, mean_srcImg, CV_32FC1, Size(rad, rad));
    boxFilter(guideImg, mean_guideImg, CV_32FC1, Size(rad, rad));

    Mat mean_guideImg_square, mean_guideImg_srcImg;
    boxFilter(guideImg.mul(guideImg), mean_guideImg_square, CV_32FC1, Size(rad, rad));
    boxFilter(guideImg.mul(srcImg), mean_guideImg_srcImg, CV_32FC1, Size(rad, rad));

    Mat var_guideImg = mean_guideImg_square - mean_guideImg.mul(mean_guideImg);
    Mat cov_guideImg_srcImg = mean_guideImg_srcImg - mean_guideImg.mul(mean_srcImg);

    Mat aImg = cov_guideImg_srcImg / (var_guideImg + eps);
    Mat bImg = mean_srcImg - aImg.mul(mean_guideImg);

    Mat mean_aImg, mean_bImg;
    boxFilter(aImg, mean_aImg, CV_32FC1, Size(rad, rad));
    boxFilter(bImg, mean_bImg, CV_32FC1, Size(rad, rad));

    dstImg = (mean_aImg.mul(guideImg) + mean_bImg);

    dstImg.convertTo(dstImg, CV_8UC1, 255);

    return true;
}

//--------------------------------------------------------------
//函数名：my_guidedFilter_threeChannel
//函数功能：用于三通道图像（RGB彩色图）的引导滤波函数；
//参数：Mat &srcImg：输入图像，为三通道图像；
//参数：Mat &guideImg：引导图像，为三通道图像，尺寸与输入图像一致；
//参数：Mat &dstImg：输出图像，尺寸、通道数与输入图像吻合；
//参数：const int rad：滤波器大小，应该保证为奇数，默认值为9；
//参数：const double eps ：防止a过大的正则化参数，

bool my_guidedFilter_threeChannel(Mat &srcImg, Mat &guideImg, Mat &dstImg, int rad = 9,  double eps = 0.01)
{
    //----------------确保输入参数正确-------------
    if(!(rad & 1)) rad++;
    vector<Mat> src_vec, guide_vec,dst_vec;
    split(srcImg, src_vec);
    split(guideImg, guide_vec);

    for (int i = 0; i < 3; i++)
    {
        Mat tempImg = Mat::zeros(srcImg.rows, srcImg.cols, CV_8UC1);
        /*Mat tempImg;*/
        my_guidedFilter_oneChannel(src_vec[i], guide_vec[i], tempImg, rad, eps);
        dst_vec.push_back(tempImg);
    }

    merge(dst_vec, dstImg);

    return true;
}
