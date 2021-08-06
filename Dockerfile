FROM gcc:latest

RUN apt-get update 
RUN apt-get -y upgrade
RUN apt-get -y install cmake
RUN apt-get install make
RUN apt-get install nano
RUN apt-get update
RUN apt-get clean
RUN mkdir project_workspace

WORKDIR /project_workspace

COPY . ./project_workspace

