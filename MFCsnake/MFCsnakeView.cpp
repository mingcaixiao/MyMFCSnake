
// MFCsnakeView.cpp: CMFCsnakeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCsnake.h"
#endif

#include "MFCsnakeDoc.h"
#include "MFCsnakeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCsnakeView

IMPLEMENT_DYNCREATE(CMFCsnakeView, CView)

BEGIN_MESSAGE_MAP(CMFCsnakeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCsnakeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_START, &CMFCsnakeView::OnStart)
	ON_COMMAND(ID_CONTINUE, &CMFCsnakeView::OnContinue)
	ON_COMMAND(ID_PAUSE, &CMFCsnakeView::OnPause)
	ON_COMMAND(ID_EXIT, &CMFCsnakeView::OnExit)
	ON_COMMAND(ID_HELP, &CMFCsnakeView::OnHelp)
	ON_COMMAND(ID_about, &CMFCsnakeView::OnAbout)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMFCsnakeView 构造/析构

CMFCsnakeView::CMFCsnakeView() noexcept
{
	// TODO: 在此处添加构造代码
	//初始化游戏区域
	rect.left =width;
	rect.top = width;
	rect.right = 20*width;
	rect.bottom =20*width;
	cWidth = rect.right - rect.left;
	cHeight = rect.bottom - rect.top;
	//初始化蛇
	pHead = (Snake*)malloc(sizeof(Snake));
	p1 = (Snake*)malloc(sizeof(Snake));
	p2 = (Snake*)malloc(sizeof(Snake));
	pHead->x = 3 * width; pHead->y = width;
	p1->x = 2 * width; p1->y = width;
	p2->x = width; p2->y = width;
	pHead->next = p1; p1->next = p2; p2->next = NULL;
	dir = RIGHT;

 
	//分数
	score=0;

	flag = 0;

	IsNew = 0;
}

CMFCsnakeView::~CMFCsnakeView()
{

}

BOOL CMFCsnakeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CMFCsnakeView 绘图

void CMFCsnakeView::OnDraw(CDC* pDC)
{
	CMFCsnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CDC MemDC;//显示设备对象
	CBitmap MemBitmap;//位图对象

	//获取客户区高度和宽度
	CRect clientRect;
	GetClientRect(&clientRect);
	nWidth = clientRect.right - clientRect.left;
	nHeight = clientRect.bottom - clientRect.top;

	//建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(NULL);
	//建立与屏幕显示兼容的位图
	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//将位图选入内存设备
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);

	//用背景色将位图清楚干净
	//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(0,0,255));

	//在内存中绘图
	
	
	CBrush NewBrush(RGB(100, 100, 100)), NewBrush1(RGB(34,139,34));
	
	MemDC.Rectangle(0,0,nWidth,nHeight);
	//打印游戏区域
	CBrush *pOldBrush;
	pOldBrush = MemDC.SelectObject(&NewBrush1);
	MemDC.Rectangle(rect);
	NewBrush1.DeleteObject();

	//打印蛇
	pOldBrush = MemDC.SelectObject(&NewBrush);
	for (Snake* p = pHead; p != NULL; p = p->next)
	{
		MemDC.Rectangle(p->x, p->y, p->x + width, p->y + width);
	}
	//打印食物
	if (IsNew == 1)
		MemDC.Rectangle(food.x, food.y, food.x + width, food.y + width);
	NewBrush.DeleteObject();


	//打印分数
	CString str;
	str.Format(_T("分数为%d"), score);
	MemDC.TextOutW(rect.right + 30, cWidth / 2,str);
	//更改状态
	IsChange = 1;
	

	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);
	// 绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

}


// CMFCsnakeView 打印


void CMFCsnakeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCsnakeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCsnakeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCsnakeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCsnakeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCsnakeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCsnakeView 诊断

