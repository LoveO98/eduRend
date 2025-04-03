
#include "Scene.h"
#include "QuadModel.h"
#include "CubeModel.h"
#include "OBJModel.h"

Scene::Scene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	m_dxdevice(dxdevice),
	m_dxdevice_context(dxdevice_context),
	m_window_width(window_width),
	m_window_height(window_height)
{ }

void Scene::OnWindowResized(
	int new_width,
	int new_height)
{
	m_window_width = new_width;
	m_window_height = new_height;
}

OurTestScene::OurTestScene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	Scene(dxdevice, dxdevice_context, window_width, window_height)
{ 
	InitTransformationBuffer();
	// + init other CBuffers
	InitPointLightBuffer();
}

//
// Called once at initialization
//
void OurTestScene::Init()
{
	m_camera = new Camera(
		45.0f * fTO_RAD,		// field-of-view (radians)
		(float)m_window_width / m_window_height,	// aspect ratio
		0.2f,					// z-near plane (everything closer will be clipped/removed)
		500.0f,					// z-far plane (everything further will be clipped/removed)
		0.005f, 0.005f);		// pitch and yaw sensetivity

	// Move camera to (0,0,5)
	m_camera->MoveTo({ 0, 0, 5 });
	// Set camera rotation to 0 pitch and 0 yaw
	m_camera->RotateTo(0.75f,0);

	// Set an initial position for the point light
	m_point_light = { 0, 5, 0, 0 };

	// Create objects
	//m_quad = new QuadModel(m_dxdevice, m_dxdevice_context);
	

	/*m_cube = new CubeModel(m_dxdevice, m_dxdevice_context);
	m_cube2 = new CubeModel(m_dxdevice, m_dxdevice_context);
	m_cube3 = new CubeModel(m_dxdevice, m_dxdevice_context);*/

	m_cube = new OBJModel("assets/TroyanPlane/TrojanPlaneTest2.obj", m_dxdevice, m_dxdevice_context);
	m_cube2 = new OBJModel("assets/TroyanPlane/TrojanPlaneTest2.obj", m_dxdevice, m_dxdevice_context);
	m_cube3 = new OBJModel("assets/TroyanPlane/TrojanPlaneTest2.obj", m_dxdevice, m_dxdevice_context);

	m_sponza = new OBJModel("assets/crytek-sponza/sponza.obj", m_dxdevice, m_dxdevice_context);

	m_troyan_plane = new OBJModel("assets/TroyanPlane/TrojanPlaneTest2.obj", m_dxdevice, m_dxdevice_context);
}

