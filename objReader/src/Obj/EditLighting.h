#pragma once


// EditLighting dialog

class EditLighting : public CDialogEx
{
	DECLARE_DYNAMIC(EditLighting)

public:
	EditLighting(CWnd* pParent = NULL);   // standard constructor
	virtual ~EditLighting();
	CString str[3];
// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
