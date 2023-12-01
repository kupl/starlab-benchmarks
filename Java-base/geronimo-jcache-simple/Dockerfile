FROM ubuntu:22.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y software-properties-common \
    && add-apt-repository ppa:deadsnakes/ppa \
    && apt-get update \
    && apt-get install -y \
        build-essential \
        git \
        vim \
        jq \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/list/*

RUN apt-get -y install sudo \
      openjdk-8-jdk \
      maven

RUN bash -c "echo 2 | update-alternatives --config java"

COPY src /workspace
WORKDIR /workspace

RUN mvn install -V -B -Denforcer.skip=true -Dcheckstyle.skip=true -Dcobertura.skip=true -Drat.skip=true -Dlicense.skip=true -Dfindbugs.skip=true -Dgpg.skip=true -Dskip.npm=true -Dskip.gulp=true -Dskip.bower=true -Drat.numUnapprovedLicenses=100 -DskipTests=true -DskipITs=true -Dtest=None -DfailIfNoTests=false

RUN mvn test -V -B -Denforcer.skip=true -Dcheckstyle.skip=true -Dcobertura.skip=true -Drat.skip=true -Dlicense.skip=true -Dfindbugs.skip=true -Dgpg.skip=true -Dskip.npm=true -Dskip.gulp=true -Dskip.bower=true -Drat.numUnapprovedLicenses=100

ENV TZ=Asia/Seoul
