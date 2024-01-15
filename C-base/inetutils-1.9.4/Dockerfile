FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    git \
    jq \
    wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /workspace

RUN mkdir -p buggy \
    && wget -q "https://ftp.gnu.org/gnu/inetutils/inetutils-1.9.4.tar.gz" -O - | tar -zxvf - -C ./buggy --strip-components=1 \
    && cd buggy \
    && ./configure
