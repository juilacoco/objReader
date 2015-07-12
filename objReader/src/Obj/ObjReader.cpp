#include "StdAfx.h"
#include "ObjReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

ObjReader::ObjReader(void)
{
	obj.nOfVertex = 0;
	obj.nOfF = 0;
	obj.vertex = new V[5000];
	obj.vnormal = new VN[5000];
	obj.vt = new VT[5000];
	obj.f = new F[10000];
	isRead = false;
}

ObjReader::~ObjReader(void)
{
}

void ObjReader::readObjFile( string fileName )
{
	ifstream objFile(fileName);
	string line;
	while(getline(objFile, line))
	{
		if(line.size() > 0)
		{
			string s1;
			float f2, f3, f4;
			if(line[0] == 'v')
			{
				//��¼������
				if(line[1] == 'n')
				{
					istringstream sin(line);
					sin >> s1 >> f2 >> f3 >> f4;
					//cout << "vector:" << f2 <<  " " << f3 << " " << f4 << endl;
					obj.vnormal[obj.nOfVertex].x = f2;
					obj.vnormal[obj.nOfVertex].y = f3;
					obj.vnormal[obj.nOfVertex].z = f4;
				}
				//��¼����
				else if(line[1] == 't')
				{
					istringstream sin(line);
					sin >> s1 >> f2 >> f3;
					//cout << "texture:" << f2 <<  " " << f3 << endl;
					obj.vt[obj.nOfVertex].x = f2;
					obj.vt[obj.nOfVertex].y = f3;
				}
				//��¼�ڵ�
				else
				{
					istringstream sin(line);
					sin >> s1 >> f2 >> f3 >> f4;
					//cout << "vertex:" << f2 <<  " " << f3 << " " << f4 << endl;
					obj.vertex[obj.nOfVertex].x = f2;
					obj.vertex[obj.nOfVertex].y = f3;
					obj.vertex[obj.nOfVertex].z = f4;
				}
				++obj.nOfVertex;
			}
			//��¼��
			else if(line[0] == 'f')
			{
				istringstream in(line);
				//ȥ��'f'
				in >> s1;
				for(int i = 0; i < 3; ++i)
				{
					in >> s1;
					//cout << s1 << endl;
					int key = 0, j = 0, a;
					//��ȡ��Ӧ�Ķ�������,�������������Ͷ��㷨������
					while(key != 3)
					{
						a = 0;
						while(j < s1.size() && s1[j] != '/')
						{
							a = 10*a + s1[j] - '0';
							j++;
						}
						//cout << a << " ";
						obj.f[obj.nOfF].x[i][key] = a;
						key++;
						j++;
					}
					//cout << endl;
				}
				obj.nOfF++;
			}
		}
	}
	isRead = true;
}

void ObjReader::deleteData()
{
	obj.nOfVertex = 0;
	obj.nOfF = 0;
	delete obj.vertex;
	delete obj.vnormal;
	delete obj.vt;
	delete obj.f;
	obj.vertex = new V[25000];
	obj.vnormal = new VN[25000];
	obj.vt = new VT[25000];
	obj.f = new F[50000];
	isRead = false;
}
