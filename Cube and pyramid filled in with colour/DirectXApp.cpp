#include "DirectXApp.h"

// Geometry.h contains the vertex and constant buffer structures
// as well as the vertices and indices for a cube

#include "Geometry.h"


// DirectX libraries that are needed
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

DirectXApp app;

DirectXApp::DirectXApp() : Framework(800, 600)
{
	// Initialise vectors used to create camera.  We will look
	// at this in detail later
	_eyePosition = Vector3(0.0f, 0.0f, -10.0f);
	_focalPointPosition = Vector3(0.0f, 0.0f, 0.0f);
	_upVector = Vector3(0.0f, 1.0f, 0.0f);
}

bool DirectXApp::Initialise()
{
	if (!GetDeviceAndSwapChain())
	{
		return false;
	}
	OnResize(SIZE_RESTORED);
	BuildGeometryBuffers();
	BuildPyramidGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRasteriserState();

	return true;
}

void DirectXApp::Update()
{
	// Update cube rotation
	_worldTransformation = Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f);

	// Update pyramid rotation
	_pyramidWorldTransformation = Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f) * Matrix::CreateTranslation(Vector3(5.0f, 0.0f, 0.0f));
	_rotationAngle = (_rotationAngle + 1) % 360;

	//This will make the pyramid rotate around the cube
	_pyramidWorldTransformation = Matrix::CreateRotationY(-_rotationAngle * XM_PI / 60.0f) * Matrix::
		CreateTranslation(Vector3(5.0f, 0.0f, 0.0f)) * Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f);

}

void DirectXApp::Render()
{
	const float clearColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColour);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Calculate the view and projection transformation
	_viewTransformation = XMMatrixLookAtLH(_eyePosition, _focalPointPosition, _upVector);
	_projectionTransformation = XMMatrixPerspectiveFovLH(XM_PIDIV4, static_cast<float>(GetWindowWidth()) / GetWindowHeight(), 1.0f, 100.0f);

	// Calculate the world x view x projection transformation for the cube
	Matrix cubeTransformation = _worldTransformation * _viewTransformation * _projectionTransformation;
	CBuffer cubeConstantBuffer;
	cubeConstantBuffer.WorldViewProjection = cubeTransformation;

	// Update the constant buffer for the cube
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &cubeConstantBuffer, 0, 0);

	// Render the cube
	UINT cubeStride = sizeof(Vertex);
	UINT cubeOffset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &cubeStride, &cubeOffset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->IASetInputLayout(_layout.Get());
	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
	_deviceContext->RSSetState(_rasteriserState.Get());
	_deviceContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);

	// Calculate the world x view x project transformation for the pyramid 
	Matrix pyramidTransformation = _pyramidWorldTransformation * _viewTransformation * _projectionTransformation;
	CBuffer pyramidConstantBuffer;
	pyramidConstantBuffer.WorldViewProjection = pyramidTransformation;

	// Update the constant buffer for the pyramid
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &pyramidConstantBuffer, 0, 0);

	// Render the pyramid
	UINT pyramidStride = sizeof(pyramidVertex);
	UINT pyramidOffset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, _pyramidVertexBuffer.GetAddressOf(), &pyramidStride, &pyramidOffset);
	_deviceContext->IASetIndexBuffer(_pyramidIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->DrawIndexed(ARRAYSIZE(pyramidIndices), 0, 0);

	// Present the back buffer
	ThrowIfFailed(_swapChain->Present(0, 0));
};




// OnResize is called by the framework whenever Windows gets a WM_Size message. We need to recreate
// the draw and depth buffers to reflect the revised height and width of the window. 

