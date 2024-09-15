WORD PrintAktAdresse(HDC hdc,WORD y,BYTE Style,BOOL *PrintFields,DEVMODE DevMode,WORD *xr)
{ WORD     x, w, yr;
  LPSTR    Fields[8];
  char     string[255];

  if (Adresse.OfsName   !=32000) Fields[0]=(LPSTR)GlobalLock(Adresse.Name   );
  if (Adresse.OfsVorname!=32000) Fields[1]=(LPSTR)GlobalLock(Adresse.Vorname);
  if (Adresse.OfsStrasse!=32000) Fields[2]=(LPSTR)GlobalLock(Adresse.Strasse);
  if (Adresse.OfsPLZ    !=32000) Fields[3]=(LPSTR)GlobalLock(Adresse.PLZ    );
  if (Adresse.OfsOrt    !=32000) Fields[4]=(LPSTR)GlobalLock(Adresse.Ort    );
  if (Adresse.OfsTelefon!=32000) Fields[5]=(LPSTR)GlobalLock(Adresse.Telefon);
  if (Adresse.OfsTelefax!=32000) Fields[6]=(LPSTR)GlobalLock(Adresse.Telefax);
  if (Adresse.OfsInfos  !=32000) Fields[7]=(LPSTR)GlobalLock(Adresse.Informationen);
  if (DevMode.dmPrintQuality<0) { x=400; } else { x=(WORD)((4/2.54)*DevMode.dmPrintQuality); }
  switch(Style){
	 case 1: yr=(WORD)((3.5/2.54)*DevMode.dmYResolution); w=(WORD)((1/2.54)*DevMode.dmYResolution);
				Escape(hdc,STARTDOC,8,"Adresse",NULL);
				sprintf(string,"%s %s",Fields[1],Fields[0]); TextOut(hdc,x,yr,string,strlen(string));
				TextOut(hdc,x,yr+w,Fields[2],strlen(Fields[2]));
				sprintf(string,"%s %s",Fields[3],Fields[4]); TextOut(hdc,x,yr+w+w,string,strlen(string));
				Escape(hdc, NEWFRAME, 0, NULL, NULL); Escape(hdc, ENDDOC, 0, NULL, NULL);
				break;
	 case 2: Escape(hdc,STARTDOC,8,"Adresse",NULL);
				sprintf(string,"%s %s;%s;%s;%s",Fields[1],Fields[0],Fields[2],Fields[3],Fields[4]);
				TextOut(hdc,0,y,string,strlen(string));
				Escape(hdc, NEWFRAME, 0, NULL, NULL); Escape(hdc, ENDDOC, 0, NULL, NULL);
				break;
	 case 3: if (y+(WORD)((1/2.54)*DevMode.dmYResolution)>DevMode.dmPaperLength) {
				  Escape(hdc, NEWFRAME, 0, NULL, NULL); Escape(hdc, ENDDOC, 0, NULL, NULL);
				  Escape(hdc,STARTDOC,8,"Adresse",NULL); yr=0;
				} else yr=y;
				SetTextColor(hdc,RGB(70,70,70));
				for (w=0;w<dBaseHeader.FieldCount;w++) { if (!PrintFields[w]) continue;
				  if (w==Handles.SortFirst) { SetTextColor(hdc,RGB(0,0,0)); }
				  if (w==Handles.SortSecond) { SetTextColor(hdc,RGB(0,0,0)); }
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"NAME",4)==0) TextOut(hdc,xr[0],yr,Fields[0],strlen(Fields[0]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"VORNAME",7)==0) TextOut(hdc,xr[1],yr,Fields[1],strlen(Fields[1]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"STRASSE",7)==0) TextOut(hdc,xr[2],yr,Fields[2],strlen(Fields[2]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"PLZ",3)==0)	TextOut(hdc,xr[3],yr,Fields[3],strlen(Fields[3]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"ORT",3)==0)	TextOut(hdc,xr[4],yr,Fields[4],strlen(Fields[4]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFON",7)==0) TextOut(hdc,xr[5],yr,Fields[5],strlen(Fields[5]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFAX",7)==0) TextOut(hdc,xr[6],yr,Fields[6],strlen(Fields[6]));
				  if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"INFORMATION",11)==0) TextOut(hdc,xr[7],yr,Fields[7],strlen(Fields[7]));
				  if (w==Handles.SortFirst) { SetTextColor(hdc,RGB(70,70,70)); }
				  if (w==Handles.SortSecond) { SetTextColor(hdc,RGB(70,70,70)); }
				} yr=yr+(WORD)((0.5/2.54)*DevMode.dmYResolution); break;
  }
  if (Adresse.OfsName   !=32000) GlobalUnlock(Adresse.Name   );
  if (Adresse.OfsVorname!=32000) GlobalUnlock(Adresse.Vorname);
  if (Adresse.OfsStrasse!=32000) GlobalUnlock(Adresse.Strasse);
  if (Adresse.OfsPLZ    !=32000) GlobalUnlock(Adresse.PLZ    );
  if (Adresse.OfsOrt    !=32000) GlobalUnlock(Adresse.Ort    );
  if (Adresse.OfsTelefon!=32000) GlobalUnlock(Adresse.Telefon);
  if (Adresse.OfsTelefax!=32000) GlobalUnlock(Adresse.Telefax);
  if (Adresse.OfsInfos  !=32000) GlobalUnlock(Adresse.Informationen);
  return yr;
}

