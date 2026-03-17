import laspy
import numpy as np
import sys

# To run: py {this file} {input laz}

files = [arg for arg in sys.argv[1:]]

laz_file = files[0]
las = laspy.read(laz_file)

total_points = len(las.points)
indices = np.arange(total_points)


x = np.array(las.x[indices])
y = np.array(las.y[indices])
z = np.array(las.z[indices])
print(f"X: min={x.min():.6f},   max={x.max():.6f},   range={x.max() - x.min():.6f}")
print(f"Y: min={y.min():.6f},   max={y.max():.6f},   range={y.max() - y.min():.6f}")
print(f"Z: min={z.min():.6f},       max={z.max():.6f},       range={z.max() - z.min():.6f}")

if hasattr(las, 'classification'):
    classification = las.classification[indices]
    unique = np.unique(classification)
    print(f"Unique classes: {unique}")
    print("Class distribution:")
    
    classes = {         #ASPRS Classes
        0: 'Never classified',
        1: 'Unclassified',
        2: 'Ground',
        3: 'Low Vegetation',
        4: 'Medium Vegetation',
        5: 'High Vegetation',
        6: 'Building',
        7: 'Low Point (noise)',
        8: 'Reserved',
        9: 'Water',
        10: 'Rail',
        11: 'Road Surface',
        12: 'Reserved',
        13: 'Wire - Guard (Shield)',
        14: 'Wire - Conductor (Phase)',
        15: 'Transmission Tower',
        16: 'Wire-structure Connector',
        17: 'Bridge Deck',
        18: 'High Noise',
    }
    for c in unique:
        count = (classification == c).sum()
        percentage = count / len(classification) * 100
        name = classes.get(c, 'Unknown')
        print(f"  Class {c}: {name:}: {count} points ({percentage:.2f}%)")



