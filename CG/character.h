#pragma once
#include "light.h"
#include "particle.h"
#define Pi 3.1415926
#define WingNum 11
#define DownTime 20
#define UpTime 30

extern float deltaTime;
extern glm::vec3 wind;

enum Character_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Character
{
public :
	Shader chaShader;
	Mesh chaMesh;

	float rotateAcce = 10.0;
	//ParticleGenerator chaPG;
	float map[200][200];


	Character() {

		Position = glm::vec3(0.0f, 10.0f, 0.0f);
		//OldPos = Position;
		Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		SPEED = 4.5f;
		angle = 180.0f;
	}

	Character(const char* path, const char* wingPath,  float in[][200]) {

		for (int i = 0; i < DownTime; i++) {
			mainAngleList[i] = maxAngle_main - i * (maxAngle_main - minAngle_main) / (DownTime);
			viceAngleList[i] = maxAngle_vice - i * (maxAngle_vice - minAngle_vice) / (DownTime);
		}
		for (int i = DownTime; i < DownTime + UpTime; i++) {
			mainAngleList[i] = minAngle_main + (i - DownTime) * (maxAngle_main - minAngle_main) / (UpTime);
			viceAngleList[i] = minAngle_vice + (i - DownTime) * (maxAngle_vice - minAngle_vice) / (UpTime);
		}

		Position = glm::vec3(0.0f, 4.5f, 0.0f);
		//OldPos = Position;
		Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		SPEED = 4.5f;
		angle = 180.0f;

		memcpy(map, in, 200 * 200 * sizeof(int));
		/*for (int i = 0; i < 200; i++) {
			for (int j = 0; j < 200; j++) {
				cout << map[i][j] << " ";
			}
			cout << endl;
		}*/

		chaShader = Shader("character.vs", "character.fs");
		chaMesh = Mesh(path, true, false, 0.003, glm::vec3(0,-0.3f, -0.1f));
		chaMesh.setupTexture("bird/Body1_0.png");
		chaShader.use();
		chaShader.setInt("texture0", chaMesh.texture);

		wingShader = Shader("wing.vs", "wing.fs");
		for (int i = 0; i < WingNum; i++) {
			wingMesh[i] = Mesh(wingPath, true, false);
			//wingMesh[i] = Mesh(true);
			if (i == 0)
				wingMesh[i].setupTexture("flame.png");
			else
				wingMesh[i].texture = wingMesh[0].texture;
		}
		wingShader.use();
		wingShader.setInt("texture0", 0);
		for (int i = 0; i < WingNum; i++) {
			wing_pos[i] = glm::vec3((i - WingNum / 2) / 10.0,0,0) ;
		}
		for (int i = 0; i < WingNum; i++) {
			glm::vec3 area;
			if (i < 5) 
				area = wing_pos[i] - wing_pos[i + 1];
			else if (i >5)		 
				area = wing_pos[i] - wing_pos[i - 1];
			else				
				area = glm::vec3(0.0f, 0.0f, 0.0f);

			//cout << deltaTime << endl;
			wing_pg[i] = ParticleGenerator(wingShader, wingMesh[i], 5000, 10, Position + wing_pos[i], 40, area, glm::vec3(0.0f, 0.0f, genLen),
				glm::vec4(0.7569f, 0.2471f, 0.1176f, 1.0f), glm::vec4(0.1, 0.1, 0.1, 0.0),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 1,
				0, lifeList[i] * 4,
				0.0, 0.0, 0.0, glm::vec3(0.0, 1.0, 0.0),
				glm::vec3(0.0, 0.0, 1.0 * vMul), glm::vec3(-0.00, -0.0, -0.0),
				0.1, 0.1, 1);
			//sizerange , size
		}
		//cout << Position.x << " " << Position.y << " " << Position.z << " " << endl;
		//cout << wing_pos[5].x << " " << wing_pos[5].y << " " << wing_pos[5].z << " " << endl;
	}

