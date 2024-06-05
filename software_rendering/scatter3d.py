import matplotlib.pyplot as plt

# Define the points
points = [
        (1553, 1186),
        (1280, 1138),
        (1553, 800),

        (1280, 1138),
        (1280, 800),
        (1553, 800),

        (1006, 1186),
        (1280, 1250),
        (1006, 800),

        (1280, 1250),
        (1280, 800),
        (1006, 800),

        (1553, 800),
        (1280, 1250),
        (1553, 1186),

        (1553, 800),
        (1280, 800),
        (1280, 1250),

        (1006, 800),
        (1280, 1138),
        (1006, 1186),

        (1006, 800),
        (1280, 800),
        (1280, 1138),

        (1553, 800),
        (1006, 800),
        (1280, 800),

        (1553, 800),
        (1280, 800),
        (1006, 800),

        (1280, 1138),
        (1280, 1250),
        (1006, 1186),

        (1280, 1138),
        (1553, 1186),
        (1280, 1250),
]

# Extract x and y coordinates
x_coords = [point[0] for point in points]
y_coords = [point[1] for point in points]

# Create the plot
plt.figure(figsize=(25.6, 16))
plt.scatter(x_coords, y_coords)
plt.xlim((0, 2560))
plt.ylim((0, 1600))

# Set plot title and labels
plt.title('Scatter Plot of Points')
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')

# Show the plot
plt.show()

