import numpy as np

def transform_normal(normal, model_matrix):
    # Compute the normal matrix (inverse transpose of the upper-left 3x3 part of the model matrix)
    normal_matrix = np.transpose(np.linalg.inv(model_matrix[:3, :3]))
    print(normal_matrix)
    # Transform the normal vector
    transformed_normal = np.dot(normal_matrix, normal)
    # Normalize the transformed normal vector
    return transformed_normal / np.linalg.norm(transformed_normal)

normal = np.array([0.0, 0.0, 1.0])

# Define a model matrix with a rotation around the Y-axis by 45 degrees
theta = np.radians(45)
cos_theta = np.cos(theta)
sin_theta = np.sin(theta)
model_matrix = np.array([
    [cos_theta, 0.0, sin_theta, 0.0],
    [0.0, 1.0, 0.0, 0.0],
    [-sin_theta, 0.0, cos_theta, 0.0],
    [0.0, 0.0, 0.0, 1.0]
])

transformed_normal = transform_normal(normal, model_matrix)
print("Transformed Normal (Rotation):", transformed_normal)
