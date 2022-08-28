#pragma once
#include "VAOUtil.h"

//Abstract interface for generating VBO based vertex data
struct IGeometryMeshIndexed:public IGeometryMesh
{
	//Populates indices data buffer based on the input 
	//parameter vaoutl stores IDX data in the corresponding buffer 
	virtual int GenerateIndicesData(VAOUtil& vaoutl) = 0;
	virtual int GenerateVerticesDataIndexed(int att, VAOUtil& vaoutl) = 0;
};
