#pragma once 
#include "SceneNode.h"
#include "DirectXFramework.h"

class CubeNode : public SceneNode 
{
public:
	CubeNode(wstring name) : CubeNode(name, Vector4(0.25f, 0.25f, 0.25f, 1.0f)) {};
	CubeNode(wstring name, const Vector4 ambientColour) : SceneNode(name) { _ambientColour = ambientColour; }

	bool Initialise(); 
	void Render(); 
	

private: 
	
	ComPtr<ID3D11Device>			_device; 
	ComPtr<ID3D11DeviceContext>		_deviceContext;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr; 
	ComPtr<ID3DBlob>				_pixelShaderByteCode =  nullptr; 
	ComPtr<ID3D11VertexShader>		_vertexShader; 
	ComPtr<ID3D11PixelShader>		_pixelShader; 
	ComPtr<ID3D11InputLayout>		_layout; 
	ComPtr<ID3D11Buffer>			_constantBuffer;

	Vector4							_ambientColour; 
	


	void BuildVertexNormals();
	void BuildGeometryBuffers(); 
	void BuildShaders(); 
	void BuildVertexLayout();
	void BuildConstantBuffer(); 

};
