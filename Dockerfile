FROM gcc:latest

LABEL maintainer="kohei_lab"
LABEL description="cpp_mt"

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ="Asia/Tokyo"

WORKDIR /home/project

ENV TERM="xterm-color"

RUN apt-get -qq update && apt-get -qq upgrade -y
RUN apt-get install git
