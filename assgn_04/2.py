import numpy as np

# Load CSV File (skipping header and using 2nd column) into a numpy array
arr = np.loadtxt("book1.csv", skiprows=1, delimiter="\t", usecols=1, dtype=int)

# Sort the numpy array
arr.sort()  # Changes the array
print(arr)
# Alt: print(np.sort(arr)) Doesn't change the array
