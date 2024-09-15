#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <print.h>
#include <dos.h>
#include "adresse.rh"
#pragma hdrstop
#include "structs.c"         // Datenstrukturen
#include "statdlg.c"         // Dialog zum Darstellen diverser Statusinformationen
#include "file_io.c"         // Funktionen zum Lesen und Schreiben der Adressen
#include "database.c"        // Funktionen zur Arbeit mit der Datenbank
#include "suchdlg.c"         // Dialog zum Suchen best. Feldeinträge
#include "qualidlg.c"        // Größe der Felder einer Datenbank ändern
#include "infodlg.c"         // Dialog mit Informationen über die aktive Datenbank
#include "sortdlg.c"         // Dialog + Algorythmen zum Sortieren der Daten
#include "print.c"           // Dialog + Algorythmen zum Drucken der Daten
#include "mainwnd.c"         // CALLBACK für den Dialog

#pragma argsused
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR CmdLine,int PrgStart)
{ MSG msg;

  Handles.ExitApp=FALSE; Handles.IsFileOpen=FALSE; Handles.hInst=hInst;
  Handles.SortFirst=0; Handles.SortSecond=1; strnset(Handles.SearchStr,0,255);
  if ((Handles.hAccel=LoadAccelerators(Handles.hInst,MAKEINTRESOURCE(AdrAccel)))==NULL) {
	 MessageBox(Handles.MainDlg,"Fehler beim Initialisieren der Variablen aufgetreten","Error",MB_ICONEXCLAMATION);
	 return 0; }
  CreateDialog(Handles.hInst,MAKEINTRESOURCE(MainDialog),NULL,(DLGPROC)MainDlgProc);
  if (!AssignAdresse(FALSE)) {
	 MessageBox(Handles.MainDlg,"Fehler beim Initialisieren der Variablen aufgetreten","Error",MB_ICONEXCLAMATION);
	 EndDialog(Handles.MainDlg,0); return 0; }
  while (GetMessage(&msg,NULL,0,0)) {
	 if (Handles.MainDlg&&IsDialogMessage(Handles.MainDlg,&msg)) {
		if (Handles.ExitApp==TRUE) return 0;
		TranslateAccelerator(Handles.MainDlg,Handles.hAccel,&msg);
	 }
  }
  return msg.wParam;
}