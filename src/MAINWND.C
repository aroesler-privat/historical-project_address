void FileMenu(void)
{ RECT rc;

  Handles.Kontext=CreatePopupMenu();
  AppendMenu(Handles.Kontext,MF_ENABLED,IDC_FileNew ,"Datei &neu\tStrg+N");
  AppendMenu(Handles.Kontext,MF_ENABLED,IDC_FileOpen,"Datei &öffnen\tStrg+O");
  AppendMenu(Handles.Kontext,MF_SEPARATOR,NULL,NULL);
  if (!Handles.IsFileOpen|Handles.ReadOnly) { AppendMenu(Handles.Kontext,MF_GRAYED,IDC_Quality,"&Feldeigenschaften\tStrg+F"); }
	 else { AppendMenu(Handles.Kontext,MF_ENABLED,IDC_Quality,"&Feldeigenschaften\tStrg+F"); }
  if (!Handles.IsFileOpen|Handles.ReadOnly) { AppendMenu(Handles.Kontext,MF_GRAYED,IDC_AdrSort,"&Sortieren nach\tStrg+S"); }
	 else { AppendMenu(Handles.Kontext,MF_ENABLED,IDC_AdrSort,"&Sortieren nach\tStrg+S"); }
  if (Handles.IsFileOpen) { AppendMenu(Handles.Kontext,MF_ENABLED,IDC_FileInfos,"&Informationen\tStrg+I"); }
	 else { AppendMenu(Handles.Kontext,MF_GRAYED,IDC_FileInfos,"&Informationen\tStrg+I"); }
  GetWindowRect(Handles.MainDlg,&rc);
  TrackPopupMenu(Handles.Kontext,TPM_RIGHTALIGN,rc.right,rc.top+61,0,Handles.MainDlg,NULL);
  DestroyMenu(Handles.Kontext);
}

void AdresseMenu(UINT flags)
{ RECT rc;

  Handles.Kontext=CreatePopupMenu();
  if (Handles.ReadOnly) AppendMenu(Handles.Kontext,flags|MF_GRAYED,IDC_AdrNew,"&Neu\tAlt+Einfg");
	 else AppendMenu(Handles.Kontext,flags,IDC_AdrNew,"&Neu\tAlt+Einfg");
  if (Handles.ReadOnly) AppendMenu(Handles.Kontext,flags|MF_GRAYED,IDC_AdrDelete,"&Löschen\tAlt+Entf");
	 else AppendMenu(Handles.Kontext,flags,IDC_AdrDelete,"&Löschen\tAlt+Entf");
  AppendMenu(Handles.Kontext,flags,IDC_AdrSave,"&Speichern\tAlt+S");
  AppendMenu(Handles.Kontext,flags,MF_SEPARATOR,NULL);
  if (Handles.ReadOnly) { AppendMenu(Handles.Kontext,flags|MF_CHECKED,IDC_AdrReadOnly,"N&ur lesen\tAlt+L"); }
	 else { AppendMenu(Handles.Kontext,flags,IDC_AdrReadOnly,"N&ur lesen\tAlt+L"); }
  GetWindowRect(Handles.MainDlg,&rc);
  TrackPopupMenu(Handles.Kontext,TPM_RIGHTALIGN,rc.right,rc.top+94,0,Handles.MainDlg,NULL);
  DestroyMenu(Handles.Kontext);
}

void ReDraw(BOOL Undo)
{ LPSTR Buffer;

  if (Adresse.OfsName!=32000) { Buffer=(char *)GlobalLock(Adresse.Name);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Name,Buffer,Adresse.LenName); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Name,Buffer); } GlobalUnlock(Adresse.Name);  }
  if (Adresse.OfsVorname!=32000) { Buffer=(char *)GlobalLock(Adresse.Vorname);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Vorname,Buffer,Adresse.LenVorname); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Vorname,Buffer); } GlobalUnlock(Adresse.Vorname);  }
  if (Adresse.OfsStrasse!=32000) { Buffer=(char *)GlobalLock(Adresse.Strasse);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Strasse,Buffer,Adresse.LenStrasse); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Strasse,Buffer); } GlobalUnlock(Adresse.Strasse);  }
  if (Adresse.OfsPLZ!=32000) { Buffer=(char *)GlobalLock(Adresse.PLZ);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_PLZ,Buffer,Adresse.LenPLZ); }
		else { SetDlgItemText(Handles.MainDlg,IDC_PLZ,Buffer); } GlobalUnlock(Adresse.PLZ);  }
  if (Adresse.OfsOrt!=32000) { Buffer=(char *)GlobalLock(Adresse.Ort);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Ort,Buffer,Adresse.LenOrt); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Ort,Buffer); } GlobalUnlock(Adresse.Ort);  }
  if (Adresse.OfsTelefon!=32000) { Buffer=(char *)GlobalLock(Adresse.Telefon);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Telefon,Buffer,Adresse.LenTelefon); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Telefon,Buffer); } GlobalUnlock(Adresse.Telefon);  }
  if (Adresse.OfsTelefax!=32000) { Buffer=(char *)GlobalLock(Adresse.Telefax);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Telefax,Buffer,Adresse.LenTelefax); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Telefax,Buffer); } GlobalUnlock(Adresse.Telefax);  }
  if (Adresse.OfsInfos!=32000) { Buffer=(char *)GlobalLock(Adresse.Informationen);
	 if (!Undo) { GetDlgItemText(Handles.MainDlg,IDC_Info,Buffer,Adresse.LenInfos); }
		else { SetDlgItemText(Handles.MainDlg,IDC_Info,Buffer); } GlobalUnlock(Adresse.Informationen);  }
}

