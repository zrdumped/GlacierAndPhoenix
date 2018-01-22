#pragma once
#include "shader_m.h"
#include "mesh.h"
using namespace std;


class Sky {
public:

	Shader skyShader;
	//vector<unsigned int> indices;
	//vector<Texture> textures;
	Mesh skyMesh;
	//unsigned int texture3;
	/*  Functions  */
	// constructor
	/*Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
	}*/
	Sky(const char* path) {
		skyShader = Shader("sky.vs", "sky.fs");
		skyMesh = Mesh(path, false, true);

		//skyMesh.setupMesh();

		// texture 3
		// ---------
		//unsigned int texture;
		/*glGenTextures(1, &texture3);
		glBindTexture(GL_TEXTURE_2D, texture3);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load("snow.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);*/

		//floorShader.use();
		//sfloorShader.setInt("texture", 0);
		//skyShader.use();
		//skyShader.setInt("texture3", 0);
	}

	// render the mesh
	void Draw(Light sun_moon, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, Fog fog)
	{
		// bind appropriate textures
		/*	unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
		number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
		number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
		number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
		number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}*/



		// draw mesh
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture3);
		float port = sun_moon.port;
		if (sun_moon.isDay) {
			radius = 1 * (1 - port) + 16 * port;

			objectcolor.x = objectcolors[1].x * port + objectcolors[0].x * (1.1 - port);
			objectcolor.y = objectcolors[1].y * port + objectcolors[0].y * (1.1 - port);
			objectcolor.z = objectcolors[1].z * port + objectcolors[0].z * (1.1 - port);

			offset_attenuation = 20 * (1 - port) + 0.7 * port;
			specularStrength = 0.1 * (1 - port) + 1.0 * port;
			offset_RGB = glm::vec3(5.0, 3.0, 1.0) * (1 - port) + glm::vec3(10.0, 10.0, 10.0) * port;
		}
		else {
			objectcolor.x = objectcolors[2].x * -port + objectcolors[0].x * (1 + port);
			objectcolor.y = objectcolors[2].y * -port + objectcolors[0].y * (1 + port);
			objectcolor.z = objectcolors[2].z * -port + objectcolors[0].z * (1 + port);

			offset_RGB = glm::vec3(5.0, 3.0, 1.0) * (1 + port) + glm::vec3(1.0, 1.0, 1.0) * -port;
			specularStrength = 0.1 * (1 + port);
			offset_attenuation = 20 * (1 + port) + 100 * -port;
		}

			skyShader.use();
			//shader.setInt("texture3", texture3);
			skyShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
			skyShader.setMat4("view", view);
			skyShader.setVec3("lightColor", sun_moon.lightColor);
			skyShader.setVec3("lightPos", sun_moon.lightPos);
			skyShader.setVec3("viewPos", viewPos);
			skyShader.setVec3("offset_RGB", offset_RGB);
			skyShader.setFloat("specularStrength", specularStrength);
			skyShader.setFloat("constant", sun_moon.constant);
			skyShader.setFloat("linear", sun_moon.linear * offset_attenuation);
			skyShader.setFloat("quadratic", sun_moon.quadratic * offset_attenuation);
			skyShader.setFloat("radius", radius);
			skyShader.setVec3("objectColor", objectcolor);

			skyShader.setFloat("FogMaxDist", fog.maxDist);
			skyShader.setFloat("FogMinDist", fog.minDist);
			skyShader.setVec4("FogColor", fog.color);

			glBindVertexArray(skyMesh.VAO);
			glm::mat4 sky(1.0f);
			//model = glm::translate(model, floorPositions);
			sky = glm::rotate(sky, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//= glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			skyShader.setMat4("sky", sky);
			//glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, skyMesh.vertices.size() * 3);

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		
	}


private:
	/*  Mesh Data  */
	glm::vec3 objectcolors[3] = { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 1.0f), glm::vec3(0.1f, 0.1f, 0.4f) };
	glm::vec3 objectcolor = glm::vec3(1.0f, 1.0f, 1.0f);
	/*  Render data  */
	float radius = 1.0f;
	float offset_attenuation = 0.15;
	glm::vec3 offset_RGB = glm::vec3(5.0, 3.0, 0.0);
	float specularStrength = 0.1;
	//Shader floorShader;
	/*  Functions    */
	// initializes all the buffer objects/arrays
	
};

