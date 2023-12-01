FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    jq

RUN apt-get install -y --no-install-recommends \
    libedit-dev  \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
RUN git clone https://github.com/yrp604/rappel buggy \
    && cd buggy \
    && git checkout ad8efd7