#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

class Drawing
{
public:
	Drawing();
	~Drawing();

	vector<glm::vec3> points;
	vector<glm::vec3> generatedPoints;
	vector<glm::vec3> trees;
	vector<glm::vec3> rightLane;
	vector<glm::vec3> leftLane;
	float dist;

	Mesh* CreateGrass();
	Mesh* CreateCircuit();
	Mesh* CreateLine();
	void CreateTrees();
	bool isOnRoad(float checkX, float checkZ);
	void generatePoints();
};