#pragma argsused
BOOL CALLBACK MainDlgProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{ switch(msg){
	 case WM_INITDIALOG: Handles.MainDlg=hwnd; ShowWindow(hwnd,SW_SHOW); return TRUE;
	 case WM_CLOSE: if (Handles.IsFileOpen) { ReDraw(FALSE); WriteAdresse(OverWrite);}
						 SendMessage(hwnd,WM_QUIT,0,0L); return FALSE;
	 case WM_COMMAND: switch(GET_WM_COMMAND_ID(wp,lp)){
		case IDCANCEL: Handles.ExitApp=TRUE; DestroyWindow(hwnd); return FALSE;
		case IDC_SearchAgain: if (strlen(Handles.SearchStr)!=0) ReadAdresse(Search,Handles.SearchStr,FALSE,TRUE,FALSE); 
									 return FALSE;
		case IDC_File: FileMenu(); return FALSE;
				case IDC_FileNew    : if (Handles.IsFileOpen) { ReDraw(FALSE); WriteAdresse(OverWrite);}
											 NewFile(); SetFocus(GetDlgItem(Handles.MainDlg,IDC_Name)); return FALSE;
				case IDC_FileOpen   : if (Handles.IsFileOpen) { ReDraw(FALSE); WriteAdresse(OverWrite);}
											 GetFileName(TRUE); SetFocus(GetDlgItem(Handles.MainDlg,IDC_Name)); return FALSE;
				case IDC_Quality    : ReDraw(FALSE); WriteAdresse(OverWrite); ShowQualityDlg();
											 return FALSE;
				case IDC_AdrSort    : if (!Handles.IsFileOpen) return FALSE;
											 ReDraw(FALSE); WriteAdresse(OverWrite);
											 ShowSortDlg(); return FALSE;
				case IDC_FileInfos  : if (!Handles.IsFileOpen) return FALSE;
											 ShowInfoDlg(); return FALSE;
		case IDC_Adresse: if (Handles.IsFileOpen) { AdresseMenu(MF_ENABLED); } else { AdresseMenu(MF_GRAYED); } return FALSE;
				case IDC_AdrNew     : if (!Handles.IsFileOpen) return FALSE;
											 if (dBaseHeader.DataBlocks!=0) { ReDraw(FALSE); WriteAdresse(OverWrite); }
											 NewAdresse(); SetFocus(GetDlgItem(Handles.MainDlg,IDC_Name)); return FALSE;
				case IDC_AdrSave    : if (!Handles.IsFileOpen) return FALSE;
											 ReDraw(FALSE); WriteAdresse(OverWrite); return FALSE;
				case IDC_AdrDelete  : if (!Handles.IsFileOpen) return FALSE;
											 DelAdresse(); return FALSE;
				case IDC_AdrReadOnly: Handles.ReadOnly=!Handles.ReadOnly; SetAdresseOnOff(!Handles.ReadOnly);
											 return FALSE;
		case IDC_Print : if (Handles.IsFileOpen) {
								 ReDraw(FALSE); ShowPrintDlg(); } return FALSE;
		case IDC_Search: if (!Handles.IsFileOpen) return FALSE; ReDraw(FALSE); WriteAdresse(OverWrite);
							  ShowSuchDlg(); return FALSE;
		case IDC_First : if (!Handles.IsFileOpen) return FALSE;
							  ReDraw(FALSE); WriteAdresse(OverWrite); ReadAdresse(First,NULL,FALSE,TRUE,FALSE); return FALSE;
		case IDC_Left  : if ((!Handles.IsFileOpen)||(Handles.AktAdr==1)) return FALSE;
							  ReDraw(FALSE); WriteAdresse(OverWrite); ReadAdresse(Prev ,NULL,FALSE,TRUE,FALSE); return FALSE;
		case IDC_Last  : if (!Handles.IsFileOpen) return FALSE;
							  ReDraw(FALSE); WriteAdresse(OverWrite); ReadAdresse(Last ,NULL,FALSE,TRUE,FALSE); return FALSE;
		case IDC_Right : if ((!Handles.IsFileOpen)||(Handles.AktAdr==dBaseHeader.DataBlocks)) return FALSE;
							  ReDraw(FALSE); WriteAdresse(OverWrite); ReadAdresse(Next ,NULL,FALSE,TRUE,FALSE); return FALSE;
		case IDC_Exit  : SendMessage(hwnd,WM_CLOSE,0,0L); return FALSE;
	 } return FALSE;
	 default: return FALSE;
  }
}


