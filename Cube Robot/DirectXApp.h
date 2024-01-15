#pragma once
#include "DirectXFramework.h"

class DirectXApp : public DirectXFramework
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();
	float _rotationAngle;
	float _yOffset;
	bool _isGoingUp;
	float shoulderOffsetX;
	float shoulderOffsetY; 
	float shoulderOffsetZ;

};

