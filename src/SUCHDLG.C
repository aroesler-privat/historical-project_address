#pragma argsused
BOOL CALLBACK SuchDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ WORD w, v;
  BOOL SearchAll, SubString;
  char string[255];

  switch(msg) {
	 case WM_INITDIALOG: if (!Handles.SearchIn) {
									Handles.SearchIn=new BOOL[dBaseHeader.FieldCount];
                           for (w=0;w<dBaseHeader.FieldCount;w++) Handles.SearchIn[w]=FALSE;
								}
								for (w=0;w<dBaseHeader.FieldCount;w++) {
								  if (dBaseHeader.dBaseFields[w].type=='C') {
									 SendMessage(GetDlgItem(hwnd,IDC_Fields),LB_ADDSTRING,0,(LPARAM)dBaseHeader.dBaseFields[w].Name);
								  if (Handles.SearchIn[w]) {
									  SendDlgItemMessage(hwnd,IDC_Fields,LB_SETSEL,(WPARAM)TRUE,MAKELPARAM(w,0));
								}}}
								SendDlgItemMessage(hwnd,IDC_Chars,EM_LIMITTEXT,(WPARAM)255,0L);
								SetDlgItemText(hwnd,IDC_Chars,Handles.SearchStr);
								return TRUE;
	 case WM_COMMAND: switch(wp) {
			case IDC_Cancel: EndDialog(hwnd,0); return FALSE;
			case IDC_Search: v=0;
								  for (w=0;w<dBaseHeader.FieldCount;w++) {
									 if (dBaseHeader.dBaseFields[w].type=='C') {
										if (SendDlgItemMessage(hwnd,IDC_Fields,LB_GETSEL,(WPARAM)v,0L)>0)
										  { Handles.SearchIn[w]=TRUE; } else { Handles.SearchIn[w]=FALSE; } v++;
								  }}
								  GetDlgItemText(hwnd,IDC_Chars,(LPSTR)string,255);
								  SearchAll=(BOOL)SendDlgItemMessage(hwnd,IDC_FromBegin,BM_GETCHECK,0,0L);
								  SubString=(BOOL)SendDlgItemMessage(hwnd,IDC_SubString,BM_GETCHECK,0,0L);
								  strncpy(Handles.SearchStr,string,strlen(string)+1);
								  EndDialog(hwnd,0); SendMessage(Handles.MainDlg,WM_PAINT,0,0L);
								  ReadAdresse(Search,(LPSTR)string,SearchAll,TRUE,SubString);
								  return FALSE;
			}return FALSE;
	 case WM_CLOSE: EndDialog(hwnd,0); return FALSE;
	 default: return FALSE;
  }
}

void ShowSuchDlg(void)
{ DialogBox(Handles.hInst,MAKEINTRESOURCE(SearchDialog),Handles.MainDlg,(DLGPROC)SuchDlgProc); }