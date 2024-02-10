import numpy as np
import cv2 as cv
import time

# Read grayscale image using OpenCV
img = cv.imread("a.png", cv.IMREAD_GRAYSCALE)

# Store the image in numpy 2D array
X = np.asarray(img).astype("int64")

# Transpose of matrix X
Y = np.transpose(X)

# Convert numpy arrays to python lists
A = X.tolist()
B = Y.tolist()

# Matrix Multiplication without using numpy library
s = time.time()
C = [[sum(a * b for a, b in zip(A_row, B_col)) for B_col in zip(*B)] for A_row in A]
print("Without using numpy:", time.time() - s, "sec")
