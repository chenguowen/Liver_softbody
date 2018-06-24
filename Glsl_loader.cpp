#define  _CRT_SECURE_NO_WARNINGS
#include "glsl_loader.h"
#include <fstream>
#include "src\tet_make.h"
#include "particlelib.h" 
// #include <PxController.h>


#include <time.h>
using namespace std;
//  
GLSL_Loader::GLSL_Loader( string f_name ) //
{ //

  // 
	size = 4;
	gHardwareSoftBodySimulation = true;
	gPause = false;
	gPhysicsSDK = NULL;
	gScene = NULL;

	gRatio = 1.0f;

	fileName = "Data\\Liver_resized_to_2018__SB";   //     

	groundPlane = NULL; // Focus actor
	gSelectedActor = NULL; // Focus actor
	gSelectedSoftBody = NULL; // Array of SoftBody objects 
	gSelectedSoftBodyVertex = -1; // Array of SoftBody objects
	//     、、
	Selected_Vertex     = (int*) calloc( 100 , sizeof(int) ) ;  // 
	gVertexProgram      = 1 ;
	gFragmentProgram    = 1 ;
	gLinkedProgram      = 1 ;
	gParam_VPMatrix     = 0 ;
	gParam_WMatrix      = 0 ;   // 
	Back_Flag           = false  ;   // 
	model_view                   ;   // 
	project_matrix  =  (float  *) calloc( 16 , sizeof(float) ) ; 
	Max_Size        =   10  ;   //  
	multi_cam2      =   new Multiple_View() ;  // Multiple_View   // 
	//                  //
	this->model_view    = ( float* ) calloc ( 16 , sizeof ( float ) ) ;   // 
	this->model_view[0] = this->model_view[5] = this->model_view[10]  = this->model_view[15] = 1 ; 
	//              //
	filename = f_name ; //   
	sprintf_s(tetFileName, "%s.tet", fileName);
	sprintf_s(objFileName, "%s.obj", fileName); 
	counter = 0 ; //   
	//              //
	shadows        = true ; // 
	gShowTetraMesh = true ; // 
	//              // 

} //
  //      初始化      // 
void GLSL_Loader::InitNx( float sizex, float sizey, float sizez ) // 
{	
	// 
	//  Initialize PhysicsSDK   // 
	// 
	gPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION , 0  , NULL      ) ; // 
	if ( !gPhysicsSDK )return; gPhysicsSDK->setParameter( NX_SKIN_WIDTH ,-0.05f ) ; // 
	// ************** Create a scene  **************  //   
	NxSceneDesc sceneDesc; sceneDesc.setToDefault() ; // gDefaultGravity //  
	sceneDesc.gravity = NxVec3(0, -98 * 2.0, 0); gScene = gPhysicsSDK->createScene(sceneDesc);
	NxMaterial * defaultMaterial = gScene->getMaterialFromIndex(0); defaultMaterial->setRestitution(0.9f); // 
	defaultMaterial->setStaticFriction(0.6f); defaultMaterial->setDynamicFriction(0.6f); //   
	// NxControllerManager* manager = NxCreateControllerManager(*scene);  	 // Controller // 

	NxPlaneShapeDesc PlaneDesc;   PlaneDesc.d = -10.0f;
	NxActorDesc ActorDesc; 	ActorDesc.shapes.pushBack(&PlaneDesc);   gScene->createActor(ActorDesc);
	NxBodyDesc  BodyDesc;   BodyDesc.angularDamping = 0.99f;   BodyDesc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);
	int   sizez2 = sizez;
	NxBoxShapeDesc BoxDesc;   BoxDesc.dimensions = NxVec3(float(sizex), float(sizey), float(sizez)); // BoxDesc.friction
	// NxCapsuleShapeDesc BoxDesc;   BoxDesc.height = float(sizez) ; // BoxDesc.friction
	// BoxDesc.radius = float(sizex);  
	//                                // 
	NxActorDesc BoxActorDesc;       BoxActorDesc.shapes.pushBack(&BoxDesc);
	BoxActorDesc.body = &BodyDesc;  BoxActorDesc.density = 0.0000000001f;   // BoxActorDesc = 0.0;
	BoxActorDesc.globalPose.t = NxVec3(-70.0, 100.0, 0.0);   //  
	gScene->createActor(BoxActorDesc)->userData = (void*)sizez2;   // 
//  //  
	//  //
	if (gHardwareSoftBodySimulation)  softBodyDesc.flags |= NX_SBF_HARDWARE ; //  // 
	//   
	NxReal      Cardic_height = 70.0f ; // 
	NxReal      stiffness     = 0.8f  ; // 
	ObjMesh   * objMesh       = new ObjMesh() ; // 
	objMesh->loadFromObjFile( objFileName )   ; //   
	NxMat33 rot;  rot.rotX( NxPiF32 )         ; // rot.rotX(NxPiF32);  
	// *******************  ******************  // 
	softBodyDesc.friction            = 1.0f    ; // 
	softBodyDesc.particleRadius      = 0.90f   ; //  
	softBodyDesc.volumeStiffness     = 1.0f    ; // 
	softBodyDesc.globalPose.M        = rot     ; // 
	softBodyDesc.dampingCoefficient  = 0.99f   ; // 
	softBodyDesc.dampingCoefficient  = 1.0f    ; // 
	softBodyDesc.density             = 1000*50.0f ; // 
	softBodyDesc.stretchingStiffness = 0.99f    ; // 
	softBodyDesc.globalPose.t        = NxVec3( 0.0f, Cardic_height , 0.0f) ; // 
	softBodyDesc.attachmentResponseCoefficient = 0.6f;
    //  **************** 向gscene注册 softbody  **************** //  
	MySoftBody* softBody_local; // 
	softBody_local = new MySoftBody(  gScene , softBodyDesc, tetFileName, objMesh, 30   ); // 55   当50更大，至多到 60 70就会出错，不知道为什么？ 
	gObjMeshes.push_back( objMesh  )     ; //
	gSoftBodies.pushBack(softBody_local) ; //    
	//                                     //  
	const NxMeshData * test = softBody_local->getMeshData();  // received_vertexes = (float *)(test->verticesPosBegin); //    
	received_vertexes = (float*)softBody_local->mReceiveBuffers.verticesPosBegin;  //   
	// 初始化
	// 

	//NxShape* aCapsuleShape = NxRigidActor::createExclusiveShape(*aCapsuleActor,
	//	PxCapsuleGeometry(radius, halfHeight), aMaterial);
	//aCapsuleShape->setLocalPose(relativePose);
	//PxRigidBodyExt::updateMassAndInertia(*aCapsuleActor, capsuleDensity);
	//aScene->addActor(aCapsuleActor);

}
// 
void GLSL_Loader::SetupParticles(int np,int ns) { } // 