//
// Called every frame
// dt (seconds) is time elapsed since the previous frame
//
void OurTestScene::Update(
	float dt,
	const InputHandler& input_handler)
{
	// Basic camera control
	if (input_handler.IsKeyPressed(Keys::W))
		m_camera->Move({ 0.0f, 0.0f, -1.0f, 0.0f}, m_camera_velocity * dt );
	if (input_handler.IsKeyPressed(Keys::S))
		m_camera->Move({ 0.0f, 0.0f, 1.0f, 0.0f}, m_camera_velocity * dt);
	if (input_handler.IsKeyPressed(Keys::D))
		m_camera->Move({ 1.0f, 0.0f, 0.0f , 0.0f}, m_camera_velocity * dt);
	if (input_handler.IsKeyPressed(Keys::A))
		m_camera->Move({ -1.0f, 0.0f, 0.0f, 0.0f }, m_camera_velocity * dt);
	
	if (!input_handler.GetMouseDeltaZ() == 0)
		m_camera->ChangeSens(input_handler.GetMouseDeltaZ() / 1200000.0, input_handler.GetMouseDeltaZ() / 1200000.0);
		
	// Camera mouse rotation control
	m_camera->Rotate(input_handler.GetMouseDeltaY(), input_handler.GetMouseDeltaX());


	// Controls for moving the point light around the scene

	if (input_handler.IsKeyPressed(Keys::Up))
		m_point_light += {0, 0, -2 * m_camera_velocity * dt, 0};
	if (input_handler.IsKeyPressed(Keys::Down))
		m_point_light += {0, 0, 2 * m_camera_velocity * dt, 0};
	if (input_handler.IsKeyPressed(Keys::Left))
		m_point_light += {-2 * m_camera_velocity * dt, 0, 0, 0};
	if (input_handler.IsKeyPressed(Keys::Right))
		m_point_light += {2 * m_camera_velocity * dt, 0, 0, 0};
	if (input_handler.IsKeyPressed(Keys::Space))
		m_point_light += {0, 2 * m_camera_velocity * dt, 0, 0};
	if (input_handler.IsKeyPressed(Keys::Ctrl))
		m_point_light += {0, -2 * m_camera_velocity * dt, 0, 0};


	// Now set/update object transformations
	// This can be done using any sequence of transformation matrices,
	// but the T*R*S order is most common; i.e. scale, then rotate, and then translate.
	// If no transformation is desired, an identity matrix can be obtained 
	// via e.g. Mquad = linalg::mat4f_identity; 


	// --------------------- REMOVED QUAD TRANSFORM HERE -----------------------
	// Quad model-to-world transformation
	//m_quad_transform = mat4f::translation(0, 0, 0) *			// No translation
	//	mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) *	// Rotate continuously around the y-axis
	//	mat4f::scaling(1.5, 1.5, 1.5);				// Scale uniformly to 150%



	// Cube model-to-world transformation
	m_cube_transform = mat4f::translation(0, 0, 0) *			// No translation
		mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) *	// Rotate continuously around the y-axis
		mat4f::scaling(1.5, 1.5, 1.5);				// Scale uniformly to 150%

	m_cube2_transform = mat4f::translation(3, 0, 0) *			// No translation
		mat4f::rotation(-m_angle, 0.0f, 0.0f, 1.0f) *	// Rotate continuously around the z-axis
		mat4f::scaling(0.5, 0.5, 0.5);				// Scale uniformly to 150%

	// Pressing C on tke keyboard makes cube2 not orbit cube
	if (input_handler.IsKeyPressed(Keys::C))
	{
		m_cube2_transform = mat4f::rotation(m_angle, 0.0f, 1.0f, 0.0f) * m_cube_transform * m_cube2_transform;
	}
	else
	{
		m_cube2_transform = m_cube_transform * m_cube2_transform;
	}

	m_cube3_transform = mat4f::translation(3, 0, 0) *			// No translation
		mat4f::rotation(m_angle, 0.0f, 0.0f, 1.0f) *	// Rotate continuously around the z-axis
		mat4f::scaling(0.5, 0.5, 0.5);				// Scale uniformly to 150%

	m_cube3_transform =  m_cube2_transform * m_cube3_transform;

	// Sponza model-to-world transformation
	m_sponza_transform = mat4f::translation(0, -5, 0) *		 // Move down 5 units
		mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f) * // Rotate pi/2 radians (90 degrees) around y
		mat4f::scaling(0.05f);						 // The scene is quite large so scale it down to 5%

	m_troyan_plane_transform = mat4f::translation(0, 0, 0) *		 // Move down 5 units
		mat4f::rotation(0, 0.0f, 1.0f, 0.0f) * // Rotate pi/2 radians (90 degrees) around y
		mat4f::scaling(1);						 // The scene is quite large so scale it down to 5%

	// Increment the rotation angle.
	m_angle += m_angular_velocity * dt;

	// Print fps
	m_fps_cooldown -= dt;
	if (m_fps_cooldown < 0.0)
	{
		std::cout << "fps " << (int)(1.0f / dt) << std::endl;
//		printf("fps %i\n", (int)(1.0f / dt));
		m_fps_cooldown = 2.0;
	}
}

