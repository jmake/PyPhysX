# PyPhysX

[![Kaggle](https://kaggle.com/static/images/open-in-kaggle.svg)](https://kaggle.com/kernels/welcome?src=https://github.com/jmake/PyPhysX/blob/main/kaggle-test.ipynb)

![example workflow](https://github.com/jmake/PyPhysX/actions/workflows/workflowa.yml/badge.svg)

[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/jmake/PyPhysX/HEAD)


In the next post I describe the development of a C++ to Python interface for a rigid body dynamic simulation which makes use of the NVIDIA® PhysX SDK 4.1 engine.


PhysX represents a rigid euclidean transform as a quaternion and a position vector. Through the interface, after each time step all this information allocated in C++ is accessible in Python via Numpy arrays.



The interface is implemented through swig, a software tool that connects programs written in C and C++ with a variety of high-level programming languages.