#ifdef _DEBUG
void CMFCsnakeView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCsnakeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCsnakeDoc* CMFCsnakeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCsnakeDoc)));
	return (CMFCsnakeDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCsnakeView 消息处理程序


void CMFCsnakeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
	case VK_DOWN:
		if (dir!=UP&&IsChange==1&&Deadflag==0)//方向不能相反并且在一个周期内只能变化一次,并且没有结束（蛇死亡）
		    dir= DOWN;
		break;
	case VK_UP:
		if (dir!= DOWN&&IsChange == 1&& Deadflag == 0)
			dir = UP;
		break;
	case VK_LEFT:
		if (dir!=RIGHT && IsChange == 1 && Deadflag == 0)
			dir= LEFT;
		break;
	case VK_RIGHT:
		if (dir!=LEFT && IsChange == 1 && Deadflag == 0)
			dir= RIGHT;
		break;
	case VK_SPACE:
		if (flag == 0)
			OnPause();
		else
			OnContinue();
		break;
	}
	IsChange = 0;
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCsnakeView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pNew = (Snake *)malloc(sizeof(Snake));

	switch (dir)
	{
	case RIGHT:
		pNew->x = pHead->x + width;
		pNew->y = pHead->y;
		break;
	case LEFT:
		pNew->x = pHead->x - width;
		pNew->y = pHead->y;
		break;
	case UP:
		pNew->x = pHead->x;
		pNew->y = pHead->y - width;
		break;
	case DOWN:
		pNew->x = pHead->x;
		pNew->y = pHead->y + width;
		break;
	}
	if (Wall()||EatSelf())
	{
		KillTimer(1);
		Deadflag = 1;
	}
	else
	{
		//插入头节点
		Snake *p = pHead;
		pHead = pNew;
		pHead->next = p;
		if (EatFood())
		{
			score += 8;
			//产生食物
			CreateFood();
		}
		else
		{
			//找到并且删除尾节点
			p = pHead;
			while (p->next->next != NULL)
			{
				p = p->next;
			}
			p->next = NULL;
		}
		//重绘
		Invalidate();
		UpdateWindow();
	}
}


void CMFCsnakeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDown(nFlags, point);
}


void CMFCsnakeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnRButtonDown(nFlags, point);
}


void CMFCsnakeView::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	SetTimer(1,100,NULL);
	initial();
	srand((unsigned int)time(NULL));
	IsNew = 1;
	Deadflag = 0;
	Invalidate();
}


void CMFCsnakeView::OnContinue()
{
	// TODO: 在此添加命令处理程序代码
	SetTimer(1,100,NULL);
	flag = 0;
}


void CMFCsnakeView::OnPause()
{
	// TODO: 在此添加命令处理程序代码
	KillTimer(1);
	flag = 1;
}


void CMFCsnakeView::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("退出游戏"));
	FreeMemory();
	exit(0);
}


bool CMFCsnakeView::Wall()
{
	// TODO: 在此处添加实现代码.
	if ((pNew->x==rect.right)|| pNew->x ==rect.left-width|| pNew->y ==rect.bottom||pNew->y==rect.top-width)
		return true;
	else
		return false;
}


bool CMFCsnakeView::EatFood()
{
	// TODO: 在此处添加实现代码.
	if (pNew->x == food.x&&pNew->y == food.y)
	{
		return true;
	}
	else
		return false;
}


void CMFCsnakeView::CreateFood()
{
	// TODO: 在此处添加实现代码.
	int x= rand() % (cWidth - width)+rect.left;
	int y= rand() % (cHeight - width)+rect.top;
	food.x = x - x % width;
	food.y = y - y % width;
	// 食物生成在蛇的身体上
	for (Snake *p=pHead->next; p != NULL; p = p->next)
	{
		if (p->x==food.x&&p->y==food.y)
		{
			CreateFood();//递归
		}
	}
}


bool CMFCsnakeView::EatSelf()
{
	// TODO: 在此处添加实现代码.
	Snake*p = pHead->next;
	while (p->next != NULL)
	{
		if (p->x == pNew->x&&p->y == pNew->y)
			return true;
		p= p->next;
	}
	return false;
}


void CMFCsnakeView::initial()
{
	// TODO: 在此处添加实现代码.
	//初始化蛇
	pHead->x = 3 * width; pHead->y = width;
	p1->x = 2 * width; p1->y = width;
	p2->x = width; p2->y = width;
	pHead->next = p1; p1->next = p2; p2->next = NULL;
	dir = RIGHT;
	CreateFood();

	score = 0;

	flag = 0;
}


void CMFCsnakeView::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("空格可以暂停和继续"));
}


void CMFCsnakeView::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("作者：肖名财"));
}

void CMFCsnakeView::FreeMemory()
{
	// TODO: 在此处添加实现代码.
	//free(pNew);
	Snake *p = pHead, *q = p->next;
	while (q!= NULL)
	{
		free(p);
		p = q;
		q = p->next;
	}
	free(q);
}


BOOL CMFCsnakeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return true;
	return CView::OnEraseBkgnd(pDC);
}
