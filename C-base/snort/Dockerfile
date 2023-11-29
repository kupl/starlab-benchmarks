FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autopoint \
        autoconf \
        automake \
        bison \
        build-essential \
        ca-certificates \
        flex \
        jq \
        libdaq-dev \
        libdumbnet-dev \
        libluajit-5.1-dev \
        libnghttp2-dev \
        libpcap-dev \
        libpcre3-dev \
        libssl-dev \
        libtasn1-dev  \
        libtool \
        pkg-config \
        wget \
        zlib1g-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY ./src /tmp/src
WORKDIR /workspace
RUN mkdir -p buggy \
    && wget -q "https://www.snort.org/downloads/archive/snort/snort-2.9.13.tar.gz" -O - | tar -zxvf - -C ./buggy --strip-components=1 \
    && mv /tmp/src/util.h /workspace/buggy/src/util.h \
    && cd buggy \
    && ./configure
