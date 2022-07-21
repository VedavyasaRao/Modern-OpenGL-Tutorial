#pragma once
#include "VAOUtil.h"

//Abstract interface for generating VBO based vertex data
struct IGeometryMesh
{
	//Populates vertex data buffer based on the input 
	//Parameter identifies if if indexed draw or not
	//parameter att contains bitwise Ored type of vertex data. e.g, POS|CLR
	//parameter vaoutl stores generated data in the corresponding buffer 
	//e.g., POS data is stored in positions
	virtual int GenerateVerticesData(bool isindexed, int att, VAOUtil& vaoutl) = 0;
	//Populates indices data buffer based on the input 
	//parameter vaoutl stores IDX data in the corresponding buffer 
	virtual int GenerateIndicesData(VAOUtil& vaoutl) = 0;
};
