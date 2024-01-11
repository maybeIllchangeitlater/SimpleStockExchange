FROM gcc:latest

WORKDIR /usr/src/app

COPY . .

RUN apt-get update && \
    apt-get install -y \
        cmake \
        libc++-dev \
        libboost-all-dev \
        libpq-dev \
        libcrypto++-dev \
        gfortran-12 \
        libcoarrays-dev \
        libcoarrays-openmpi-dev \
        libibverbs-dev \
        libopenmpi-dev \
        libhdf5-openmpi-dev libblas-dev gfortran-\
        && \
    rm -rf /var/lib/apt/lists/* && \
    dpkg --configure -a  # Configure any pending packages

WORKDIR /usr/src/app/3rdParty/cryptopp890
RUN make clean && make static dynamic
WORKDIR /usr/src/app

RUN cmake -Bbuild -H. && \
    cmake --build build

EXPOSE 5050

CMD ["./build/ServerR"]
