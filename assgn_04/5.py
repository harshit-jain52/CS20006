import numpy as np
import cv2 as cv

# Read color image using OpenCV
img = cv.imread("a.png", cv.IMREAD_COLOR)

# Store the image in numpy 3D array
X = np.asarray(img)
print(X)

# Display Image
cv.imshow("OUTPUT", img)
cv.waitKey(2000)  # Wait 2 seconds
