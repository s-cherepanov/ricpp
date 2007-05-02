// logfile.cpp

#include "stdafx.h"

#include <time.h>
#include "logfile.h"

/*
* time2asc()
* ----------------------------------------------------------------------------
*   Konvertiert einen time_t Wert in einen entsprechenden
*   String ("yyyy-mm-dd hh:mm:ss"), verwendet localtime()
*
* Parameter
* ----------------------------------------------------------------------------
*   buf: Rueckgabe-Puffer (Platz fuer mindestens 20 Byte)
*   t  : Zeit, die umgewandelt werden soll
*
* Rueckgabe
* ----------------------------------------------------------------------------
*   -1: Fehler
*    t: OK
*/
time_t time2asc(char *buf, time_t t)
{
	struct tm *tm;

	if ( !buf || t <= 0 ) return -1;

	tm = localtime(&t);

	if ( tm )
	{
		sprintf((LPSTR)buf, "%04d-%02d-%02d %02d:%02d:%02d",
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
		return t;
	}

	return -1;
}


/*
* TLogfile
* ============================================================================
*   Behandelt eine Protokolldatei, ist keine Datei geoeffnet, ist das
*   Protokollieren deaktiviert (die Funktionen koennen trotzdem aufgerufen
*   werden)
*/


/*
* TLogfile
* ----------------------------------------------------------------------------
*   Konstruktor der Protokolldatei, oeffnet bei vorhandenen Dateinamen
*   die Protokolldatei
*
* Parameter
* ----------------------------------------------------------------------------
*   szFilename : Name der Protokolldatei (optional)
*   szModulName: Name des Moduls (z.B. CIMPORT)
*   szError    : Fehlermarkierung (Def.: ***) - steht in jeder Fehlerzeile
*/
TLogfile::TLogfile(const char *szFilename, const char *szModuleName, bool append,
                   const char *szPrefix, const char *szError)
{
	m_strFilename   = "";
	m_strModuleName = szModuleName ? szModuleName : "";
	m_strPrefix     = szPrefix ? szPrefix : "";
	m_strError      = szError ? szError : "";
	m_pfhLogfile    = NULL;
	m_dwLineNo      = 0;
	m_bIsValid      = 1;

	if ( szFilename && *szFilename )
	{
		// Oeffnen der Logdatei, setzt m_strFilename
		OpenNewFile(szFilename, append);
		m_bIsValid = m_pfhLogfile != NULL;
	}
}


/*
* ~TLogfile()
* ----------------------------------------------------------------------------
*   Destruktor, schliesst die Protokolldatei
*/
TLogfile::~TLogfile()
{
	Close();
}


/*
* IsValid(), GetReportLogFile(), GetLineNo(), GetFilename()
* getModuleName(), GetError()
* ----------------------------------------------------------------------------
*   Liefert die entsprechenden Members
*/
int         TLogfile::IsValid()          { return m_bIsValid;      }
FILE *      TLogfile::GetReportLogfile() { return m_pfhLogfile;    }
long        TLogfile::GetLineNo()        { return m_dwLineNo;      }
const char *TLogfile::GetFilename()      { return m_strFilename.c_str();   }
const char *TLogfile::GetModuleName()    { return m_strModuleName.c_str(); }
const char *TLogfile::GetError()         { return m_strError.c_str();      }
const char *TLogfile::GetPrefix()        { return m_strPrefix.c_str();      }


/*
* OpenNewFile()
* ----------------------------------------------------------------------------
*   Oeffnet eine neue Protokolldatei, schlieﬂt vorher bei Bedarf die alte.
*
* Parameter
* ----------------------------------------------------------------------------
*   szFilename: Name der Protokolldatei, ist der Dateiname leer, wird
*               das Protokollieren abgeschaltet
*
* Rueckgabe
* ----------------------------------------------------------------------------
*   0: Datei konnte nicht geoeffnet werden
*/
int TLogfile::OpenNewFile(const char *szFilename, bool append)
{
	Close();

	m_pfhLogfile = NULL;
	m_bIsValid = 1;
	m_strFilename = szFilename ? szFilename : "";
	if ( szFilename && *szFilename )
	{
		m_pfhLogfile = fopen(szFilename, append ? "a" : "w");
		m_bIsValid = m_pfhLogfile != NULL;
	}

	return m_bIsValid;
}


/*
* ReportLogHeader()
* ----------------------------------------------------------------------------
*   Schreibt den Zeilenvorspann mit einem Blank am Ende,
*   die Aktion ist optional:
*   yyyy-mm-dd hh:mm:ss nnnnnn Modulname Aktion
*
* Parameter
* ----------------------------------------------------------------------------
*   szAction: Name der Aktion (optional)
*/
void TLogfile::ReportLogHeader(const char *szAction, bool endLine)
{
	char szBuf[32] = "0000-00-00 00:00:00";

	if ( !m_pfhLogfile ) return;

	time2asc(szBuf, time(NULL));
	fprintf(m_pfhLogfile, "%s%s%s %06ld %s ", endLine ? "\n" : "", m_strPrefix.c_str(),
		szBuf, ++m_dwLineNo, m_strModuleName.c_str());

	if ( szAction && *szAction )
	{
		fputs(szAction, m_pfhLogfile);
		if ( szAction[strlen(szAction)-1] != '\n' )
			fputs(" ", m_pfhLogfile);
	}

	fflush(m_pfhLogfile);
}


/*
* ReportErrorHeader()
* ----------------------------------------------------------------------------
*   Schreibt den Zeilenvorspann mit einem Blank am Ende,
*   der Fehlername ist optional:
*   yyyy-mm-dd hh:mm:ss nnnnnn Modulname ***Fehlername
*
* Parameter
* ----------------------------------------------------------------------------
*   szErrorName: Name des Fehlers (optional)
*/
void TLogfile::ReportErrorHeader(const char *szErrorName)
{
	char szBuf[32] = "0000-00-00 00:00:00";

	if ( !m_pfhLogfile ) return;

	time2asc(szBuf, time(NULL));
	fprintf(m_pfhLogfile, "\n%s%s %06ld %s %s",
		m_strPrefix.c_str(), szBuf, ++m_dwLineNo, m_strModuleName.c_str(),
		m_strError.c_str());

	if ( szErrorName && *szErrorName )
	{
		fputs(szErrorName, m_pfhLogfile);
		if ( szErrorName[strlen(szErrorName)-1] != '\n' )
			fputs(" ", m_pfhLogfile);
	}

	fflush(m_pfhLogfile);
}


/*
* ReportLogStart()
* ----------------------------------------------------------------------------
*   Blockstart-Zeile ausgeben
*
* Parameter
* ----------------------------------------------------------------------------
*   szObjectName: Blockname
*   szComment   : optionaler Kommentar
*   szStart     : Start-Kennung (START)
*/
void TLogfile::ReportLogStart(const char *szObjectName,
                              const char *szComment, const char *szStart)
{
	if ( !m_pfhLogfile ) return;

	ReportLogHeader(szStart, false);
	fputs(szObjectName ? szObjectName : "", m_pfhLogfile);
	if ( szComment && *szComment )
	{
		if ( szObjectName && *szObjectName )
			fputc((int)' ', m_pfhLogfile);
		fputs(szComment, m_pfhLogfile);
	}
	// fputc((int)'\n', m_pfhLogfile);
	fflush(m_pfhLogfile);
}


/*
* ReportLogEnd()
* ----------------------------------------------------------------------------
*   Blockende-Zeile ausgeben
*
* Parameter
* ----------------------------------------------------------------------------
*   szObjectName: Blockname
*   szComment   : optionaler Kommentar
*   szEnd       : Ende-Kennung (ENDE)
*/
void TLogfile::ReportLogEnd(const char *szObjectName,
                            const char *szComment, const char *szEnd)
{
	if ( !m_pfhLogfile ) return;

	ReportLogHeader(szEnd);
	fputs(szObjectName ? szObjectName : "", m_pfhLogfile);
	if ( szComment && *szComment )
	{
		if ( szObjectName && *szObjectName )
			fputc((int)' ', m_pfhLogfile);
		fputs(szComment, m_pfhLogfile);
	}
	fputc((int)'\n', m_pfhLogfile);
	fflush(m_pfhLogfile);
}


/*
* ReportLogText()
* ----------------------------------------------------------------------------
*   Textzeile ausgeben. Das Zeilenende muss bei Bedarf in szText enthalten
*   sein. Ist szAction leer, wird kein Header ausgegeben
*
* Parameter
* ----------------------------------------------------------------------------
*   szText  : Auszugebener Text
*   szAction: Aktionskennung
*/
void TLogfile::ReportLogText(const char *szText, const char *szAction)
{
	if ( !m_pfhLogfile ) return;
	if ( szAction ) ReportLogHeader(szAction);
	if ( szText && *szText )
		fputs(szText, m_pfhLogfile);
	fflush(m_pfhLogfile);
}


/*
* ReportErrorLine()
* ----------------------------------------------------------------------------
*   Schreibt eine Fehlerzeile in die Protokolldatei. Ein Zeilenende-Zeichen
*   muss bei Bedarf in szErrortext aufgenommen sein, ist szErrorName leer
*   wird kein Zeilen-header geschrieben.
*
* Parameter
* ----------------------------------------------------------------------------
*   szErrorText: Beschreibender Text
*   szErrorName: Fehlerbezeichnung
*/
void TLogfile::ReportErrorLine(const char *szErrorText, const char *szErrorName)
{
	// Nach stderr protokollieren
	/*
	if ( szErrorName && *szErrorName )
	{
    fputs(m_strModuleName, stderr);
    fputs(" ", stderr);
    fputs(szErrorName, stderr);
	}

	if ( szErrorText && *szErrorText )
	{
		if (szErrorName && *szErrorName ) fputs(" ", stderr);
		fputs(szErrorText, stderr);
	}
	*/

	if ( !m_pfhLogfile ) return;

	// Ins Logfile protokollieren
	if ( szErrorName ) ReportErrorHeader(szErrorName);
	if ( szErrorText && *szErrorText )
		fputs(szErrorText, m_pfhLogfile);

	fflush(m_pfhLogfile);
}


/*
* ReportErrno()
* ----------------------------------------------------------------------------
*   Protokolliert einen Dateisystemfehler (errno!) in
*   die Protokolldatei
*
* Parameter
* ----------------------------------------------------------------------------
*   szComment  : Wird falls voranden in Klammern ausgegeben
*   szErrorName: Die Fehlerbezeichnung
*/
void TLogfile::ReportErrno(const char *szComment, const char *szErrorName)
{
	// Auf stderr schreiben
	/*
	fprintf(stderr, "%s %s%s%s[%d]: %s\n", (const char *)m_strModuleName,
		szComment ? "(" : "",
		szComment ? szComment : "",
		szComment ? ") " : "",
		errno, strerror(errno));
	*/

	if ( !m_pfhLogfile ) return;

	// Protokollieren
	ReportErrorHeader(szErrorName);
	fprintf(m_pfhLogfile, "%s%s%s[%d]: %s\n",
		szComment ? "(" : "",
		szComment ? szComment : "",
		szComment ? ") " : "",
		errno, strerror(errno));
}