//
// Called every frame, after update
//
void OurTestScene::Render()
{
	// Bind transformation_buffer to slot b0 of the VS
	m_dxdevice_context->VSSetConstantBuffers(0, 1, &m_transformation_buffer);
	// Bind other buffers from lab 2
	m_dxdevice_context->PSSetConstantBuffers(0, 1, &m_point_light_buffer);
	m_dxdevice_context->PSSetConstantBuffers(1, 1, &m_phong_components_buffer);


	// Obtain the matrices needed for rendering from the camera
	m_view_matrix = m_camera->WorldToViewMatrix();
	m_projection_matrix = m_camera->ProjectionMatrix();

	// ----------------- REMOVED QUAD HERE-------------------------
	// Load matrices + the Quad's transformation to the device and render it
	//UpdateTransformationBuffer(m_quad_transform, m_view_matrix, m_projection_matrix);
	//m_quad->Render();
	
	// Finally update the point light with the cam and light position
	UpdatePointLightBuffer(m_point_light, m_camera->GetCamPosVec4f());



	// Load matrices + the Cube's transformation to the device and render it
	UpdateTransformationBuffer(m_cube_transform, m_view_matrix, m_projection_matrix);
	m_cube->Render();
	UpdateTransformationBuffer(m_cube2_transform, m_view_matrix, m_projection_matrix);
	m_cube2->Render();
	UpdateTransformationBuffer(m_cube3_transform, m_view_matrix, m_projection_matrix);
	m_cube3->Render();



	// Load matrices + Sponza's transformation to the device and render it
	UpdateTransformationBuffer(m_sponza_transform, m_view_matrix, m_projection_matrix);
	m_sponza->Render();

	UpdateTransformationBuffer(m_troyan_plane_transform, m_view_matrix, m_projection_matrix);
	m_troyan_plane->Render();

	
}

void OurTestScene::Release()
{
	SAFE_DELETE(m_quad);
	SAFE_DELETE(m_cube);
	SAFE_DELETE(m_cube2);
	SAFE_DELETE(m_cube3);
	SAFE_DELETE(m_sponza);
	SAFE_DELETE(m_troyan_plane);
	SAFE_DELETE(m_camera);

	SAFE_RELEASE(m_transformation_buffer);
	// + release other CBuffers
	SAFE_RELEASE(m_point_light_buffer);
	SAFE_RELEASE(m_phong_components_buffer);
}

void OurTestScene::OnWindowResized(
	int new_width,
	int new_height)
{
	if (m_camera)
		m_camera->SetAspect(float(new_width) / new_height);

	Scene::OnWindowResized(new_width, new_height);
}

void OurTestScene::InitTransformationBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC matrixBufferDesc = { 0 };
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(TransformationBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&matrixBufferDesc, nullptr, &m_transformation_buffer));
}

void OurTestScene::UpdateTransformationBuffer(
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_transformation_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TransformationBuffer* matrixBuffer = (TransformationBuffer*)resource.pData;
	matrixBuffer->ModelToWorldMatrix = ModelToWorldMatrix;
	matrixBuffer->WorldToViewMatrix = WorldToViewMatrix;
	matrixBuffer->ProjectionMatrix = ProjectionMatrix;
	m_dxdevice_context->Unmap(m_transformation_buffer, 0);
}

// Point light buffer init and update
void OurTestScene::InitPointLightBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC point_light_buffer_desc = { 0 };
	point_light_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	point_light_buffer_desc.ByteWidth = sizeof(PointLightBuffer);
	point_light_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	point_light_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	point_light_buffer_desc.MiscFlags = 0;
	point_light_buffer_desc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&point_light_buffer_desc, nullptr, &m_point_light_buffer));
}

void OurTestScene::UpdatePointLightBuffer(
	vec4f light_world_pos_vector, 
	vec4f cam_world_pos_vector)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_point_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	PointLightBuffer* point_light_buffer = (PointLightBuffer*)resource.pData;
	point_light_buffer->LightPos = light_world_pos_vector;
	point_light_buffer->CameraPos = cam_world_pos_vector;
	m_dxdevice_context->Unmap(m_point_light_buffer, 0);
}