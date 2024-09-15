BOOL ReadHeader(LPSTR FileName)
{ OFSTRUCT   of;
  HFILE      fh = Handles.File;
  char       string[255];
  WORD       w;
  BYTE       Version;

  if (fh) { _lclose(fh); }
  if ((fh=OpenFile(FileName,&of,OF_READWRITE))==HFILE_ERROR) {
	 sprintf(string,"Fehler Nr. %i beim Öffnen der Datei",of.nErrCode);
	 MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION); return FALSE;
  }
  if (_lread(fh,&Version,1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Lesen des Headers","Error",MB_ICONEXCLAMATION);
	 _lclose(fh); return FALSE;
  } _llseek(fh,0,0);
  switch(Version){
	 case 2 : MessageBox(Handles.MainDlg,"dBASE II-Dateien können nicht verarbeitet werden.","Error",MB_ICONEXCLAMATION);
				 return FALSE;
	 default: if (_lread(fh,&dBaseHeader,32)!=32) {
					 MessageBox(Handles.MainDlg,"Fehler beim Lesen des Headers","Error",MB_ICONEXCLAMATION);
					 _lclose(fh); return FALSE; }
  }
  if (!((dBaseHeader.Version==2)||(dBaseHeader.Version==3)||(dBaseHeader.Version==0x83))) {
	 MessageBox(Handles.MainDlg,"Ungültige dBASE-Version!","Error",MB_ICONEXCLAMATION);
	 _lclose(fh); return FALSE;
  }
  dBaseHeader.FieldCount=((dBaseHeader.HeaderSize-32)/32);
  dBaseHeader.dBaseFields=new dBaseField[dBaseHeader.FieldCount];
  for (w=0;w<dBaseHeader.FieldCount;w++) {
	 if (_lread(fh,&dBaseHeader.dBaseFields[w],32)!=32) {
		MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
		_lclose(fh); return FALSE;
	 }
  } Handles.IsFileOpen=TRUE; Handles.File=fh; return TRUE;
}

void InitAdresse(void)
{ WORD  w, ofs = 0;

  Handles.AktAdrOffset=dBaseHeader.HeaderSize+1;
  Adresse.OfsName    = 32000; Adresse.OfsVorname = 32000; Adresse.OfsStrasse = 32000;
  Adresse.OfsPLZ     = 32000; Adresse.OfsOrt     = 32000; Adresse.OfsTelefon = 32000;
  Adresse.OfsTelefax = 32000; Adresse.OfsInfos   = 32000;
  Adresse.LenName    = 0; Adresse.LenVorname = 0; Adresse.LenStrasse = 0;
  Adresse.LenPLZ     = 0; Adresse.LenOrt     = 0; Adresse.LenTelefon = 0;
  Adresse.LenTelefax = 0; Adresse.LenInfos   = 0;
  SetDlgItemText(Handles.MainDlg,IDC_Vorname,""); SetDlgItemText(Handles.MainDlg,IDC_Name,"");
  SetDlgItemText(Handles.MainDlg,IDC_Strasse,""); SetDlgItemText(Handles.MainDlg,IDC_PLZ,"");
  SetDlgItemText(Handles.MainDlg,IDC_Telefon,""); SetDlgItemText(Handles.MainDlg,IDC_Ort,"");
  SetDlgItemText(Handles.MainDlg,IDC_Telefax,""); SetDlgItemText(Handles.MainDlg,IDC_Info,"");
  for (w=0;w<dBaseHeader.FieldCount;w++) {
// evt. kein Character-Feld -------------------------------------------------------------------
	 if (dBaseHeader.dBaseFields[w].type!='C') {
		ofs=ofs+dBaseHeader.dBaseFields[w].Length;
		MessageBox(Handles.MainDlg,"Dieses Feld ist kein 'Character'-Feld und kann nicht dargestellt werden!",
		dBaseHeader.dBaseFields[w].Name,MB_ICONEXCLAMATION); continue; }
// Name ---------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"NAME",4)==0) {
		Adresse.LenName=dBaseHeader.dBaseFields[w].Length; Adresse.OfsName=ofs;
		if (Adresse.Name) { GlobalFree(Adresse.Name); }
		Adresse.Name=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenName);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Name),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,118),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Name),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsName==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Name),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,118),FALSE); }}
