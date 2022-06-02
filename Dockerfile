## 
## SEE : 
##   jupyter/base-notebook @ https://jupyter-docker-stacks.readthedocs.io/en/latest/using/selecting.html
##   https://github.com/spicytechs/PhysX/blob/4.1/Actions/Dockerfile.binder
##  
FROM jupyter/base-notebook:latest AS notebook_setup 

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
