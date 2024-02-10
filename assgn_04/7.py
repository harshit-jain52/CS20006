import numpy as np
import cv2 as cv
import time

# Read grayscale image using OpenCV
img = cv.imread("a.png", cv.IMREAD_GRAYSCALE)

# Store the image in numpy 2D array
X = np.asarray(img).astype("int64")

# Transpose of matrix X
Y = np.transpose(X)

# Matrix Multiplication using '@' operator of numpy library
s = time.time()
Z = X @ Y
print("Using numpy @:", time.time() - s, "sec")

# Matrix Multiplication using matmul() function of numpy library
s = time.time()
Z = np.matmul(X, Y)
print("Using numpy matmul:", time.time() - s, "sec")
