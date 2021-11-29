# @Start date : 2021/11/27

# @Last update date : 2021/11/27

# @Author : jc

# @File : sift_in

# @Software: PyCharm

import numpy as np

import cv2 as cv

from matplotlib import pyplot as plt


if __name__ == "__main__":

    imageA = cv.imread('1.jpg')

    imageB = cv.imread('2.jpg')

    grayA = cv.cvtColor(imageA, cv.COLOR_BGR2GRAY)

    cv.imshow("grayA", grayA)

    grayB = cv.cvtColor(imageB, cv.COLOR_BGR2GRAY)

    cv.imshow("grayB", grayB)

    min_hessian = 1000

    sift = cv.SIFT_create(min_hessian)

    keypointsA, featuresA = sift.detectAndCompute(grayA,None)

    keypointsB, featuresB = sift.detectAndCompute(grayB,None)

    kpImgA=cv.drawKeypoints(grayA,keypointsA,imageA)

    kpImgB=cv.drawKeypoints(grayB,keypointsB,imageB)

    cv.imshow("kpImgA", kpImgA)

    cv.imshow("kpImgB", kpImgB)

    FLANN_INDEX_KDTREE = 0

    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)

    search_params = dict(checks=50)

    # 使用FlannBasedMatcher 寻找最近邻近似匹配

    flann = cv.FlannBasedMatcher(index_params,search_params)

    # 使用knnMatch匹配处理，并返回匹配matches

    matches = flann.knnMatch(featuresA, featuresB, k=2)

    matchesMask = [[0,0] for i in range(len(matches))]

    coff = 0.8 # 0.1 0.7  0.8

    for i,(m,n) in enumerate(matches):

        if m.distance < coff * n.distance:

            matchesMask[i]=[1,0]

    draw_params = dict(matchColor = (0,0,255),

                       singlePointColor = (0,0,255),

                       matchesMask = matchesMask,

                       flags = 0)

    resultImg = cv.drawMatchesKnn(grayA, keypointsA, grayB,keypointsB, matches,None,**draw_params)

    resultImg1 = cv.drawMatchesKnn(imageA, keypointsA, imageB,keypointsB, matches,None,**draw_params)



    cv.imshow("resultImg", resultImg)

    cv.imshow("resultImg_gray", resultImg1)

    cv.waitKey(0)

    cv.destroyAllWindows()