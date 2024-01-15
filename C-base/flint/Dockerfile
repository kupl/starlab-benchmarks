FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autoconf \
        automake \
        build-essential \
        ca-certificates \
	git \
        jq \
        libgmp-dev \
        libmpfr-dev \
        libtool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
RUN git clone https://github.com/flintlib/flint.git buggy \
    && cd buggy \
    && git checkout 4ed43a \
    && ./bootstrap.sh \
    && ./configure

