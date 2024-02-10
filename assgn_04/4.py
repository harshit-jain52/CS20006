import numpy as np

# Load 3 CSV files into separate numpy 1D arrays, calculate their respective means and store using list comprehension
means = [
    np.loadtxt(
        f"book{i}.csv", skiprows=1, delimiter="\t", usecols=1, dtype=float
    ).mean()
    for i in range(1, 4)
]
print(means)
