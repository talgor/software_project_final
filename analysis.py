import sys
import numpy as np
import math
import symnmf
from sklearn.metrics import silhouette_score

EPSILON = 0.0001
ITER = 300

# Classes and functions for K-Means implementation from HW1
class Point:
    def __init__(self, coordinates):
        self.coords = tuple(coordinates)
        self.d = len(self.coords)
        self.closestCenter = None

    def updateClosestCenter(self, CenterPoints):
        for center in CenterPoints:
            dist = self.distanceTo(center)
            if self.closestCenter is None or dist < self.distanceTo(self.closestCenter):
                self.closestCenter = center

    def distanceTo(self, otherPoint):
        if self.d != otherPoint.d:
            raise ValueError("Points must have the same dimension to calculate distance.")
        return math.sqrt(sum((a - b) ** 2 for a, b in zip(self.coords, otherPoint.coords)))

class CenterPoint:
    def __init__(self, coordinates):
        self.coords = tuple(coordinates)
        self.d = len(self.coords)
        self.assignedPoints = []

    def updateCoordinates(self):
        if not self.assignedPoints:
            return
        sum_squared = 0
        new_coords = []
        for i in range(self.d):
            coord_sum = sum(point.coords[i] for point in self.assignedPoints)
            new_coords.append(coord_sum / len(self.assignedPoints))
            squared_diff = (new_coords[-1] - self.coords[i]) ** 2
            sum_squared += squared_diff
        if len(new_coords) != self.d:
            raise ValueError("Dimension mismatch when updating coordinates.")
        self.coords = tuple(new_coords)
        self.assignedPoints = []
        movement_distance = math.sqrt(sum_squared)
        return movement_distance > EPSILON

def updateAllPoints(points, centerPoints):
    for point in points:
        try:
            point.updateClosestCenter(centerPoints)
        except Exception as e:
            print(f"Error updating closest center for a point: {e}")
            continue
        point.closestCenter.assignedPoints.append(point)

def updateAllCenters(centerPoints):
    stopRun = True
    for center in centerPoints:
        if center.updateCoordinates():
            stopRun = False
    return stopRun

def kmeans(data_points, k, max_iterations=ITER):
    # Initialize points and start centroids from the first k points
    points = [Point(p) for p in data_points]
    centerPoints = [CenterPoint(point.coords) for point in points[:k]]
    for _ in range(max_iterations):
        # Assign each point to the closest cluster head
        updateAllPoints(points, centerPoints)

        # Update centroid coordinates and check for convergence (epsilon)
        if updateAllCenters(centerPoints):
            break
    
    # Return the final labels (indices) for silhouette calculation
    return [centerPoints.index(p.closestCenter) for p in points]

def main():
    if len(sys.argv) != 3:
        print("An Error Has Occurred")
        sys.exit(1)
    try:
        # Parse command line arguments
        k= int(sys.argv[1])
        file_name = sys.argv[2]

        # Load data from file and convert to list format
        data = np.loadtxt(file_name, delimiter=',')
        data_list = data.tolist()

        # Calculate SymNMF labels
        final_H = symnmf.compute_H(file_name, k)
        # Determine cluster labels by finding the index of the maximum value in each row
        sym_labels = np.argmax(final_H, axis=1)

        # Calculate K-Means labels using the implemented algorithm
        km_labels = kmeans(data_list, k)

        # Compute Silhouette scores for both methods
        score_sym = silhouette_score(data, sym_labels)
        score_km = silhouette_score(data, km_labels)

        print(f"nmf: {score_sym:.4f}")
        print(f"kmeans: {score_km:.4f}")
    
    except Exception:
        print("An Error Has Occurred")
        sys.exit(1)

if __name__ == "__main__":
    main()