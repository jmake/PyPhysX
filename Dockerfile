## https://hub.docker.com/repository/docker/jmake/pyphysx
#FROM jmake/pyphysx:a AS PHYSICS
#
#USER root 
#
#ENV NB_USER="jovyan" 
#RUN chown -R ${NB_USER} /home/jovyan/work


FROM jupyter/base-notebook:latest

RUN conda install -c conda-forge cxx-compiler vim 

RUN conda install -c anaconda numpy

RUN conda install -c anaconda swig

RUN conda install -c anaconda make

RUN conda install -c anaconda cmake

## jupyter password
## http://localhost:10000/?token=TOKEN_CODE
## jupyter server list
## jupyter nbconvert --execute --clear-output 
