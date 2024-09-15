#pragma argsused
BOOL CALLBACK QualityDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ WORD        w;
  DWORD       Index;
  char    	  string[32];
  int         istring;

  switch(msg){
	 case WM_INITDIALOG: sprintf(string,"Eigenschaften von '%s'",Handles.FileName); SetWindowText(hwnd,(LPSTR)string);
								for (w=0;(w<dBaseHeader.FieldCount)&&(w<10);w++) {
								  if  (dBaseHeader.dBaseFields[w].type=='C') {
									 SendMessage(GetDlgItem(hwnd,IDC_Fields),LB_ADDSTRING,0,(LPARAM)
										(LPSTR)dBaseHeader.dBaseFields[w].Name); }}
								SendDlgItemMessage(hwnd,IDC_Length,EM_LIMITTEXT,5,0);
								SendDlgItemMessage(hwnd,IDC_Fields,LB_SETCURSEL,0,0); Index=0;
								sprintf(string,"%i",dBaseHeader.dBaseFields[LOWORD(Index)].Length);
								SetDlgItemText(hwnd,IDC_Length,string); return TRUE;
	 case WM_CLOSE: EndDialog(hwnd,0); return FALSE;
	 case WM_COMMAND: switch(wp){
		case IDC_Fields: switch(HIWORD(lp)) {
			  case 1: Index=SendDlgItemMessage(hwnd,IDC_Fields,LB_GETCURSEL,0,0);
						 sprintf(string,"%i",dBaseHeader.dBaseFields[LOWORD(Index)].Length);
						 SetDlgItemText(hwnd,IDC_Length,string); return FALSE;
			  default: return FALSE; }
		case IDC_Length: switch(HIWORD(lp)) {
			  case EN_KILLFOCUS: GetDlgItemText(hwnd,IDC_Length,(LPSTR)string,6); istring=0;
										for (istring=0;istring<strlen(string);istring++) {
										  if ((string[istring]=='0')||(string[istring]=='1')||(string[istring]=='2')||
												(string[istring]=='3')||(string[istring]=='4')||(string[istring]=='5')||
												(string[istring]=='6')||(string[istring]=='7')||(string[istring]=='8')||
												(string[istring]=='9')) { continue; }
										  else { MessageBox(hwnd,"Es sind nur numerische Werte erlaubt","Error",MB_ICONEXCLAMATION);
													sprintf(string,"%i",dBaseHeader.dBaseFields[LOWORD(Index)].Length);
													SetDlgItemText(hwnd,IDC_Length,(LPSTR)string); istring=256; }
										}
										if (istring!=256) {
										  istring=atoi(string);
										  if ((istring<=0)||(istring>255)) {
											 MessageBox(hwnd,"Wert muﬂ zwischen 1 und 255 liegen","Error",MB_ICONEXCLAMATION);
											 sprintf(string,"%i",dBaseHeader.dBaseFields[LOWORD(Index)].Length);
											 SetDlgItemText(hwnd,IDC_Length,(LPSTR)string); }
										  else { Index=SendDlgItemMessage(hwnd,IDC_Fields,LB_GETCURSEL,0,0);
											 dBaseHeader.dBaseFields[LOWORD(Index)].Length=LOBYTE(istring); }
										} return FALSE;
			  default: return FALSE; }
		case IDC_OK    : EndDialog(hwnd,0); SendMessage(Handles.MainDlg,WM_PAINT,0,0L);
							  if (MessageBox(Handles.MainDlg,"Sollen die Datens‰tze nach den aktuellen Einstellungen geordnet werden?",
									"Frage",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES) { AktDataBase(TRUE); }
								else {AktDataBase(FALSE); } return FALSE;
		case IDC_Cancel: Index=_llseek(Handles.File,0,1); _llseek(Handles.File,32,0);
							  for (w=0;w<dBaseHeader.FieldCount;w++) {
								 if (_lread(Handles.File,&dBaseHeader.dBaseFields[w],32)!=32) {
								 MessageBox(Handles.MainDlg,"Fehler beim Wiederherstellen der Datens‰tze","Error",MB_ICONEXCLAMATION);
								 _lclose(Handles.File); break; }}
							  _llseek(Handles.File,Index,0);
							  EndDialog(hwnd,0); return FALSE;
	 }
	 default: return FALSE;
  }
}

void ShowQualityDlg(void)
{ DialogBox(Handles.hInst,MAKEINTRESOURCE(QualityDialog),Handles.MainDlg,(DLGPROC)QualityDlgProc); }
