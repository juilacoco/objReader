// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ObjView.cpp : implementation of the CObjView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Obj.h"
#endif

#include "ObjDoc.h"
#include "ObjView.h"
#include <string>
#include "EditLighting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CObjView

IMPLEMENT_DYNCREATE(CObjView, CView)

BEGIN_MESSAGE_MAP(CObjView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CObjView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_FILE_OPEN, &CObjView::OnFileOpen)
	ON_COMMAND(ID_BUTTON2, &CObjView::OnButton2)
	ON_COMMAND(ID_BUTTON4, &CObjView::OnButton4)
	ON_COMMAND(ID_BUTTON5, &CObjView::OnButton5)
	ON_COMMAND(ID_BUTTON7, &CObjView::OnButton7)
	ON_COMMAND(ID_BUTTON8, &CObjView::OnButton8)
END_MESSAGE_MAP()

// CObjView construction/destruction

CObjView::CObjView()
{
	// TODO: add construction code here
	angleX = 0;
	angleY = 0;
	angleFlagX = 0;
	angleFlagY = 0;
	disX = 0;
	disY = 0;
	disFlagX = 0;
	disFlagY = 0;
	eyeZ = 0;
	btn = 0;
	viewMode = 1;
	modelList = 0;
	light_position[0] = 5.0;
	light_position[1] = 5.0;
	light_position[2] = 2.0;
	light_position[3] = 0.0;
}

CObjView::~CObjView()
{
}

BOOL CObjView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

// CObjView drawing

void CObjView::OnDraw(CDC* /*pDC*/)
{
	CObjDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	// Clear out the color & depth buffers
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	RenderScene();
	// Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
	::SwapBuffers( m_pDC->GetSafeHdc() );
}


// CObjView printing


void CObjView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CObjView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CObjView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CObjView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CObjView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CObjView diagnostics

#ifdef _DEBUG
void CObjView::AssertValid() const
{
	CView::AssertValid();
}

void CObjView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CObjDoc* CObjView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CObjDoc)));
	return (CObjDoc*)m_pDocument;
}
#endif //_DEBUG


// CObjView message handlers


int CObjView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//Initialize OpenGL Here
	InitializeOpenGL();
	return 0;
}


void CObjView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//Make the RC non-current
	if(::wglMakeCurrent (0,0) == FALSE)
	{
// 		string str = "Could not make RC non-current";
// 		LPCTSTR lp = str.c_str();
// 		MessageBox(lp);
	}

	//Delete the rendering context
	if(::wglDeleteContext (m_hRC)==FALSE)
	{
// 		string str = "Could not delete RC";
// 		LPCTSTR lp = str.c_str();
// 		MessageBoxW(lp);
	}
	//Delete the DC
	if(m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;
}


void CObjView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	GLdouble aspect_ratio; // width/height ratio

	if ( 0 >= cx || 0 >= cy )
	{
		return;
	}
	// select the full client area
	::glViewport(0, 0, cx, cy);
	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;
	// select the projection matrix and clear it
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	// select the viewing volume
	::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);

	// switch back to the modelview matrix and clear it
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
}


BOOL CObjView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

BOOL CObjView::InitializeOpenGL()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if(m_pDC == NULL)
	{
// 		string str = "Error Obtaining DC";
// 		LPCTSTR lp = str.c_str();
// 		MessageBox(lp);
		return FALSE;
	}
	//Failure to set the pixel format
	if(!SetupPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
	//Failure to Create Rendering Context
	if(m_hRC == 0)
	{
// 		string str = "Error Creating RC";
// 		LPCTSTR lp = str.c_str();
// 		MessageBox(lp);
		return FALSE;
	}
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
	{
// 		string str = "Error making RC Current";
// 		LPCTSTR lp = str.c_str();
// 		MessageBox(lp);
		return FALSE;
	}
	//Specify Black as the clear color
	::glClearColor(0.0f,0.0f,0.0f,0.0f);
	//Specify the back of the buffer as clear depth
	::glClearDepth(1.0f);

	return TRUE;
}

BOOL CObjView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if ( m_nPixelFormat == 0 )
	{
		return FALSE;
	}
	if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

