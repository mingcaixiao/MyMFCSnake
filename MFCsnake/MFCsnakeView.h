
// MFCsnakeView.h: CMFCsnakeView 类的接口
//
#include<stdlib.h>
#pragma once
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define UP 3
//蛇身的每节宽度
#define width 30
//蛇身的一个节点
struct Snake
{
	// 左上顶点的x轴坐标
	int x;
	//左上顶点的y轴坐标
	int y;
	struct Snake *next;
};

class CMFCsnakeView : public CView
{
protected: // 仅从序列化创建
	CMFCsnakeView() noexcept;
	DECLARE_DYNCREATE(CMFCsnakeView)

// 特性
public:
	CMFCsnakeDoc* GetDocument() const;

// 操作
public:
	Snake *pHead;//蛇头指针
	Snake *p1, *p2;//  第二节和第三节
	CPoint food;//食物;
	int dir;//方向
	Snake *pNew;
	//游戏区域
	CRect rect;
	int cWidth;
	int cHeight;

	//分数
	int score;

	//游戏暂停结束标志
		int flag;

	//一个周期内是否变化的标志
		int IsChange=1;

	//新游戏的标志
		int IsNew;

    //客户区宽度，高度
		int nHeight;
		int nWidth;
	// 蛇死亡的的标志
		int Deadflag;
//开始
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCsnakeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStart();
	afx_msg void OnContinue();
	afx_msg void OnPause();
	afx_msg void OnExit();
	bool Wall();
	bool EatFood();
	void CreateFood();
	bool EatSelf();
	void initial();
	afx_msg void OnHelp();
	afx_msg void OnAbout();
	void FreeMemory();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // MFCsnakeView.cpp 中的调试版本
inline CMFCsnakeDoc* CMFCsnakeView::GetDocument() const
   { return reinterpret_cast<CMFCsnakeDoc*>(m_pDocument); }
#endif