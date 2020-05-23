#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "Scene1.h"
#include "Model.h"
#include "ObjLoader.h"
#include "Physics.h"
#include "Randomizer.h"
#include "SimplexRebuild.h"

using namespace GAME;
using namespace MATH;


Scene1::Scene1(Window& windowRef):Scene(windowRef) {
	shader = nullptr;
	//collisionCheckObj = Simplex(colStarter);
}


bool Scene1::OnCreate() {
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
	OnResizeWindow(windowPtr->GetWidth(),windowPtr->GetHeight());
	
	
	/// Load Assets: as needed 
	lightPos = Vec3(-10.0f, 16.0f, 20.0f);

	rightWall = Plane(-1.0f, 0.0f, 0.0f, -10.0f);
	leftWall = Plane(1.0f, 0.0f, 0.0f, -10.0f);
	floor = Plane(0.0f, 1.0f, 0.0f, -7.0f);
	ceiling = Plane(0.0f, -1.0f, 0.0f, -7.0f);
	front = Plane(0.0f, 0.0f, -1.0f, 1.0f);
	back = Plane(0.0f, 0.0f, 1.0f, -21.0f);
	if (ObjLoader::loadOBJ("tetrahedron.obj") == false) {
		return false;
	}
	/// Get the data out of the ObjLoader and into our own mesh
	meshes.push_back(new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords));


	/// Create a shader with attributes
	shader = new Shader("phongVert.glsl", "phongFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
	if (shader == nullptr) {
		OnDestroy();
		return false;
	}

	Randomizer r;
	for (int i = 0; i < 2; ++i) {
		Model* m = new Model(meshes, shader);
		m = new Model(meshes, shader);
		if (m == nullptr) {
			OnDestroy();
			return false;
		}

		m->setGrav(false);
		m->setMass(1.0f);
		m->setBoundingSphere(1.0f);
		//m->setPos(Vec3(r.box_muller(0.0,1.0), r.box_muller(5.0, 1.0), r.box_muller(0.0, 1.0)));
		//m->setVel(Vec3(r.box_muller(0.0, 0.001), r.box_muller(0.0, 0.001), r.box_muller(0.0, 0.001)));
		
		// Now set the intial orientation and angular velocity
		m->setOrientation(Quaternion(1.0, 0.0, 0.0, 0.0));
		Vec3 angularVelocity(0.0, 1.0, 0.0); // axis of rotation
		float angleInDegrees = 45.0;
		angularVelocity *= (angleInDegrees * 2 * 3.141593 / 360.0); // multiplying with angle in radians
		
		if (i == 0){
			m->setPos(MATH::Vec3(0, 0.5, 0));
			m->setVel(MATH::Vec3(2, 0, 0));
			m->setAngularVel(angularVelocity);

		}
		else if (i == 1){
			m->setPos(MATH::Vec3(4, 0, 0));
			m->setVel(MATH::Vec3(-2, 0, 0));
			m->setAngularVel(-angularVelocity);
			
		}
		//std::cout << "models.size() =" << models.size() << std::endl;
		models.push_back(m);
	}
	return true;
}

