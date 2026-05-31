#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\FPSCamera.h"
#include "Geometry\Objects\Quad\Quad.h"

class Scene:public BaseScene
{
public:
	//message handler
	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()

	//override
	int Init(RECT rect, WCHAR *windowname)
	{
		//create host window and context
		BaseScene::Init(rect, windowname);
		//attach mouse keyboard input handler
		camera = new FPSCamera(m_hWnd);

		SceneCamera()->updateWH();
		SceneCamera()->CenterCursor();
		SceneCamera()->PPM.setFOV(45.0f);
		SceneCamera()->PPM.setProjectionMatrix(1.0f, 1000.0f);
		SceneCamera()->setViewMatrix2({ 26.0f, 6.0f, 6.0f }, { 0.0f, 0.0f, -0.5f }, { 0.0f, 1.0f, 0.0f });
		SceneCamera()->setSenseivity(0.50f, 0.01f);
		
		generateFloor();
		floor.Init(1, R"(..\resources\textures\blocks3.bmp)");
		floor.updateTextureMap(6.0f);
		floor.updateVertices(vertices);
		floor.GenerateVertices();


		generateRoof();
		roof.Init(2, R"(..\resources\textures\plain.bmp)");
		roof.updateTextureMap(2.0f);
		roof.updateVertices(vertices);
		roof.GenerateVertices();

		generatePhoto();
		photo.Init(3, R"(..\resources\textures\Khri$ha.jpg)");
		photo.updateTextureMap(1.0f);
		photo.updateVertices(vertices);
		photo.GenerateVertices();

		generateOuterWall();
		wall.Init(4, R"(..\resources\textures\plain2.bmp)");
		wall.updateTextureMap(1.0f);
		wall.updateVertices(vertices);
		wall.GenerateVertices();

		generateWoodBoxes();
		boxes.Init(5, R"(..\resources\textures\rocks2.bmp)");
		boxes.updateTextureMap(1.0f);
		boxes.updateVertices(vertices);
		boxes.GenerateVertices();
		return 0;
	}

	//release resources
	void Cleanup()
	{
		floor.Cleanup();
		roof.Cleanup();
		photo.Cleanup();
		wall.Cleanup();
		boxes.Cleanup();
		delete camera;
	}
	
