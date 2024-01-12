FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    jq \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /workspace
RUN git clone https://github.com/mirror/x264 buggy \
    && cd buggy \
    && git checkout d4099dd \
    && ./configure --disable-asm