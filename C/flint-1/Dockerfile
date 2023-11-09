FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autoconf \
        automake \
        build-essential \
        jq \
        libgmp-dev \
        libmpfr-dev \
        libtool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace
COPY . /workspace

WORKDIR /workspace
RUN export BUGGY_PATH=$(cat metadata.json | jq -r ".buggyPath") \
    && cd $BUGGY_PATH \
    && ./bootstrap.sh \
    && ./configure