#pragma once
#include "..\VAO\VAOUtil.h"

//Abstract interface for generating VBO based vertex data
struct IGeometryMesh
{
	//Populates vertex data buffer based on the input 
	//parameter att contains bitwise Ored type of vertex data. e.g, POS|CLR
	//parameter vaoutl stores generated data in the corresponding buffer 
	//e.g., POS data is stored in positions
	virtual int GenerateVerticesData(int att, VAOUtil& vaoutl) = 0;
};