void DirectXApp::OnResize(WPARAM wParam)
{
	if (wParam == SIZE_MINIMIZED)
	{
		return;
	}
	// Free any existing render and depth views (which
	// would be the case if the window was being resized)
	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_depthStencilBuffer = nullptr;

	ThrowIfFailed(_swapChain->ResizeBuffers(1, GetWindowWidth(), GetWindowHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Create a drawing surface for DirectX to render to
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(_device->CreateRenderTargetView(backBuffer.Get(), NULL, _renderTargetView.GetAddressOf()));

	// The depth buffer is used by DirectX to ensure
	// that pixels of closer objects are drawn over pixels of more
	// distant objects.

	// First, we need to create a texture (bitmap) for the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferTexture = { 0 };
	depthBufferTexture.Width = GetWindowWidth();
	depthBufferTexture.Height = GetWindowHeight();
	depthBufferTexture.ArraySize = 1;
	depthBufferTexture.MipLevels = 1;
	depthBufferTexture.SampleDesc.Count = 4;
	depthBufferTexture.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferTexture.Usage = D3D11_USAGE_DEFAULT;
	depthBufferTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Create the depth buffer.  
	ComPtr<ID3D11Texture2D> depthBuffer;
	ThrowIfFailed(_device->CreateTexture2D(&depthBufferTexture, NULL, depthBuffer.GetAddressOf()));
	ThrowIfFailed(_device->CreateDepthStencilView(depthBuffer.Get(), 0, _depthStencilView.GetAddressOf()));

	// Bind the render target view buffer and the depth stencil view buffer to the output-merger stage
	// of the pipeline. 
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// Specify a viewport of the required size
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.Width = static_cast<float>(GetWindowWidth());
	viewPort.Height = static_cast<float>(GetWindowHeight());
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &viewPort);
}

bool DirectXApp::GetDeviceAndSwapChain()
{
	UINT createDeviceFlags = 0;

	// We are going to only accept a hardware driver or a WARP
	// driver
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = GetWindowWidth();
	swapChainDesc.BufferDesc.Height = GetWindowHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate to 0 and let DXGI determine the best option (refer to DXGI best practices)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetHWnd();
	// Start out windowed
	swapChainDesc.Windowed = true;
	// Enable multi-sampling to give smoother lines (set to 1 if performance becomes an issue)
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;

	// Loop through the driver types to determine which one is available to us
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;

	for (unsigned int driver = 0; driver < totalDriverTypes && driverType == D3D_DRIVER_TYPE_UNKNOWN; driver++)
	{
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(0,
			driverTypes[driver],
			0,
			createDeviceFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			_swapChain.GetAddressOf(),
			_device.GetAddressOf(),
			0,
			_deviceContext.GetAddressOf()
		)))

		{
			driverType = driverTypes[driver];
		}
	}
	if (driverType == D3D_DRIVER_TYPE_UNKNOWN)
	{
		// Unable to find a suitable device driver
		return false;
	}
	return true;
}

void DirectXApp::BuildGeometryBuffers()
{
	// This method uses the arrays defined in Geometry.h
	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor = { 0 };
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationData = { 0 };
	vertexInitialisationData.pSysMem = &vertices;

	// and create the vertex buffer
	ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	// Setup the structure that specifies how big the index 
	// buffer should be
	D3D11_BUFFER_DESC indexBufferDescriptor = { 0 };
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the indices from
	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &indices;

	// and create the index buffer
	ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void DirectXApp::BuildPyramidGeometryBuffers()
{
	//Setup the structure that specifies how big the pyramid vertex buffer
	//should be 
	D3D11_BUFFER_DESC vertexBufferDescriptorPyramid = { 0 };
	vertexBufferDescriptorPyramid.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptorPyramid.ByteWidth = sizeof(Vertex) * ARRAYSIZE(pyramidVertices);
	vertexBufferDescriptorPyramid.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptorPyramid.CPUAccessFlags = 0;
	vertexBufferDescriptorPyramid.MiscFlags = 0;
	vertexBufferDescriptorPyramid.StructureByteStride = 0;

	//Now set up a structure that tells DirectX where to get the
   // data for the pyramid vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationDataPyramid = { 0 };
	vertexInitialisationDataPyramid.pSysMem = &pyramidVertices;

	// and create the pyramid vertex buffer
	ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptorPyramid, &vertexInitialisationDataPyramid, _pyramidVertexBuffer.GetAddressOf()));

	// Set up the structure that specifies how big the pyramid index buffer
	// should be 
	D3D11_BUFFER_DESC indexBufferDescriptorPyramid = { 0 };
	indexBufferDescriptorPyramid.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptorPyramid.ByteWidth = sizeof(UINT) * ARRAYSIZE(pyramidIndices);
	indexBufferDescriptorPyramid.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptorPyramid.CPUAccessFlags = 0;
	indexBufferDescriptorPyramid.MiscFlags = 0;
	indexBufferDescriptorPyramid.StructureByteStride = 0;

	// This is a structure that tells where to get the 
	// pyramid index from 
	D3D11_SUBRESOURCE_DATA indexInitialisationDataPyramid = { 0 };
	indexInitialisationDataPyramid.pSysMem = &pyramidIndices;

	// and create the pyramid index buffer 
	ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptorPyramid, &indexInitialisationDataPyramid, _pyramidIndexBuffer.GetAddressOf()));

}

void DirectXApp::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(ShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		VertexShaderName, "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

	// Compile pixel shader
	hr = D3DCompileFromFile(ShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		PixelShaderName, "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}




void DirectXApp::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it. The vertexDesc array is
	// defined in Geometry.h

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void DirectXApp::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void DirectXApp::BuildRasteriserState()
{	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	// The following tells the rasteriser to draw a wireframe model.  For solid models, set it to D3D11_FILL_SOLID
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _rasteriserState.GetAddressOf()));
}
