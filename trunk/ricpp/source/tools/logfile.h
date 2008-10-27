#ifndef _RICPP_TOOLS_LOGFILE_H
#define _RICPP_TOOLS_LOGFILE_H


/*
 * This is for some q&d logging capability
 */

// Logfile.h: Objekt zum schreiben einer Protokolldatei

// Zeilenaufbau:
// yyyy-mm-dd hh:mm:ss nnnnnn MMMMM Aktion beschreibender Text
// --------------------------------
// Datum      Zeit     Zeile  Modulname

// Fehler-Zeilenaufbau:
// yyyy-mm-dd hh:mm:ss nnnnnn MMMMM ***Fehlerkennung Fehlertext
// yyyy-mm-dd hh:mm:ss nnnnnn MMMMM ***weiterer Fehlertext
// ------------------------------------
// ...                              Fehler-Markierung

// Block Start:
// yyyy-mm-dd hh:mm:ss nnnnnn MMMMM START Blockname Kommentar
// --------------------------------------
// ...                              Anfangs-Markierung

// Block Ende:
// yyyy-mm-dd hh:mm:ss nnnnnn MMMMM ENDE Blockname Kommentar
// -------------------------------------
// ...                              Ende-Markierung

#include <stdio.h>
#include <string>

// Zeit als yyyy-mm-dd hh:mm:ss in den Buffer schreiben
time_t time2asc(char *buf, time_t t);

class TLogfile
{
	std::string m_strError;       // Fehlerkennung
	std::string m_strFilename;    // Dateiname der Logdatei 
	FILE *m_pfhLogfile;           // Datei-Handle
	long m_dwLineNo;              // Aktuelle Zeilennummer
	std::string m_strModuleName;  // Modulname des Logfiles
	int m_bIsValid;               // Logfile ist gueltig
	std::string m_strPrefix;      // Präfix vor dem Zeilenheader
	
public:
	TLogfile(const char *szFilename, const char *szModuleName, bool append = true,
		const char *szPrefix = NULL, const char *szError="***");
	~TLogfile();
	
	int IsValid();
	// Liefert 0 falls Fehlerstatus

	static inline const char *endl() { return "\n";}
	
	// Liefert die entsprechenden Members
	FILE *GetReportLogfile();
	long GetLineNo();
	const char *GetFilename();
	const char *GetModuleName();
	const char *GetError();
	const char *TLogfile::GetPrefix();
	
	int OpenNewFile(const char *szFilename, bool append=true);
	// Schliesst alte Protokoll-Datei und oeffnet neue
	
	inline void Close() { if ( m_pfhLogfile ) fclose(m_pfhLogfile); m_pfhLogfile = NULL; }

	void ReportLogHeader(const char *szAction = NULL, bool endLine=true);
	// Schreibt einen Zeilenheader fuer Ausgabezeilen
	// endline: \n vor die Zeile
	
	void ReportErrorHeader(const char *szErrorName = NULL);
	// Schreibt einen Zeilenheader fuer Fehlerzeilen,
	// der Header wird *nicht* auf stderr ausgegeben
	
	void ReportLogStart(const char *szObjectName = NULL,
		const char *szComment = NULL,
		const char *szStart = "START");
	// Start eines Ausgabeblocks
	
	void ReportLogEnd(const char *szObjectName = NULL,
		const char *szComment = NULL,
		const char *szEnd = "ENDE");
	// Ende eines Ausgabeblocks
	
	
	void ReportLogText(const char *szText, const char *szAction="TEXT");
	// Eine Zeile Text protokollieren (szText bei Bedarf mit mit \n)
	// Ist szAction nicht vorhanden wird kein Header ausgegeben,
	// es koennen so mehrere Aufrufe in die selbe Zeile schreiben
	// (z.B. fuer formatierte Ausgaben)
	
	void ReportErrorLine(const char *szErrorText,
					   const char *szErrorName="ERROR");
	// Einen Fehler protokollieren, s.a. ReportLogText(),
	
	void ReportErrno(const char *szComment = NULL,
		const char *ErrorName = "ERROR SYSTEM:");
	// Sytemfehler (errno) protokollieren
};


extern TLogfile logfile;

inline TLogfile &operator<<(TLogfile &logfile, const char *str) {
	logfile.ReportLogText(str, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, float f) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%f",f);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, double f) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%f",f);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, char c) {
	char buf[8];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%c",c);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, unsigned char c) {
	char buf[8];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%c",c);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, short i) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%d",i);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, unsigned short i) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%u",i);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, int i) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%d",i);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, unsigned int i) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%u",i);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, long l) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%ld",l);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
inline TLogfile &operator<<(TLogfile &logfile, unsigned long l) {
	char buf[64];
	buf[sizeof(buf)-1] = 0;
	_snprintf(buf,sizeof(buf)-1,"%lu",l);
	logfile.ReportLogText(buf, NULL);
	return logfile;
}
#endif // _RICPP_TOOLS_LOGFILE_H
