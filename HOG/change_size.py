# @Start date : 2021/12/1 

# @Last update date : 2021/12/1 

# @Author : jc

# @File : change_size

# @Software: PyCharm

#coding=utf-8
from PIL import Image
import os
image_width = 100
image_height = 128
def fixed_size(filePath,savePath):
    """按照固定尺寸处理图片"""
    im = Image.open(filePath)
    out = im.resize((image_width, image_height), Image.ANTIALIAS)
    out.save(savePath)


def changeSize():
    filePath = r'C:\Users\jc\Desktop\Assignments_DigitalMediaComputing\HOG\img_test'
    destPath = r'C:\Users\jc\Desktop\Assignments_DigitalMediaComputing\HOG\img_test_changed'
    if not os.path.exists(destPath):
        os.makedirs(destPath)
    for root, dirs, files in os.walk(filePath):
        for file in files:
            if file[-1]=='g':
                fixed_size(os.path.join(filePath, file), os.path.join(destPath, file))
    print('Done')

if __name__ == '__main__':
    changeSize()
