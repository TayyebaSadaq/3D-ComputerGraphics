#pragma once


#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"



// Format of the constant buffer. This must match the format of the
// cbuffer structure in the shader

struct CBuffer
{
	Matrix		WorldViewProjection;
	Matrix		World;
	Vector4		MaterialColour;
	Vector4		AmbientLightColour;
	Vector4		DirectionalLightColour;
	Vector4		DirectionalLightVector;
};

// Structure of a single vertex.  This must match the
// structure of the input vertex in the shader

struct Vertex
{
	Vector3		Position;
	Vector3		Normal;
};

// The description of the vertex that is passed to CreateInputLayout.  This must
// match the format of the vertex above and the format of the input vertex in the shader

D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// This example uses hard-coded vertices and indices for a cube. Usually, you will load the verticesa and indices from a model file. 
// We will see this later in the module. 


Vertex vertices[] =
{
	{ Vector3(-1.0f, -1.0f, 1.0f),  Vector3(0.0f, 0.0f, 0.0f) },    // side 1
	{ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)  },
	{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)},

	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f)  },    // side 2
	{ Vector3(-1.0f, 1.0f, -1.0f) ,Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, 1.0f, -1.0f) , Vector3(0.0f, 0.0f, 0.0f)},

	{ Vector3(-1.0f, 1.0f, -1.0f) ,Vector3(0.0f, 0.0f, 0.0f)  },    // side 3
	{ Vector3(-1.0f, 1.0f, 1.0f) , Vector3(0.0f, 0.0f, 0.0f)},
	{ Vector3(1.0f, 1.0f, -1.0f) ,Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, 1.0f, 1.0f),Vector3(0.0f, 0.0f, 0.0f)  },

	{ Vector3(-1.0f, -1.0f, -1.0f) , Vector3(0.0f, 0.0f, 0.0f)  },    // side 4
	{ Vector3(1.0f, -1.0f, -1.0f) ,Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)  },
	{ Vector3(1.0f, -1.0f, 1.0f) , Vector3(0.0f, 0.0f, 0.0f) },

	{ Vector3(1.0f, -1.0f, -1.0f) , Vector3(0.0f, 0.0f, 0.0f) },    // side 5
	{ Vector3(1.0f, 1.0f, -1.0f) , Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)   },
	{ Vector3(1.0f, 1.0f, 1.0f) , Vector3(0.0f, 0.0f, 0.0f) },

	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f) },    // side 6
	{ Vector3(-1.0f, -1.0f, 1.0f) ,Vector3(0.0f, 0.0f, 0.0f)  },
	{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f)   },
	{ Vector3(-1.0f, 1.0f, 1.0f) , Vector3(0.0f, 0.0f, 0.0f) }
};

UINT indices[] = {
			0, 1, 2,       // side 1
			2, 1, 3,
			4, 5, 6,       // side 2
			6, 5, 7,
			8, 9, 10,      // side 3
			10, 9, 11,
			12, 13, 14,    // side 4
			14, 13, 15,
			16, 17, 18,    // side 5
			18, 17, 19,
			20, 21, 22,    // side 6
			22, 21, 23,
};


struct pyramidVertex
{
	Vector3 Position;
	Vector3 Normal;
};



D3D11_INPUT_ELEMENT_DESC pyramidVertexDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// This example uses hard-coded vertices and indices for a cube. Usually, you will load the vertices and indices from a model file. 
// We will see this later in the module. 





pyramidVertex pyramidVertices[] =

{
	{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f)},
	{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f) },
	{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f) },

	{Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f) }


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
