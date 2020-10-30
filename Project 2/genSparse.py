from scipy.sparse import rand
from scipy.stats import uniform
import numpy as np

n = 20
m = 20

file = open("mtx.txt", "w")

A_dense = np.random.randint(2, size=(n, m))

file.write(str(n))
file.write("\n")
file.write(str(m))
file.write("\n")
file.write('\n'.join(' '.join(str(cell) for cell in row) for row in A_dense))
file.write("\n")

file.close()
