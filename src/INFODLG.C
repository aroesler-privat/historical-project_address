#pragma argsused
BOOL CALLBACK InfoDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ char   string[255];
  WORD   w;

  switch(msg){
	 case WM_INITDIALOG: SetWindowText(hwnd,(LPSTR)Handles.FileName);
								switch(dBaseHeader.Version){
								  case 0x02: sprintf(string,"dBASE II"); break;
								  case 0x03: sprintf(string,"dBASE III / IV"); break;
								  case 0x83: sprintf(string,"dBASE III / IV"); break;
								  default: Handles.IsFileOpen=FALSE; EndDialog(hwnd,0); return FALSE;
								} SetDlgItemText(hwnd,103,(LPCSTR)string); // Version
								sprintf(string,"%i.%i.%i",dBaseHeader.Date[2],dBaseHeader.Date[1],dBaseHeader.Date[0]);
								SetDlgItemText(hwnd,104,(LPCSTR)string);  // Datum
								sprintf(string,"%i",dBaseHeader.DataBlocks);
								SetDlgItemText(hwnd,105,(LPCSTR)string);  // Datenblöcke
								if (dBaseHeader.Version!=2) {
								  if (dBaseHeader.Coded==1) { SetDlgItemText(hwnd,106,"Ja"); }
									 else { SetDlgItemText(hwnd,106,"Nein"); }
								} else EnableWindow(GetDlgItem(hwnd,107),FALSE);
								for (w=0;w<dBaseHeader.FieldCount;w++) {
								  SendMessage(GetDlgItem(hwnd,IDC_Felder),LB_ADDSTRING,0,(LPARAM)
												  dBaseHeader.dBaseFields[w].Name);
								}
								return TRUE;
	 case WM_CLOSE: EndDialog(hwnd,0); return FALSE;
	 case WM_COMMAND: switch(wp){
		case IDOK: EndDialog(hwnd,0); return FALSE;
	 }
	 default: return FALSE;
  }
}

void ShowInfoDlg(void)
{ DialogBox(Handles.hInst,MAKEINTRESOURCE(DatenDialog),Handles.MainDlg,(DLGPROC)InfoDlgProc); }
