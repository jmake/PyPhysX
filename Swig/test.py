#import os; os.system("clear")
import numpy as np

import sys; 
sys.path.append("./BUILD")

import swig_example

## X.1. 
physx = swig_example.Physx() 
physx.Init()
physx.Evolve()

## X.1. 
Keys = swig_example.vectorc(physx.GetKeysSize()); 
physx.GetKeys(Keys)

narray = physx.GetArraySize()
Fields = {k:np.zeros(narray) for k in Keys}

## X.1. 
nsteps = 3
for istep in range(nsteps):
    physx.Evolve()
    for k in Fields.keys() : 
        physx.GetArray(k,Fields[k])
    print(istep, Fields["px"][0])
      
physx.CleanUp()
