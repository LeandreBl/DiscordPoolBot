FROM ubuntu:18.04

ENV PYTHONUNBUFFERED 1

RUN mkdir -p /app/bot

WORKDIR /app/bot

COPY . .

RUN apt-get update -y                                       \
    && apt-get install -y software-properties-common        \
    && apt-get update -y                                    \
    && add-apt-repository ppa:snaipewastaken/ppa            \
    && apt-get upgrade -y                                   \
    && apt-get install -y python3-pip wget criterion-dev    \
    && python3 -m pip install --upgrade pip                 \
    && python3 -m pip install discord
