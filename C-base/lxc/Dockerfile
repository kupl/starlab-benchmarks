FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    pkg-config \
    jq

RUN apt-get install -y --no-install-recommends \
    clang \
    libtool \
    m4 \
    autopoint

WORKDIR /workspace
RUN git clone https://github.com/lxc/lxc buggy \
    && cd buggy \
    && git checkout 72cc48f \
    && ./autogen.sh \
    && ./configure CC=clang