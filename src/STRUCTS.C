struct {
  HINSTANCE   hInst;
  HWND        MainDlg, StatusWnd;
  HDC         StatusHDC;
  PAINTSTRUCT StatusPs;
  RECT        StatusRect;
  HMENU       Kontext;
  HACCEL		  hAccel;
  HFILE       File;
  BOOL        ExitApp, IsFileOpen, ReadOnly;
  LONG        AktAdr, AktAdrOffset;
  char        FileName[255], SearchStr[255];
  BOOL        *SearchIn;
  WORD        SortFirst, SortSecond, LastPerCent;
} Handles;

struct {
  HANDLE Name, Vorname, Strasse, PLZ, Ort, Telefon, Telefax, Informationen;
  WORD   OfsName, OfsVorname, OfsStrasse, OfsPLZ, OfsOrt, OfsTelefon, OfsTelefax, OfsInfos;
  WORD   LenName, LenVorname, LenStrasse, LenPLZ, LenOrt, LenTelefon, LenTelefax, LenInfos;
  DWORD  Number;
} Adresse;

struct dBaseField {
  char Name[11], type, res1[4];
  BYTE Length, Komma, Res2[14], MDX;
};

struct {
  BYTE       Version, Date[3];
  DWORD      DataBlocks;
  WORD       HeaderSize, DataBlockSize, Res1;
  BYTE       UnStopped, Coded, Res2[12], MDX, Res3[3];
  WORD       FieldCount;
  dBaseField *dBaseFields;
} dBaseHeader;

//Adresse einlesen
#define Next      1
#define Prev      2
#define First     3
#define Last      4
#define Search    5
//Adresse abspeichern
#define New       FALSE
#define OverWrite TRUE
