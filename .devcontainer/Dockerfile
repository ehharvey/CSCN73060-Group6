FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update
RUN apt-get install -y python3 python3-pip python-is-python3
RUN apt-get install -y build-essential
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y gdb
RUN apt-get install -y clang-format

COPY ./add-bazel.sh /tmp/add-bazel.sh
RUN chmod +x /tmp/add-bazel.sh
RUN /tmp/add-bazel.sh && rm -f /tmp/add-bazel.sh
