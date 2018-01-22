#pragma once
#include "shader_m.h"
#include "mesh.h"
using namespace std;



class Floor {
public:

	Shader floorShader;
	//vector<unsigned int> indices;
	//vector<Texture> textures;
	Mesh floorMesh;
	glm::mat4 floor;
	float relief[200][200];
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
	Floor(const char* path) {
		floorShader = Shader("floor.vs", "floor.fs");

		//loadModel(path, &vertices_floor);

		floorMesh = Mesh(path, true, false);
		floorMesh.setupTexture("snow.jpg");
		floorShader.use();
		floorShader.setInt("texture3", 0);

		floor = glm::mat4(1.0f);
		//model = glm::translate(model, floorPositions);
		floor = glm::rotate(floor, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//= glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		buildFloor();
	}

	// render the mesh
	void Draw(Light sun_moon, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, Fog fog, Fire fire, Character c)
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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorMesh.texture);
		floorShader.use();
		//shader.setInt("texture3", texture3);
		floorShader.setBool("isDay", sun_moon.isDay);
		floorShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		floorShader.setMat4("view", view);
		floorShader.setVec3("lightColor", sun_moon.lightColor);
		floorShader.setVec3("lightDirection", sun_moon.lightDir);
		floorShader.setVec3("lightPos", sun_moon.lightPos);
		floorShader.setVec3("viewPos", viewPos);
		floorShader.setFloat("constant", sun_moon.constant);
		floorShader.setFloat("linear", sun_moon.linear * 1.0);
		floorShader.setFloat("quadratic", sun_moon.quadratic  * 1.0);

		floorShader.setFloat("FogMaxDist", fog.maxDist);
		floorShader.setFloat("FogMinDist", fog.minDist);
		floorShader.setVec4("FogColor", fog.color);

		fire.setShader(floorShader);
		c.setShader(floorShader);


		glBindVertexArray(floorMesh.VAO);
		

		floorShader.setMat4("floor", floor);
		//glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, floorMesh.vertices.size() );

		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
		//glDisableVertexAttribArray
	}


private:
	/*  Mesh Data  */
	//vector<Vertex> vertices_floor;
	/*  Render data  */
	//unsigned int VBO;
	//Shader floorShader;
	/*  Functions    */
	// initializes all the buffer objects/arrays

	void buildFloor() {
		for (int i = 0; i < 200; i++) {
			for (int j = 0; j < 200; j++) {
				relief[i][j] = -1;
			}
		}
		for (int i = 0; i < floorMesh.vertices.size(); i++) {
			int x = floorMesh.vertices[i].Position.x + 100;
			int y = floorMesh.vertices[i].Position.y + 100;
			float z = floorMesh.vertices[i].Position.z;
			relief[x][y] = relief[x][y] > z ? relief[x][y] : z;
		}

	start:
		float temp[200][200];
		memcpy(temp, relief, 200 * 200 * sizeof(float));
		for (int i = 1; i < 199; i++) {
			for (int j = 1; j < 199; j++) {
				if (relief[i][j] != -1) {
					float num = relief[i][j];
					temp[i - 1][j - 1] = relief[i - 1][j - 1] == -1 ? num : relief[i - 1][j - 1];
					temp[i - 1][j - 0] = relief[i - 1][j - 0] == -1 ? num : relief[i - 1][j - 0];
					temp[i - 1][j + 1] = relief[i - 1][j + 1] == -1 ? num : relief[i - 1][j + 1];
					temp[i - 0][j - 1] = relief[i - 0][j - 1] == -1 ? num : relief[i - 0][j - 1];
					temp[i - 0][j + 1] = relief[i - 0][j + 1] == -1 ? num : relief[i - 0][j + 1];
					temp[i + 1][j - 1] = relief[i + 1][j - 1] == -1 ? num : relief[i + 1][j - 1];
					temp[i + 1][j - 0] = relief[i + 1][j - 0] == -1 ? num : relief[i + 1][j - 0];
					temp[i + 1][j + 1] = relief[i + 1][j + 1] == -1 ? num : relief[i + 1][j + 1];
				}
			}
		}
		memcpy(relief, temp, 200 * 200 * sizeof(float));
		for (int i = 1; i < 199; i++) {
			for (int j = 1; j < 199; j++) {
				if (relief[i][j] == -1) {
					goto start;
				}
			}
		}


		//for(int i = 0; i < floor.)
	}


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
