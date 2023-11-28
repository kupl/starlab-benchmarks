FROM base

RUN mkdir -p /workspace
WORKDIR /workspace
COPY ./metadata.json /workspace/metadata.json

RUN export BUGGY_PATH=$(cat metadata.json | jq -r ".buggyPath") \
    && mv /opt/snort-2.9.13 /${BUGGY_PATH} \
    && cd $BUGGY_PATH \
    && ./configure

