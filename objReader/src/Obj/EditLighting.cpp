// EditLighting.cpp : implementation file
//

#include "stdafx.h"
#include "Obj.h"
#include "EditLighting.h"
#include "afxdialogex.h"


// EditLighting dialog

IMPLEMENT_DYNAMIC(EditLighting, CDialogEx)

EditLighting::EditLighting(CWnd* pParent /*=NULL*/)
	: CDialogEx(EditLighting::IDD, pParent)
{

}

EditLighting::~EditLighting()
{
}

void EditLighting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,str[0]);
	DDX_Text(pDX,IDC_EDIT2,str[1]);
	DDX_Text(pDX,IDC_EDIT3,str[2]);
}


BEGIN_MESSAGE_MAP(EditLighting, CDialogEx)
END_MESSAGE_MAP()


// EditLighting message handlers