void PrintAdresse(BYTE AktStyle,BYTE Style,BOOL *PrintFields,char *From,char *To,PRINTDLG *pd)
{ DEVMODE  DevMode;
  WORD     *x, y, w;
  DWORD    Number, Count;
  LPSTR    Buffer;
  SIZE     sz;
  void     *ptr;

//  if (PrintDlg(pd)==0) {
//	 MessageBox(Handles.MainDlg,"Fehler beim Drucken der Daten aufgetreten!","Error",MB_ICONEXCLAMATION);
//	 return;
//  }
  ptr=GlobalLock(pd->hDevMode); memcpy(&DevMode,ptr,sizeof(DevMode)); GlobalUnlock(pd->hDevMode);
  switch(DevMode.dmPaperSize) {
	 case DMPAPER_ENV_PERSONAL      : DevMode.dmPaperLength= 6.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 3.625*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_MONARCH       : DevMode.dmPaperLength= 7.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 3.875*DevMode.dmYResolution; break;
	 case DMPAPER_STATEMENT         : DevMode.dmPaperLength= 8.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 5.500*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_9	           : DevMode.dmPaperLength= 8.875*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 3.875*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_10            : DevMode.dmPaperLength= 9.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 4.125*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_11	           : DevMode.dmPaperLength=10.375*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 4.500*DevMode.dmYResolution; break;
	 case DMPAPER_EXECUTIVE         : DevMode.dmPaperLength=10.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 7.500*DevMode.dmYResolution; break;
	 case DMPAPER_NOTE              : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_FANFOLD_US	     : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =14.875*DevMode.dmYResolution; break;
	 case DMPAPER_LETTER            : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_LETTERSMALL       : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_12            : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 4.500*DevMode.dmYResolution; break;
	 case DMPAPER_LEDGER            : DevMode.dmPaperLength=11.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =17.000*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_14            : DevMode.dmPaperLength=11.500*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 5.000*DevMode.dmYResolution; break;
	 case DMPAPER_FANFOLD_STD_GERMAN: DevMode.dmPaperLength=12.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_FOLIO             : DevMode.dmPaperLength=13.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.000*DevMode.dmYResolution; break;
	 case DMPAPER_FANFOLD_LGL_GERMAN: DevMode.dmPaperLength=13.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_10X14	           : DevMode.dmPaperLength=14.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =10.000*DevMode.dmYResolution; break;
	 case DMPAPER_LEGAL	           : DevMode.dmPaperLength=14.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth = 8.500*DevMode.dmYResolution; break;
	 case DMPAPER_11X17             : DevMode.dmPaperLength=17.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =11.000*DevMode.dmYResolution; break;
	 case DMPAPER_TABLOID           : DevMode.dmPaperLength=17.000*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =11.000*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_B6            : DevMode.dmPaperLength=125/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =176/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_C6            : DevMode.dmPaperLength=162/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =114/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_A5                : DevMode.dmPaperLength=210/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =148/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_DL            : DevMode.dmPaperLength=220/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =110/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_C5            : DevMode.dmPaperLength=229/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =162/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_C65           : DevMode.dmPaperLength=229/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =114/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_ITALY         : DevMode.dmPaperLength=230/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =110/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_B5            : DevMode.dmPaperLength=250/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =176/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_B5                : DevMode.dmPaperLength=257/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =182/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_QUARTO            : DevMode.dmPaperLength=275/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =215/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_A4                : DevMode.dmPaperLength=297/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =210/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_A4SMALL           : DevMode.dmPaperLength=297/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =210/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_C4            : DevMode.dmPaperLength=324/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =229/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_B4            : DevMode.dmPaperLength=353/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =250/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_B4	              : DevMode.dmPaperLength=354/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =250/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_A3	              : DevMode.dmPaperLength=420/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =297/25.4*DevMode.dmYResolution; break;
	 case DMPAPER_ENV_C3            : DevMode.dmPaperLength=458/25.4*DevMode.dmYResolution;
												 DevMode.dmPaperWidth =324/25.4*DevMode.dmYResolution; break;
	 default                        : DevMode.dmPaperLength=32700;
												 DevMode.dmPaperWidth =32700;
  }
  if (DevMode.dmPaperLength!=0) DevMode.dmPaperLength=DevMode.dmPaperLength*254*DevMode.dmYResolution;
  if (DevMode.dmPaperWidth !=0) DevMode.dmPaperWidth =DevMode.dmPaperWidth *254*DevMode.dmPrintQuality;
  if (Style==3) {
	 x=new WORD[dBaseHeader.FieldCount]; for(y=0;y<dBaseHeader.FieldCount;y++) x[y]=0;
	 if (AktStyle!=1) { ReadAdresse(First,NULL,FALSE,FALSE,FALSE); ShowStatusDlg(); Number=0; }
	 do { for(y=0;y<dBaseHeader.FieldCount;y++) { if (!PrintFields[y]) continue;
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"NAME",4)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Name);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Name); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"VORNAME",7)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Vorname);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Vorname); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"STRASSE",7)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Strasse);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Strasse); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"PLZ",3)==0)	{
		  Buffer=(char*)GlobalLock(Adresse.PLZ);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.PLZ); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"ORT",3)==0)	{
		  Buffer=(char*)GlobalLock(Adresse.Ort);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Ort); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"TELEFON",7)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Telefon);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Telefon); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"TELEFAX",7)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Telefax);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Telefax); }
		if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[y].Name),"INFORMATION",11)==0) {
		  Buffer=(char*)GlobalLock(Adresse.Informationen);	if (x[y]<strlen(Buffer)) x[y]=strlen(Buffer)+1; GlobalUnlock(Adresse.Informationen); }
		} if (AktStyle==1) goto Weiter;
		Number++; SetStatusPerCent((BYTE)(Number*100/dBaseHeader.DataBlocks));
	 }while (ReadAdresse(Next,NULL,FALSE,FALSE,FALSE)); HideStatusDlg(); Weiter:
	 for(y=dBaseHeader.FieldCount-1;y>0;y--) { x[y]=1; for (w=0;w<y;w++) x[y]=x[y]+x[w]; } x[0]=0;
	 strnset(Buffer,0x20,255);
	 for(y=0;y<dBaseHeader.FieldCount;y++) {
		GetTextExtentPoint(pd->hDC,Buffer,x[y],&sz); x[y]=sz.cx; }
	 Escape(pd->hDC,STARTDOC,8,"Adresse",NULL); }
  switch(AktStyle) {
	 case 1: PrintAktAdresse(pd->hDC,0,Style,PrintFields,DevMode,x); break;
	 case 2: ReadAdresse(First,NULL,FALSE,FALSE,FALSE);
				y=PrintAktAdresse(pd->hDC,0,Style,PrintFields,DevMode,x);
				ShowStatusDlg(); SetStatusPerCent(0);
				for (Number=2;Number<=dBaseHeader.DataBlocks;Number++) {
				  SetStatusPerCent((WORD)(Number*100/dBaseHeader.DataBlocks));
				  ReadAdresse(Next,NULL,FALSE,FALSE,FALSE);
				  y=PrintAktAdresse(pd->hDC,y,Style,PrintFields,DevMode,x); }
				ReadAdresse(Last,NULL,FALSE,TRUE,FALSE); HideStatusDlg(); break;
	 case 3: if (strlen(To)==0) { Count=dBaseHeader.DataBlocks; } else {
				  if (!ReadAdresse(Search,To,TRUE,FALSE,FALSE)) {
					  MessageBox(Handles.MainDlg,"Fehler beim Drucken: Letzten Eintrag nicht gefunden!","Error",MB_ICONEXCLAMATION);
					  ReadAdresse(First,NULL,TRUE,TRUE,FALSE); return; }
				  Count=Handles.AktAdr;
				}
				if (!ReadAdresse(Search,From,TRUE,FALSE,FALSE)) {
					MessageBox(Handles.MainDlg,"Fehler beim Drucken: Ersten Eintrag nicht gefunden!","Error",MB_ICONEXCLAMATION);
					ReadAdresse(First,NULL,TRUE,TRUE,FALSE); return; }
				if (Count<Handles.AktAdr) {
				  Number=Count; Count=Handles.AktAdr; ReadAdresse(Search,To,TRUE,FALSE,FALSE); }
				Count=Count-Handles.AktAdr; ShowStatusDlg(); SetStatusPerCent(0);
				y=PrintAktAdresse(pd->hDC,0,Style,PrintFields,DevMode,x);
				for (Number=1;Number<=Count;Number++) {
				  SetStatusPerCent((WORD)(Number*100/Count));
				  if (!ReadAdresse(Next,NULL,FALSE,FALSE,FALSE)) {
					 ReadAdresse(Search,To,TRUE,TRUE,FALSE); HideStatusDlg(); return; }
				  y=PrintAktAdresse(pd->hDC,y,Style,PrintFields,DevMode,x);
				}
				ReadAdresse(Search,To,TRUE,TRUE,FALSE); HideStatusDlg();  break;
  }
  if (Style==3) {Escape(pd->hDC, NEWFRAME, 0, NULL, NULL); Escape(pd->hDC, ENDDOC, 0, NULL, NULL);}
}