	void DrawCharacter(glm::mat4 view, glm::mat4 projection, Light sun_moon, glm::vec3 cameraPos) {
		//glActiveTexture(GL_TEXTURE0);

		//Position += Direction * rate;
		



/*
		chaShader.use();
		chaShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		chaShader.setMat4("view", view);
		chaShader.setVec3("lightColor", sun_moon.lightColor);
		chaShader.setVec3("lightPos", sun_moon.lightPos);

		//ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		glBindVertexArray(chaMesh.VAO);
		chaShader.setMat4("model", GetModel());
		//std::cout << sun_moon.lightPos.x << std::endl;
		//std::cout << sun_moon.lightPos.y << std::endl;
		//std::cout << sun_moon.lightPos.z << std::endl;
		glDrawArrays(GL_TRIANGLES, 0, chaMesh.vertices.size());*/
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//cout << Position.x << " " << Position.y << " " << Position.z << " " << endl;
		//cout << wing_pos[5].x << " " << wing_pos[5].y << " " << wing_pos[5].z << " " << endl;
		/*glm::vec3 distance = Direction * 0.1f;
		if (distance != 0) {
			float port = (Position.x - static_cast<int>(Position.x)) / distance;
			int x = (Position + Direction * 0.2f).x + 100;
			int z = (Position + Direction * 0.2f).z + 100;
			float y0 = map[static_cast<int>(Position.x) + 100][static_cast<int>(Position.z) + 100];
			float y1 = map[x][z];
			//if (map[x][z] + 2.0> Position.y) Position.y += 0.01;
			//else Position.y -= 0.01;
			Position.y = y0 * (1 - port) + y1 * port;
			//cout << map[x][z] << endl;
		}*/
		//Position.y = map[static_cast<int>(Position.x) + 100][static_cast<int>(Position.z) + 100] + 2.0;
	}