void Scene1::Update(const float deltaTime) {
	for (Model* m : models) {
		m->Update(deltaTime);
	}
	for (Model* m : models) {
		///* Disabling Collision Response
		if (Physics::PlaneSphereCollision(*m, rightWall) == true) {
			Physics::PlaneSphereCollisionResponse(*m, rightWall);
		}

		if (Physics::PlaneSphereCollision(*m, leftWall) == true) {
			Physics::PlaneSphereCollisionResponse(*m, leftWall);
		}

		if (Physics::PlaneSphereCollision(*m, floor) == true) {
			Physics::PlaneSphereCollisionResponse(*m, floor);
		}

		if (Physics::PlaneSphereCollision(*m, ceiling) == true) {
			Physics::PlaneSphereCollisionResponse(*m, ceiling);
		}

		if (Physics::PlaneSphereCollision(*m, front) == true) {
			Physics::PlaneSphereCollisionResponse(*m, front);
		}

		if (Physics::PlaneSphereCollision(*m, back) == true) {
			Physics::PlaneSphereCollisionResponse(*m, back);
		}
	}

	for (unsigned int i = 0; i < models.size() - 1; i++) {
		for (unsigned int j = i + 1; j < models.size(); j++) {
			Model* m_i = models.at(i);
			Model* m_j = models.at(j);
			
			SimplexRebuild cCheckObj = SimplexRebuild(*m_i,*m_j);
			//Simplex collisionCheckObj = Simplex(Physics::getMinkowskiDiff(*m_i, *m_j), *m_i->simplexDeterminingPointHighDot, *m_j->simplexDeterminingPointLowDot);
			//collisionCheckObj.print();
			//collisionCheckObj.getImportantVertData();
			//collisionCheckObj.getMostCenteredPoint();
			//collisionCheckObj.startGJK();

			//if (cCheckObj.collision && lTest) { //if the last check was true and this check is true then do nothing
			//}
			//else if (cCheckObj.collision == false && lTest == false) { //if the last check was false and this check was false then do nothing
			//}
			//else if (cCheckObj.collision && lTest == false) { //if the last check was false and this check was true then its the first frame of collision
			//	printf("First");
			//}
			//else if (cCheckObj.collision == false && lTest) { //if the last check was true and this check was false then its the first frame after collision
			//	printf("Last");
			//}

			//if (cCheckObj.collision) {
			//	lTest = true;
			//}
			//else {
			//	lTest = false;
			//}

			if (cCheckObj.collision) {
				m_j->setVel(-m_j->getVelocity());
				m_i->setVel(-m_i->getVelocity());
			}
			std::cout << std::endl;

			if (Physics::SphereSphereCollision(*m_i, *m_j)) {
				//Physics::SphereSphereCollisionResponse(*m_i, *m_j);
				//std::cout << "Boop";
			}
		}
	}
}

void Scene1::OnResizeWindow(int w_, int h_){
	windowPtr->SetWindowSize(w_,h_);
	glViewport(0,0,w_,h_);
	float aspectRatio = (float)w_ / (float)h_;
	/***if (w_ > h_) {
		projectionMatrix = MMath::orthographic(-10.0f, 10.0f, -10.0f / aspectRatio, 10.0f / aspectRatio, -10.0f, 10.0f);
	} else {
		projectionMatrix = MMath::orthographic(-10.0f* aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -10.0f, 10.0f);
	}***/

	projectionMatrix = MMath::perspective(40.0f, aspectRatio, 2.0f, 50.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, -10.0f, 20.0f), 
							   Vec3(0.0f, 0.0f, 0.0f), 
							   Vec3(0.0f, 1.0f, 0.0f));
}

void Scene1::Render() const{
	GLint projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
	GLint viewMatrixID = glGetUniformLocation(shader->getProgram(), "viewMatrix");
	GLint normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
	GLint lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	/// Draw your scene here
	glClearColor(0.01, 0.01, 0.2, 0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glUseProgram(shader->getProgram());
	Matrix4 rotation = MMath::rotate(0.7, 0.7, 0.0, 22.0);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, viewMatrix * rotation );
	Matrix3 normalMatrix = rotation;
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	glUniform3fv(lightPosID, 1, lightPos);
	int count = 0;
	for (Model* m : models) {
		//std::cout << "Rendering model[" << count << "]" << std::endl;
		count++;
		m->Render();
	}
	//count = 0;
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
}

void Scene1::HandleEvents(const SDL_Event& SDLEvent){
	if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN){
		
	}
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		
	}
		
}

Scene1::~Scene1() {
	OnDestroy();
}
void Scene1::OnDestroy() {
	/// Cleanup Assets
	for (Model* m : models) {
		if (m) delete m;
	}
	
	for (Mesh* mesh : meshes) {
		if (mesh) delete mesh;
	}

	if (shader) delete shader;
}