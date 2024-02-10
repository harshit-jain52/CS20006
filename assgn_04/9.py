import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

# Read grayscale image using OpenCV
img = cv.imread("a.png", cv.IMREAD_GRAYSCALE)

# Store the image in numpy 2D array
X = np.asarray(img)

# Flatten the numpy array and plot the pixel intensity histogram
plt.hist(np.ravel(X), 256, [0, 256])
plt.title("Pixel Intensity Histogram")
plt.get_current_fig_manager().canvas.set_window_title("Grayscale a.PNG")
plt.show()
