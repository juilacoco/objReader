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

// ObjView.h : interface of the CObjView class
//

#pragma once
#include "ObjReader.h"
#include "glm.h"

class CObjView : public CView
{
protected: // create from serialization only
	CObjView();
	DECLARE_DYNCREATE(CObjView)

// Attributes
public:
	CObjDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CObjView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	HGLRC m_hRC;    //Rendering Context
	CDC* m_pDC;        //Device Context
	BOOL InitializeOpenGL();    //Initialize OpenGL
	BOOL SetupPixelFormat();    //Set up the Pixel Format
	void RenderScene();            //Render the Scene

	float angleX, angleFlagX;
	float angleY, angleFlagY;
	double disX, disFlagX;
	double disY, disFlagY;
	double eyeZ;
	int btn;
	ObjReader objReader;
	int viewMode;
	GLuint modelList;
	GLMmodel *object1;
	GLfloat light_position[4];
	CString str;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnButton2();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
};

#ifndef _DEBUG  // debug version in ObjView.cpp
inline CObjDoc* CObjView::GetDocument() const
   { return reinterpret_cast<CObjDoc*>(m_pDocument); }
#endif