//  **************************  // 
void GLSL_Loader::InitialiseSystem( float sizex, float sizey, float sizez )
{
	this->InitNx(  sizex,   sizey,   sizez) ; //  // 软体 和 Actor是并列的关系 //  //
	if (gSoftBodies.size() > 0) {  gSelectedSoftBody = gSoftBodies[0]->getNxSoftBody(); }
	else { gSelectedSoftBody = NULL; }
	if (gScene->getNbActors() > 0) { gSelectedActor = *gScene->getActors(); }
	else { gSelectedActor = NULL; } 
} // 

void GLSL_Loader::tet_deal(  int *Select_Original_Vertex  )   // Meffy:四面体模块处理
{    
	glBindBuffer   (  GL_ARRAY_BUFFER ,  gVertexBuffer);  // 绑定buffer
	glBufferData   (  GL_ARRAY_BUFFER ,  sizeof(MyVertex)*Mesh_Size  ,  &pvertex[0].x   , GL_DYNAMIC_DRAW); //  GL_STATIC_DRAW  GL_DYNAMIC_DRAW
	glIndexPointer (  GL_DOUBLE       ,  sizeof(USHORT  )*    3      ,  BUFFER_OFFSET(0)  );   //The starting point of texcoords, 24 bytes away
	// 
	glVertexPointer  ( 3 , GL_FLOAT, sizeof(MyVertex)     , BUFFER_OFFSET(0)               );   //The starting point of the VBO, for the vertices
	glNormalPointer  (     GL_FLOAT, sizeof(MyVertex)     , BUFFER_OFFSET(sizeof(float)*3 ));   //The starting point of the VBO, for the vertices
	glTexCoordPointer( 2 , GL_FLOAT, sizeof(MyVertex)     , BUFFER_OFFSET(sizeof(float)*6 ));   //The starting point of texcoords, 24 bytes away
} 
// 
void GLSL_Loader::RenderCall_Back( NxVec3 velocity ) // 
{

	int nbActors     = gScene->getNbActors();
	NxActor** actors = gScene->getActors();
	while (nbActors--)
	{
		actor = *actors++ ; //  
		actor->setLinearVelocity( velocity ) ; //   
	}
	gScene->fetchResults(NX_RIGID_BODY_FINISHED) ;  // NX_RIGID_BODY_FINISHED
	gScene->simulate( 1 / 20.0f );  // NX_SOFT_BODY_FINISHED  // 
	gScene->flushStream();        // Keep physics & graphics in sync 

	MySoftBody **softBody = gSoftBodies.begin(); //  
	const NxMeshData * test = (*softBody)->getMeshData();  // received_vertexes = (float *)(test->verticesPosBegin); //    
	received_vertexes = (float*)(*softBody)->mReceiveBuffers.verticesPosBegin;  //   
																				// 
} 
// 
// 
void GLSL_Loader::RenderCall_Back(NxVec3 velocity , NxMat33 stylus_orient ) // 
{ 
	// 
	int nbActors = gScene->getNbActors();
	NxActor** actors = gScene->getActors();  
	// 
	while (nbActors--)
	{

		actor = *actors++;  

		NxVec3 velocity_now = actor->getLinearVelocity(); 

		// actor->setGlobalOrientation(stylus_orient); // 
		if( velocity[0]==0 ) // 
		{
			actor->setLinearVelocity (  velocity / 1.0  );  //  
		}
		else
		{
			actor->setLinearVelocity (  (velocity ) / 1.0 );  //    
		}
		// actor->s  
		// actor->addForce(NxVec3(0, 0.001 , 0 ))     ;  //   整体均匀受力    // 
		// actor->addForceAtPos(NxVec3(0, 0.001, 0))  ;  //   需要指定加力的位置 // 
		actor->setGlobalOrientation(stylus_orient); // 
		// float* a =stylus_orient.getColumnMajorStride4 ; 
	}
	// 
	gScene->fetchResults(NX_RIGID_BODY_FINISHED);  // NX_RIGID_BODY_FINISHED
	gScene->simulate(1 / 70.0f); // NX_SOFT_BODY_FINISHED  // 
	gScene->flushStream();  // Keep physics & graphics in sync // 
	// 
	MySoftBody **softBody = gSoftBodies.begin(); //  
	const NxMeshData * test = (*softBody)->getMeshData();  // received_vertexes = (float *)(test->verticesPosBegin); //    
	received_vertexes = (float*)(*softBody)->mReceiveBuffers.verticesPosBegin;  //  
	// 
}
// 
//
void GLSL_Loader::RenderCall_Back( NxMat34 stylus_pose)
{  
	int nbActors = gScene->getNbActors();
	NxActor** actors = gScene->getActors();
	while (nbActors--)
	{   
		actor   = *actors++   ;  
		actor->setGlobalPose(stylus_pose) ; 
	} 
    // 更改一下仿真的时间即可  // 这个应该是实际的  
	gScene->simulate ( 1/10.0f ) ; // NX_SOFT_BODY_FINISHED  // 
	gScene->flushStream( );  // Keep physics & graphics in sync // 
	//             // 
	gScene->fetchResults(NX_RIGID_BODY_FINISHED);  // NX_RIGID_BODY_FINISHED
    //             //  
	MySoftBody **softBody    = gSoftBodies.begin();           //  
	const NxMeshData * test  = (*softBody)->getMeshData();  // received_vertexes = (float *)(test->verticesPosBegin); //    
	received_vertexes = (float*)(*softBody)->mReceiveBuffers.verticesPosBegin;  //     
	//    //   
	//    //  
} 
// 
// 
void  GLSL_Loader::Model_Load_Soft_Simulation( float3 size  ) // 
{//  
	AUX_RGBImageRec *texture1     ; // spongebob 
	loadObj ( filename , mesh )   ; // spongebob 
	Face_Num  = mesh.faces.size() ; // spongebob 
	Mesh_Size = mesh.verts.size() ; // spongebob 
	//   
	cout << "Mesh_Size = " << Mesh_Size << endl;  //system("pause"); //
	cout << "Face_Num  = " << Face_Num  << endl;  //system("pause"); //
	indexData = ( int    * ) malloc(sizeof(int) * 3  * Face_Num   );     // 索引表 Face_Num行，3列     //
	pvertex   = (MyVertex* ) malloc(sizeof(MyVertex) * Mesh_Size  );     // 顶点数组
	int  a, b, c ,  D, E, F; //   
	//                       //
	Vertex_face_Linkage      = (int**  ) calloc( Mesh_Size     , sizeof(int*)  ); //   
	updated_normals          = (float* ) calloc( Mesh_Size * 3 , sizeof(float )); //   
	for (int i = 0; i<Mesh_Size; i++)
	{
		pvertex[i].x = Max_Size * mesh.verts[i].x;
		pvertex[i].y = Max_Size * mesh.verts[i].y;
		pvertex[i].z = Max_Size * mesh.verts[i].z; 
		Vertex_face_Linkage[i] = (int*)calloc( 21 , sizeof(int)) ;  //   
	}
	// 
	//  filling  // 
	for (int i = 0; i < Face_Num; i++)
	{
		int a = mesh.faces[i].v[0] - 1;
		int b = mesh.faces[i].v[1] - 1;
		int c = mesh.faces[i].v[2] - 1;
		///
		if (Vertex_face_Linkage[a][0] < 10)
		{
			Vertex_face_Linkage[a][Vertex_face_Linkage[a][0] * 2 + 1] = b; // 
			Vertex_face_Linkage[a][Vertex_face_Linkage[a][0] * 2 + 2] = c; // 
			Vertex_face_Linkage[a][0]++;
		}
		if (Vertex_face_Linkage[b][0] < 10)
		{
			Vertex_face_Linkage[b][Vertex_face_Linkage[b][0] * 2 + 1] = c; // 
			Vertex_face_Linkage[b][Vertex_face_Linkage[b][0] * 2 + 2] = a; // 
			Vertex_face_Linkage[b][0]++;
		}
		if (Vertex_face_Linkage[c][0] < 10)
		{
			Vertex_face_Linkage[c][Vertex_face_Linkage[c][0] * 2 + 1] = a; // 
			Vertex_face_Linkage[c][Vertex_face_Linkage[c][0] * 2 + 2] = b; //   
			Vertex_face_Linkage[c][0]++; 
		}
	}
	for (int i = 0; i < Mesh_Size; i++)
	{
		if ( Vertex_face_Linkage[i][0] == 0 ) // 
		{
			cout << i << endl; 
		}
	}


	for ( int i = 0 ; i< Face_Num ; i++ ) //      // 
	{
		indexData[3 * i + 0] = mesh.faces[i].v[0] - 1; 
		indexData[3 * i + 1] = mesh.faces[i].v[1] - 1;
		indexData[3 * i + 2] = mesh.faces[i].v[2] - 1;
		a = indexData[3 * i + 0];    b = indexData[3 * i + 1];    c = indexData[3 * i + 2];
		// 
		D = mesh.faces[i].Define_index[0] - 1, E = mesh.faces[i].Define_index[1] - 1, F = mesh.faces[i].Define_index[2] - 1;
		pvertex[a].s0 = mesh.Texture_2D[D].x; 	pvertex[a].t0 = mesh.Texture_2D[D].y; 
		pvertex[b].s0 = mesh.Texture_2D[E].x; 	pvertex[b].t0 = mesh.Texture_2D[E].y;
		pvertex[c].s0 = mesh.Texture_2D[F].x; 	pvertex[c].t0 = mesh.Texture_2D[F].y;
		//  
		D = mesh.faces[i].Normal_index[0] - 1; E = mesh.faces[i].Normal_index[1] - 1; F = mesh.faces[i].Normal_index[2] - 1; // 
		pvertex[a].nx = mesh.Normals[D].x; 	pvertex[a].ny = mesh.Normals[D].y; 	pvertex[a].nz = mesh.Normals[D].z; //
		pvertex[b].nx = mesh.Normals[E].x;  pvertex[b].ny = mesh.Normals[E].y;  pvertex[b].nz = mesh.Normals[E].z; // 
		pvertex[c].nx = mesh.Normals[F].x;  pvertex[c].ny = mesh.Normals[F].y;  pvertex[c].nz = mesh.Normals[F].z; //  
	}

	mesh.bounding_box[0] = mesh.bounding_box[0] * Max_Size;  //
	mesh.bounding_box[1] = mesh.bounding_box[1] * Max_Size;  //
	if (_DEBUG_FLAG_)
	{
		TriangleMesh *octree_tmp = &mesh;
		cout << " octree_tmp->bound_box_xyz =  " << octree_tmp->bounding_box[0].x << "   " << octree_tmp->bounding_box[0].y << "   " << octree_tmp->bounding_box[0].z << endl;
		cout << " octree_tmp->bound_box_xyz =  " << octree_tmp->bounding_box[1].x << "   " << octree_tmp->bounding_box[1].y << "   " << octree_tmp->bounding_box[1].z << endl;
		system("pause"); // 
	}
	// 
	mesh.bounding_sphere_c = mesh.bounding_sphere_c *  Max_Size; // 
	mesh.bounding_sphere_r = mesh.bounding_sphere_r *  Max_Size; // 
 	//                          // 
	glGenBuffers(1, &gIndexBuffer);   GLERR(); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);   GLERR(); // Meffy: GL_ELEMENT_ARRAY_BUFFER 顶点属性 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * Face_Num * 3 , indexData, GL_DYNAMIC_DRAW  ); GLERR(); // 
	glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0) );   //The starting point of texcoords, 24 bytes away  定义颜色索引数组 
	// *********************** test *********************** // 
	glGenBuffers(1, &gVertexBuffer);  GLERR();  // 
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);  GLERR();   // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) *Mesh_Size, &pvertex[0].x, GL_DYNAMIC_DRAW  ) ; // 
	glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away 
	// // *********************** test ***********************  // // 
	glVertexPointer( 3,GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0))   ;  //The starting point of the VBO, for the vertices
	glNormalPointer( GL_FLOAT  , sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));   //The starting point of the VBO, for the vertices
	glTexCoordPointer( 2, GL_FLOAT , sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6));   //The starting point of texcoords, 24 bytes away
	// 
	InitialiseSystem  ( size.x / 2.0, size.y / 2.0, size.z/2.0 )   ;   //    //   
	Octree_construtct (  &mesh, mesh.obj_Octree, 4, 0);  // Octree_Show( mesh.obj_Octree ) ; //    
	maxVertices = Mesh_Size ; // 
}
// 
void  GLSL_Loader::Model_Load()
{ //  
	AUX_RGBImageRec *texture1;    // spongebob 
	loadObj(filename, mesh); 
	// 
	cout << "mesh.faces.size()" << mesh.faces.size() << endl;
	cout << "mesh.verts.size()" << mesh.verts.size() << endl;
	// 
	Face_Num = mesh.faces.size(); // 
	Mesh_Size = mesh.faces.size() * 3; // 
	cout << "Mesh_Size = " << Mesh_Size << endl; // 
	cout << "Face_Num  = " << Face_Num << endl; // 
	// 
	LoadGLTextures();                   // 
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_COLOR_MATERIAL);
	//  
	indexData      = (int   *   )malloc(sizeof(int)  * Face_Num * 3);    // 索引表 Face_Num行，3列     //
	pvertex        = (MyVertex *)malloc(sizeof(MyVertex)  * Mesh_Size);  // 顶点数组 Mesh_Size行，3列  //
	pvertex_backup = (MyVertex *)malloc(sizeof(MyVertex)  * Mesh_Size);  // 顶点数组 Mesh_Size行，3列  // 
	int         a,   b,   c   ; // 
	int         D,   E,   F   ; //  
	int         a2,  b2,  c2  ; //  
	float3      ab,  bc,  ca  ; // ac = float3(2,3,8 );
	float3      ab2, bc2, ca2 ; // 
	//                        // 
	for (int i = 0; i < mesh.verts.size(); i++)
	{ // 
		mesh.verts[i].x = Max_Size * mesh.verts[i].x; // 
		mesh.verts[i].y = Max_Size * mesh.verts[i].y; // 
		mesh.verts[i].z = Max_Size * mesh.verts[i].z; // 
	} // 
	// 
	float offset_x[] = { 0.0 , 0.0 , 0.5 , 0.5 }, offset_y[] = { 0.5  , 0.0 , 0.0 ,  0.5 };  //  

	float OFFSET_x = 0.0; /// 
	float OFFSET_y = 0.5; /// 
	cout << "pvertex[0].x=" << pvertex[0].x << endl;

	for (int i = 0; i<Face_Num; i++)
	{
		indexData[3 * i + 0] = 3 * (i - 0)   + 0 ; // 
		indexData[3 * i + 1] = 3 * (i - (0)) + 1 ; //
		indexData[3 * i + 2] = 3 * (i - (0)) + 2 ; //  
												  // 
		a = mesh.faces[i].v[0] - 1; //三角形 顶点的索引
		b = mesh.faces[i].v[1] - 1; //三角形 顶点的索引
		c = mesh.faces[i].v[2] - 1; //三角形 顶点的索引 
									// 
		D = mesh.faces[i].Define_index[0] - 1; E = mesh.faces[i].Define_index[1] - 1; F = mesh.faces[i].Define_index[2] - 1;
		// 
		pvertex[3 * i + 0].x = mesh.verts[a].x; pvertex[3 * i + 0].y = mesh.verts[a].y; pvertex[3 * i + 0].z = mesh.verts[a].z; 
		pvertex[3 * i + 1].x = mesh.verts[b].x; pvertex[3 * i + 1].y = mesh.verts[b].y; pvertex[3 * i + 1].z = mesh.verts[b].z;
		pvertex[3 * i + 2].x = mesh.verts[c].x; pvertex[3 * i + 2].y = mesh.verts[c].y; pvertex[3 * i + 2].z = mesh.verts[c].z;
		// 
		pvertex[3 * i + 0].s0 = mesh.Texture_2D[D].x  + offset_x[mesh.faces[i].Define_index[6]]; //  //  / 2.0 + OFFSET_x  ;
		pvertex[3 * i + 0].t0 = mesh.Texture_2D[D].y  + offset_y[mesh.faces[i].Define_index[6]]; //  
		pvertex[3 * i + 1].s0 = mesh.Texture_2D[E].x  + offset_x[mesh.faces[i].Define_index[6]]; //  / 2.0 + OFFSET_x ;
		pvertex[3 * i + 1].t0 = mesh.Texture_2D[E].y  + offset_y[mesh.faces[i].Define_index[6]]; // 
		pvertex[3 * i + 2].s0 = mesh.Texture_2D[F].x  + offset_x[mesh.faces[i].Define_index[6]]; // / 2.0 + OFFSET_x ;
		pvertex[3 * i + 2].t0 = mesh.Texture_2D[F].y  + offset_y[mesh.faces[i].Define_index[6]]; // / 2.0 + OFFSET_y ; 
		// 
		D = mesh.faces[i].Normal_index[0] - 1; E = mesh.faces[i].Normal_index[1] - 1; F = mesh.faces[i].Normal_index[2] - 1;
		// 
		a = 3 * (i - (0)) + 0; 	b = 3 * (i - (0)) + 1; c = 3 * (i - (0)) + 2; //   
		pvertex[a].nx = mesh.Normals[D].x; pvertex[a].ny = mesh.Normals[D].y; pvertex[a].nz = mesh.Normals[D].z; //
		pvertex[b].nx = mesh.Normals[E].x; pvertex[b].ny = mesh.Normals[E].y; pvertex[b].nz = mesh.Normals[E].z; //
		pvertex[c].nx = mesh.Normals[F].x; pvertex[c].ny = mesh.Normals[F].y; pvertex[c].nz = mesh.Normals[F].z; //  
	}
	//cout<<"indexData[0]="<<indexData[0]<<endl; // 
	// 
	mesh.bounding_box[0] = mesh.bounding_box[0] * Max_Size;  //
	mesh.bounding_box[1] = mesh.bounding_box[1] * Max_Size;  //
	if (_DEBUG_FLAG_)
	{
		TriangleMesh *octree_tmp = &mesh;
		cout << " octree_tmp->bound_box_xyz =  " << octree_tmp->bounding_box[0].x << "   " << octree_tmp->bounding_box[0].y << "   " << octree_tmp->bounding_box[0].z << endl;
		cout << " octree_tmp->bound_box_xyz =  " << octree_tmp->bounding_box[1].x << "   " << octree_tmp->bounding_box[1].y << "   " << octree_tmp->bounding_box[1].z << endl;
		system("pause"); // 
	}
	// 
	mesh.bounding_sphere_c = mesh.bounding_sphere_c *  Max_Size;
	mesh.bounding_sphere_r = mesh.bounding_sphere_r *  Max_Size;
	//
	//                          // 
	glGenBuffers(1, &gIndexBuffer);  GLERR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);  GLERR();  // GL_DYNAMIC_DRAW // GL_DYNAMIC_DRAW   GL_STATIC_DRAW
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * Face_Num * 3, indexData, GL_DYNAMIC_DRAW ); GLERR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 	GLERR();

	glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away  定义颜色索引数组
	glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away
	// *********************** test *********************** // 
	glGenBuffers(1, &gVertexBuffer); // 
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) *Mesh_Size, &pvertex[0].x, GL_DYNAMIC_DRAW ); // 
	glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away
																	// 
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
	glNormalPointer(  GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));   //The starting point of the VBO, for the vertices
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6));   //The starting point of texcoords, 24 bytes away //  
	memcpy( pvertex_backup , pvertex , sizeof(MyVertex) * Mesh_Size)  ; 
} // 
// 集中 
#pragma region dfasf
//      // 
void  GLSL_Loader::GLSL_Vertex_Frag_Enable(float center_x, float center_y, float window_x, float window_y, float pitch, float pitchy, float backfront_flag, GLdouble *clip_plane)
{
	glUseProgramObjectARB(gLinkedProgram_normal);           	GLERR();
	glUniform4fvARB(gParam_WMatrix_normal, 4, model_view);    	GLERR(); 
	glUniform4fvARB(gParam_VPMatrix_normal, 4, project_matrix); GLERR();
	glUniform4fARB(gParam_IV_Params_normal, center_x, center_y, window_x, window_y);  GLERR();  
	glUniform3fARB(glParam_ot_Param_normal, pitch, pitchy, backfront_flag);           GLERR();  

	// Draw primitive
	// Bind buffers
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer); GLERR(); 
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer)      ;  GLERR(); 

	glEnableClientState(GL_VERTEX_ARRAY); 	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));  GLERR();
	glClientActiveTexture(GL_TEXTURE1);     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3)); 	GLERR();

	glClientActiveTexture(GL_TEXTURE0); glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6)); 	GLERR();

	glEnable(GL_DEPTH_TEST);    glClipPlane(GL_CLIP_PLANE0, clip_plane);  GLERR(); 
	glEnable(GL_CLIP_PLANE0); 	GLERR();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture[0]);      // 选择纹理 //  此处纹理为单个物体的从属纹理i  
	glEnableClientState(GL_VERTEX_ARRAY) ; //               
	glEnableClientState(GL_NORMAL_ARRAY) ; // 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);     // 
	glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE0);
	//
	// Validate program
	//
	glsl_validate_program(); // 
	glDrawElements(GL_TRIANGLES, (this->Face_Num) * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));   //The starting point of the IBO

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_DEPTH_TEST);
}
void GLSL_Loader::Normal_Update()
{ 


	float min_xyz[3], max_xyz[3]; //  

	int    ia, ib, inow;
	float3 left, right, result;
	float  norm_dist;
	for (int i = 0; i < Mesh_Size; i++)
	{
		//this->pvertex[i].nx = 0;
		//this->pvertex[i].ny = 0;
		//this->pvertex[i].nz = 0;  
		updated_normals[3 * i + 0] = 0;
		updated_normals[3 * i + 1] = 0;
		updated_normals[3 * i + 2] = 0;
		// 
		if (Vertex_face_Linkage[i][0]>0.1)
		{
			for (int j = 0; j < Vertex_face_Linkage[i][0]; j++)
			{
				ia = Vertex_face_Linkage[i][2 * j + 1];
				ib = Vertex_face_Linkage[i][2 * j + 2];

				left = float3(
					received_vertexes[3 * ia + 0] - received_vertexes[3 * i + 0],
					received_vertexes[3 * ia + 1] - received_vertexes[3 * i + 1],
					received_vertexes[3 * ia + 2] - received_vertexes[3 * i + 2]); //  

				right = float3(
					received_vertexes[3 * ib + 0] - received_vertexes[3 * i + 0],
					received_vertexes[3 * ib + 1] - received_vertexes[3 * i + 1],
					received_vertexes[3 * ib + 2] - received_vertexes[3 * i + 2]); //   
				// 
				result.cross(left, right); //  
				result.normalize();  //  
				updated_normals[3 * i + 0] += result.x;
				updated_normals[3 * i + 1] += result.y;
				updated_normals[3 * i + 2] += result.z;
			}
			// ****************************** //  
			norm_dist = norm_3(updated_normals[3 * i + 0], updated_normals[3 * i + 1], updated_normals[3 * i + 2]); // 
			updated_normals[3 * i + 0] = updated_normals[3 * i + 0] / norm_dist; //  
			updated_normals[3 * i + 1] = updated_normals[3 * i + 1] / norm_dist; //
			updated_normals[3 * i + 2] = updated_normals[3 * i + 2] / norm_dist; //  
			this->pvertex[i].nx = updated_normals[3 * i + 0];
			this->pvertex[i].ny = updated_normals[3 * i + 1];
			this->pvertex[i].nz = updated_normals[3 * i + 2];
			//                      // 
			this->pvertex[i].x = received_vertexes[3 * i + 0]; // 
			this->pvertex[i].y = received_vertexes[3 * i + 1]; //
			this->pvertex[i].z = received_vertexes[3 * i + 2]; // 

			for (int t = 0; t < 3; t++)
			{
				if (min_xyz[t] > received_vertexes[3 * i + t])
				{
					min_xyz[t] =  received_vertexes[3 * i + t]; 
				}
				if (max_xyz[t] < received_vertexes[3 * i + t])
				{
					max_xyz[t] = received_vertexes[3 * i + t];
				}
			}

			// ****************************** // 
		} 
	}

	mesh.bounding_box[0].x = min_xyz[0]; 
	mesh.bounding_box[1].x = max_xyz[0];

	mesh.bounding_box[0].y = min_xyz[1];
	mesh.bounding_box[1].y = max_xyz[1];

	mesh.bounding_box[0].z = min_xyz[2];
	mesh.bounding_box[1].z = max_xyz[2];

}
// 
//  ******************************   //
//  
void  GLSL_Loader::GLSL_Vertex_Frag_Enable2( int soft )
{   


	glEnableClientState(GL_VERTEX_ARRAY);  glEnable(GL_DEPTH_TEST); // 
	glEnableClientState(GL_NORMAL_ARRAY);  glEnable(GL_DEPTH_TEST); //   

	glUseProgramObjectARB(gLinkedProgram);   GLERR();
	glUniform4fvARB(gParam_WMatrix, 4, model_view); GLERR();
	glUniform4fvARB(gParam_VPMatrix, 4, project_matrix); GLERR();
	//   // 
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer); GLERR(); 

	// glVertexPointer(3, GL_FLOAT, sizeof(float) * 0, received_vertexes); // 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);  //  GLERR(); 
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer); 	//GLERR();

	glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));
	glClientActiveTexture(GL_TEXTURE0);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6)); //GLERR(); 
	 
 
	// Set clipping plane (gl_ClipVertex is in WORLD space, so out clip plane in WORLD space to)  // 
	glActiveTexture(GL_TEXTURE0);  // 
	glBindTexture(GL_TEXTURE_2D, this->texture[0]);      // 选择纹理 //  此处纹理为单个物体的从属纹理i  
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);     // 
	glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE0);
	//  
	//   ***********  Validate program   ***********    //   
	//  
	glsl_validate_program();
	glDrawElements(   GL_TRIANGLES, (this->Face_Num) * 3  ,  GL_UNSIGNED_INT  , BUFFER_OFFSET(0)    );   //The starting point of the IBO
	glDisable(GL_DEPTH_TEST);
}
// 
void  GLSL_Loader::GLSL_Vertex_Frag_Enable2()
{
	glUseProgramObjectARB(gLinkedProgram);	GLERR();
	glUniform4fvARB(gParam_WMatrix , 4, model_view    );   	GLERR();
	glUniform4fvARB(gParam_VPMatrix, 4, project_matrix);	GLERR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);  //  GLERR(); 
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer); 	//GLERR();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));  //GLERR();
	glClientActiveTexture(GL_TEXTURE1);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));
	glClientActiveTexture(GL_TEXTURE0);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6)); //GLERR();
	// 
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);   glBindTexture(GL_TEXTURE_2D, this->texture[0]);      // 选择纹理 //  此处纹理为单个物体的从属纹理i  
	glEnableClientState(GL_VERTEX_ARRAY);         glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);           glClientActiveTexture(GL_TEXTURE0);

	glsl_validate_program();
	glDrawElements(GL_TRIANGLES, (this->Face_Num) * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));   //The starting point of the IBO
	glDisable(GL_DEPTH_TEST);
}
// 
void  GLSL_Loader::GLSL_Vertex_Frag_Enable3(int begin_triangle, int length)
{
	glUseProgramObjectARB(gLinkedProgram);
	glUniform4fvARB(gParam_WMatrix, 4, model_view);
	// glGetFloatv(GL_PROJECTION_MATRIX     , project_matrix);
	glUniform4fvARB(gParam_VPMatrix, 4, project_matrix);

	// Draw primitive
	// Bind buffers
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
	//GLERR();

	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	//GLERR();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));
	//GLERR();

	glClientActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));
	//GLERR();

	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6));
	GLERR();

	glEnable(GL_DEPTH_TEST);
	//////// Set clipping plane (gl_ClipVertex is in WORLD space, so out clip plane in WORLD space to) 
	// 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture[0]);      // 选择纹理 //  此处纹理为单个物体的从属纹理i  
	glEnableClientState(GL_VERTEX_ARRAY);
	// glEnable ( GL_CULL_FACE)             ;  	             
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);     // 
	glEnableClientState(GL_VERTEX_ARRAY);
	glClientActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE0);
	//
	// Validate program
	//
	glsl_validate_program();
	glDrawElements(GL_TRIANGLES, (length)* 3, GL_UNSIGNED_INT, BUFFER_OFFSET(begin_triangle * 3 * sizeof(int)));   //The starting point of the IBO
	glDisable(GL_DEPTH_TEST);
}
//
void  GLSL_Loader::GLSL_Vertex_Frag_Disable()
{
	glDisableClientState(GL_VERTEX_ARRAY); 	//GLERR();
	glDisableClientState(GL_NORMAL_ARRAY); 	//GLERR();
	glClientActiveTexture(GL_TEXTURE0);                // 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);      // 
	glDisable(GL_CULL_FACE);  	                       // CG part               
	glClientActiveTexture(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); //GLERR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); //GLERR();
	glUseProgramObjectARB(0); 	//GLERR();
}//
void  GLSL_Loader::GLSL_Vertex_Frag_Disable2()
{
	glDisableClientState(GL_VERTEX_ARRAY); 	//GLERR();
	glDisableClientState(GL_NORMAL_ARRAY); 	//GLERR();

	glClientActiveTexture(GL_TEXTURE0);              // 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);    // 
	glDisable(GL_CULL_FACE);  	                     // CG part               
	glClientActiveTexture(GL_TEXTURE1); glDisableClientState(GL_TEXTURE_COORD_ARRAY); 	//GLERR();
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); 	//GLERR();
	glUseProgramObjectARB(0); 	//GLERR();
}
// 
void  GLSL_Loader::GLSL_Vertex_Frag_Disable3()
{
	glDisableClientState(GL_VERTEX_ARRAY); 	//GLERR();

	glDisableClientState(GL_NORMAL_ARRAY); 	//GLERR();

	glClientActiveTexture(GL_TEXTURE0);                // 
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);         // 
	glDisable(GL_CULL_FACE);  	                          // CG part               

	glClientActiveTexture(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); 	//GLERR();

	glBindBuffer(GL_ARRAY_BUFFER, 0); 	//GLERR();

	glUseProgramObjectARB(0); 	//GLERR();
}
// 
void GLSL_Loader::glsl_validate_program()
{
	static int ProgramValidated = FALSE;

	if (ProgramValidated == FALSE)
	{
		ProgramValidated = TRUE;

		glValidateProgramARB(gLinkedProgram);
		GLERR();

		int Valid = 0;
		glGetObjectParameterivARB(gLinkedProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &Valid);
		GLERR();

		if (!Valid)
		{
			int BLen = 0;
			int SLen = 0;

			XPPrint("Error while validating program !");

			glGetObjectParameterivARB(gLinkedProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &BLen);
			GLERR();

			if (BLen > 1)
			{
				char *LogData = (char *)malloc(BLen + 1);

				glGetInfoLogARB(gLinkedProgram, BLen, &SLen, LogData);
				GLERR();

				XPPrint("Validation error log: \n%s\n", LogData);
				free(LogData);
			}
		}
		else
		{
			int BLen = 0;
			int SLen = 0;

			XPPrint("Program validated successfully !");

			glGetObjectParameterivARB(gLinkedProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &BLen);
			GLERR();

			if (BLen > 1)
			{
				char *LogData = (char *)malloc(BLen + 1);

				glGetInfoLogARB(gLinkedProgram, BLen, &SLen, LogData);
				GLERR();

				XPPrint("Validation warnings log: \n%s\n", LogData);
				free(LogData);
			}
		}
	}
};
// 
GLSL_Loader::~GLSL_Loader() { }

