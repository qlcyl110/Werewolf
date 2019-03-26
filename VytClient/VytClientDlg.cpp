
// VytClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "VytClient.h"
#include "VytClientDlg.h"
#include "afxdialogex.h"
#include "ClientPeer.h"
#include "MsgHandler.h"
#include "LoginDlg.h"

using namespace vyt;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVytClientDlg 对话框



CVytClientDlg::CVytClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VYTCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVytClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVytClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CVytClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVytClientDlg 消息处理程序

BOOL CVytClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	bool success = vyt::ClientPeer::Get().Connect(_T("127.0.0.1"), short(38564));
	int data = 3;
	CString username = _T("Vyterm");
	CString password = _T("123456");
	vytsize unSize = username.GetLength()*2, pwSize = password.GetLength()*2, intSize = sizeof(int), sizesize = sizeof(vytsize);
	vyt::ClientPeer::Get().Send({ 0, 0, { { &data, intSize } } });
	vyt::ClientPeer::Get().Send({ 1, 0, {
		{ &unSize, sizesize },
		{ username.GetBuffer(), unSize },
		{ &pwSize, sizesize },
		{ password.GetBuffer(), unSize },
		} });
	SetTimer(0, 10, nullptr);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVytClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVytClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVytClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	bool remainPacket = true;
	do
	{
		Packet packet = vyt::ClientPeer::Get().Recv();
		if (nullptr != packet)
			HandlePacket(this, packet);
		else
			remainPacket = false;
	} while (remainPacket);
}


void CVytClientDlg::OnBnClickedOk()
{
	ShowWindow(SW_HIDE);
	LoginDlg login(this);
	login.DoModal();
	ShowWindow(SW_SHOW);
}
