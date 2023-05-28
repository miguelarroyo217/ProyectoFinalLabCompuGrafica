#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	IntenD = 0.0f;
	IntenA = 0.0f;
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	IntenD = dIntensity;
	IntenA = aIntensity;
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, IntenA);
	//glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, IntenD);
	//glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

void DirectionalLight::SetInten(GLfloat ambiente, GLfloat difuso)
{
	IntenA = ambiente;
	IntenD = difuso;
}

DirectionalLight::~DirectionalLight()
{
}
