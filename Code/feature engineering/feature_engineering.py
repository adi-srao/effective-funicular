import numpy as np
import pandas as pd
import laspy
from scipy.spatial import cKDTree
from tqdm import tqdm
import sys

# To run: py {this file} {input las} {output csv}

las = laspy.read(sys.argv[1])

points = np.vstack([las.x, las.y, las.z]).T
labels = las.classification

print(f"Processing {len(points):,} points...")

# Build KD-tree (in this case OctTree)
tree = cKDTree(points)

# Initialise features (linearity, scattering and planarity)
n_points = len(points)
cl = np.zeros(n_points)
cs = np.zeros(n_points)
cp = np.zeros(n_points)

for i in tqdm(range(n_points)):
    _, indices = tree.query(points[i], k=20)
    neighbors = points[indices]
    
    cov = np.cov(neighbors.T)
    
    # Eigenvalues in descending order (representing extent of the ellipsoid neighbourhood) 
    eigenvalues = np.sort(np.linalg.eigvalsh(cov))[::-1]
    eigenvalues = np.maximum(eigenvalues, 0) 
    
    lambda1, lambda2, lambda3 = eigenvalues[0], eigenvalues[1], eigenvalues[2]
    if lambda1 > 1e-10: # ensuring non-zero dibision
        cl[i] = (lambda1 - lambda2) / lambda1      # Linearity
        cs[i] = lambda3 / lambda1                   # Scattering
        cp[i] = (lambda2 - lambda3) / lambda1       # Planarity

df = pd.DataFrame({
    'x': points[:, 0],
    'y': points[:, 1],
    'z': points[:, 2],
    'cl': cl,
    'cs': cs,
    'cp': cp,
    'label': labels
})

output = sys.argv[2] 
df.to_csv(output, index=False)
print(f"Saved {len(df):,} rows to {output} \n")