void GLSL_Loader::XPPrint(const char *format, ...)
{
	if (format == NULL)
		return;
	va_list arglist;
	static char buffer[8192];

	va_start(arglist, format);
	vsprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]), format, arglist);
	va_end(arglist);

	::OutputDebugStringA(buffer);
	::OutputDebugStringA("\n");
	printf("%s\n", buffer);
	return;
}

void  GLSL_Loader::GLERR()
{
	GLenum err = glGetError();
	if (err == GL_NO_ERROR)
		return;

	switch (err)
	{
	case GL_INVALID_ENUM:
		XPPrint("GL_INVALID_ENUM");
		break;
	case GL_INVALID_VALUE:
		XPPrint("GL_INVALID_VALUE");
		break;
	case GL_INVALID_OPERATION:
		XPPrint("GL_INVALID_OPERATION");
		system("pause");
		break;
	case GL_STACK_OVERFLOW:
		XPPrint("GL_STACK_OVERFLOW");
		break;
	case GL_STACK_UNDERFLOW:
		XPPrint("GL_STACK_UNDERFLOW");
		break;
	case GL_OUT_OF_MEMORY:
		XPPrint("GL_OUT_OF_MEMORY");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		XPPrint("GL_INVALID_FRAMEBUFFER_OPERATION_OES");
		break;
	default:
		XPPrint("GL_UNKNOWN (%X)", err);
	} 
	return;
}

