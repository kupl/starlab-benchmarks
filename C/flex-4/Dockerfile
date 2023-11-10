FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autoconf \
        automake \
        autopoint \
        bison \
        build-essential \
        ca-certificates \
        flex \
        git \
        jq \
        libtool \
        texinfo \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace
COPY . /workspace
WORKDIR /workspace

RUN export BUGGY_PATH=$(cat metadata.json | jq -r ".buggyPath") \
    && git clone https://github.com/westes/flex.git $BUGGY_PATH \
    && cd $BUGGY_PATH \
    && git checkout d3de49f84224b18c5287653f20525291b24cc26e \
    && ./autogen.sh \
    && ./configure

