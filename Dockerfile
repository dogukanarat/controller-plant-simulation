FROM gcc:latest

RUN apt-get update 
RUN apt-get -y upgrade
RUN apt-get install make
RUN apt-get install nano
RUN apt-get update
RUN apt-get clean
RUN mkdir little-puffin

WORKDIR /little-puffin

COPY . ./little-puffin

