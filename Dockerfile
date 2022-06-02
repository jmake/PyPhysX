## https://hub.docker.com/repository/docker/jmake/pyphysx
#FROM jmake/pyphysx:a AS PHYSICS
#
#USER root 
#
#ENV NB_USER="jovyan" 
#RUN chown -R ${NB_USER} /home/jovyan/work

FROM jmake/repository:bccc06d9833b59f03b99d8dde4e46ca473a4fbc7

ENV IPYNB_FILE="basic_test.ipynb"

ENV NB_USER="jovyan" 

USER root 

WORKDIR /home/jovyan/work 

COPY ${IPYNB_FILE} /home/jovyan/work 

RUN chown -R ${NB_USER} /home/jovyan/work

USER ${NB_USER}



## jupyter password
## http://localhost:10000/?token=TOKEN_CODE
## jupyter server list
## jupyter nbconvert --execute --clear-output 
