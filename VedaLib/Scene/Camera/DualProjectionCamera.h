#pragma once
#include "PerspectiveProjectionCamera.h"

/////////////////////OrthographicProjectionCamera///////////////////////////////////
class DualProjectionCamera :public PerspectiveProjectionCamera
{
public:
	DualProjectionCamera(HWND hwnd) :PerspectiveProjectionCamera(hwnd) {}

	void setOrthographicProjectionMatrix(BaseGeometry& geo)
	{
		geo.ProjectionMatrix = OPM.getProjectionMatrix();
	}

protected:

public:
	OrthographicProjectionMatrixData OPM;
};
/////////////////////OrthographicProjectionCamera///////////////////////////////////

