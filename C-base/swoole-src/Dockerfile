FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    jq

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
    php-dev

WORKDIR /workspace

RUN git clone https://github.com/swoole/swoole-src buggy \
    && cd buggy \
    && git checkout a4256e4 \
    && phpize \
    && ./configure