# Use an official Ubuntu as a parent image
FROM ubuntu:latest

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    zlib1g-dev

# Create a working directory
WORKDIR /usr/src/app

# Copy your project files to the container
COPY . .

# Run your build commands
# Assuming you have a CMakeLists.txt file configured for FetchContent
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j 12

# The following line is optional if you want to run tests or the application directly
CMD ["./app"]