// Vorname ------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"VORNAME",7)==0) {
		Adresse.LenVorname=dBaseHeader.dBaseFields[w].Length; Adresse.OfsVorname=ofs;
		if (Adresse.Vorname) { GlobalFree(Adresse.Vorname); }
		Adresse.Vorname=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenVorname);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Vorname),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,119),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Vorname),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsVorname==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Vorname),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,119),FALSE); }}
// Straße -------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"STRASSE",7)==0) {
		Adresse.LenStrasse=dBaseHeader.dBaseFields[w].Length; Adresse.OfsStrasse=ofs;
		if (Adresse.Strasse) { GlobalFree(Adresse.Strasse); }
		Adresse.Strasse=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenStrasse);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Strasse),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,120),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Strasse),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsStrasse==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Strasse),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,120),FALSE); }}
// Postleitzahl -------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"PLZ",3)==0) {
		Adresse.LenPLZ=dBaseHeader.dBaseFields[w].Length; Adresse.OfsPLZ=ofs;
		if (Adresse.PLZ) { GlobalFree(Adresse.PLZ); }
		Adresse.PLZ=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenPLZ);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_PLZ),TRUE); EnableWindow(GetDlgItem(Handles.MainDlg,121),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_PLZ),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsPLZ==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_PLZ),FALSE);	EnableWindow(GetDlgItem(Handles.MainDlg,121),FALSE); }}
// Ort ----------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"ORT",3)==0) {
		Adresse.LenOrt=dBaseHeader.dBaseFields[w].Length; Adresse.OfsOrt=ofs;
		if (Adresse.Ort) { GlobalFree(Adresse.Ort); }
		Adresse.Ort=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenOrt);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Ort),TRUE); EnableWindow(GetDlgItem(Handles.MainDlg,122),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Ort),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsOrt==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Ort),FALSE);	EnableWindow(GetDlgItem(Handles.MainDlg,122),FALSE); }}
// Telefon ------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFON",7)==0) {
		Adresse.LenTelefon=dBaseHeader.dBaseFields[w].Length; Adresse.OfsTelefon=ofs;
		if (Adresse.Telefon) { GlobalFree(Adresse.Telefon); }
		Adresse.Telefon=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenTelefon);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefon),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,123),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Telefon),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsTelefon==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefon),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,123),FALSE); }}
// Telefax ------------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFAX",7)==0) {
		Adresse.LenTelefax=dBaseHeader.dBaseFields[w].Length; Adresse.OfsTelefax=ofs;
		if (Adresse.Telefax) { GlobalFree(Adresse.Telefax); }
		Adresse.Telefax=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenTelefax);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefax),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,124),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Telefax),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsTelefax==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefax),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,124),FALSE); }}
// Informationen ------------------------------------------------------------------------------
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"INFORMATION",11)==0) {
		Adresse.LenInfos=dBaseHeader.dBaseFields[w].Length; Adresse.OfsInfos=ofs;
		if (Adresse.Informationen) { GlobalFree(Adresse.Informationen); }
		Adresse.Informationen=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,(DWORD)Adresse.LenInfos);
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Info),TRUE);	EnableWindow(GetDlgItem(Handles.MainDlg,125),TRUE);
		SendMessage(GetDlgItem(Handles.MainDlg,IDC_Info),EM_LIMITTEXT,dBaseHeader.dBaseFields[w].Length,0L);
	 } else { if (Adresse.OfsStrasse==32000) {
		EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Info),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,125),FALSE); }}
	 ofs=ofs+dBaseHeader.dBaseFields[w].Length;
  }
}

