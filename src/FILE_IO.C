BOOL CmpStr(char *s1,char *s2,BYTE Len,BOOL SubString)
{ BYTE    b, b1 = 0;

  for (b=0;b<Len;b++) {
	 if (SubString) {
		if ((s2[b1]=='?')|(AnsiUpper((LPSTR)s1[b])!=AnsiUpper((LPSTR)s2[b1]))) b1++;
		if ((s2[b1]!='?')&(AnsiUpper((LPSTR)s1[b])!=AnsiUpper((LPSTR)s2[b1]))) b1=0;
		if (s2[b1]=='*') b1=strlen(s2);
		if (b1==strlen(s2)-1) return TRUE;
	 } else {
		if ((s2[b]!='?')&(AnsiUpper((LPSTR)s1[b])!=AnsiUpper((LPSTR)s2[b]))) return FALSE;
		if (s2[b]=='*') return TRUE;
	 }
  } return !SubString;
}

BOOL ReadAdresse(BYTE Flags,char *SearchFor,BOOL Begin,BOOL Show,BOOL SubString)
{ HFILE       fh = Handles.File;
  BYTE        Stat;
  LPSTR       Buffer;
  DWORD       Number, Offset;
  WORD        w, Field;
  char        SearchStr[255];
  BOOL        Found;

  if (!Handles.IsFileOpen) return NULL; _llseek(fh,Handles.AktAdrOffset,0);
  switch(Flags) {
	 case 1: do { if (_llseek(fh,dBaseHeader.DataBlockSize-1,1)==HFILE_ERROR) return FALSE;
					  if (_lread(fh,&Stat,1)!=1) return FALSE; if (Stat!=0x02A) break;
				} while(1); Handles.AktAdr++; break;
	 case 2: do { if (_llseek(fh,_llseek(fh,0L,1)-dBaseHeader.DataBlockSize-1,0)==HFILE_ERROR) return FALSE;
					  if (_lread(fh,&Stat,1)!=1) return FALSE; if (Stat!=0x02A) break;
				} while(1); Handles.AktAdr--; break;
	 case 3: if (dBaseHeader.DataBlocks==0) return FALSE; _llseek(fh,dBaseHeader.HeaderSize,0);
				do { if (_lread(fh,&Stat,1)!=1) return FALSE; if (Stat!=0x02A) break;
					  if (Stat==0x02A) {	if (_llseek(fh,dBaseHeader.DataBlockSize-1,1)==HFILE_ERROR) return FALSE; } else break;
				} while(1); Handles.AktAdr=1; break;
	 case 4: if (dBaseHeader.DataBlocks==0) return FALSE; _llseek(fh,dBaseHeader.HeaderSize,0); Number=0;
				do { if (_lread(fh,&Stat,1)!=1) return FALSE; if (Stat!=0x2A) Number++;
					  if (Number==dBaseHeader.DataBlocks) break;
					  if (_llseek(fh,dBaseHeader.DataBlockSize-1,1)==HFILE_ERROR) return FALSE;
				} while(1); Handles.AktAdr=dBaseHeader.DataBlocks; break;
	 case 5: if (Begin) { _llseek(fh,dBaseHeader.HeaderSize,0); Offset=0; } else
							  { if (Handles.AktAdr==dBaseHeader.DataBlocks) {
									_llseek(fh,Handles.AktAdrOffset-1,0);
									Offset=Handles.AktAdr-1;
								 } else {
									_llseek(fh,Handles.AktAdrOffset-1+dBaseHeader.DataBlockSize,0);
									Offset=Handles.AktAdr;
								 }}
				ShowStatusDlg(); SetStatusPerCent(0);
				for (Number=Offset;Number<dBaseHeader.DataBlocks;Number++) {
				  SetStatusPerCent((BYTE)(Number*100/dBaseHeader.DataBlocks));
				  do { if (_lread(fh,&Stat,1)!=1) return FALSE; if (Stat!=0x02A) break;
						 if (_llseek(fh,dBaseHeader.DataBlockSize-1,1)==HFILE_ERROR) {
							HideStatusDlg(); return FALSE; }
				  } while(1); Offset=_llseek(fh,0,1); Found=FALSE;
				  for (w=0;w<dBaseHeader.FieldCount;w++) {
					 if (_lread(fh,SearchStr,dBaseHeader.dBaseFields[w].Length)!=dBaseHeader.dBaseFields[w].Length) {
						HideStatusDlg(); return FALSE; }
					 if (!Handles.SearchIn[w]) { continue; }
					 if (SubString) {
						if (CmpStr(SearchStr,SearchFor,dBaseHeader.dBaseFields[w].Length,TRUE)) { Found=TRUE; break; }
					 } else {
						if (CmpStr(SearchStr,SearchFor,strlen(SearchFor),FALSE)) { Found=TRUE; break; }
					 }
				  } if (Found) break; }
				HideStatusDlg(); Field=w;
				if (!Found) {
				  MessageBox(Handles.MainDlg,"Keine übereinstimmende Zeichenkette gefunden","suchen nach",MB_ICONEXCLAMATION);
				  _llseek(fh,Handles.AktAdrOffset,0); return FALSE; }
				Handles.AktAdr=Number+1; _llseek(fh,Offset,0);
  } Handles.AktAdrOffset=_llseek(fh,0,1); //Offset der aktuellen Adresse
  if (Show) {
	 SetDlgItemText(Handles.MainDlg,IDC_Vorname,""); SetDlgItemText(Handles.MainDlg,IDC_Name,"");
	 SetDlgItemText(Handles.MainDlg,IDC_Vorname,""); SetDlgItemText(Handles.MainDlg,IDC_Ort ,"");
	 SetDlgItemText(Handles.MainDlg,IDC_Strasse,""); SetDlgItemText(Handles.MainDlg,IDC_PLZ ,"");
	 SetDlgItemText(Handles.MainDlg,IDC_Telefon,""); SetDlgItemText(Handles.MainDlg,IDC_Info,"");
	 SetDlgItemText(Handles.MainDlg,IDC_Telefax,"");
  }
  if (Adresse.OfsName!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Name); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsName,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenName); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Name,Buffer); } GlobalUnlock(Adresse.Name);  }
  if (Adresse.OfsVorname!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Vorname); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsVorname,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenVorname); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Vorname,Buffer); } GlobalUnlock(Adresse.Vorname);  }
  if (Adresse.OfsStrasse!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Strasse); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsStrasse,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenStrasse); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Strasse,Buffer); } GlobalUnlock(Adresse.Strasse);  }
  if (Adresse.OfsPLZ!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.PLZ); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsPLZ,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenPLZ); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_PLZ,Buffer); } GlobalUnlock(Adresse.PLZ);  }
  if (Adresse.OfsOrt!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Ort); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsOrt,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenOrt); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Ort,Buffer); } GlobalUnlock(Adresse.Ort);  }
  if (Adresse.OfsTelefon!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Telefon); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsTelefon,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenTelefon); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Telefon,Buffer); } GlobalUnlock(Adresse.Telefon);  }
  if (Adresse.OfsTelefax!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Telefax); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsTelefax,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenTelefax); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Telefax,Buffer); } GlobalUnlock(Adresse.Telefax); }
  _llseek(fh,Handles.AktAdrOffset,0);
  if (Adresse.OfsInfos!=32000) {
	 Buffer=(char *)GlobalLock(Adresse.Informationen); _llseek(fh,Handles.AktAdrOffset+Adresse.OfsInfos,0);
	 _lread(fh,Buffer,(UINT)Adresse.LenInfos); OemToAnsi(Buffer,Buffer);
	 if (Show) { SetDlgItemText(Handles.MainDlg,IDC_Info,Buffer); } GlobalUnlock(Adresse.Informationen); }
  _llseek(fh,Handles.AktAdrOffset,0);
  if (Handles.AktAdr==1) {
	 EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Left),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_First),FALSE);
	 if (dBaseHeader.DataBlocks==1) { SetFocus(GetDlgItem(Handles.MainDlg,IDC_Search)); }
		else { SetFocus(GetDlgItem(Handles.MainDlg,IDC_Right)); }
  } else { EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Left),TRUE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_First),TRUE); }
  if (Handles.AktAdr==dBaseHeader.DataBlocks) {
	 EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Right),FALSE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Last),FALSE);
	 if (Handles.AktAdr==1) { SetFocus(GetDlgItem(Handles.MainDlg,IDC_Search)); }
		else { SetFocus(GetDlgItem(Handles.MainDlg,IDC_Left)); }
  } else { EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Right),TRUE); EnableWindow(GetDlgItem(Handles.MainDlg,IDC_Last),TRUE); }
  sprintf(Buffer,"%i",Handles.AktAdr); SetDlgItemText(Handles.MainDlg,IDC_AdrNr,Buffer);
  if (Flags==5) {
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"NAME",4)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Name));
		SendDlgItemMessage(Handles.MainDlg,IDC_Name,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"VORNAME",7)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Vorname));
		SendDlgItemMessage(Handles.MainDlg,IDC_Vorname,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"STRASSE",7)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Strasse));
		SendDlgItemMessage(Handles.MainDlg,IDC_Strasse,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"PLZ",3)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_PLZ));
		SendDlgItemMessage(Handles.MainDlg,IDC_PLZ,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"ORT",3)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Ort));
		SendDlgItemMessage(Handles.MainDlg,IDC_Ort,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"TELEFON",7)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Telefon));
		SendDlgItemMessage(Handles.MainDlg,IDC_Telefon,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"TELEFAX",7)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Telefax));
		SendDlgItemMessage(Handles.MainDlg,IDC_Telefax,EM_SETSEL,0,MAKELONG(0,-1)); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[Field].Name),"INFORMATION",11)==0) {
		SetFocus(GetDlgItem(Handles.MainDlg,IDC_Info));
		SendDlgItemMessage(Handles.MainDlg,IDC_Info,EM_SETSEL,0,MAKELONG(0,-1)); }
  }
  return TRUE;
}