//  vec_in.z  = vec_in.z  - 100 ;  // 
// 	vec_in.z  = vec_in.z  - 100 ;  // 
float3 Vec_rotation ( float3 vec_in , GLfloat * Rotation_4_by_4 )
{ 
	vec_in.z  = vec_in.z  - 100 ; // 
	float3 vec_out ; 
	vec_out.x = Rotation_4_by_4[0+0] *  vec_in.x  + Rotation_4_by_4[1+0] *  vec_in.y + Rotation_4_by_4[2+0] *  vec_in.z   ; 
	vec_out.y = Rotation_4_by_4[0+4] *  vec_in.x  + Rotation_4_by_4[1+4] *  vec_in.y + Rotation_4_by_4[2+4] *  vec_in.z   ; 
	vec_out.z = Rotation_4_by_4[0+8] *  vec_in.x  + Rotation_4_by_4[1+8] *  vec_in.y + Rotation_4_by_4[2+8] *  vec_in.z   ; //  
	return vec_out ; // 
}
// 
float3 Vec_rotation_2 ( float3 vec_in , GLfloat * Rotation_4_by_4 )
{
	float3 vec_out ; 
	vec_out.x = Rotation_4_by_4[0+0] *  vec_in.x  + Rotation_4_by_4[1+0] *  vec_in.y + Rotation_4_by_4[2+0] *  vec_in.z   ; 
	vec_out.y = Rotation_4_by_4[0+4] *  vec_in.x  + Rotation_4_by_4[1+4] *  vec_in.y + Rotation_4_by_4[2+4] *  vec_in.z   ; 
	vec_out.z = Rotation_4_by_4[0+8] *  vec_in.x  + Rotation_4_by_4[1+8] *  vec_in.y + Rotation_4_by_4[2+8] *  vec_in.z   ; //  
	return vec_out ; // 
}
/////////////////   ///////////////////
int  GLSL_Loader::Intersect_leap_probe( GLfloat * Transformation_of_Model ) // 
{
	Selected_vertex_count  =  0  ;  //  //
	int If_intersect_flag  = -1  ;  //  //   
	float3 Probe_direction_post_rot =  Vec_rotation_2 ( Probe_Hand_direction ,  Transformation_of_Model ) ; //    
	float3 Probe_Origin_post_rot    =  Vec_rotation   ( Probe_Hand_Origin    ,  Transformation_of_Model ) ; //  
	// ***********    *********** //   
 	//If_intersect_flag = Octree_Intersection( received_vertexes , maxVertices , Probe_Hand_Origin , 6 ) ;
	If_intersect_flag = Octree_Intersection(received_vertexes, maxVertices, Probe_Origin_post_rot ,  6 ) ; // 
 	return If_intersect_flag ; // 
}
// 
GLenum GLSL_Loader::CreateShader(const char *filename, GLenum type)
{
	FILE *f = fopen(filename, "rb");
	if (f == NULL) { XPPrint("Cannot open file [%s]", filename); exit(0); }  

	fseek(f, 0, SEEK_END);  DWORD s = ftell(f); fseek(f, 0, SEEK_SET); 
	char *buffer = (char*)malloc(s+1); 	fread(buffer, s, 1, f); 	fclose(f); 	buffer[s] = 0; 
	GLenum  Shader = glCreateShaderObjectARB(type);  GLERR();  
	glShaderSourceARB(Shader, 1, (const GLcharARB **)&buffer, NULL); GLERR(); 
	free(buffer);  
	glCompileShader(Shader); 	GLERR(); 
	int Compiled = 0; 	glGetObjectParameterivARB(Shader, GL_OBJECT_COMPILE_STATUS_ARB, &Compiled); GLERR(); // 
	if (!Compiled) 
	{ 
		XPPrint("There was error during compilation !");

		int BLen = 0;	 
		int SLen = 0;	

		glGetObjectParameterivARB(Shader, GL_OBJECT_INFO_LOG_LENGTH_ARB , &BLen);
		GLERR();

		if (BLen > 1)
		{
			char *LogData = (char *)malloc(BLen+1); 
			glGetInfoLogARB(Shader, BLen, &SLen, LogData); 	GLERR();
			XPPrint("Compilation error log: \n%s\n", LogData);  
			free(LogData);   
		}

		glDeleteObjectARB(Shader);
		return 0;
	}

	return Shader;
}
// 
void   GLSL_Loader::GLSL_INIT() 
{
	//
	GLERR();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	// glClearColor(0.2, 0.2, 0.2, 1.0); 
	glClearColor(0.0, 0.0, 0, 1.0); 
	glewInit(); //
	// Load programs
	//
	gVertexProgram_normal   = CreateShader("VertexProgram.glsl", GL_VERTEX_SHADER_ARB);
	gFragmentProgram_normal = CreateShader("FragmentProgram.glsl", GL_FRAGMENT_SHADER_ARB); 
	//// //  这个是正常的渲染器 
	//gVertexProgram   = CreateShader("Shader\\Vertex_conventional.glsl"  , GL_VERTEX_SHADER_ARB  );
	//gFragmentProgram = CreateShader("Shader\\Fragment_conventional.glsl", GL_FRAGMENT_SHADER_ARB);
	//// // 

	//
	// Link programs
	//
	gLinkedProgram_normal = glCreateProgramObjectARB();
	GLERR();

	glAttachObjectARB(gLinkedProgram_normal, gVertexProgram_normal  );
	GLERR();

	glAttachObjectARB(gLinkedProgram_normal, gFragmentProgram_normal );
	GLERR();

	glLinkProgramARB( gLinkedProgram_normal );
	GLERR();

	int Linked = 0;
	glGetObjectParameterivARB( gLinkedProgram_normal , GL_OBJECT_LINK_STATUS_ARB, &Linked);
	GLERR();

	if (!Linked)
	{
		XPPrint("There was error during linking !");
		// 
		int BLen = 0;	 
		int SLen = 0;	 
		// 
		glGetObjectParameterivARB( gLinkedProgram_normal , GL_OBJECT_INFO_LOG_LENGTH_ARB , &BLen);
		GLERR(); 
		// 
		if (BLen > 1)
		{
			char *LogData = (char *)malloc(BLen+1) ;  // 
			glGetInfoLogARB( gLinkedProgram_normal , BLen, &SLen, LogData);
			GLERR(); // 
			XPPrint("Linking error log: \n%s\n", LogData);
			free(LogData);   
		} 
		// 
		glDeleteObjectARB( gLinkedProgram_normal );
		GLERR(); 
		//  
		gLinkedProgram_normal = 0;
	}

	if (  gLinkedProgram_normal  ) 
	{
		gParam_VPMatrix_normal   = glGetUniformLocationARB( gLinkedProgram_normal , "VPMatrix");
		gParam_WMatrix_normal    = glGetUniformLocationARB( gLinkedProgram_normal , "WMatrix" );
		gParam_IV_Params_normal  = glGetUniformLocationARB( gLinkedProgram_normal , "IV_Parameters" );
		glParam_ot_Param_normal  = glGetUniformLocationARB( gLinkedProgram_normal , "other_Parameters" );
	}

	std::cout<<"gVertexProgram_normal   = "<<gVertexProgram_normal <<std::endl ;
	std::cout<<"gFragmentProgram_normal = "<<gFragmentProgram_normal<<std::endl ;
	std::cout<<"gParam_VPMatrix_normal  = "<<gParam_VPMatrix_normal<<std::endl ;
	std::cout<<"gParam_WMatrix_normal   = "<<gParam_WMatrix_normal<<std::endl ;
	std::cout<<"gLinkedProgram_normal   = "<<gLinkedProgram_normal <<std::endl ;
	std::cout<<"gParam_IV_Params_normal = "<<gParam_IV_Params_normal<<std::endl ;
	std::cout<<"glParam_ot_Param_normal = "<<glParam_ot_Param_normal<<std::endl ;
	std::cout<<"*************  Shader1  *************"<<std::endl ;

	GLERR();
	return;
}
// 
// 
void   GLSL_Loader::GLSL_INIT_2() 
{
	//
	GLERR();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glewInit();

	//
	// Load programs
	//gVertexProgram   = CreateShader("VertexProgram.glsl", GL_VERTEX_SHADER_ARB);
	//gFragmentProgram = CreateShader("FragmentProgram.glsl", GL_FRAGMENT_SHADER_ARB);
	//
	//
	gVertexProgram   = CreateShader("Shader\\Vertex_conventional.glsl"  , GL_VERTEX_SHADER_ARB  );
	gFragmentProgram = CreateShader("Shader\\Fragment_conventional.glsl", GL_FRAGMENT_SHADER_ARB);
	//

	//
	// Link programs
	//
	gLinkedProgram = glCreateProgramObjectARB();
	GLERR();

	glAttachObjectARB(gLinkedProgram, gVertexProgram);
	GLERR();

	glAttachObjectARB(gLinkedProgram, gFragmentProgram);
	GLERR();

	glLinkProgramARB(gLinkedProgram);
	GLERR();

	int Linked = 0;
	glGetObjectParameterivARB(gLinkedProgram, GL_OBJECT_LINK_STATUS_ARB, &Linked);
	GLERR();

	if (!Linked)
	{
		XPPrint("There was error during linking !");

		int BLen = 0;	 
		int SLen = 0;	

		glGetObjectParameterivARB(gLinkedProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB , &BLen);
		GLERR();

		if (BLen > 1)
		{
			char *LogData = (char *)malloc(BLen+1);

			glGetInfoLogARB(gLinkedProgram, BLen, &SLen, LogData);
			GLERR();

			XPPrint("Linking error log: \n%s\n", LogData);
			free(LogData);   
		}

		glDeleteObjectARB(gLinkedProgram);
		GLERR();

		gLinkedProgram = 0;
	}

	if (gLinkedProgram)
	{
		gParam_VPMatrix = glGetUniformLocationARB( gLinkedProgram , "VPMatrix");
		gParam_WMatrix  = glGetUniformLocationARB( gLinkedProgram , "WMatrix" );
		gParam_IV_Params= glGetUniformLocationARB( gLinkedProgram , "IV_Parameters" );
		glParam_ot_Param= glGetUniformLocationARB( gLinkedProgram , "other_Parameters" );
	}

	std::cout<<"gVertexProgram   = "<<gVertexProgram<<std::endl ;
	std::cout<<"gFragmentProgram = "<<gFragmentProgram<<std::endl ;
	std::cout<<"gParam_VPMatrix  = "<<gParam_VPMatrix<<std::endl ;
	std::cout<<"gParam_WMatrix   = "<<gParam_WMatrix<<std::endl ;
	std::cout<<"gLinkedProgram   = "<<gLinkedProgram<<std::endl ;
	std::cout<<"gParam_IV_Params = "<<gParam_IV_Params<<std::endl ;
	std::cout<<"glParam_ot_Param = "<<glParam_ot_Param<<std::endl ;
	std::cout<<"*************  Shader2  *************"<<std::endl ;
	GLERR();
	return;
}