LPSTR GetFileName(BOOL fOpen)
{ OPENFILENAME ofn;
  HINSTANCE    hInst = Handles.hInst;
  LPSTR        Filter = "dBASE III / IV (*.dbf)\0*.dbf\0Alle Dateien (*.*)\0*.*\0";
  char         FileName[255] = "";
  char         DlgTitle[34] = "Adressenverwaltung - ";

  ofn.lStructSize       = sizeof(ofn);
  ofn.hwndOwner         = Handles.MainDlg;
  ofn.hInstance         = hInst;
  ofn.lpstrFilter       = Filter;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter    = 0;
  ofn.nFilterIndex      = 1;
  ofn.lpstrFile         = FileName;
  ofn.nMaxFile          = sizeof(FileName);
  ofn.lpstrFileTitle    = NULL;
  ofn.nMaxFileTitle     = 0;
  ofn.lpstrInitialDir   = NULL;
  ofn.Flags             = OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
  ofn.nFileOffset       = 0;
  ofn.nFileExtension    = 0;
  ofn.lpstrDefExt       = "dbf";
  ofn.lCustData         = 0L;
  ofn.lpfnHook          = 0L;
  ofn.lpTemplateName    = (LPSTR)NULL;
  if (fOpen) { ofn.lpstrTitle = "Datenbank öffnen";
					ofn.Flags     |= OFN_FILEMUSTEXIST;
					if (!GetOpenFileName(&ofn)) return NULL;
					if (!ReadHeader((LPSTR)FileName)) return NULL;
					InitAdresse(); ReadAdresse(First,NULL,FALSE,TRUE,FALSE);
  } else {     ofn.lpstrTitle = "neue Datenbank anlegen";
					ofn.Flags     |= OFN_OVERWRITEPROMPT;
					if (!GetSaveFileName(&ofn)) return NULL;
  }
  strcat(DlgTitle,&FileName[ofn.nFileOffset]);
  strcpy(Handles.FileName,&FileName[ofn.nFileOffset]);
  SetWindowText(Handles.MainDlg,(LPCSTR)DlgTitle);
  return &FileName[ofn.nFileOffset];
}

BOOL AssignAdresse(BOOL Mode)
{
  if (!Mode) {
	 if ((Adresse.Name         =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Vorname      =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Strasse      =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.PLZ          =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Ort          =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Telefon      =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Telefax      =GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE;
	 if ((Adresse.Informationen=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,255))==NULL) return FALSE; }
  Adresse.OfsName    = 0; Adresse.OfsVorname = 0; Adresse.OfsStrasse = 0;
  Adresse.OfsPLZ     = 0; Adresse.OfsOrt     = 0; Adresse.OfsTelefon = 0;
  Adresse.OfsTelefax = 0; Adresse.OfsInfos   = 0;
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Name   ),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,118),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Vorname),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,119),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Strasse),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,120),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_PLZ    ),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,121),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Ort    ),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,122),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefon),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,123),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Telefax),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,124),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Info   ),Mode); EnableWindow(GetDlgItem(Handles.MainDlg,125),Mode);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Left),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_First),FALSE);
  EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Right),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Last),FALSE);
  return TRUE;
}

void NewAdresse(void)
{ LPSTR Buffer;

  Buffer=(char *)GlobalLock(Adresse.Name   ); strnset(Buffer,0,Adresse.LenName   ); GlobalUnlock(Adresse.Name   );
  Buffer=(char *)GlobalLock(Adresse.Vorname); strnset(Buffer,0,Adresse.LenVorname); GlobalUnlock(Adresse.Vorname);
  Buffer=(char *)GlobalLock(Adresse.Strasse); strnset(Buffer,0,Adresse.LenStrasse); GlobalUnlock(Adresse.Strasse);
  Buffer=(char *)GlobalLock(Adresse.PLZ    ); strnset(Buffer,0,Adresse.LenPLZ    ); GlobalUnlock(Adresse.PLZ    );
  Buffer=(char *)GlobalLock(Adresse.Ort    ); strnset(Buffer,0,Adresse.LenOrt    ); GlobalUnlock(Adresse.Ort    );
  Buffer=(char *)GlobalLock(Adresse.Telefon); strnset(Buffer,0,Adresse.LenTelefon); GlobalUnlock(Adresse.Telefon);
  Buffer=(char *)GlobalLock(Adresse.Telefax); strnset(Buffer,0,Adresse.LenTelefax); GlobalUnlock(Adresse.Telefax);
  Buffer=(char *)GlobalLock(Adresse.Informationen); strnset(Buffer,0,Adresse.LenInfos); GlobalUnlock(Adresse.Informationen);
  WriteAdresse(New); SetFocus(GetDlgItem(Handles.MainDlg,IDC_Name));
}

