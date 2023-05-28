/*
Semestre 2023-2
Nombre: Arroyo Quiroz José Miguel
Proyecto Final: Diorama de la Vida Cotidiana (Lucario y Mordecai)
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include <ctime>
#include <iostream>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float toffsetu;
float toffsetv;

float movVol;
float movVolOffset;
int rotVol;
int rotVolOffset;
bool arriba;
float movLuc;
float movLucOffset;
int rotLuc;
int rotLucOffset;
bool avanza;
bool gira;

//Banderas
int bandia = 0;
/*unsigned t0, t1;
double time;*/

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;


Model Pueblo;
Model Arboles;
Model Voltorb;
Model Electrode;
Model Onix;

//Avatar
Model LucCuerpo;
Model LucCabeza;
Model LucCola;
Model LucBraDer;
Model LucBraIzq;
Model LucPierDer;
Model LucPierIzq;
Model CasaParque;
Model Fuente;
Model Dulceria;
Model AreaInf;
Model Reja;

Model CentroPokemon;
Model Laboratorio;
Model Gym;
Model Tienda;
Model Casa;
Model CentroComercial;
Model Torre;
Model Casa2;

Skybox skyboxDia;
Skybox skyboxNoche;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();


	Pueblo = Model();
	Pueblo.LoadModel("Models/PuebloCentro.obj");
	Arboles = Model();
	Arboles.LoadModel("Models/Arboles.obj");
	Voltorb = Model();
	Voltorb.LoadModel("Models/Voltorb.obj");
	Electrode = Model();
	Electrode.LoadModel("Models/Electrode.obj");
	Onix = Model();
	Onix.LoadModel("Models/Onix.obj");

	//Avatar
	LucCuerpo = Model();
	LucCuerpo.LoadModel("Models/LucarioCuerpo.obj");
	LucCabeza = Model();
	LucCabeza.LoadModel("Models/LucarioCabeza.obj");
	LucCola = Model();
	LucCola.LoadModel("Models/LucarioCola.obj");
	LucBraDer = Model();
	LucBraDer.LoadModel("Models/LucarioBraDer.obj");
	LucBraIzq = Model();
	LucBraIzq.LoadModel("Models/LucarioBraIzq.obj");
	LucPierDer = Model();
	LucPierDer.LoadModel("Models/LucarioPierDer.obj");
	LucPierIzq = Model();
	LucPierIzq.LoadModel("Models/LucarioPierIzq.obj");
	CasaParque = Model();
	CasaParque.LoadModel("Models/CasaParque.obj");
	Fuente = Model();
	Fuente.LoadModel("Models/FuenteParque.obj");
	Dulceria = Model();
	Dulceria.LoadModel("Models/SnackArea.obj");
	AreaInf = Model();
	AreaInf.LoadModel("Models/AreaInfantil.obj");
	Reja = Model();
	Reja.LoadModel("Models/Reja.obj");

	// EDIFICACIONES
	Laboratorio = Model();
	Laboratorio.LoadModel("Models/LabPokemon.fbx");
	CentroPokemon = Model();
	CentroPokemon.LoadModel("Models/CentroPokemon.fbx");
	Gym = Model();
	Gym.LoadModel("Models/GYM.fbx");
	Tienda = Model();
	Tienda.LoadModel("Models/Shop.fbx");
	Casa = Model();
	Casa.LoadModel("Models/CasaP.fbx");
	Casa2 = Model();
	Casa2.LoadModel("Models/Casa2.fbx");
	Torre = Model();
	Torre.LoadModel("Models/TorreLavanda.fbx");
	CentroComercial = Model();
	CentroComercial.LoadModel("Models/CentroComercial.fbx");


	std::vector<std::string> skyboxFacesDia;
	
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFacesDia.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	std::vector<std::string> skyboxFacesNoche;

	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");

	

	skyboxDia = Skybox(skyboxFacesDia);
	skyboxNoche = Skybox(skyboxFacesNoche);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Inicialización de Variables de Animación
	movVol = 0.0;
	movVolOffset = 0.1;
	rotVol = 0;
	rotVolOffset = 5;
	arriba = false;
	movLuc = 0.0;
	movLucOffset = 0.1;
	rotLuc = 0;
	rotLucOffset = 1;
	avanza = false;
	gira = false;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//t0 = clock();

		//Animación Voltorb y Electrode
		if (movVol < 10.0f && arriba == false && mainWindow.getBanOnAnim() == true) {
			movVol += movVolOffset * deltaTime;
			rotVol += rotVolOffset * deltaTime;
			if (movVol < 10.5f && movVol > 9.5f)
				arriba = true;
		}
		else if (movVol > 0.0f && arriba == true && mainWindow.getBanOnAnim() == true) {
			movVol -= movVolOffset * deltaTime;
			rotVol -= rotVolOffset * deltaTime;
			if (movVol < 0.5f && movVol > -0.5f)
				arriba = false;
		}

		//Animación Avatar
		if (rotLuc < 20 && gira == false && mainWindow.getBanOnAnim() == true) {
			rotLuc += movLucOffset * deltaTime;
			if (rotLuc < 21 && movLuc > 19) {
				rotLuc -= rotLucOffset * deltaTime;
				gira = true;
			}
		}
		else if (rotLuc > -20 && gira == true && mainWindow.getBanOnAnim() == true) {
			rotLuc += movLucOffset * deltaTime;
		}

		if (movLuc < 100.5f && avanza == false && mainWindow.getBanOnAnim() == true) {
			movLuc += movLucOffset * deltaTime;
			//rotLuc += rotLucOffset * deltaTime;
			if (movLuc < 101.0f && movLuc > 100.0f ) {
				//rotLuc += rotLucOffset * deltaTime;
				//avanza = true;
				//gira = true;
			}
		}
		/*else if (movVol > 0.0f && arriba == true && mainWindow.getBanOnAnim() == true) {
			movVol -= movVolOffset * deltaTime;
			rotVol -= rotVolOffset * deltaTime;
			if (movVol < 0.5f && movVol > -0.5f)
				arriba = false;
		}*/

		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (mainWindow.getBanDia() == 1) {
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelLuc(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);//Se declara al inicio para que las texturas no se muevan

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(14.0f, 1.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//Pueblo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pueblo.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 115.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, -115.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles.RenderModel();

		//Voltorb
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, movVol, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, rotVol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Voltorb.RenderModel();

		//Electrode
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, movVol, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180+rotVol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Electrode.RenderModel();

		//Onix
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Onix.RenderModel();

		
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f+movLuc, 0.0f, 50.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelLuc = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCuerpo.RenderModel();

		//Cabeza
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCabeza.RenderModel();

		//Cola
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucCola.RenderModel();

		//Brazo Derecho
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraDer.RenderModel();

		//Brazo Izquierdo
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucBraIzq.RenderModel();

		//Pierna Derecha
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierDer.RenderModel();

		//Pierna Izquierda
		model = glm::mat4(1.0);
		model = modelLuc;
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LucPierIzq.RenderModel();

		//##########################//
		//#### Casa del Parque  ####//
		//##########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaParque.RenderModel();

		//##########################//
		//#### Fuente del Parque####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuente.RenderModel();

		//##########################//
		//####     Dulcería	    ####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dulceria.RenderModel();

		//##########################//
		//#### Area Infantil    ####//
		//##########################//
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AreaInf.RenderModel();

		//##########################//
		//#### Rejas del parque ####//
		//##########################// Cada barra mide 0.188f
		model = modelaux;
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 45.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 0.188f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f + 0.376f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();
		
		
		//######################//
		//#### Laboratorio  ####//
		//######################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.398f, 0.0f, 14.775f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Laboratorio.RenderModel();

		//#########################//
		//#### Centro Pokémon  ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.493f, 0.0f, -39.164f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CentroPokemon.RenderModel();

		//#########################//
		//####		GYM		   ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(42.699f, 0.0f, -14.528f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Gym.RenderModel();

		//#########################//
		//#### Tienda Pokémon  ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.285f, 0.0f, 34.026f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Tienda.RenderModel();

		//#########################//
		//####   Casa Pokémon  ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.695f, 0.0f, -15.845f));
		model = glm::scale(model, glm::vec3(0.21f, 0.21f, 0.21f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Casa.RenderModel();

		//#########################//
		//####  Casa Pokémon 2 ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.467f, 0.0f, 37.66f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Casa2.RenderModel();

		//#########################//
		//####  Torre lavanda  ####//
		//#########################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(43.903f, 0.0f, 15.084f));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Torre.RenderModel();

		//############################//
		//####  Centro Comercial  ####//
		//############################//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.851f, 0.0f, -42.168f));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CentroComercial.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -10.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);//Para indicar trasparencia y traslucidez
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Va antes de la textura
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();


		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001 * deltaTime;
		toffsetv += 0.0 * deltaTime;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetu, toffsetv);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		FlechaTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		glDisable(GL_BLEND);//Desactiva el blender
		toffset = glm::vec2(0.0f, 0.0f);

		/*t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		std::cout << "Execution Time: " << time;*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
