# Use the official Ubuntu image as a base
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    libpqxx-dev \
    libboost-system-dev \
    libboost-thread-dev \
    wget

# Install websocketpp (since it's a header-only library, we just download it)
RUN wget https://github.com/zaphoyd/websocketpp/archive/refs/tags/0.8.2.tar.gz -O /tmp/websocketpp.tar.gz && \
    tar -xvzf /tmp/websocketpp.tar.gz -C /tmp && \
    cp -r /tmp/websocketpp-0.8.2/websocketpp /usr/local/include/

# Set the working directory
WORKDIR /BinanceTicker

# Copy the current directory contents into the container
COPY . /BinanceTicker

# Create build directory
RUN mkdir -p build

# Build the application
RUN cd build && cmake .. && make

# Run the application
CMD ["./build/BinanceTicker"]
