#pragma argsused
LRESULT CALLBACK StatusWndProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ switch(msg) {
//	 case WM_INITDIALOG : Handles.StatusWnd=hwnd; return 0;
//	 case WM_CLOSE      : return 0;
	 default: return DefWindowProc(hwnd,msg,wp,lp);
  }
}

void ShowStatusDlg(void)
{ WNDCLASS wc;
  LPCSTR   StatusWnd = "StatusWnd";

  wc.style         = CS_OWNDC;
  wc.lpfnWndProc   = StatusWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = Handles.hInst;
  wc.hIcon         = NULL;
  wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground = (HBRUSH)WHITE_BRUSH;
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = StatusWnd;

  if (!RegisterClass(&wc)) return;
  GetWindowRect(GetDlgItem(Handles.MainDlg,IDC_StatusBar),&Handles.StatusRect);
  Handles.StatusWnd=CreateWindow(StatusWnd,NULL,WS_POPUP,Handles.StatusRect.left+2,
						  Handles.StatusRect.top+2,Handles.StatusRect.right-Handles.StatusRect.left-4,
						  Handles.StatusRect.bottom-Handles.StatusRect.top-4,Handles.MainDlg,NULL,
						  Handles.hInst,NULL);
  ShowWindow(Handles.StatusWnd,SW_SHOW);
  Handles.LastPerCent=0; Handles.StatusHDC=BeginPaint(Handles.StatusWnd,&Handles.StatusPs);
}

void HideStatusDlg(void) {
  EndPaint(Handles.StatusWnd,&Handles.StatusPs);
  DestroyWindow(Handles.StatusWnd);
  UnregisterClass("StatusWnd",Handles.hInst);
}

void SetStatusPerCent(BYTE PerCent)
{ int  x, y;

  for (x=Handles.LastPerCent;x<=((Handles.StatusRect.right-Handles.StatusRect.left)*PerCent)/100;x++)
	 for (y=0;y<=Handles.StatusRect.bottom-Handles.StatusRect.top;y++) {SetPixel(Handles.StatusHDC,x,y,RGB(0,0,255));}
  Handles.LastPerCent=1+((Handles.StatusRect.right-Handles.StatusRect.left)*PerCent)/100;
}
