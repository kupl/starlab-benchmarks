FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autopoint \
        autoconf \
        automake \
        bison \
        build-essential \
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

WORKDIR /opt
RUN wget "https://www.snort.org/downloads/archive/snort/snort-2.9.13.tar.gz" --no-check-certificate \
    && tar xvf snort-2.9.13.tar.gz \
    && rm snort-2.9.13.tar.gz
COPY ./src/util.h /opt/snort-2.9.13/src/util.h
