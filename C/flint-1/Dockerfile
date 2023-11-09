FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autoconf \
        build-essential \
        libgmp-dev \
        libmpfr-dev \
        libtool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace
WORKDIR /workspace
COPY . /workspace
