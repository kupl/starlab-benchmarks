FROM java-base/commons-beanutils:cached

ENV TZ=Asia/Seoul

COPY ./metadata.json .
COPY ./npe.json .
COPY ./buggy.java /tmp/buggy.java
RUN export BUGGY_PATH=$(cat metadata.json | jq -r ".npe.filepath") \
    && export BUGGY_LINE=$(cat metadata.json | jq -r ".npe.line") \
    && export BUGGY_MTHD=$(cat metadata.json | jq -r ".npe.npe_method") \
    && mv /tmp/buggy.java $BUGGY_PATH \
    && echo "[{\"filepath\": \"$BUGGY_PATH\", \"line\": $BUGGY_LINE, \"method_name\": \"$BUGGY_MTHD\"}]" | jq . > traces.json

RUN git init . && git add -A

RUN $(cat metadata.json | jq -r ".buildCommand")

RUN $(cat metadata.json | jq -r ".testCommand"); if [ $? -eq 0 ]; then exit 1; fi
