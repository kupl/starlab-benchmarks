FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    jq \
    automake \
    libtool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /workspace

RUN git clone https://github.com/dbry/WavPack buggy \
    && cd buggy \
    && git checkout 22977b2 \
    && ./autogen.sh \
    && ./configure \