import numpy as np
import cv2 as cv

# Read grayscale image using OpenCV
img = cv.imread("a.png", cv.IMREAD_GRAYSCALE)

# Store the image in numpy 2D array
X = np.asarray(img)

# Two ways to Binarize the grayscale image with different thresholds (*255 to get black-white distinction)
Z50 = np.array(X > 50).astype("uint8") * 255
Z70 = np.array(X > 70).astype("uint8") * 255
_, Z100 = cv.threshold(X, 100, 255, cv.THRESH_BINARY)
_, Z150 = cv.threshold(X, 150, 255, cv.THRESH_BINARY)

# Display binarized images
for z, thresh in zip([Z50, Z70, Z100, Z150], [50, 70, 100, 150]):
    cv.imshow(f"threshold={thresh}", z)
    cv.waitKey()  # Wait for key press
    cv.destroyAllWindows()
