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
	gettext \
	pkg-config \
	libffi-dev \
	libtasn1-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
RUN git clone https://github.com/p11-glue/p11-kit buggy \
    && cd buggy \
    && git checkout ead7a4a2 \
    && ./autogen.sh \
    && ./configure