void NewFile(void)
{ date Date;
  OFSTRUCT   of;
  LPSTR      FileName;
  HFILE      fh = Handles.File;
  char       string[255];
  WORD       v, w;

  dBaseHeader.Version=0x03; getdate(&Date);
  dBaseHeader.Date[0]=(Date.da_year-1900); dBaseHeader.Date[1]=Date.da_mon; dBaseHeader.Date[2]=Date.da_day;
  dBaseHeader.DataBlocks=0; dBaseHeader.HeaderSize=289; dBaseHeader.DataBlockSize=1792;
  dBaseHeader.Res1=0x04E; dBaseHeader.UnStopped=0; dBaseHeader.Coded=0; dBaseHeader.FieldCount=8;
  dBaseHeader.dBaseFields=new dBaseField[8];
  if ((FileName=GetFileName(FALSE))==NULL) {
	 MessageBox(Handles.MainDlg,"Datei kann nicht geöffnet werden!","Error",MB_ICONEXCLAMATION);
	 return; }
  if (fh) { _lclose(fh); }
  if ((fh=OpenFile(FileName,&of,OF_CREATE))==HFILE_ERROR) {
	 sprintf(string,"Fehler Nr. %i beim Erstellen der Datei",of.nErrCode);
	 MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION); return;
  }
  if (_lwrite(fh,&dBaseHeader,32)!=32) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(FileName,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der Datenbank","Error",MB_ICONEXCLAMATION);
	 } return;
  }
  for (w=0;w<8;w++) {
	 dBaseHeader.dBaseFields[w].type='C'; dBaseHeader.dBaseFields[w].Length=255;
	 dBaseHeader.dBaseFields[w].Komma=0;  dBaseHeader.dBaseFields[w].MDX=0;
	 for (v=0;v<11;v++) {dBaseHeader.dBaseFields[w].Name[v]=0; }
	 switch(w) {
		case 0: strncpy(dBaseHeader.dBaseFields[0].Name,(LPCSTR)"Name",4); break;
		case 1: strncpy(dBaseHeader.dBaseFields[1].Name,(LPCSTR)"Vorname",7); break;
		case 2: strncpy(dBaseHeader.dBaseFields[2].Name,(LPCSTR)"Strasse",7); break;
		case 3: strncpy(dBaseHeader.dBaseFields[3].Name,(LPCSTR)"PLZ",3);
				  dBaseHeader.dBaseFields[w].Length=6; break;
		case 4: strncpy(dBaseHeader.dBaseFields[4].Name,(LPCSTR)"Ort",3); break;
		case 5: strncpy(dBaseHeader.dBaseFields[5].Name,(LPCSTR)"Telefon",7); break;
		case 6: strncpy(dBaseHeader.dBaseFields[6].Name,(LPCSTR)"Telefax",7); break;
		case 7: strncpy(dBaseHeader.dBaseFields[7].Name,(LPCSTR)"Information",11); break;
	 }
	 for (v=0;v< 4;v++) { dBaseHeader.dBaseFields[w].res1[v]=0; }
	 for (v=0;v<14;v++) { dBaseHeader.dBaseFields[w].Res2[v]=0; }
	 if (_lwrite(fh,&dBaseHeader.dBaseFields[w],32)!=32) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(FileName,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der Datenbank","Error",MB_ICONEXCLAMATION);
		} return;
  }} dBaseHeader.Res3[0]=0x0D;
  if (_lwrite(fh,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(FileName,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der Datenbank","Error",MB_ICONEXCLAMATION);
	 } return;
  } dBaseHeader.Res3[0]=0x1A;
  if (_lwrite(fh,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(FileName,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der Datenbank","Error",MB_ICONEXCLAMATION);
	 } return;
  } Handles.IsFileOpen=TRUE; Handles.File=fh; InitAdresse(); NewAdresse();
}

void DelAdresse(void)
{ HFILE fh = Handles.File;
  BYTE  Stat = 0x02A;

  dBaseHeader.DataBlocks--; _llseek(fh,4,0); _lwrite(fh,&dBaseHeader.DataBlocks,4);
  _llseek(fh,Handles.AktAdrOffset-1,0); _lwrite(fh,&Stat,1);
  if (Handles.AktAdr>1) { ReadAdresse(Prev,NULL,FALSE,TRUE,FALSE); } else {
	 if (dBaseHeader.DataBlocks==0) { NewAdresse(); } else { ReadAdresse(First,NULL,FALSE,TRUE,FALSE); }
  }
}

BOOL UpdateDataBase(BOOL Sort)
{ HFILE      th, fh = Handles.File;
  OFSTRUCT   of;
  dBaseField *Fields;
  char       string[255], string1[255], str[255], str1[255], TempFile[255];
  DWORD      Number, Number1, SmallestOfs, Ofs;
  WORD	    w, v;
  BOOL       FirstBlockReaden = FALSE;

  GetTempFileName(NULL,"ADR",0,string); lstrcpy(TempFile,(LPCSTR)string);
  if ((th=OpenFile(TempFile,&of,OF_CREATE))==HFILE_ERROR) {
	 sprintf(string,"Fehler Nr. %i beim Erstellen der temporären Datei",of.nErrCode);
	 MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION); return FALSE;
  }
  if (_lwrite(th,&dBaseHeader,32)!=32) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE;
  }
  for (w=0;w<dBaseHeader.FieldCount;w++) {
	 if (_lwrite(th,&dBaseHeader.dBaseFields[w],32)!=32) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		} return FALSE;
  }} dBaseHeader.Res3[0]=0x0D;
  if (_lwrite(th,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE; } Fields=new dBaseField[dBaseHeader.FieldCount]; _llseek(fh,32,0);
  for (w=0;w <dBaseHeader.FieldCount;w++) {
	 if (_lread(fh,&Fields[w],32)!=32) {
		MessageBox(Handles.MainDlg,"Fehler beim Lesen des alten Headers","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		} return FALSE;
  }} _llseek(fh,1,1);
  SetStatusPerCent(0);
  for (Number=0;Number<dBaseHeader.DataBlocks;Number++) {
	 if (!Sort) { SetStatusPerCent((BYTE)((Number*100)/dBaseHeader.DataBlocks)); }
		else { SetStatusPerCent((BYTE)((Number*50)/dBaseHeader.DataBlocks)); }
	 do { if (_lread(fh,&dBaseHeader.Res3[0],1)!=1) {
			  MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
			  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			  } return FALSE; }
			  if (dBaseHeader.Res3[0]==0x2A) { _llseek(fh,dBaseHeader.DataBlockSize-1,1); dBaseHeader.DataBlocks--; } else break;
	 } while(0); dBaseHeader.Res3[0]=0x20;
	 if (_lwrite(th,&dBaseHeader.Res3[0],1)!=1) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		} return FALSE; }
	 for (w=0;w<dBaseHeader.FieldCount;w++) {
		 if (_lread(fh,string,Fields[w].Length)!=Fields[w].Length) {
			MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
			if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
			  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			} return FALSE; }
		 if (dBaseHeader.dBaseFields[w].Length<=Fields[w].Length) {
			if (_lwrite(th,string,dBaseHeader.dBaseFields[w].Length)!=dBaseHeader.dBaseFields[w].Length) {
			  MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
			  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			  } return FALSE; }
		 } else {
			if (_lwrite(th,string,Fields[w].Length)!=Fields[w].Length) {
			  MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
			  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			  } return FALSE; } string[0]=0;
			for (v=0;v<dBaseHeader.dBaseFields[w].Length-Fields[w].Length;v++) {
			  if (_lwrite(th,&string[0],1)!=1) {
				 MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
				 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
					MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
				 } return FALSE; }
			}
		 }
	 }
  } dBaseHeader.Res3[0]=0x1A;
  if (_lwrite(th,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE; } v=1; for (w=0;w<dBaseHeader.FieldCount;w++) { v=v+dBaseHeader.dBaseFields[w].Length; }
  _llseek(th,10,0); _lwrite(th,&v,2); dBaseHeader.DataBlockSize=v;
  _llseek(th,4,0); _lwrite(th,&dBaseHeader.DataBlocks,4); delete[] Fields; //Alles aktualisiert
  if (fh) { _lclose(fh); }
  if ((fh=OpenFile(Handles.FileName,&of,OF_CREATE))==HFILE_ERROR) {
		sprintf(string,"Fehler Nr. %i beim Erstellen der aktualisierten Datei",of.nErrCode);
		MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION);
		return FALSE; } Handles.File=fh;
  if (!Sort) { _llseek(th,0L,0); _llseek(fh,0L,0);
	 do { w=_lread(th,string,255);
			if (_lwrite(fh,string,w)!=w) {
			  MessageBox(Handles.MainDlg,"Fehler beim Schreiben der aktualisierten Datei","Error",MB_ICONEXCLAMATION);
			  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			  } return FALSE; } if (w!=255) break; } while (1);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } if (!ReadHeader(Handles.FileName)) return FALSE; InitAdresse();
	 ReadAdresse(First,NULL,FALSE,TRUE,FALSE); return TRUE; }
  if (_lwrite(fh,&dBaseHeader,32)!=32) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE;
  }
  for (w=0;w<dBaseHeader.FieldCount;w++) {
	 if (_lwrite(fh,&dBaseHeader.dBaseFields[w],32)!=32) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		} return FALSE;
  }} dBaseHeader.Res3[0]=0x0D;
  if (_lwrite(fh,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben des Headers","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE; }
  for (Number=0;Number<dBaseHeader.DataBlocks;Number++) { //Alle Adressen durchlaufen
	 SetStatusPerCent(50+((BYTE)((Number*50)/dBaseHeader.DataBlocks)));
	 _llseek(th,dBaseHeader.HeaderSize,0); FirstBlockReaden=FALSE;
	 strnset(str,0,255); strnset(str1,0,255); strnset(string,0,255); strnset(string1,0,255);
	 for (Number1=0;Number1<dBaseHeader.DataBlocks;Number1++) { //kleinsten Wert suchen
		if (_lread(th,&dBaseHeader.Res3[0],1)!=1) {
		  MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
		  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
			 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		  } return FALSE; } Ofs=_llseek(th,0,1)-1;
		if (dBaseHeader.Res3[0]==0xFF) { _llseek(th,dBaseHeader.DataBlockSize-1,1); continue; }
		for (v=0;v<dBaseHeader.FieldCount;v++) {
		  if (v==Handles.SortFirst) { if (_lread(th,string,dBaseHeader.dBaseFields[v].Length)!=dBaseHeader.dBaseFields[v].Length) {
			 MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
			 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			 } return FALSE; } continue; }
		  if (v==Handles.SortSecond) { if (_lread(th,string1,dBaseHeader.dBaseFields[v].Length)!=dBaseHeader.dBaseFields[v].Length) {
			 MessageBox(Handles.MainDlg,"Fehler beim Lesen der Datensätze","Error",MB_ICONEXCLAMATION);
			 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			 } return FALSE; } continue; }
		  _llseek(th,dBaseHeader.dBaseFields[v].Length,1);
		}
		if (!FirstBlockReaden) {
		  lstrcpy(str,(LPCSTR)string); lstrcpy(str1,(LPCSTR)string1); SmallestOfs=Ofs; FirstBlockReaden=TRUE; }
		if (lstrcmp((LPCSTR)str,(LPCSTR)string)==0) {
		  if (lstrcmp((LPCSTR)str1,(LPCSTR)string1)>0) { lstrcpy(str,(LPCSTR)string); lstrcpy(str1,(LPCSTR)string1);	SmallestOfs=Ofs; }}
		if (lstrcmp((LPCSTR)str,(LPCSTR)string)>0) { lstrcpy(str,(LPCSTR)string); lstrcpy(str1,(LPCSTR)string1); SmallestOfs=Ofs; }
	 } _llseek(th,SmallestOfs,0); dBaseHeader.Res3[0]=0xFF; _lwrite(th,&dBaseHeader.Res3[0],1);
	 dBaseHeader.Res3[0]=0x20; _llseek(fh,dBaseHeader.HeaderSize+(Number*dBaseHeader.DataBlockSize),0);
	 if (_lwrite(fh,&dBaseHeader.Res3[0],1)!=1) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		} return FALSE; }
	 for (w=0;w<dBaseHeader.FieldCount;w++) {
		strnset(str1,0,255);	_lread(th,str1,dBaseHeader.dBaseFields[w].Length);
		if (_lwrite(fh,str1,dBaseHeader.dBaseFields[w].Length)!=dBaseHeader.dBaseFields[w].Length) {
		  MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
		  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
			 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		  } return FALSE; }
	 }
  } dBaseHeader.Res3[0]=0x1A;
  if (_lwrite(fh,&dBaseHeader.Res3[0],1)!=1) {
	 MessageBox(Handles.MainDlg,"Fehler beim Schreiben der Datensätze","Error",MB_ICONEXCLAMATION);
	 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
	 } return FALSE; }
  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
	 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
  }
  if (!ReadHeader(Handles.FileName)) return FALSE; InitAdresse(); ReadAdresse(First,NULL,FALSE,TRUE,FALSE);
  return TRUE;
}

