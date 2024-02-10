import numpy as np

# Load CSV File (skipping header and using 2nd column) into a numpy array
arr = np.loadtxt("book1.csv", skiprows=1, delimiter="\t", usecols=1, dtype=int)

# Maximum and Minimum of elements in the numpy array
print(f"Max: {arr.max()} | Min: {arr.min()}")
# Alt: np.max(arr), np.min(arr)

print(arr)
