import numpy as np
import cv2 as cv

# Read color image using OpenCV
img = cv.imread("a.png", cv.IMREAD_COLOR)

# Convert color image to grayscale image by calculating mean of (R,G,B) values
X = np.asarray([[col.mean() for col in row] for row in img]).astype("uint8")
print(X)

# Display Image
cv.imshow("OUTPUT", X)
cv.waitKey(2000)  # Wait 2 seconds
