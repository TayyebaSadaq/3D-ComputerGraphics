#pragma once

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

// Format of the constant buffer. This must match the format of the
// cbuffer structure in the shader

struct CBuffer
{
	Matrix		WorldViewProjection;
};

// Structure of a single vertex.  This must match the
// structure of the input vertex in the shader

struct Vertex
{
	Vector4		Position;
	Vector4     Colour;

};

struct pyramidVertex
{
	Vector4 Position;
	Vector4 Colour;
};

// The description of the vertex that is passed to CreateInputLayout.  This must
// match the format of the vertex above and the format of the input vertex in the shader

D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC pyramidVertexDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// This example uses hard-coded vertices and indices for a cube. Usually, you will load the verticesa and indices from a model file. 
// We will see this later in the module. 


Vertex vertices[] =
{
	{ Vector4(-1.0f, -1.0f, -1.0f, 1.0f),  Vector4(Colors::White)},
	{ Vector4(-1.0f, +1.0f, -1.0f, 1.0f),  Vector4(Colors::Black)},
	{ Vector4(+1.0f, +1.0f, -1.0f, 1.0f),  Vector4(Colors::Red)},
	{ Vector4(+1.0f, -1.0f, -1.0f, 1.0f),  Vector4(Colors::Green)},
	{ Vector4(-1.0f, -1.0f, +1.0f, 1.0f),  Vector4(Colors::Blue)},
	{ Vector4(-1.0f, +1.0f, +1.0f, 1.0f),  Vector4(Colors::Yellow)},
	{ Vector4(+1.0f, +1.0f, +1.0f, 1.0f),  Vector4(Colors::Cyan)},
	{ Vector4(+1.0f, -1.0f, +1.0f, 1.0f),  Vector4(Colors::Magenta)}
};

UINT indices[] = {
	// front face
	0, 1, 2,
	0, 2, 3,

	// back face
	4, 6, 5,
	4, 7, 6,

	// left face
	4, 5, 1,
	4, 1, 0,

	// right face
	3, 2, 6,
	3, 6, 7,

	// top face
	1, 5, 6,
	1, 6, 2,

	// bottom face
	4, 0, 3,
	4, 3, 7
};
/*
pyramidVertex pyramidVertices[] =
{
	{ Vector3(0.0f, 1.0f, 0.0f), Vector4(Colors::Red) },      // Apex
	{ Vector3(-1.0f, -1.0f, -1.0f), Vector4(Colors::White) }, // Base corner 1
	{ Vector3(1.0f, -1.0f, -1.0f), Vector4(Colors::Black) },  // Base corner 2
	{ Vector3(1.0f, -1.0f, 1.0f), Vector4(Colors::Blue) },    // Base corner 3
	{ Vector3(-1.0f, -1.0f, 1.0f), Vector4(Colors::Green) }   // Base corner 4
};*/
/*{ Vector3(0.0f, 1.0f, 0.0f), Vector4(Colors::Red) },      // Apex
    { Vector3(-1.0f, -1.0f, -1.0f), Vector4(Colors::White) }, // Base corner 1
    { Vector3(1.0f, -1.0f, -1.0f), Vector4(Colors::Black) },  // Base corner 2
    { Vector3(1.0f, -1.0f, 1.0f), Vector4(Colors::Blue) },    // Base corner 3
    { Vector3(-1.0f, -1.0f, 1.0f), Vector4(Colors::Green) },   // Base corner 4

    { Vector4(-1.5f, -1.0f, -1.5f, 1.0f), Vector4(Colors::Red) },
    { Vector4(-1.5f, -1.0f, +1.5f, 1.0f), Vector4(Colors::Green) },
    { Vector4(+1.5f, -1.0f, +1.5f, 1.0f), Vector4(Colors::Blue) },
    { Vector4(+1.5f, -1.0f, -1.5f, 1.0f), Vector4(Colors::Yellow) },
    { Vector4(0.0f, +1.0f, 0.0f, 1.0f), Vector4(Colors::Cyan) } this is */

pyramidVertex pyramidVertices[] =

{
	{ Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Vector4(Colors::Red) },
	{ Vector4(1.0f, -1.0f, 1.0f, 1.0f), Vector4(Colors::Green) },
	{ Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector4(Colors::Blue) },
	{ Vector4(1.0f, -1.0f, -1.0f, 1.0f), Vector4(Colors::Yellow) },

	{Vector4(0.0f, 2.0f, 0.0f, 1.0f), Vector4(Colors::Cyan) }

};

UINT pyramidIndices[] =
{


	0, 2, 1,    // base
	1, 2, 3,
	0, 1, 4,    // sides
	1, 3, 4,
	3, 2, 4,
	2, 0, 4,
	
};
/*
pyramidVertex pyramidVertices[] =
{
	{ Vector3(-1.5f, -1.0f, -1.5f, 1.0f), Vector4(Colors::Red) },
	{ Vector4(-1.5f, -1.0f, +1.5f, 1.0f), Vector4(Colors::Green) },
	{ Vector4(+1.5f, -1.0f, +1.5f, 1.0f), Vector4(Colors::Blue) },
	{ Vector4(+1.5f, -1.0f, -1.5f, 1.0f), Vector4(Colors::Yellow) },
	{ Vector4(0.0f, +1.0f, 0.0f, 1.0f), Vector4(Colors::Cyan) }  // Apex of the pyramid
};

UINT pyramidIndices[] = {
	// Base of the pyramid
	8, 9, 10,
	10, 11, 8,

	// Side faces
	8, 9, 11,
	9, 10, 11
};
*/
/*	// base 
	1, 2, 3,
	1, 3, 4,

	// sides 
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 1, 3,*/