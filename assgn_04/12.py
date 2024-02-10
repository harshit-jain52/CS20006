import numpy as np
import cv2 as cv

# Read color image using OpenCV
img = cv.imread("a.png", cv.IMREAD_COLOR)

# Store the image in numpy 3D array
X = np.asarray(img)

# Store the given filter as numpy 2D array
filter = np.array(
    [[-1, -1, -1], [0, 0, 0], [1, 1, 1]],
)

# Apply filter on the image using convolution
flt_img = cv.filter2D(src=img, ddepth=-1, kernel=filter)

# Display image
cv.imshow("Convolved with filter", flt_img)
cv.waitKey(2000)  # Wait 2 seconds
