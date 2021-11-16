#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <thread>


void BoxFilter(cv::Mat frame){
    cv::namedWindow("BoxFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("BoxFilter",cv::Size(frame.rows, frame.cols));
    int Kernelsizeinit = 1;
    cv::createTrackbar("KernelSize", "BoxFilter",&Kernelsizeinit, 100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSize = cv::getTrackbarPos("KernelSize","BoxFilter");
        if(!(KernelSize & 1)){
            KernelSize++;
        }
        cv::blur(frame,changed,cv::Size(KernelSize,KernelSize));
        cv::imshow("BoxFilter",changed);
    }
}


void BoxFilter_Naive(cv::Mat frame, int size){
    cv::namedWindow("BoxFilter_Naive",cv::WINDOW_NORMAL);
    cv::resizeWindow("BoxFilter_Naive",cv::Size(frame.rows, frame.cols));
    cv::Mat changed = frame;
    size /= 2;
    int weight = (size*2+1)*(size*2+1);
//    std::cout<<weight<<std::endl;
    for(int i = 0; i < changed.rows; i++){
        for(int j = 0; j < changed.cols; j++){
            for(int k = 0; k < 3; k++) {
                int sum = 0;
                for (int i_kernel = i - size; i_kernel <= i + size; i_kernel++) {
                    for (int j_kernel = j - size; j_kernel <= j + size; j_kernel++) {
                        if (i_kernel < 0 || j_kernel < 0 || i_kernel >= changed.rows || j_kernel >= changed.rows) {
                            sum += frame.at<cv::Vec3b>(i,j)[k];
                        }
                        else {
                            sum += frame.at<cv::Vec3b>(i_kernel,j_kernel)[k];
                        }

                    }
                }
                sum /= weight;
                changed.at<cv::Vec3b>(i,j)[k] = sum;
            }
        }
    }
    cv::imshow("BoxFilter_Naive",changed);
    cv::waitKey(0);
}
void MedianFilter(cv::Mat frame){
    cv::namedWindow("MedianFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("MedianFilter",cv::Size(frame.rows, frame.cols));
    int Kernelsizeinit = 1;
    cv::createTrackbar("KernelSize", "MedianFilter",&Kernelsizeinit, 100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSize = cv::getTrackbarPos("KernelSize","MedianFilter");
        if(!(KernelSize & 1)){
            KernelSize++;
        }
        cv::medianBlur(frame,changed,KernelSize);
        cv::imshow("MedianFilter",changed);
    }
}

void MedianFilterNaive(cv::Mat frame, int size){
    cv::namedWindow("MedianFilter_Naive",cv::WINDOW_NORMAL);
    cv::resizeWindow("MedianFilter_Naive",cv::Size(frame.rows, frame.cols));
    cv::Mat changed = frame;
    size /= 2;

    for(int i = 0; i < changed.rows; i++){
        for(int j = 0; j < changed.cols; j++){
            for(int k = 0; k < 3; k++){
                std::vector<uchar> V;
                for(int i_kernel = i-size; i_kernel <= i+size; i_kernel++ ){
                    for(int j_kernel = j - size; j_kernel <= j+size; j_kernel++){
                        if(i_kernel < 0 || j_kernel < 0 || i_kernel >= changed.rows || j_kernel >= changed.cols){
                            continue;
                        }
                        else{
                            V.push_back(frame.at<cv::Vec3b>(i_kernel,j_kernel)[k]);
                        }
                    }
                }
                std::sort(V.begin(),V.end());
                int half_len = V.size() / 2;
                changed.at<cv::Vec3b>(i,j)[k] = V[half_len];

            }
        }
    }
    cv::imshow("MedianFilter_Naive",changed);
    cv::waitKey(0);


}


void GuassianFilter(cv::Mat frame){
    cv::namedWindow("GuassianFilter",cv::WINDOW_NORMAL);
    cv::resizeWindow("GuassianFilter",cv::Size(frame.rows, frame.cols+50));
    int Kernelsizeinit = 1;
    cv::createTrackbar("KernelSize", "GuassianFilter",&Kernelsizeinit, 100);
    cv::createTrackbar("Sigma", "GuassianFilter",&Kernelsizeinit, 100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }
        int KernelSizeGaussian = cv::getTrackbarPos("KernelSize","GuassianFilter");
        if(!(KernelSizeGaussian & 1)){
            KernelSizeGaussian++;
        }

        int sigma = cv::getTrackbarPos("Sigma","GuassianFilter");
        if(!(KernelSizeGaussian & 1)){
            KernelSizeGaussian++;
        }
        cv::GaussianBlur(frame,changed,cv::Size(KernelSizeGaussian,KernelSizeGaussian),sigma,sigma);
        cv::imshow("GuassianFilter",changed);
    }
}

void GaussianFilter(const cv::Mat &src, cv::Mat &dst, int ksize, double sigma)
{
    CV_Assert(src.channels() || src.channels() == 3);
    const static double pi = 3.1415926;
    // 根据窗口大小和sigma生成高斯滤波器模板
    // 申请一个二维数组，存放生成的高斯模板矩阵
    double **templateMatrix = new double*[ksize];
    for (int i = 0; i < ksize; i++)
        templateMatrix[i] = new double[ksize];
    int origin = ksize / 2; // 以模板的中心为原点
    double x2, y2;
    double sum = 0;
    for (int i = 0; i < ksize; i++)
    {
        x2 = pow(i - origin, 2);
        for (int j = 0; j < ksize; j++)
        {
            y2 = pow(j - origin, 2);
            // 高斯函数前的常数可以不用计算，会在归一化的过程中给消去
            double g = exp(-(x2 + y2) / (2 * sigma * sigma));
            sum += g;
            templateMatrix[i][j] = g;
        }
    }
    for (int i = 0; i < ksize; i++)
    {
        for (int j = 0; j < ksize; j++)
        {
            templateMatrix[i][j] /= sum;
            std::cout << templateMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    // 将模板应用到图像中
    int border = ksize / 2;
    copyMakeBorder(src, dst, border, border, border, border, cv::BorderTypes::BORDER_REFLECT);
    int channels = dst.channels();
    int rows = dst.rows - border;
    int cols = dst.cols - border;
    for (int i = border; i < rows; i++)
    {
        for (int j = border; j < cols; j++)
        {
            double sum[3] = { 0 };
            for (int a = -border; a <= border; a++)
            {
                for (int b = -border; b <= border; b++)
                {
                    if (channels == 1)
                    {
                        sum[0] += templateMatrix[border + a][border + b] * dst.at<uchar>(i + a, j + b);
                    }
                    else if (channels == 3)
                    {
                        cv::Vec3b rgb = dst.at<cv::Vec3b>(i + a, j + b);
                        auto k = templateMatrix[border + a][border + b];
                        sum[0] += k * rgb[0];
                        sum[1] += k * rgb[1];
                        sum[2] += k * rgb[2];
                    }
                }
            }
            for (int k = 0; k < channels; k++)
            {
                if (sum[k] < 0)
                    sum[k] = 0;
                else if (sum[k] > 255)
                    sum[k] = 255;
            }
            if (channels == 1)
                dst.at<uchar>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3)
            {
                cv::Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
                dst.at<cv::Vec3b>(i, j) = rgb;
            }
        }
    }
}


void BilateralFilter(cv::Mat frame){
    cv::namedWindow("BilateralFilter",cv::WINDOW_GUI_NORMAL);
    cv::resizeWindow("BilateralFilter",cv::Size(frame.rows, frame.cols+100));
    int Kernelsizeinit = 1;
    cv::createTrackbar("Diameter","BilateralFilter",&Kernelsizeinit,100);
    cv::createTrackbar("SigmaColor","BilateralFilter",&Kernelsizeinit,100);
    cv::createTrackbar("SigmaSpace","BilateralFilter",&Kernelsizeinit,100);
    cv::Mat changed;
    while(true){
        if(cv::waitKey(1) == 'q'){
            break;
        }


        int sigmaColor = cv::getTrackbarPos("SigmaColor", "BilateralFilter");
        int sigmaSpace = cv::getTrackbarPos("SigmaSpace", "BilateralFilter");
        int diameter = cv::getTrackbarPos("Diameter", "BilateralFilter");



        cv::bilateralFilter(frame,changed,diameter,sigmaColor,sigmaSpace);
        cv::imshow("BilateralFilter",changed);
    }
}

bool my_guidedFilter_threeChannel(cv::Mat &srcImg, cv::Mat &guideImg, cv::Mat &dstImg, const int rad = 9, const double eps = 0.01);



int main() {

    cv::String path = "C:\\Users\\jc\\Desktop\\pycharmprojects\\Assignments_DigitalMediaComputing\\img.jpg";
    cv::String path1 = "C:\\Users\\jc\\Desktop\\Assignments_DigitalMediaComputing\\HW2\\wei.jpg";
    cv::Mat origin_img = cv::imread(path);
    cv::Mat guidedimg = cv::imread(path1);
    //cv::resize(guidedimg,guidedimg,cv::Size(origin_img.rows, origin_img.cols));
    cv::Mat frame = origin_img.clone();
    cv::Mat changed = origin_img.clone();
//    std::thread BoxThread(BoxFilter,frame), MedianThread(MedianFilter,frame),GuassianThread(GuassianFilter,frame),
//                BilateralThreaad(BilateralFilter,frame);
//    BoxThread.join();
//    MedianThread.join();
//    GuassianThread.join();
//    BilateralThreaad.join();
//    BoxFilter_Naive(frame,5);
//    MedianFilterNaive(frame,5);
    int init = 1;
    cv::namedWindow("guidedfilter");
    cv::createTrackbar("size","guidedfilter",&init,100);
    cv::createTrackbar("regularation","guidedfilter",&init,100);
    while(true){
        if(cv::waitKey(1) == 'q') break;
        int size = cv::getTrackbarPos("size","guidedfilter");
        double eps = 1.0 / cv::getTrackbarPos("regularation","guidedfilter") ;
        my_guidedFilter_threeChannel(frame,guidedimg,changed,size,eps);
        cv::imshow("guidedfilter",changed);
    }

    return 0;

//    single_thread head^ version
//    -------------------------------------------------------------------------------------------------------------
//    cv::namedWindow("BoxFilter",cv::WINDOW_NORMAL);
//    cv::resizeWindow("BoxFilter",cv::Size(frame.rows, frame.cols));
//    cv::namedWindow("MedianFilter",cv::WINDOW_NORMAL);
//    cv::resizeWindow("MedianFilter",cv::Size(frame.rows, frame.cols));
//    cv::namedWindow("GuassianFilter",cv::WINDOW_NORMAL);
//    cv::resizeWindow("GuassianFilter",cv::Size(frame.rows, frame.cols+50));
//    cv::namedWindow("BilateralFilter",cv::WINDOW_GUI_NORMAL);
//    cv::resizeWindow("BilateralFilter",cv::Size(frame.rows, frame.cols+100));
//    int Kernelsizeinit = 1;
//    cv::createTrackbar("KernelSize", "BoxFilter",&Kernelsizeinit, 100);
//    cv::createTrackbar("KernelSize", "MedianFilter",&Kernelsizeinit, 100);
//    cv::createTrackbar("KernelSize", "GuassianFilter",&Kernelsizeinit, 100);
//    cv::createTrackbar("Sigma", "GuassianFilter",&Kernelsizeinit, 100);
//    cv::createTrackbar("SigmaColor","BilateralFilter",&Kernelsizeinit,100);
//    cv::createTrackbar("SigmaSpace","BilateralFilter",&Kernelsizeinit,100);
//    cv::createTrackbar("Diameter","BilateralFilter",&Kernelsizeinit,100);
//
//    cv::Mat changedBox;
//    cv::Mat changedMed;
//    cv::Mat changedGus;
//    cv::Mat changedBil;
//    while(true){
//        if(cv::waitKey(1) == 'q'){
//            break;
//        }
//        int KernelSize = cv::getTrackbarPos("KernelSize","BoxFilter");
//        if(!(KernelSize & 1)){
//            KernelSize++;
//        }
//        int KernelSizeMedian = cv::getTrackbarPos("KernelSize","MedianFilter");
//        if(!(KernelSizeMedian & 1)){
//            KernelSizeMedian++;
//        }
//        int KernelSizeGaussian = cv::getTrackbarPos("KernelSize","GuassianFilter");
//        if(!(KernelSizeGaussian & 1)){
//            KernelSizeGaussian++;
//        }
//
//        int sigma = cv::getTrackbarPos("Sigma","GuassianFilter");
//
//
//        int sigmaColor = cv::getTrackbarPos("SigmaColor", "BilateralFilter");
//        int sigmaSpace = cv::getTrackbarPos("SigmaSpace", "BilateralFilter");
//        int diameter = cv::getTrackbarPos("Diameter", "BilateralFilter");
//
//        cv::blur(frame,changedBox,cv::Size(KernelSize,KernelSize));
//        cv::imshow("BoxFilter",changedBox);
//
//        cv::medianBlur(frame,changedMed,KernelSizeMedian);
//        cv::imshow("MedianFilter",changedMed);
//
//        cv::GaussianBlur(frame,changedGus,cv::Size(KernelSizeGaussian,KernelSizeGaussian),sigma,sigma);
//        cv::imshow("GuassianFilter",changedGus);
//
//        cv::bilateralFilter(frame,changedBil,diameter,sigmaColor,sigmaSpace);
//        cv::imshow("BilateralFilter",changedBil);
//    }

//    cv::Mat kernal = (cv::Mat_<uchar>(3,3) << 0,0,0,0,1,0,0,0,0 );
//    cv::Mat in = (cv::Mat_<uchar>(3,3) << 1,2,3,4,5,6,7,8,9 );
//    cv::Mat out;
//    cv::filter2D(in,out,in.depth(),kernal,cv::Point(0,0));
//    std::cout<<out<<std::endl;
}
