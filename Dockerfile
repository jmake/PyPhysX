## https://hub.docker.com/repository/docker/jmake/pyphysx
FROM jmake/pyphysx:a AS PHYSICS

USER root 

ENV NB_USER="jovyan" 
RUN chown -R ${NB_USER} /home/jovyan/work


## jupyter password
## http://localhost:10000/?token=TOKEN_CODE
## jupyter server list
## jupyter nbconvert --execute --clear-output 
