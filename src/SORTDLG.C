void SetAdresseOnOff(BOOL On)
{
  if (Adresse.OfsName   !=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Name   ),On);
  if (Adresse.OfsVorname!=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Vorname),On);
  if (Adresse.OfsStrasse!=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Strasse),On);
  if (Adresse.OfsPLZ    !=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_PLZ    ),On);
  if (Adresse.OfsOrt    !=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Ort    ),On);
  if (Adresse.OfsTelefon!=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefon),On);
  if (Adresse.OfsTelefax!=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefax),On);
  if (Adresse.OfsInfos  !=32000) EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Info   ),On);
}

#pragma argsused
BOOL CALLBACK SortDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ WORD  w, first, second;
  DWORD dw;
  char  Title[11];

  switch(msg) {
	 case WM_INITDIALOG: for (w=0;w<dBaseHeader.FieldCount;w++) {
								  if  (dBaseHeader.dBaseFields[w].type=='C') {
									 SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_ADDSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
									 SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_ADDSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
								}}
								if (dBaseHeader.dBaseFields[Handles.SortFirst].type=='C') {
								  SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[Handles.SortFirst].Name);
								} else { for (w=0;w<dBaseHeader.FieldCount;w++) {
								  if  (dBaseHeader.dBaseFields[w].type=='C') {
									 SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
									 Handles.SortFirst=w; break; }}}
								if ((dBaseHeader.dBaseFields[Handles.SortSecond].type=='C')&&(Handles.SortFirst!=Handles.SortSecond)) {
								  SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[Handles.SortSecond].Name);
								} else { for (w=0;w<dBaseHeader.FieldCount;w++) {
								  if  ((dBaseHeader.dBaseFields[w].type=='C')&&(Handles.SortFirst!=w)) {
									 SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
									 Handles.SortSecond=w; break; }}}
								SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_DELETESTRING,(WPARAM)Handles.SortSecond,0L);
								SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_DELETESTRING,(WPARAM)Handles.SortFirst,0L);
								first=Handles.SortFirst; second=Handles.SortSecond;
								return TRUE;
	 case WM_COMMAND: switch(wp){
		case IDC_SortFirst: switch(HIWORD(lp)) {
			  case 1: dw=SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETCURSEL,0,0L);
						 if (dw!=(DWORD)CB_ERR) {
							SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
							for (w=0;w<dBaseHeader.FieldCount;w++) {
							  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
								 first=w; }}}
						 dw=SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETCURSEL,0,0L);
						 if (dw!=(DWORD)CB_ERR) {
							SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
							for (w=0;w<dBaseHeader.FieldCount;w++) {
							  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
								 second=w; }}}
						 SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_RESETCONTENT,0,0L);
						 for (w=0;w<dBaseHeader.FieldCount;w++) {
							if  ((dBaseHeader.dBaseFields[w].type=='C')&&(w!=first)) {
							  SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_ADDSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
							}}
						 SendMessage(GetDlgItem(hwnd,IDC_SortSecond),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[second].Name);
						 return FALSE;
			  default: return FALSE; }
		case IDC_SortSecond: switch(HIWORD(lp)) {
			  case 1: dw=SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETCURSEL,0,0L);
						 if (dw!=(DWORD)CB_ERR) {
							SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
							for (w=0;w<dBaseHeader.FieldCount;w++) {
							  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
								 second=w; }}}
						 dw=SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETCURSEL,0,0L);
						 if (dw!=(DWORD)CB_ERR) {
							SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
							for (w=0;w<dBaseHeader.FieldCount;w++) {
							  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
								 first=w; }}}
						 SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_RESETCONTENT,0,0L);
						 for (w=0;w<dBaseHeader.FieldCount;w++) {
							if  ((dBaseHeader.dBaseFields[w].type=='C')&&(w!=second)) {
							  SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_ADDSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[w].Name);
							}}
						 SendMessage(GetDlgItem(hwnd,IDC_SortFirst),CB_SELECTSTRING,0,(LPARAM)(LPSTR)dBaseHeader.dBaseFields[first].Name);
						 return FALSE;
			  default: return FALSE; }
		case IDCANCEL: EndDialog(hwnd,0); return FALSE;
		case IDOK: dw=SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETCURSEL,0,0L);
					  if (dw!=(DWORD)CB_ERR) {
						 SendDlgItemMessage(hwnd,IDC_SortFirst,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
						 for (w=0;w<dBaseHeader.FieldCount;w++) {
							if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
							  Handles.SortFirst=w; break;
					  }}}
					  dw=SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETCURSEL,0,0L);
					  if (dw!=(DWORD)CB_ERR) {
						 SendDlgItemMessage(hwnd,IDC_SortSecond,CB_GETLBTEXT,(WPARAM)dw,(LPARAM)(LPSTR)Title);
						 for (w=0;w<dBaseHeader.FieldCount;w++) {
							if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),Title,strlen(Title))==0) {
							  Handles.SortSecond=w; break;
					  }}}
					  EndDialog(hwnd,0); SendMessage(Handles.MainDlg,WM_PAINT,0,0L);
					  AktDataBase(TRUE);  return FALSE;
	 }
	 default: return FALSE;
}}

void ShowSortDlg(void)
{ DialogBox(Handles.hInst,MAKEINTRESOURCE(SortDialog),Handles.MainDlg,(DLGPROC)SortDlgProc); }