	void DrawWing(glm::mat4 view, glm::mat4 projection, Light sun_moon, glm::vec3 cameraPos) {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		updateTime += 1;
		if (updateTime == DownTime + UpTime) updateTime -= DownTime + UpTime;

		if (updateTime < DownTime)  up = updateTime * upMax / DownTime;
		else up = upMax - (updateTime - DownTime) * upMax / UpTime;

		calWing(mainAngleList[updateTime], viceAngleList[updateTime]);

		//glm::vec4 color = glm::vec4(myrand(), myrand(), myrand(), 1.0);
		for (int i = 0; i < WingNum; i++) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, wingMesh[i].texture);
			wing_pg[i].DrawParticle(view, projection, sun_moon.lightPos, cameraPos);
			//wing_pg[i].UpdateColor(color, color);
			//wing_pg[i].Update(0.2, Position + wing_pos[i]);
			if (i < 5) {
				wing_pg[i].UpdateGenPos(Position + wing_pos[i], wing_pos[i + 1] - wing_pos[i], glm::vec3(-Direction.x * genLen, Direction.y, -Direction.z * genLen));
				wing_pg[i].UpdateVelocity(-Direction * glm::vec3(vMul), glm::vec3(0.1, 0.3, 0.1));
			}
			else if (i > 5) {
				wing_pg[i].UpdateGenPos(Position + wing_pos[i], wing_pos[i - 1] - wing_pos[i], glm::vec3(-Direction.x * genLen, Direction.y, -Direction.z * genLen));
				wing_pg[i].UpdateVelocity(-Direction * glm::vec3(vMul), glm::vec3(0.1, 0.3, 0.1));
			}
			else {
				wing_pg[i].UpdateGenPos(Position + wing_pos[i], glm::vec3(0, 0, 0), glm::vec3(-Direction.x * genLen, Direction.y, -Direction.z * genLen));
				wing_pg[i].UpdateVelocity(-Direction * glm::vec3(vMul), glm::vec3(1.0, 0.3, 1.0));
			}
			wing_pg[i].Update(0.1);
		}

		genLen = 0.15;
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	glm::vec3 GetPosition() {
		return Position;
	}

	glm::vec3 GetDirection() {
		return Direction;
	}

	glm::mat4 GetModel() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, Position + glm::vec3(0.0, up, 0.0));
		//model = glm::scale(model, glm::vec3(10.0f));
		//OldPos = Position;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		return model;
	}

	void ProcessKeyboard(Character_Movement dir, glm::vec3 cameraDir)
	{

		float tmp_angle = atan(cameraDir.z / cameraDir.x);
		if (cameraDir.x < 0) tmp_angle += Pi;
		float x = cos(tmp_angle);
		float z = sin(tmp_angle);

		if ((dir == FORWARD || dir == BACKWARD) && (x != Direction.x || z != Direction.z)) {
			//Direction = cameraDir;
			double endAngle = atan((double)cameraDir.z / (double)cameraDir.x);
			double startAngle = atan((double)Direction.z / (double)Direction.x);
			//std::cout << endAngle << " end " << startAngle << " start" << std::endl;
			//std::cout << Xstart << " i " << Ystart << " j " << Xend << " x " << Yend << " y " << std::endl;
			//防止180跳变
			if (Direction.x >= 0 && cameraDir.x <= 0)endAngle += Pi;
			if (Direction.x < 0 && cameraDir.x > 0)endAngle += Pi;
			//if (cameraDir.z > 0 && Direction.z < 0) angle += pi;
			Direction = glm::vec3(x, 0.0f, z);

			
			angle -= endAngle * 180.0f / Pi - startAngle * 180.0f / Pi;
			//std::cout << angle << std::endl;
		}

		float velocity = SPEED * deltaTime ;
		glm::vec3 windAcce = wind * glm::vec3(0.03);

		if (dir == FORWARD) {
			Position += Direction * velocity;
			Position += windAcce;
			genLen = 0.3;
		}
		if (dir == BACKWARD) {
			Position -= Direction * velocity;
			Position -= windAcce;
			genLen = 0.3;
		}
		if (dir == LEFT ) {
			float tmp = glm::radians(-rotateAcce);
			float x = Direction.x * cos(tmp) - Direction.z * sin(tmp);
			float y = Direction.x * sin(tmp) + Direction.z * cos(tmp);
			Direction.x = x; Direction.z = y;
			angle += rotateAcce;
		}
		if (dir == RIGHT) {
			float tmp = glm::radians(rotateAcce);
			float x = Direction.x * cos(tmp) - Direction.z * sin(tmp);
			float y = Direction.x * sin(tmp) + Direction.z * cos(tmp);
			Direction.x = x; Direction.z = y;
			angle -= rotateAcce;
		}
		//std::cout << AngularAcc << " " << angle << std::endl;
		//std::cout << Position.x << std::endl;
	}

	/*void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		angle -= xoffset;
		float tmp = glm::radians(xoffset);
		tmp_x = Direction.x * cos(tmp) - Direction.z * sin(tmp);
		tmp_y = Direction.x * sin(tmp) + Direction.z * cos(tmp);
		
	}*/

	void setShader(Shader target, bool isSnow = false) {
		char varName[40];
		int wing_index[5] = { 0, 2, 5, 8, 10 };
		if(isSnow){
			for (int i = 0; i < 5; i++) {
				sprintf(varName, "wings[%d].position", i);
				target.setVec3(varName, Position + wing_pos[wing_index[i]]);
				sprintf(varName, "wings[%d].lightColor", i);
				target.setVec3(varName, glm::vec3(0.7569f, 0.2471f, 0.1176f));
				sprintf(varName, "wings[%d].constant", i);
				target.setFloat(varName, 2.0);
				sprintf(varName, "wings[%d].linear", i);
				target.setFloat(varName, 0.2);
				sprintf(varName, "wings[%d].quadratic", i);
				target.setFloat(varName, 0.12);
				sprintf(varName, "wings[%d].ambient", i);
				target.setVec3(varName, glm::vec3(0.5));
				sprintf(varName, "wings[%d].diffuse", i);
				target.setVec3(varName, glm::vec3(1.0));
				sprintf(varName, "wings[%d].specular", i);
				target.setVec3(varName, glm::vec3(0.8));
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				sprintf(varName, "wings[%d].position", i);
				target.setVec3(varName, Position + wing_pos[wing_index[i]]);
				sprintf(varName, "wings[%d].lightColor", i);
				target.setVec3(varName, glm::vec3(0.7569f, 0.2471f, 0.1176f));
				sprintf(varName, "wings[%d].constant", i);
				target.setFloat(varName, 5.0);
				sprintf(varName, "wings[%d].linear", i);
				target.setFloat(varName, 0.49);
				sprintf(varName, "wings[%d].quadratic", i);
				target.setFloat(varName, 0.22);
				sprintf(varName, "wings[%d].ambient", i);
				target.setVec3(varName, glm::vec3(0.05));
				sprintf(varName, "wings[%d].diffuse", i);
				target.setVec3(varName, glm::vec3(0.8));
				sprintf(varName, "wings[%d].specular", i);
				target.setVec3(varName, glm::vec3(1.0));
			}
		}
	}


