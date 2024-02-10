import cv2 as cv

# Read grayscale image using OpenCV
img = cv.imread("a.png", cv.IMREAD_GRAYSCALE)

# Draw a black rectangle at the position [(40,100) top right, (70, 200) bottom left] on the image
cv.rectangle(img, (40, 100), (70, 200), color=(0, 0, 0), thickness=cv.FILLED)

# Display Image
cv.imshow("OUTPUT", img)
cv.waitKey(2000)  # Wait 2 seconds