	//draw the scene
	void DrawScene()
	{
		glClearColor(0.0f, 0.0f, 255.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SceneCamera()->updateViewMatrix();
		SceneCamera()->augumentModelMatrix(floor);
		SceneCamera()->setViewMatrix(floor);
		SceneCamera()->setPerspectiveProjectionMatrix(floor);
		floor.Draw();

		SceneCamera()->augumentModelMatrix(roof);
		SceneCamera()->setViewMatrix(roof);
		SceneCamera()->setPerspectiveProjectionMatrix(roof);
		roof.Draw();

		SceneCamera()->augumentModelMatrix(photo);
		SceneCamera()->setViewMatrix(photo);
		SceneCamera()->setPerspectiveProjectionMatrix(photo);
		photo.Draw();

		SceneCamera()->augumentModelMatrix(wall);
		SceneCamera()->setViewMatrix(wall);
		SceneCamera()->setPerspectiveProjectionMatrix(wall);
		wall.Draw();

		SceneCamera()->augumentModelMatrix(boxes);
		SceneCamera()->setViewMatrix(boxes);
		SceneCamera()->setPerspectiveProjectionMatrix(boxes);
		boxes.Draw();
	}

	//Close the window
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

private:
	void generateFloor()
	{
		vertices.clear();
		for (float a = -10.0f; a <= 170.0f; a += 10.0f)
		{
			vertices.push_back({ 0.0 + a, 0.0, 10.0 });
			vertices.push_back({ 0.0 + a, 0.0, 0.0 });
			vertices.push_back({ 10.0 + a, 0.0, 0.0 });
			vertices.push_back({ 10.0 + a, 0.0, 10.0 });
		}

		for (float b = -10.0f; b <= 320.0f; b += 10.0f)
		{
			for (float a = -10.0f; a <= 170.0f; a += 10.0f)
			{
				vertices.push_back({ 0.0 + a, 0.0, -(b - 10.0) });
				vertices.push_back({ 0.0 + a, 0.0, -b });
				vertices.push_back({ 10.0 + a, 0.0, -b });
				vertices.push_back({ 10.0 + a, 0.0, -(b - 10.0) });
			}
		}

	}

	void generateRoof()
	{
		vertices.clear();
		for (float a = -10.0f; a <= 170.0f; a += 10.0f)
		{
			vertices.push_back({ 0.0 + a, 50.0, 20.0 });
			vertices.push_back({ 0.0 + a, 50.0, 0.0 });
			vertices.push_back({ 10.0 + a, 50.0, 0.0 });
			vertices.push_back({ 10.0 + a, 50.0, 20.0 });
		}

		for (float b = -10.0f; b <= 320.0f; b += 10.0f)
		{
			for (float a = -10.0f; a <= 170.0f; a += 10.0f)
			{

				vertices.push_back({ 0.0 + a, 50.0, -(b - 10.0) });
				vertices.push_back({ 0.0 + a, 50.0, -b });
				vertices.push_back({ 10.0 + a, 50.0, -b });
				vertices.push_back({ 10.0 + a, 50.0, -(b - 10.0) });
			}
		}
	}

	void generatePhoto()
	{
		vertices.clear();
		vertices.push_back({ 179.8, 30.0, -80.0 });
		vertices.push_back({ 179.8, 30.0, -64.0 });
		vertices.push_back({ 179.8, 10.0, -64.0 });
		vertices.push_back({ 179.8, 10.0, -80.0 });

		vertices.push_back({ -9.9, 30.0, -64.0 });
		vertices.push_back({ -9.9, 30.0, -80.0 });
		vertices.push_back({ -9.9, 10.0, -80.0 });
		vertices.push_back({ -9.9, 10.0, -64.0 });

		vertices.push_back({ 80.0 , 30.0, -304.0 });
		vertices.push_back({ 50.0 , 30.0, -320.0 });
		vertices.push_back({ 50.0 , 10.0, -304.0 });
		vertices.push_back({ 80.0 , 10.0, -320.0 });


		vertices.push_back({ 80.0 , 30.0, 14.0 });
		vertices.push_back({ 50.0 , 30.0, 19.8 });
		vertices.push_back({ 50.0 , 10.0, 19.8 });
		vertices.push_back({ 80.0 , 10.0, 14.0 });


	}

	void generateOuterWall()
	{
		vertices.clear();
		//glColor3f(0.7, 0.7, 0.5);

		//wall 1
		for (float a = 0.0f; a <= 180.0f; a += 10.0f)
		{
			vertices.push_back({ -10.0 + a, 0.0, 20.0 });
			vertices.push_back({ -10.0 + a, 50.0, 20.0 });
			vertices.push_back({ 0.0 + a, 50.0, 20.0 });
			vertices.push_back({ 0.0 + a, 0.0, 20.0 });
		}

		//wall 2
		for (float a = 0.0f; a <= 320.0f; a = a + 20.0f)
		{
			vertices.push_back({ 180.0, 0.0, 0.0 - a });
			vertices.push_back({ 180.0, 50.0, 0.0 - a });
			vertices.push_back({ 180.0, 50.0, 20.0 - a });
			vertices.push_back({ 180.0, 0.0, 20.0 - a });
		}

		//wall 3
		for (float a = 0.0f; a <= 180.0f; a += 10.0f)
		{
			vertices.push_back({ 180.0 - a, 0.0, -320.0 });
			vertices.push_back({ 180.0 - a, 50.0, -320.0 });
			vertices.push_back({ 170.0 - a, 50.0, -320.0 });
			vertices.push_back({ 170.0 - a, 0.0, -320.0 });
		}


		//wall 4
		for (float a = -10.0f; a <= 320.0f; a += 10.0f)
		{
			vertices.push_back({ -10.0, 0.0, 0.0 - a });
			vertices.push_back({ -10.0, 50.0, 0.0 - a });
			vertices.push_back({ -10.0, 50.0, 10.0 - a });
			vertices.push_back({ -10.0, 0.0, 10.0 - a });
		}
	}

	void generateWoodBoxes()
	{
		vertices.clear();
		//glColor3f(0.7, 0.6, 0.4);
		//draw 3 wood boxes
		for (float a = 0.0f; a <= 60.0f; a += 30.0f)
		{
			vertices.push_back({ 10.0 + a, 9.0, -34.0 });
			vertices.push_back({ 20.0 + a, 9.0, -34.0 });
			vertices.push_back({ 20.0 + a, 0.1, -34.0 });
			vertices.push_back({ 10.0 + a, 0.1, -34.0 });

			vertices.push_back({ 10.0 + a, 9.0, -34.0 });
			vertices.push_back({ 20.0 + a, 9.0, -34.0 });
			vertices.push_back({ 20.0 + a, 9.0, -39.8 });
			vertices.push_back({ 10.0 + a, 9.0, -39.8 });

			vertices.push_back({ 10.0 + a, 9.0, -34.0 });
			vertices.push_back({ 10.0 + a, 9.0, -39.8 });
			vertices.push_back({ 10.0 + a, 0.1, -39.8 });
			vertices.push_back({ 10.0 + a, 0.1, -34.0 });

			vertices.push_back({ 20.0 + a, 9.0, -34.0 });
			vertices.push_back({ 20.0 + a, 9.0, -39.8 });
			vertices.push_back({ 20.0 + a, 0.1, -39.8 });
			vertices.push_back({ 20.0 + a, 0.1, -34.0 });
		}
	}

	inline FPSCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<FPSCamera*>(camera);
		return ret;
	}

private:
	Quad  floor;
	Quad  roof;
	Quad  photo;
	Quad  wall;
	Quad  boxes;
	vector<vec3> vertices;
};
