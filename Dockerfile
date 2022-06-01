## 
## SEE : 
##   jupyter/base-notebook @ https://jupyter-docker-stacks.readthedocs.io/en/latest/using/selecting.html
##   https://github.com/spicytechs/PhysX/blob/4.1/Actions/Dockerfile.binder
##  
## https://hub.docker.com/repository/docker/jmake/mybinder
##FROM jmake/mybinder:last AS PHYSICS
## https://hub.docker.com/repository/docker/jmake/pyphysx
FROM jmake/pyphysx:a AS PHYSICS 

## BASIC 
USER root
RUN apt-get --yes -qq update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN apt-get --yes -qq update
RUN apt-get --yes -qq install vim  


## JOVYAN  
FROM notebook_setup AS notebook_execute 
ENV NB_USER="jovyan" 
WORKDIR /home/jovyan/work 
RUN chown -R ${NB_USER} /home/jovyan/work

COPY ./ /home/jovyan/work/ 

## jupyter password
## http://localhost:10000/?token=TOKEN_CODE
## jupyter server list
## jupyter nbconvert --execute --clear-output 
