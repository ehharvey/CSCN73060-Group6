FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update
RUN apt-get install -y python3 python3-pip python-is-python3
RUN apt-get install -y build-essential
RUN apt-get install -ylibboost-all-dev

COPY ./add-bazel.sh /tmp
RUN /tmp/add-bazel.sh && rm -f /tmp/add-bazel.sh
