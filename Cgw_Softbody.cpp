#include "Cgw_Softbody.h" //  
// 
// 默认初始化函数 // 
// 
My_softbody_class::My_softbody_class()
{
	// mesh 
	sprintf_s(tetFileName, "%s.tet", fileName);
	sprintf_s(objFileName, "%s.obj", fileName);
}

///   初始化 
void My_softbody_class::InitNx()
{
	// Initialize PhysicsSDK
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, 0, NULL);
	if (!gPhysicsSDK)    return;  	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, -0.05f); // 
	// Create a scene
	NxSceneDesc sceneDesc;  sceneDesc.setToDefault(); // gDefaultGravity
	sceneDesc.gravity = NxVec3(0, -9, 0); 
	gScene = gPhysicsSDK->createScene(sceneDesc);
	NxMaterial * defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.9f);
	defaultMaterial->setStaticFriction(0.1f);                       defaultMaterial->setDynamicFriction(0.1f); //  
	// 	 Create ground plane
	NxPlaneShapeDesc PlaneDesc; PlaneDesc.d = 0.0f;
	NxActorDesc ActorDesc; 	ActorDesc.shapes.pushBack(&PlaneDesc);  gScene->createActor(ActorDesc);
	NxBodyDesc BodyDesc; BodyDesc.angularDamping = 000.0f; BodyDesc.linearVelocity = NxVec3(1.0f, 0.0f, 0.0f);
	NxBoxShapeDesc BoxDesc; BoxDesc.dimensions = NxVec3(float(size), float(size), float(size));
	// 
	NxActorDesc BoxActorDesc; BoxActorDesc.shapes.pushBack(&BoxDesc);
	BoxActorDesc.body = &BodyDesc;  
	BoxActorDesc.density = 0.10f;
	BoxActorDesc.globalPose.t   =  NxVec3(-160.0, 0.0, 0.0); //  
	gScene->createActor(BoxActorDesc)->userData = (void*)size;
	// 
	NxSoftBodyDesc softBodyDesc;   softBodyDesc.particleRadius = 0.10f; 	softBodyDesc.volumeStiffness = 1.0f;
	softBodyDesc.friction = 1.0f;  softBodyDesc.stretchingStiffness = 2.0f; softBodyDesc.attachmentResponseCoefficient = 0.5f;
	if (gHardwareSoftBodySimulation) softBodyDesc.flags |= NX_SBF_HARDWARE;
	// 
	MySoftBody *softBody;
	// 
	NxReal carHeight = 39.50f;
	NxReal stiffness = 0.8f;
	ObjMesh *objMesh = new ObjMesh();
	// 
	objMesh->loadFromObjFile(objFileName);
	loadObj(fileName_2, mesh); // cgw // 

	NxMat33 rot ;  // 
	rot.rotX(NxPiF32);                       softBodyDesc.globalPose.t = NxVec3(1.0f, carHeight, 0.0f);
	softBodyDesc.globalPose.M = rot;  	     softBodyDesc.stretchingStiffness = stiffness; // 
	softBodyDesc.dampingCoefficient = 0.99;  softBodyDesc.dampingCoefficient = 1; // 
	//  向gscene注册 softbody // 
	softBody = new MySoftBody(gScene, softBodyDesc, tetFileName, objMesh, 40); 	// 不知道什么意思 ？   
	gObjMeshes.push_back(objMesh);   	gSoftBodies.pushBack(softBody); //   
	// 软体 和 Actor是并列的关系 // 
	if (gSoftBodies.size() > 0) { gSelectedSoftBody = gSoftBodies[0]->getNxSoftBody(); }
	else {  gSelectedSoftBody = NULL;}
	if (gScene->getNbActors() > 0) 	{		gSelectedActor = *gScene->getActors(); }
	else 	{  gSelectedActor = NULL;  }
}

void My_softbody_class::RenderCall_Back()
{
	gScene->fetchResults(NX_RIGID_BODY_FINISHED); // NX_RIGID_BODY_FINISHED
	gScene->simulate(1 / 60.0f); 	// NX_SOFT_BODY_FINISHED    
	gScene->flushStream(); //  // Keep physics & graphics in sync
	glPushMatrix()   ;  // 
	counter = 1      ;  // 
	glPopMatrix()    ;  //  
	glEnable(GL_LIGHTING) ;
	glEnable(GL_LIGHT0)   ;

	glPushMatrix();  // 
	{ // Render a ll the soft bodies in the scene
		for (MySoftBody **softBody = gSoftBodies.begin(); softBody != gSoftBodies.end(); softBody++)
		{
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			const NxMeshData* test = (*softBody)->getMeshData();
			(*softBody)->getNxSoftBody();
			float *vertexes = (float*)malloc(sizeof(float) * 3); // 
			vertexes = (float *)(test->verticesPosBegin);
			uint32_t * index = (uint32_t *)test->indicesBegin;
			NxSoftBody * tmp = (*softBody)->getNxSoftBody();
			for (int i = 1; i < mesh.faces.size(); i++)
			{
				int a = mesh.faces[i].v[0] - 1, b = mesh.faces[i].v[1] - 1, c = mesh.faces[i].v[2] - 1;   // 顶点编号
				glBegin(GL_TRIANGLES);
				glNormal3f(mesh.Normals[a].x, mesh.Normals[a].y, mesh.Normals[a].z);  //  顶点坐标(x,y,z)
				glVertex3f(vertexes[3 * a + 0], vertexes[3 * a + 1], vertexes[3 * a + 2]);  //  顶点坐标(x,y,z)
				glNormal3f(mesh.Normals[b].x, mesh.Normals[b].y, mesh.Normals[b].z);  //  顶点坐标(x,y,z)
				glVertex3f(vertexes[3 * b + 0], vertexes[3 * b + 1], vertexes[3 * b + 2]);  //  

				glNormal3f(mesh.Normals[c].x, mesh.Normals[c].y, mesh.Normals[c].z);  //  顶点坐标(x,y,z)
				glVertex3f(vertexes[3 * c + 0], vertexes[3 * c + 1], vertexes[3 * c + 2]);  //  
				glEnd(); //  
			}
		}
	}
	//glPopMatrix();  //   
	counter++;  //  绘制刚体 /// 
	int nbActors = gScene->getNbActors();
	NxActor** actors = gScene->getActors();
	while (nbActors--)
	{
		glPushMatrix();
		NxActor* actor = *actors++ ; // 
		if (!actor->userData) //  
			continue;
		NxVec3 force = NxVec3(00, 0, 0);
		actor->addForce(force);
		if (counter < 500000) //  
		{
			if (counter - (counter / 10) * 10 == 0)
			{
				actor->setLinearVelocity(NxVec3(8.0f * 6, 0.0f, 0.0f));
			}
			else
			{
				actor->setLinearVelocity(NxVec3(-0.40f * 6, 0.0f, 0.0f));
			}
		}
		float glmat[16]; //  
		actor->getGlobalPose().getColumnMajor44(glmat);
		glMultMatrixf(glmat); 	// actor->setGlobalPose( NxMat34 glmat ); 
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // 
		glutWireCube(float(int(actor->userData))*2.0f); // 
		glPopMatrix(); //  
	} // 
}


// 