private:
	glm::vec3 Position;
	glm::vec3 Direction;
	//float rate = 0;
	//float accelerate = 0.1f;
	float angle;
	float SPEED = 4.5f;
	float MouseSensitivity = 0.1f;
	float tmp_x;
	float tmp_y;
	ParticleGenerator wing_pg[WingNum];
	glm::vec3 wing_pos[WingNum];
	Shader wingShader;
	Mesh wingMesh[WingNum];
	float mainAngleList[DownTime + UpTime];
	float viceAngleList[DownTime + UpTime];

	float maxAngle_main = Pi / 4.0;
	float minAngle_main = - Pi / 3.0;
	float maxAngle_vice = Pi / 4.0;
	float minAngle_vice = -Pi / 2.0;
	int updateTime = 0;
	float genLen = 0.15f;
	float vMul = 4.0;

	float lifeList[11] = { 0.1f, 0.3f, 0.5f,0.4f,0.3f, 1.5f, 0.3f, 0.4f,0.5f, 0.3f, 0.1f };

	float upMax = 0.2;
	float up = 0;

	void calWing(float mainAngle, float viceAngle) {
		//direction 为 （0，0，1）时
		for (int i = 5; i <= 8; i++) {
			wing_pos[i] = glm::vec3((i - 5) * 0.1 * cos(mainAngle), (i - 5) * 0.1 * sin(mainAngle) + up, 0.0);
		}
		for (int i = 2; i <= 4; i++) {
			wing_pos[i] = glm::vec3((i - 5) * 0.1 * cos(mainAngle), -(i - 5) * 0.1 * sin(mainAngle) + up, 0.0);
		}
		wing_pos[9] = glm::vec3(wing_pos[8].x + 0.1 * cos(viceAngle), wing_pos[8].y + 0.1 * sin(viceAngle), 0.0);
		wing_pos[10] = glm::vec3(wing_pos[9].x + 0.1 * cos(viceAngle), wing_pos[9].y + 0.1 * sin(viceAngle), 0.0);
		wing_pos[1] = glm::vec3(wing_pos[2].x - 0.1 * cos(viceAngle), wing_pos[2].y + 0.1 * sin(viceAngle), 0.0);
		wing_pos[0] = glm::vec3(wing_pos[1].x - 0.1 * cos(viceAngle), wing_pos[1].y + 0.1 * sin(viceAngle),0.0);
		//将pos转到与direction垂直的平面

		glm::vec3 tmp = glm::normalize(Direction);
		float angle_cos = glm::dot(tmp, glm::vec3(0, 0, -1));
		float angle_sin = sqrt(1 - angle_cos * angle_cos);
		/*if (Direction.x < 0) {
			for (int i = 0; i < 5; i++) 
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, wing_pos[i].x * angle_sin);
			for (int i = 6; i < 11; i++) 
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, -wing_pos[i].x * angle_sin);
		}
		else if (Direction.x > 0) {
			for (int i = 0; i < 5; i++)
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, -wing_pos[i].x * angle_sin);
			for (int i = 6; i < 11; i++)
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, wing_pos[i].x * angle_sin);
		}*/
		if (Direction.x < 0) {
			for (int i = 0; i < WingNum; i++)
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, -wing_pos[i].x * angle_sin);
		}
		else if (Direction.x > 0) {
			for (int i = 0; i < WingNum; i++)
				wing_pos[i] = glm::vec3(wing_pos[i].x * angle_cos, wing_pos[i].y, wing_pos[i].x * angle_sin);
		}
		wing_pos[5] += Direction * glm::vec3(0.1);
	}

	float myrand() {
		static double V1, V2, S;
		static int phase = 0;
		float X;

		if (phase == 0) {
			do {
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
			X = V2 * sqrt(-2 * log(S) / S);

		phase = 1 - phase;

		return X * 0.1 + 0.4;

	}
};