clear 

CHECKING()
{ 
  echo "[DOCKER] LIBRARIES"
  ls -la /home/jovyan/work/physx/install/linux/PhysXSO/bin/linux.clang/release/
  
  echo "[DOCKER] INCLUDES:"
  ls /home/jovyan/work/physx/install/linux/PhysX/include/PxPhysicsAPI.h
  ls /home/jovyan/work/physx/install/linux/PxShared/include/foundation/Px.h
}


SIMULATION_RUN()
{
  rm -rf BUILD
  mkdir BUILD
  cd BUILD

  cmake .. \
    -DCMAKE_RULE_MESSAGES=OFF \
    -DPHYSX_ROOT_DIR=/home/jovyan/work/physx/install/

  make

  bash ../link.txt >> /dev/null
}


TESTING()
{
  SWIG_PATH=/home/jovyan/work/PyPhysX/Swig
  cd $SWIG_PATH

  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/jovyan/work/physx/install/linux/PhysXSO/bin/linux.clang/release/
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${SWIG_PATH}/BUILD

  python test.py
}


CHECKING
SIMULATION_RUN # libLibraryName.so swig_example.py  _SWIG_fileName.so
TESTING