// 
void GLSL_Loader::LoadGLTextures( string f_name  ) 
{
	AUX_RGBImageRec *texture1;  // Human_Heart    
	size_t size = f_name.length();

	wchar_t *buffer = new wchar_t[size+1];

	MultiByteToWideChar( CP_ACP, 0, f_name.c_str(), size, buffer, size * sizeof(wchar_t) );

	buffer[size] = 0;  // 确保以 '\0' 结尾  // 

	// texture1 = auxDIBImageLoadW(L"Data\\Human_Heart-2.bmp");  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	texture1 = auxDIBImageLoadW( buffer );  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	delete buffer;

	if (!texture1) exit(1);
	// Create Texture
	glGenTextures(1, &texture[0]) ; 
	//  
	glBindTexture(GL_TEXTURE_2D, texture[0]); // 
	/*
	GL_REPEAT	对纹理的默认行为。重复纹理图像。
	GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
	GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
	GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
	*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //  

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB ); 

	// 制作了Mipmap 
	glTexImage2D(GL_TEXTURE_2D, 0,  4 , texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data );     
	glGenerateMipmap(GL_TEXTURE_2D ) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	free(texture1->data); //释放位图占据的内存资源
	free(texture1);	
} 

// 
void GLSL_Loader::LoadGLTextures( string f_name , int m  ) 
{
	AUX_RGBImageRec *texture1     ;  // Human_Heart    
	size_t size = f_name.length() ;  // 
	wchar_t *buffer = new wchar_t[size+1];

	MultiByteToWideChar( CP_ACP, 0, f_name.c_str(), size, buffer, size * sizeof(wchar_t) );

	buffer[size] = 0;  // 确保以 '\0' 结尾 

	// texture1 = auxDIBImageLoadW(L"Data\\Human_Heart-2.bmp");  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	texture1 = auxDIBImageLoadW( buffer );  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	delete buffer;

	if (!texture1) exit(1);
	// Create Texture
	glGenTextures(1, &texture[0]) ; 
	glBindTexture(GL_TEXTURE_2D, texture[0]); // 
	/*
	GL_REPEAT	对纹理的默认行为。重复纹理图像。
	GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
	GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
	GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
	*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //  

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); 

	// 制作了Mipmap 
	glTexImage2D(GL_TEXTURE_2D, 0,  4 , texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data );     
	// glGenerateMipmap(GL_TEXTURE_2D ) ;
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	free(texture1->data); //释放位图占据的内存资源
	free(texture1);	
} 
//// 
////// 
void GLSL_Loader::LoadGLTextures()
{
	AUX_RGBImageRec *texture1;  // Human_Heart   
								// texture1 = auxDIBImageLoadW(L"Data\\Human_Heart-2.bmp");  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	texture1 = auxDIBImageLoadW(L"Data\\heart_texture_composited.bmp");  // Human_Heartz_simplifyed Pattern_resolution  spongebob_2
	if (!texture1) exit(1);
	// Create Texture
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // 
											  /*
											  GL_REPEAT	对纹理的默认行为。重复纹理图像。
											  GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的。
											  GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
											  GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色。
											  */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //  

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB);

	// 制作了Mipmap 
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	free(texture1->data); //释放位图占据的内存资源
	free(texture1);
}

#pragma endregion dfasf   