BOOL WriteAdresse(BOOL Flags)
{ HFILE fh = Handles.File;
  LPSTR Buffer;
  DWORD Count;
  BYTE  Stat;
  WORD  w;

  if (Flags) { _llseek(fh,Handles.AktAdrOffset-1,0); } else { Count=1;
	 do { _llseek(fh,_llseek(fh,0,2)-Count,0); if (_lread(fh,&Stat,1)!=1) return FALSE;
			if (Stat==0x1A) { break; } else { Count++; }
	 } while(1); _llseek(fh,_llseek(fh,0,2)-Count,0); }
  Stat=0x020; if (_lwrite(fh,&Stat,1)!=1) { _llseek(fh,Handles.AktAdrOffset,0); return FALSE; }
  Handles.AktAdrOffset=_llseek(fh,0,1);
  for (w=0;w<dBaseHeader.FieldCount;w++) {
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"NAME",4)==0) {
		Buffer=(char *)GlobalLock(Adresse.Name); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenName); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Name); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"VORNAME",7)==0) {
		Buffer=(char *)GlobalLock(Adresse.Vorname); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenVorname); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Vorname); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"STRASSE",7)==0) {
		Buffer=(char *)GlobalLock(Adresse.Strasse); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenStrasse); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Strasse); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"PLZ",3)==0) {
		Buffer=(char *)GlobalLock(Adresse.PLZ); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenPLZ); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.PLZ); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"ORT",3)==0) {
		Buffer=(char *)GlobalLock(Adresse.Ort); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenOrt); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Ort); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFON",7)==0) {
		Buffer=(char *)GlobalLock(Adresse.Telefon); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenTelefon); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Telefon); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"TELEFAX",7)==0) {
		Buffer=(char *)GlobalLock(Adresse.Telefax); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenTelefax); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Telefax); }
	 if (strncmp(AnsiUpper(dBaseHeader.dBaseFields[w].Name),"INFORMATION",11)==0) {
		Buffer=(char *)GlobalLock(Adresse.Informationen); AnsiToOem(Buffer,Buffer);
		_lwrite(fh,Buffer,(UINT)Adresse.LenInfos); OemToAnsi(Buffer,Buffer); GlobalUnlock(Adresse.Informationen); }
  }
  if (!Flags) {
	 Stat=0x01A; _lwrite(fh,&Stat,1); _llseek(fh,4,0); dBaseHeader.DataBlocks++;
	 _llseek(fh,4,0); _lwrite(fh,&dBaseHeader.DataBlocks,4); Handles.AktAdr=dBaseHeader.DataBlocks;
	 _llseek(fh,Handles.AktAdrOffset,0); ReadAdresse(Last,NULL,FALSE,TRUE,FALSE);
  }
  _llseek(fh,Handles.AktAdrOffset,0); return TRUE;
}
