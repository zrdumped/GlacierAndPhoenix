#pragma once
using namespace std;

extern WEATHER w;

class Fog {

public:
	float minDist;
	float maxDist;
	glm::vec4 color;

	Fog() {
		minDist = 120;
		maxDist = 120;
		color = dayColor;
	}

	void update(Light l) {
		float port;
		/*if (l.angle_total < Pi / 6.0 && l.angle_total > -Pi / 6.0)
			port = (l.angle_total + Pi / 6.0) / (Pi / 3.0);
		else if (l.angle_total > 5.0 * Pi / 6.0 && l.angle_total < 7.0 * Pi / 6.0)
			port = 1 - (l.angle_total - 5.0 *  Pi / 6.0) / (Pi / 3.0);*/
		if (l.angle_total < Pi / 6.0 && l.angle_total > 0)
			port = (l.angle_total + Pi / 6.0) / (Pi / 6.0);
		else if (l.angle_total > 5.0 * Pi / 6.0 && l.angle_total < Pi)
			port = 1 - (l.angle_total - 5.0 *  Pi / 6.0) / (Pi / 6.0);
		else if (l.angle_total < Pi)
			port = 1;
		else port = 0;
		color = glm::mix(dayColor, nightColor, 1 - port);

		if (w != SUN && minDist > minMinDist) minDist -= 1;
		else if (w == SUN && minDist < maxMinDist) minDist += 1;

		if (w != SUN && maxDist > minMaxDist) maxDist -= 0.5;
		else if (w == SUN && maxDist < maxMaxDist) maxDist += 0.5;
	}
private:
	glm::vec4 dayColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 nightColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float minMinDist = 15;
	float minMaxDist = 70;
	float maxMinDist = 120;
	float maxMaxDist = 120;
};