#pragma argsused
BOOL CALLBACK PrintDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ WORD   w, v;
  BOOL   *PrintFields;
  static PRINTDLG pd;
  char   From[255], To[255];

  switch(msg) {
	 case WM_INITDIALOG: memset(&pd, 0, sizeof(PRINTDLG));
								pd.lStructSize = sizeof(PRINTDLG);
								pd.hwndOwner   = hwnd;
								pd.Flags       = PD_RETURNDC|PD_RETURNDEFAULT;
								if (PrintDlg(&pd)==0) {
								  MessageBox(Handles.MainDlg,"Fehler beim Drucken der Daten aufgetreten!","Error",MB_ICONEXCLAMATION);
								  if (pd.hDC) DeleteDC(pd.hDC);
								  if (pd.hDevMode != NULL) GlobalFree(pd.hDevMode);
								  if (pd.hDevNames != NULL) GlobalFree(pd.hDevNames);
								  EndDialog(hwnd,0); return FALSE;
								}
								if (!Handles.SearchIn) {
									Handles.SearchIn=new BOOL[dBaseHeader.FieldCount];
									for (w=0;w<dBaseHeader.FieldCount;w++) Handles.SearchIn[w]=FALSE;
								}
								for (w=0;w<dBaseHeader.FieldCount;w++) { Handles.SearchIn[w]=FALSE;
								  if  (dBaseHeader.dBaseFields[w].type=='C') {
									 SendDlgItemMessage(hwnd,IDC_PrintFields,LB_ADDSTRING,0,(LPARAM)
															 (LPSTR)dBaseHeader.dBaseFields[w].Name);
									 if (w==Handles.SortFirst) Handles.SearchIn[w]=TRUE;
								}}
								SendDlgItemMessage(hwnd,IDC_AdrFrom,EM_LIMITTEXT,dBaseHeader.dBaseFields[Handles.SortFirst].Length,0);
								SendDlgItemMessage(hwnd,IDC_AdrTo  ,EM_LIMITTEXT,dBaseHeader.dBaseFields[Handles.SortFirst].Length,0);
								EnableWindow(GetDlgItem(hwnd,IDC_AdrFrom),FALSE);
								EnableWindow(GetDlgItem(hwnd,IDC_AdrTo),FALSE);
								SendDlgItemMessage(hwnd,IDC_PrintFields,LB_SETCURSEL,0,0);
								SendDlgItemMessage(hwnd,IDC_OnlyThis,BM_SETCHECK,TRUE,0L);
								SendDlgItemMessage(hwnd,IDC_AdrAnschrift,BM_SETCHECK,TRUE,0L);
								return TRUE;
	 case WM_CLOSE: EndDialog(hwnd,0); return FALSE;
	 case WM_COMMAND: switch(wp) {
		case IDC_AdrListe    : EnableWindow(GetDlgItem(hwnd,IDC_PrintFields),TRUE); return FALSE;
		case IDC_AdrAnschrift:
		case IDC_AdrAbsender : EnableWindow(GetDlgItem(hwnd,IDC_PrintFields),FALSE); return FALSE;
		case IDC_FromTo      : EnableWindow(GetDlgItem(hwnd,IDC_AdrFrom),TRUE);
									  EnableWindow(GetDlgItem(hwnd,IDC_AdrTo),TRUE); return FALSE;
		case IDC_All         :
		case IDC_OnlyThis    : EnableWindow(GetDlgItem(hwnd,IDC_AdrFrom),FALSE);
									  EnableWindow(GetDlgItem(hwnd,IDC_AdrTo),FALSE); return FALSE;
		case IDC_PrinterSetup: pd.Flags = PD_PRINTSETUP|PD_RETURNDC;
									  PrintDlg(&pd); return FALSE;
		case IDOK    : PrintFields=new BOOL[dBaseHeader.FieldCount]; v=0;
							for (w=0;w<dBaseHeader.FieldCount;w++) {
							  if (dBaseHeader.dBaseFields[w].type=='C') {
								 if (SendDlgItemMessage(hwnd,IDC_PrintFields,LB_GETSEL,(WPARAM)v,0L)>0)
									{ PrintFields[w]=TRUE; } else { PrintFields[w]=FALSE; } v++;
							}}
							if (SendDlgItemMessage(hwnd,IDC_AdrAnschrift,BM_GETSTATE,0,0L)==1) w=1;
							if (SendDlgItemMessage(hwnd,IDC_AdrAbsender ,BM_GETSTATE,0,0L)==1) w=2;
							if (SendDlgItemMessage(hwnd,IDC_AdrListe    ,BM_GETSTATE,0,0L)==1) w=3;
							if (SendDlgItemMessage(hwnd,IDC_OnlyThis    ,BM_GETSTATE,0,0L)==1) v=1;
							if (SendDlgItemMessage(hwnd,IDC_All         ,BM_GETSTATE,0,0L)==1) v=2;
							if (SendDlgItemMessage(hwnd,IDC_FromTo      ,BM_GETSTATE,0,0L)==1) v=3;
							GetDlgItemText(hwnd,IDC_AdrFrom,(LPSTR)From,dBaseHeader.dBaseFields[Handles.SortFirst].Length);
							GetDlgItemText(hwnd,IDC_AdrTo  ,(LPSTR)To  ,dBaseHeader.dBaseFields[Handles.SortFirst].Length);
							EndDialog(hwnd,0); SendMessage(Handles.MainDlg,WM_PAINT,0,0L);
							PrintAdresse(v,w,PrintFields,From,To,&pd);
                     if (pd.hDC) DeleteDC(pd.hDC);
							if (pd.hDevMode != NULL) GlobalFree(pd.hDevMode);
							if (pd.hDevNames != NULL) GlobalFree(pd.hDevNames);
							return FALSE;
		case IDCANCEL: if (pd.hDC) DeleteDC(pd.hDC);
							if (pd.hDevMode != NULL) GlobalFree(pd.hDevMode);
							if (pd.hDevNames != NULL) GlobalFree(pd.hDevNames);
							EndDialog(hwnd,0); return FALSE;
	 }
	 default: return FALSE;
}}

void ShowPrintDlg(void)
{ DialogBox(Handles.hInst,MAKEINTRESOURCE(PrintDialog),Handles.MainDlg,(DLGPROC)PrintDlgProc); }