void Temp2DataBase(char *TempFile,BOOL Restore)
{ HFILE      th, fh = Handles.File;
  OFSTRUCT   of;
  DWORD      Offset = _llseek(fh,0,1);
  WORD       w, f;
  char       string[255];

  if (Restore==TRUE) {
	 if ((th=OpenFile(TempFile,&of,OF_READWRITE))==HFILE_ERROR) {
		sprintf(string,"Fehler Nr. %i beim Öffnen der temporären Datei",of.nErrCode);
		MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION);
		return; }
	 if (fh) { _lclose(fh); }
	 if ((fh=OpenFile(Handles.FileName,&of,OF_CREATE))==HFILE_ERROR) {
		sprintf(string,"Fehler Nr. %i beim Erstellen der wiederhergestellten Datei",of.nErrCode);
		MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION);
		return; } Handles.File=fh; _llseek(th,0,0);
	 for (f=0;f==1;f=0) {
		  w=_lread(th,string,255);
		  if (_lwrite(fh,string,w)!=w) {
			 MessageBox(Handles.MainDlg,"Fehler beim Schreiben der aktualisierten Datei","Error",MB_ICONEXCLAMATION);
			 if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
				MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
			 } return; } if (w!=255) break; } _llseek(fh,Offset,0); }
  if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
	 MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
  }
}

