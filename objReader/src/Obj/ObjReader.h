#pragma once
#include <string>

using namespace std;

class ObjReader
{
public:
	ObjReader(void);
	~ObjReader(void);
	typedef struct{
		float x,y,z;
	}V, VN, VT;

	typedef struct{
		int x[3][3];
	}F;

	typedef struct{
		int nOfVertex;
		V *vertex;
		VN *vnormal;
		VT *vt;
		int nOfF;
		F *f;
	}OBJECT;
	OBJECT obj;
	void readObjFile(string fileName);
	BOOL isRead;
	void deleteData();
};