/*bool loadOBJ(
const char * path,
std::vector<std::vector<vec3>> & out_vertices,
std::vector<vec2> & out_uvs,
std::vector<std::vector<vec3>> & out_normals
) {
printf("Loading OBJ file %s...\n", path);

std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
std::vector<vec3> temp_vertices;
std::vector<vec2> temp_uvs;
std::vector<vec3> temp_normals;

float maxY = 1.0, minY = -1.0;


FILE * file = NULL;
fopen_s(&file, path, "r");
if (file == NULL) {
printf("Impossibl to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
return false;
}
while (1) {

char lineHeader[128];
// read the first word of the line
int res = fscanf(file, "%s", lineHeader);
if (res == EOF)
break; // EOF = End Of File. Quit the loop.

// else : parse lineHeader

if (strcmp(lineHeader, "v") == 0) {
//std::cout << "Get v" << endl;
vec3 vertex;
fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

maxY = (maxY < vertex.y) ? vertex.y : maxY;
minY = (minY > vertex.y) ? vertex.y : minY;

temp_vertices.push_back(vertex);
}
else if (strcmp(lineHeader, "vt") == 0) {
//std::cout << "Get vt" << endl;
vec2 uv;
fscanf(file, "%f %f\n", &uv.x, &uv.y);
uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
temp_uvs.push_back(uv);
}
else if (strcmp(lineHeader, "vn") == 0) {
//std::cout << "Get vn" << endl;
vec3 normal;
fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
temp_normals.push_back(normal);
}
else if (strcmp(lineHeader, "f") == 0) {
//std::cout << "Get f" << endl;
std::string vertex1, vertex2, vertex3, vertex4;
unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
int matches = fscanf(file, "%d//%d %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3]);
if (matches == 6) {
//printf("666\n");
std::vector<vec3> vec_temp;
vec_temp.push_back(temp_vertices[vertexIndex[0] - 1]);
vec_temp.push_back(temp_vertices[vertexIndex[1] - 1]);
vec_temp.push_back(temp_vertices[vertexIndex[2] - 1]);
out_vertices.push_back(vec_temp);
std::vector<vec3> nor_temp;
nor_temp.push_back(temp_normals[normalIndex[0] - 1]);
nor_temp.push_back(temp_normals[normalIndex[1] - 1]);
nor_temp.push_back(temp_normals[normalIndex[2] - 1]);
out_normals.push_back(nor_temp);
//printf("%d\n", normalIndex[2]);
}
else if (matches == 8) {
//printf("888\n");
std::vector<vec3> vec_temp;
vec_temp.push_back(temp_vertices[vertexIndex[0] - 1]);
vec_temp.push_back(temp_vertices[vertexIndex[1] - 1]);
vec_temp.push_back(temp_vertices[vertexIndex[2] - 1]);
vec_temp.push_back(temp_vertices[vertexIndex[3] - 1]);
out_vertices.push_back(vec_temp);
std::vector<vec3> nor_temp;
nor_temp.push_back(temp_normals[normalIndex[0] - 1]);
nor_temp.push_back(temp_normals[normalIndex[1] - 1]);
nor_temp.push_back(temp_normals[normalIndex[2] - 1]);
nor_temp.push_back(temp_normals[normalIndex[3] - 1]);
out_normals.push_back(nor_temp);
//printf("%d\n", normalIndex[3]);
}
else {
printf("error\n");
return false;
}
}
else {
// Probably a comment, eat up the rest of the line
char stupidBuffer[1000];
fgets(stupidBuffer, 1000, file);
}
}
YU = (maxY - ((maxY + minY) / 2.0)) / 4;
radius = (maxY - ((maxY + minY) / 2.0)) / YU;
printf("%f", radius);
return true;
}*/
