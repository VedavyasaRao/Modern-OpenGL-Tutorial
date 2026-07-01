#pragma once
#define WGL_WGLEXT_PROTOTYPES
#pragma include_alias( "GLad/GL.h", "..\..\externals\GLExtns\GLad\GL.h" )
#pragma include_alias( "GLad/wgl.h", "..\..\externals\GLExtns\GLad\wgl.h" )
#pragma include_alias( "stb_image.h", "..\..\externals\stb\stb_image.h" )
#pragma include_alias( "glm/glm.hpp", "..\..\externals\glm\glm\glm.hpp" )
#pragma include_alias( "glm/gtc/matrix_transform.hpp", "..\..\externals\glm\glm\gtc\matrix_transform.hpp" )
#pragma include_alias( "glm/gtx/transform.hpp", "..\..\externals\glm\glm\gtx\transform.hpp" )
#pragma include_alias( "glm/gtc/type_ptr.hpp", "..\..\externals\glm\glm\gtc\type_ptr.hpp" )
#pragma include_alias( "glm/ext/matrix_projection.hpp", "..\..\externals\glm\glm\ext\matrix_projection.hpp" )


#ifdef _DEBUG 
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "..\\..\\externals\\assimp\\lib\\assimp-vc143-mtd.lib")
#pragma comment(lib, "..\\..\\externals\\assimp\\lib\\zlibstaticd.lib")
#else
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "..\\..\\externals\\assimp\\lib\\assimp-vc143-mtd.lib")
#pragma comment(lib, "..\\..\\externals\\assimp\lib\\zlibstaticd.lib")
#endif

