#include "Canvas\Scene\Base\BaseScene.h"
#include "Canvas\Camera\OrbitCamera.h"
#include "Geometry\Objects\TextImage\TextImageSketcher.h"
#include "Geometry\Objects\Cube\TexturedCube.h"
#include "Geometry\Utils\Generic\GenericParser.h"
#include "Geometry\Objects\Generic\GenericObj.h"



class Scene:public BaseScene
{
public:
	//message handler
	BEGIN_MSG_MAP(Scene0)
		MESSAGE_HANDLER(WM_CLOSE, OnCloseWindow)
		CHAIN_MSG_MAP(BaseScene)
	END_MSG_MAP()
	enum {sun,mercury,venus,earth,moon,mars,satrun,jupiter,uranus,neptune};

	void populate()
	{
		interstellarsky.Init(TextureInfo(R"(..\resources\Models\solar system\stars.bmp)"));
		interstellarsky.MM.Scaleby = glm::vec3(10.0f, 10.0f, 10.0f);

		bodies[sun].init(R"(..\resources\Models\solar system\sun.obj)", vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, -3.0f),0.0f, 0.0f);
		bodies[mercury].init(R"(..\resources\Models\solar system\mercury.obj)", vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.0f, -3.0f), -0.8f, 30.0f);
		bodies[venus].init(R"(..\resources\Models\solar system\venus.obj)", vec3(0.3f, 0.3f, 0.3f), vec3(0.0f, 0.0f, -3.0f), -1.2f, 60.0f);
		bodies[earth].init(R"(..\resources\Models\solar system\earth.obj)", vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -3.0f),-1.8f,110.0f);
		bodies[moon].init(R"(..\resources\Models\solar system\moon.obj)", vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.5f, -3.0f), -1.8f,110.0f);
		bodies[mars].init(R"(..\resources\Models\solar system\mars.obj)", vec3(0.35f, 0.35f, 0.35f), vec3(0.0f, 0.0f, -3.0f), -2.45f,150.0f);
		bodies[jupiter].init(R"(..\resources\Models\solar system\jupiter.obj)", vec3(0.8f, 0.8f, 0.8f), vec3(0.0f, 0.0f, -3.0f), -3.0f,190.0f);
		bodies[satrun].init(R"(..\resources\Models\solar system\satrun.obj)", vec3(0.2f, 0.2f, 0.2f), vec3(0.0f, 0.0f, -3.0f),-3.4f,220.0f);
		bodies[uranus].init(R"(..\resources\Models\solar system\uranus.obj)", vec3(0.6f, 0.6f, 0.6f), vec3(0.0f, 0.0f, -3.0f), -3.8f, 240.0f);
		bodies[neptune].init(R"(..\resources\Models\solar system\neptune.obj)", vec3(0.6f, 0.6f, 0.6f), vec3(0.0f, 0.0f, -3.0f), -4.2f, 260.0f);

		ZeroMemory(&lf, sizeof lf);
		lf.lfHeight = -27;
		lf.lfWeight = 400;
		lf.lfClipPrecision = 2;
		lf.lfOutPrecision = 3;
		lf.lfQuality = 1;
		lf.lfPitchAndFamily = 34;
		wcscpy_s(lf.lfFaceName, 32, L"Segoe UI Emoji");
		hf = CreateFontIndirectW(&lf);

		pbrush = new SolidBrush(0xffff00ff);
		textutl.Init(TextureInfo(), 256, 256);
		textutl.ClearCanvas(false);
		textutl.Drawtext({ 0,0 }, text, hf, &fmt, pbrush);
		textutl.FlipYAxis();
		textutl.DrawCanvas();
		textutl.MM.Translateby = glm::vec3(-4.8f, 2.3f, -3.0f);
	}


	//override
	int Init(RECT rect, WCHAR *windowname)
	{
		//create host window and context
		BaseScene::Init(rect, windowname);

		//attach mouse keyboard input handler
		camera = new OrbitCamera(m_hWnd);

		SceneCamera()->updateWH();
		SceneCamera()->CenterCursor();

		SceneCamera()->PPM.setFOV(45.0f);
		SceneCamera()->PPM.setProjectionMatrix(0.1f, 100.0f);
		
		populate();
		CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)MyTimerCallback, (PVOID)this, 500, 500, 0);

		return 0;
	}

	static VOID CALLBACK MyTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		auto pscene = (Scene*)lpParameter;
		for (short i : {sun, mercury, venus, earth, moon, mars, jupiter, satrun, uranus, neptune})
			pscene->bodies[i].updateangleposition();

		pscene->Invalidate();

	}

	//release resources
	void Cleanup()
	{
		interstellarsky.Cleanup();
		for (short i : {sun, mercury, venus, earth, moon, mars, jupiter, satrun, uranus, neptune})
			bodies[i].cleanup();
		textutl.Cleanup();
		delete camera;
	}
	
	//draw the scene
	void DrawScene()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SceneCamera()->updateViewMatrix();

		for (short i : {sun, mercury, venus, earth, moon, mars, jupiter, satrun, uranus, neptune})
			updateanddraw(SceneCamera(), bodies[i].body);

		SceneCamera()->MM.Reset();

		updateanddraw(SceneCamera(), interstellarsky);
		updateanddraw(SceneCamera(), textutl);
	}

	//Close the window
	LRESULT OnCloseWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	}

	inline OrbitCamera*  SceneCamera()
	{
		static auto ret = dynamic_cast<OrbitCamera*>(camera);
		return ret;
	}

	void updateanddraw(OrbitCamera* camera, BaseGeometry& geo)
	{
		camera->augumentModelMatrix(geo);
		camera->setViewMatrix(geo);
		camera->setPerspectiveProjectionMatrix(geo);
		geo.Draw();
	}

	class Celestial
	{
	public:
		void init(const string& texturefile, vec3 scale, vec3 position, float radius, float startangle)
		{
			Scene::aiobjparser.clear();
			Scene::aiobjparser.Parse(texturefile);
			body.Init(0, LightSrcInfo{}, &aiobjparser);
			this->scale = scale;
			this->position = position;
			this->radius = radius;
			this->startangle = startangle;
			body.MM.Scaleby = scale;
		}

		void updateangleposition()
		{
			startangle += 10.0f;
			if (startangle == 360.0f)
				startangle = 0.0f;
			body.MM.Yaw = (short)startangle;
			auto x = position.x + radius * cos(radians(startangle));
			auto y = position.y + radius * sin(radians(startangle));
			body.MM.Translateby = glm::vec3(x, y, -3.0f);
		}

		void cleanup()
		{
			body.Cleanup();
		}

	private:
		string texture;
		vec3 scale;
		vec3 position;
		float radius;
		float startangle;
		GenericObj  body;

		friend class Scene;
	};

private:
	TexturedCube interstellarsky;
	Celestial   bodies[10];

	static AssertImpParser aiobjparser;
	TextImageSketcher textutl;

	wstring text = L"Press ESC to close";
	HFONT	hf;
	LOGFONT lf;
	StringFormat fmt;
	SolidBrush *pbrush;

	HANDLE hTimer = NULL;
};

AssertImpParser Scene::aiobjparser;
