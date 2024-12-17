# Use an official GCC image as a base
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy only the necessary files first (e.g., headers and source files)
COPY include /app/include
COPY src /app/src
COPY main.cpp /app

# Compile the C++ source files
RUN g++ -o file_system_simulator main.cpp src/models/*.cpp src/services/*.cpp src/storage/*.cpp -I include

# Set the command to run the binary
CMD ["./file_system_simulator"]