void CObjView::RenderScene()
{
	//设置光照
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	//设置观察点
	glLoadIdentity();
	gluLookAt(
		0, 0, 10 + eyeZ,
		0, 0, 0,
		0, 1, 0);
	glColor3f(1,0,0);
	//设置旋转平移
	glPushMatrix();
	glTranslatef(disX + disFlagX, -(disY + disFlagY), 0);
	glRotatef(angleX + angleFlagX, 1.0, 0, 0);
	glRotatef(angleY + angleFlagY, 0, 1.0, 0);
	
	//曲面模式
	if(viewMode == 1)
		glCallList(modelList);
	else{
		for(int i = 0; i < object1->numtriangles; ++i)
		{
			int v, vn, vt;
			if(viewMode == 2)    //网格模式
				glBegin(GL_LINE_LOOP);
			else if(viewMode == 3)    //顶点模式
			{	
				glPointSize(2.0);
				glBegin(GL_POINTS);
			}
			GLMtriangle triangle = object1->triangles[i];
			for(int j = 0; j < 3; ++j){
				float x, y, z;
				int key = triangle.vindices[j];
				x = object1->vertices[3*key];
				y = object1->vertices[3*key + 1];
				z = object1->vertices[3*key + 2];
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
	glPopMatrix();
}

void CObjView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar){
	case VK_UP:
		eyeZ -= 0.2;
		InvalidateRect(NULL, FALSE);
		break;
	case VK_DOWN:
		eyeZ += 0.2;
		InvalidateRect(NULL, FALSE);
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CObjView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	angleFlagY += angleY;
	angleFlagX += angleX;
	angleX = 0;
	angleY = 0;
	btn = 0;
	CView::OnLButtonUp(nFlags, point);
}

int beginX, beginY;
void CObjView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	btn = 1;
	beginX = point.x;
	beginY = point.y;

	CView::OnLButtonDown(nFlags, point);
}


void CObjView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(btn == 1){
		angleY = (point.x - beginX);
		angleX = (point.y - beginY);
		InvalidateRect(NULL, FALSE);
	}
	else if(btn == 2){
		disY = (point.y - beginY)/50.0;
		disX = (point.x - beginX)/50.0;
		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}


void CObjView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	btn = 2;
	beginX = point.x;
	beginY = point.y;
	CView::OnRButtonDown(nFlags, point);
}

void CObjView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	disFlagY += disY;
	disFlagX += disX;
	disY = 0;
	disX = 0;
	btn = 0;
// 	ClientToScreen(&point);
// 	OnContextMenu(this, point);
}

void CObjView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog  FileDlg(TRUE);
	FileDlg.m_ofn.lpstrFilter="obj File(*.obj)\0*.obj\0"; 
	FileDlg.m_ofn.lpstrDefExt="txt";  

	if(FileDlg.DoModal() == IDOK)
	{ 
 		CString name = FileDlg.GetPathName();
		name.Replace("\\", "/");
		string fileName = name;
		
		GLfloat scalefactor = 0.0;
		//Load Computer from file
		const int l = strlen(fileName.c_str());
		char *path = new char[l];
		strcpy(path, fileName.c_str());
		object1 = glmReadOBJ(path);
		/* build a display list */
		modelList = glmList(object1, GLM_SMOOTH);
		/* nuke it, we don't need it anymore */
		//glmDelete(object1);    
	}
	InvalidateRect(NULL, FALSE);
}

//曲面模式
void CObjView::OnButton2()
{
	// TODO: Add your command handler code here
	viewMode = 1;
	InvalidateRect(NULL, FALSE);
}

//顶点模式
void CObjView::OnButton4()
{
	// TODO: Add your command handler code here
	viewMode = 3;
	InvalidateRect(NULL, FALSE);
}

//网格模式
void CObjView::OnButton5()
{
	// TODO: Add your command handler code here
	viewMode = 2;
	InvalidateRect(NULL, FALSE);
	
}

//更改光源点
void CObjView::OnButton7()
{
	// TODO: Add your command handler code here
	EditLighting ed;
	ed.DoModal();

	for(int i = 0; i < 3; ++i){
		float num = atof((LPSTR)(LPCTSTR)ed.str[i]);
		light_position[i] = num;
	}
	InvalidateRect(NULL, FALSE);
}


void CObjView::OnButton8()
{
	// TODO: Add your command handler code here
	viewMode = 1;
	InvalidateRect(NULL, FALSE);
}
