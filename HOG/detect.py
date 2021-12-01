# @Start date : 2021/12/1 

# @Last update date : 2021/12/1 

# @Author : jc

# @File : detect

# @Software: PyCharm

import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

if __name__ == '__main__':
    src = cv.imread(r"C:\Users\jc\Desktop\Assignments_DigitalMediaComputing\HOG\img_test\11.jpg")
    cv.imshow("input", src)

    hog = cv.HOGDescriptor()
    hog.setSVMDetector(cv.HOGDescriptor_getDefaultPeopleDetector())
    # Detect people in the image
    (rects, weights) = hog.detectMultiScale(src,
                                            winStride=(2, 4),
                                            padding=(8, 8),
                                            scale=1.2,
                                            useMeanshiftGrouping=False)
    for (x, y, w, h) in rects:
        cv.rectangle(src, (x, y), (x + w, y + h), (0, 255, 0), 2)

    cv.imshow("hog-detector", src)
    cv.imwrite("hog-detector.jpg", src)
    cv.waitKey(0)
    cv.destroyAllWindows()