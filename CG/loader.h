#pragma once
#include <vector>
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	//glm::vec3 Tangent;
	// bitangent
	//glm::vec3 Bitangent;
};


struct Texture {
	unsigned int id;
	string type;
	string path;
};


void loadModel(const char* path, float scalePort, glm::vec3 offset, vector<Vertex>* result, bool opposite = false) {

	FILE * file = NULL;
	fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return;
	}
	// position
	vector<glm::vec3> Positions;
	// normals
	vector<glm::vec3> Normals;
	// texCoords
	vector<glm::vec2> TexCoords;

	while (1) {

		char lineHeader[128];
		// read the first word of the line  

		int res = fscanf(file, "%s", lineHeader);
		//cout << lineHeader << endl;
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.  

				   // else : parse lineHeader  

		if (strcmp(lineHeader, "v") == 0) {
			//std::cout << "Get v" << endl;
			glm::vec3 v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			v *= glm::vec3(scalePort);
			v += offset;
			//v.x *= scalePort;
			//v.y *= scalePort;
			//v.z *= scalePort;
			Positions.push_back(v);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			//std::cout << "Get vt" << endl;
			glm::vec2 t;
			fscanf(file, "%f %f\n", &t.x, &t.y);
			TexCoords.push_back(t);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 n;
			fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
			if (opposite) {
				//cout << "s" << endl;
				n.x *= -1.0f;
				n.y *= -1.0f;
				n.z *= -1.0f;
			}
			/*float r = rand() % 200 - 100;
			n.x *= 1 - r / 100.0;
			r = rand() % 200 - 100;
			n.y *= 1 - r / 100.0;
			r = rand() % 200 - 100;
			n.z *= 1 - r / 100.0;*/

			Normals.push_back(n);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			//std::cout << "Get f" << endl;
			unsigned int posIndex[3], textIndex[3], normalIndex[3];
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &posIndex[0], &textIndex[0], &normalIndex[0], &posIndex[1], &textIndex[1], &normalIndex[1], &posIndex[2], &textIndex[2], &normalIndex[2]);
			//cout << posIndex[0] << endl;
			for (int i = 0; i < 3; i++) {
				Vertex v;
				v.Position = Positions[posIndex[i] - 1];
				v.TexCoords = TexCoords[textIndex[i] - 1];
				v.Normal = Normals[normalIndex[i] - 1];
				result->push_back(v);
				//indices.push_back(vertices.size());
			}

		}
		else {
			// Probably a comment, eat up the rest of the line  
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	/*if (rm != NULL) {
		glm::mat4 temp = glm::mat4(1.0f);
		for (int i = 0; i < Positions.size(); i++) {
			int x = Positions[i].x;
			int z = Positions[i].z;
			temp[x][z] = Positions[i].y;
		}

	}*/

	return;
}
