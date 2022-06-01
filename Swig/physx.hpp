#ifndef PHYSXJS_H
#define PHYSXJS_H

#include <iostream>
#include <vector>
#include <map>
#include <limits>

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

#define PVD_HOST "127.0.0.1" 
#define PX_RELEASE(x)  if(x){x->release(); x = NULL;}

using REAL = float;

using VEC1D = std::vector<REAL>;  

using VEC2D = std::vector< std::vector<REAL> >;

using PXFIELDS = std::map<std::string,VEC1D>; 

class Physx 
{
  public : 
 ~Physx();
  Physx();

  void Init();
  int  Evolve();
  void CleanUp();

  int GetKeysSize(){return cols.size();}; 
  void GetKeys(std::vector<std::string>&);  

  int GetArraySize();
  void GetArray(char* key, double* array, int narray);
  VEC1D GetField(std::string key);

  private:

  physx::PxRigidDynamic* 
  CreateDynamic(const physx::PxTransform&, 
                const physx::PxGeometry&, 
                const physx::PxVec3& v = physx::PxVec3(0));

  void 
  CreateStack(const physx::PxTransform&, physx::PxU32, physx::PxReal);


  physx::PxPhysics*  gPhysics  = NULL;
  physx::PxMaterial* gMaterial  = NULL;
  physx::PxScene* gScene = NULL;

  physx::PxPvd* gPvd = NULL;
  physx::PxDefaultCpuDispatcher*  gDispatcher = NULL;
  physx::PxReal stackZ = 10.0f;

  physx::PxFoundation*  gFoundation = NULL;
  physx::PxDefaultAllocator    gAllocator;
  physx::PxDefaultErrorCallback  gErrorCallback;

  PXFIELDS Fields;
 
  int nRows = -1; 

  std::vector<std::string> cols{"id","px","py","pz","qx","qy","qz","qw"};  
}; 



#endif /* PHYSXJS_H */