void AktDataBase(BOOL Sort)
{ HFILE      th, fh = Handles.File;
  OFSTRUCT   of;
  char       string[255], TempFile[255];
  DWORD      Offset = _llseek(fh,0,1);
  WORD	    w, f;

  GetTempFileName(NULL,"ADR",0,TempFile);
  if ((th=OpenFile(TempFile,&of,OF_CREATE))==HFILE_ERROR) {
	 sprintf(string,"Fehler Nr. %i beim Erstellen der temporären Datei",of.nErrCode);
	 MessageBox(Handles.MainDlg,(LPCSTR)string,"Error",MB_ICONEXCLAMATION); _llseek(fh,Offset,0);
	 return; } _llseek(fh,0,0);
  for (f=0;f==1;f=0) {
	 w=_lread(fh,string,255);
	 if (_lwrite(th,string,w)!=w) {
		MessageBox(Handles.MainDlg,"Fehler beim Schreiben der aktualisierten Datei","Error",MB_ICONEXCLAMATION);
		if ((OpenFile(TempFile,&of,OF_DELETE))==HFILE_ERROR) {
		  MessageBox(Handles.MainDlg,"Fehler beim Löschen der temporären Datei","Error",MB_ICONEXCLAMATION);
		  } return; } if (w!=255) break; } _llseek(fh,Offset,0);
  ShowStatusDlg();
  if (!UpdateDataBase(Sort)) {
	 if (MessageBox(Handles.MainDlg,"Die Datenbank wurde nicht ordnungsgemäß aktualisiert! Soll versucht werden, sie wiederherzustellen?",
		  "Error",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES) Temp2DataBase(TempFile,TRUE);
  } else { Temp2DataBase(TempFile,FALSE); } HideStatusDlg(); _lclose(th);
}
