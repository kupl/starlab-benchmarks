FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    wget \
    jq \
    libtool \
    flex \
    bison \
    autopoint \
    autoconf \
    automake \
    build-essential \
    pkg-config \
    libssl-dev \
    libtasn1-dev  \
    libpcap-dev \
    libdaq-dev \
    libdumbnet-dev \
    libluajit-5.1-dev \
    libpcre3-dev \
    libnghttp2-dev \
    zlib1g-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace
COPY . /workspace
WORKDIR /workspace

RUN export BUGGY_PATH=$(cat metadata.json | jq -r ".buggyPath") \
    && wget "https://www.snort.org/downloads/archive/snort/snort-2.9.13.tar.gz" --no-check-certificate \
    && tar xvf snort-2.9.13.tar.gz \
    && mv snort-2.9.13 $BUGGY_PATH \
    && cp util.h $BUGGY_PATH/src/ \
    && cd $BUGGY_PATH \
    && ./configure

