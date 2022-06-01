#include <iostream>
#include <assert.h>
#include "physx.hpp"


void VEC1D_INIT(VEC1D &V1d, int ncols) 
{
   V1d = VEC1D(ncols,std::numeric_limits<REAL>::max()); 
}


void VEC2D_INIT(VEC2D &V2d, int nrows, int ncols) 
{
   V2d = VEC2D(); 
   V2d.assign(nrows, std::vector<REAL>(ncols,std::numeric_limits<REAL>::max()) ); 
}


physx::PxRigidDynamic*
Physx::CreateDynamic(const physx::PxTransform& t,
                       const physx::PxGeometry& geometry,
                       const physx::PxVec3& velocity)
{
  physx::PxRigidDynamic* dynamic = physx::PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
  dynamic->setAngularDamping(0.5f);
  dynamic->setLinearVelocity(velocity);
  gScene->addActor(*dynamic);
  return dynamic;
}


void
Physx::CreateStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
{
  physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
  for(physx::PxU32 i=0; i<size;i++)
  {
    for(physx::PxU32 j=0;j<size-i;j++)
    {
      physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j*2) - physx::PxReal(size-i), physx::PxReal(i*2+1), 0) * halfExtent);
      physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
      body->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
      gScene->addActor(*body);
    }
  }
  shape->release();
}


Physx::~Physx()
{
  std::cout<<"[Physx] End!!\n\n";
};


Physx::Physx()
{
  nRows = 0;

  std::cout<<"[Physx] Created\n";
};




void 
Physx::CleanUp()
{
  PX_RELEASE(gScene);
  PX_RELEASE(gDispatcher);
  PX_RELEASE(gPhysics);
  if(gPvd)
  {
    physx::PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();  gPvd = NULL;
    PX_RELEASE(transport);
  }

  nRows = -1;
  Fields.clear();

  std::cout<<"[Physx] CleanUp\n";
};


void 
Physx::Init()
{
  bool interactive = false; 

  gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

  gPvd = PxCreatePvd(*gFoundation);
  physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
  gPvd->connect(*transport,physx::PxPvdInstrumentationFlag::eALL);

  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(),true,gPvd);

  physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
  gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher  = gDispatcher;
  sceneDesc.filterShader  = physx::PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

  physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
  if(pvdClient)
  {
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
  physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*gPhysics, physx::PxPlane(0,1,0,0), *gMaterial);
  gScene->addActor(*groundPlane);

  for(physx::PxU32 i=0;i<5;i++)
    CreateStack(physx::PxTransform(physx::PxVec3(0,0,stackZ-=10.0f)), 10, 2.0f);

  if(!interactive)
    CreateDynamic(physx::PxTransform(physx::PxVec3(0,40,100)), physx::PxSphereGeometry(10), physx::PxVec3(0,-50,-100));

  std::cout<<"[Physx] Init\n";
};


int 
Physx::Evolve()
{
  gScene->simulate(1.0f/60.0f);
  gScene->fetchResults(true);

  physx::PxU32 nStatics = gScene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC); 
  physx::PxU32 nDynamics = gScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);  

  physx::PxActor **userBuffer = NULL;  
  physx::PxU32   bufferSize = -1; 

  std::vector<physx::PxActor*> Statics(nStatics); 
  std::vector<physx::PxActor*> Dynamics(nDynamics); 

  nDynamics = gScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, Dynamics.data(), Dynamics.size() ); 
  nStatics = gScene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, Statics.data(), Dynamics.size() );


  if(nRows == nDynamics){}
  else
  {
     Fields.clear();
     for(auto col : cols) VEC1D_INIT(Fields[col],nDynamics);
     nRows = nDynamics;
  }

  for(int i=0; i < nDynamics; i++)
  {
    physx::PxActor* a = Dynamics[i];

    physx::PxRigidDynamic *b = static_cast<physx::PxRigidDynamic*>(a);

    physx::PxTransform com = b->getGlobalPose(); 
    physx::PxVec3   p = com.p; 
    physx::PxQuat   q = com.q;

    Fields["id"][i] = i; 
    Fields["px"][i] = p.x; 
    Fields["py"][i] = p.y; 
    Fields["pz"][i] = p.z; 

    Fields["qx"][i] = q.x;
    Fields["qy"][i] = q.y;
    Fields["qz"][i] = q.z;
    Fields["qw"][i] = q.w;

    /* 
    // angle,axis
    float angle = 0.0; 
    physx::PxVec3 axis;
    q.toRadiansAndUnitAxis(angle,axis);

    // mass,vel 
    physx::PxReal mass = b->getMass();
    physx::PxVec3 vel = b->getLinearVelocity();
    */
  }

  Statics.clear();
  Dynamics.clear(); 

  //std::cout<<"[Physx] Evolve!\n";
  return nDynamics; 
};



VEC1D
Physx::GetField(std::string key) 
{
  bool ok1=false, ok2=false, ok3=false;

  VEC1D field;
  for (const auto& kv : Fields) 
  {
    ok1 = kv.first.find(key) != std::string::npos;   
    ok2 = key.find(kv.first) != std::string::npos;
    ok3 = ok1 || ok2; 
    if(ok3) field = kv.second;
  } 
  assert(ok3);
 
  return field;
}


int
Physx::GetArraySize()
{
  return nRows;  
}


void
Physx::GetArray(char* key, double* array, int narray)
{
  //std::cout<<" [GetArray] '"<< key <<"' \n";
  VEC1D f = GetField(key); 
  assert(f.size() == narray);  

  for(int i=0; i<narray; i++) array[i] = f[i]; 

  f.clear(); 
}


void 
Physx::GetKeys(std::vector<std::string>& array)
{
  assert(array.size() ==  cols.size());

  int i=0; 
  for(auto c: cols) array[i++] = c;  
};

