#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#define _RICPP_RIBPARSER_RIBPARSER_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! \file ribparser.h
 *  \brief Declaration for the parser for RIB files
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ribistream.h"
#include "riconstants.h"
#include "paramclasses.h"

#include <vector>
#include <deque>
#include <map>

typedef long RibHandelNumber; //! Representation of a handle number in a RIB file.
//! Maps a long (as read from RIB file) to the appropriate handle.
//!{
typedef std::map<RibHandelNumber, RtLightHandle> NUM2LIGHT;	  //!< Maps long to light handle.
typedef std::map<std::string, RtLightHandle> STR2LIGHT;       //!< Maps a string to a light handle.
typedef std::map<RibHandelNumber, RtObjectHandle> NUM2OBJECT; //!< Maps long to object handle.
typedef std::map<std::string, RtObjectHandle> STR2OBJECT;     //!< Maps a string to an object handle.
//!}

//! Maps an integer to a string to encode string tokens.
typedef std::map<RibHandelNumber, std::string> NUM2STRING;    // Maps string tokens.

//! Container class for one parameter (array) read by the parser (RIB)
/*! Types float, int or string are supported.
 */
class TRibParameter {

	IndexType m_lineCount; //!< Start line of the parameter.

	ETypeID m_typeID;      //!< Type of value (TYPEID_INT, TYPEID_FLOAT, TYPEID_STRING).
	bool    m_isArray;     //!< Parameter is array yes/no.

	std::vector <RtFloat>     m_vFloat;  //!< Values are Floats
	std::vector <RtInt>       m_vInt;    //!< Values are intergers
	std::deque  <std::string> m_vString; //!< Values are strings

	//! Value container if parameter is an array of string.
	/*! pointers to the strings of m_vString, m_vString is not changed
	 *  after a value is read (so pointers stay valid).
	 */
	std::vector<const char *> *m_cstrVector;

	void freeValue();			//!< clears m_vFloat, m_vInt, m_vString.

public:	
	TRibParameter();			//!< Standard constructor, initializes the members as empty.
	~TRibParameter();			//!< Destructor, cleans up and frees all memory allocated.

	//! Copy constructor for a TRibParameter.
	/*! \param p The TRibParameter to copy from
	 */
	TRibParameter(const TRibParameter &p);
	// TRibParameter &duplicate() const;	// duplication is not used

	//! assigns an instance to *this.
	TRibParameter &operator=(const TRibParameter &p);

	//*! Is called if parameter will be an array.
	inline void startArray() {
		m_isArray = true;
	}

	//! Called to get the line counter, set by lineCount(long count).
	/*! \return The line count.
	 */
	inline IndexType lineCount() const {
		return m_lineCount;
	}

	//! Asks if parameter is an array.
	/*! \return true if parameter is an array, false if parameter is a single value.
	 */
	inline bool isArray() const {
		return m_isArray;
	}

	//! Called to set the line counter, set by lineCount(long count).
	/*! \param count The new line counter value.
	 *  \return The old line counter value.
	 */
	inline IndexType lineCount(IndexType count) {
		IndexType old = m_lineCount;
		m_lineCount = count;
		return old;
	}

	//! Ask the type of the parameter.
	/*! \return id of the type of the contained values (TYPEID_INT, TYPEID_FLOAT, TYPEID_STRING).
	 */
	inline int typeID() const {
		return m_typeID;
	}

	// set single values (appends if isArray).
	//! Sets a single float value (appends if isArray()).
	/*! Converts array to floats if needed (i.e. array contains integers)
	 *  \param v The float value to be set
	 *  \return true: value could be set, false: otherwise
	 */
	inline bool setFloat(RtFloat v) {
		if ( m_typeID == TYPEID_INT ) {
			convertIntToFloat();
			m_typeID = TYPEID_FLOAT;
			// do not return
		}
		if ( m_typeID == -1 || m_typeID == TYPEID_FLOAT ) {
			m_vFloat.push_back(v);
			m_typeID = TYPEID_FLOAT;
			return true;
		}
		return false;
	}


	//! Sets a single float value (appends if isArray()).
	/*! Converts v to floats if needed (i.e. array contains floats)
	 *  \param v The integer value to be set
	 *  \return true: value could be set, false: otherwise
	 */
	inline bool setInt(RtInt v) {
		if ( m_typeID == TYPEID_FLOAT ) {
			m_vFloat.push_back((RtFloat)v);
			return true;
		}
		if ( m_typeID == -1 || m_typeID == TYPEID_INT ) {
			m_vInt.push_back(v);
			m_typeID = TYPEID_INT;
			return true;
		}
		return false;
	}

	//! Sets a single string value (appends if isArray()).
	/*! \param v The string value to be set
	 *  \return true: value could be set, false: otherwise
	 */
	inline bool setString(const char *v) {
		if ( m_typeID == -1 || m_typeID == TYPEID_STRING ) {
			m_vString.push_back(v);
			m_typeID = TYPEID_STRING;
			return true;
		}
		return false;
	}

	//! Gets single value.
	/*! \return Pointer to value.
	 */
	void *getValue();

	//! Gets array value.
	/*! \param i Ondex of value.
	 *  \return Pointer to value, NULL if i is out of range.
	 */
	void *getValue(size_t i) const;

	// get single values

	//! Get a single float value, integers are converted.
	/*! \retval v Float value is stored here.
	 *  \return true if value could be set.
	 */
	inline bool getFloat(RtFloat &v) const {
		if ( m_typeID == TYPEID_INT ) {
			if ( m_vInt.empty() )
				return false;
			v = (RtFloat)m_vInt[0];
			return true;
		}
		if ( m_typeID == TYPEID_FLOAT ) {
			if ( m_vFloat.empty() )
				return false;
			v = m_vFloat[0];
			return true;
		}
		return false;
	}

	//! Get a single integer value.
	/*! \retval v Integer value is stored here.
	 *  \return true if value could be set.
	 */
	inline bool getInt(RtInt &v) const {
		if ( m_typeID == TYPEID_INT ) {
			if ( m_vInt.empty() )
				return false;
			v = m_vInt[0];
			return true;
		}
		if ( m_typeID == TYPEID_FLOAT ) {
			if ( m_vFloat.empty() )
				return false;
			v = (RtInt)m_vFloat[0];
			return true;
		}
		return false;
	}

	//! Get a single string value pointer.
	/*! \retval v String value pointer is stored here.
	 *  \return true if value could be set.
	 */
	inline bool getString(const char * &v) const {
		if ( m_typeID != TYPEID_STRING )
			return false;
		v = m_vString.empty() ? NULL : m_vString[0].c_str();
		return true;
	}

	//! Get number of values in parameter (1 or size of array).
	/*! \return Number of single parameters.
	 */
	size_t getCard() const;

	//! Converts float values to integer values.
	/*! Sometimes integers are needed,
	 *  numbers are always stored as float at first.
	 *  \return true if parameters could be converted.
	 */
	bool convertFloatToInt();

	//! Converts integer values to float values.
	/*! \return true if parameters could be converted.
	 */
	bool convertIntToFloat();
}; // TRibParameter


//! Parser for RIB files
/*! Parses ASCII and binary RIB files (can be mixed). can read zipped files
 */
class TRibParser {
private:
	//! Various status codes of the parser
	//!{
	static const int RIBPARSER_EOF;                //< Used as token for end of file

	static const int RIBPARSER_NORMAL_COMMENT;     //< Used as token for a normal comment, return code for handleComment()
	static const int RIBPARSER_STRUCTURED_COMMENT; //< Used as token for a structured comment, return code for handleComment()

	static const int RIBPARSER_NUMBER;             //< Used as token for a number, return code for insertNumber(), handleNumber()
	static const int RIBPARSER_STRING;             //< Used as token for a string, return code for handleString()
	static const int RIBPARSER_ARRAY_START;        //< Used as token for a '[', return code for handleArrayStart()
	static const int RIBPARSER_ARRAY_END;          //< Used as token for a ']', return code for handleArrayEnd()

	static const int RIBPARSER_NOT_A_TOKEN;        //< Used if a token is not found in token list.
	//!}

	static bool m_tokenInitialized;              //!< All valid tokens are inserted
	static const char *m_tokenset[N_TOKENS];     //!< Tokens, set if m_tokenInitialized is true, indexed by IDX_ERROR_HANDLER etc.
	static void initTokens();                    //!< Initialize tokens, sets the pointers of m_tokenset to the appropriate strings.

	//! Asks if token stands for a comment
	/*! \return true: token stands for a comment (structured or normal), false: otherwise
	 */
	inline static bool isCommentToken(int token) {
		return token != RIBPARSER_NORMAL_COMMENT || token != RIBPARSER_STRUCTURED_COMMENT;
	}

	//! Token is a Ri token, comment, value, array start, array end or eof
	/*! \return true: token stands for a valid token, false: otherwise
	 */
	inline static bool isValidToken(int token) {
		return (token > RIBPARSER_NOT_A_TOKEN) && (token < N_TOKENS);
	}

	//! Token is a Ri token (Sphere, Patch, Format etc.)
	/*! \return true: token stands for a Ri token, false: otherwise
	 */
	inline static bool isRiToken(int token) {
		return (token >= 0) && (token < N_TOKENS);
	}

	//! Stores a comment
	/*! Comments found within an interface call are handled after the interface
	 *  call itself is handled, so the comments are stored until this is happend.
	 */
	class TComment {
	public:
		std::vector<char> m_comment; //!< Storage for a comment.
		bool m_isStructured;         //!< Comment is structured comment (##).
		IndexType m_lineCount;       //!< Line count where the comment was found.
		//! Assignes another comment to *this.
		//! \return A reference to this.
		inline TComment &operator=(const TComment &c) {
			m_comment = c.m_comment;
			m_isStructured = c.m_isStructured;
			m_lineCount = c.m_lineCount;
			return *this;
		}
	};

	std::vector<TComment> m_deferedCommentList; //!< Comments, that are defered in this interface call

	//! Handles any defered comment lines
	/*! Called after an interface call is parsed to handle comments found in
	 *  or after the interface call. Calls the TRi::archiveRecordV() and the
	 *  TRiArchiveCallback callback (stored in m_fctpArchiveCallback)
	 *  (the interface functions are directly handled by m_ri)
	 */
	void handleDeferedComments();

	std::vector<TRibParameter> m_parameters;     //!< all parameteres parsed within one interface call
	std::vector<const char *> m_tokenList;       //!< Tokens of the token-value parameterlist of an interface call, inserted by getTokenList()
	std::vector<void *> m_valueList;             //!< Values of the token-value parameterlist of an interface call, inserted by getTokenList()

	IndexType m_lineCount;                       //!< Current line number of current interface call
	std::string m_strFileName;                   //!< Current name of file to be parsed
	std::vector<char> m_token;                   //!< Current token to be handled as string
	RtInt m_lookahead;                           //!< One character look ahead
	RtInt m_braketDepth;                         //!< Braket [] nesting depth

	RtInt m_currentCallId;                       //!< Is set in handleCall() - current request id, init with -1 (no request id found)

	bool m_checkParameters;                      //!< Check the size and types of the parameter list while parsing

	int m_code;                                  //!< If >= 0 Code of a RIB call to encode (next String) (used by binary encoding)
	int m_ribEncode[256];                        //!< 256 (0-255) Rib-Codes can be defined (used by binary encoding)

	long m_defineString;                         //!< If >= 0 encode a string token
	NUM2STRING m_stringMap;                      //!< Map of string tokens

	TRiCallbackInterface *m_ricb;                //!< Query interface for error messages (usally the same instance as m_ri)
	TRi *m_ri;                                   //!< Renderer object, interface routines are called after
	RtArchiveCallback m_fctpArchiveCallback;     //!< Callback funktion of RiReadArchive

	TRibIStream *m_pifStream;                    //!< inputstream containing RIB code
	
	const char *m_lastToken;                     //!< Cached last token
	int m_lastTokenId;                           //!< Cached token nmber for m_lastToken

	RtInt m_n;                                   //!< Number of token value pairs (copy of TRi::readArchive())
	RtToken *m_tokens;                           //!< Tokens (copy of TRi::readArchive())
	RtPointer *m_params;                         //!< Values (copy of TRi::readArchive())

	//! Closes input stream
	void closeStream();

	//! Opens input stream, filname is previously set by constructor or reset()
	/*! \return true: file is opened, false: stream could not be opend
	 */
	bool openStream();

	//! Inserts the value of a number token (float)
	/*! \param flt Inserts float flt into m_parameters
	 *  \return RIBPARSER_NUMBER, the token number for numbers
	 */
	int insertNumber(RtFloat flt);

	//! Inserts the value of a number token (integer)
	/*! \param num Inserts an interger num into m_parameters
	 *  \return RIBPARSER_NUMBER, the token number for numbers
	 */
	int insertNumber(RtInt num);

	//! Finds the Id-Number of m_token (searches in m_tokenset)
	/*! Appends a '0' to m_token (end of string)
	 *  and finds the IDX_..., declared in riconstants.h, that belongs to it
	 *  \return The index number of the token (0 <= index && N_TOKENS > index),
	 *          returns RIBPARSER_NOT_A_TOKEN if token is not found in m_tokenset
	 */
	RtInt findIdentifier();

	//! Calls the interface routine for a parsed command
	/*! Sets m_currentCallId = callId, calls the appropriate interface function
	 *  with the parameters stored in m_parameters
	 *  \param callId id of an interface call (found by findIdentifier())
	 */
	void handleCall(RtInt callId);

	//! Handles a (structured) comment
	/*! Stores comment in m_deferedCommentList, terminates token
	 *  \param token The comment
	 *  \param isStructured, comment is a structured comment, at least two '#' are found in front of the comment
	 *  \return RIBPARSER_STRUCTURED_COMMENT, if isStructured - comment is structured, RIBPARSER_NORMAL_COMMENT otherwise
	 */
	RtInt handleComment(std::vector<char> &token, bool isStructured);

	//! Handles the start of an array, '[' was found.
	/*! Increments m_braketDepth for braket depth, pushes an array parameter
	 *  to m_parameters
	 *  \return RIBPARSER_ARRAY_START, the token for an array start
	 */
	RtInt handleArrayStart();

	//! Handles the end of an array, ']' was found.
	/*! Decrements the braket depth m_braketDepth, calls the
	 *  error handler, if the braket depth is negative
	 *  \return RIBPARSER_ARRAY_END, the token for an array end
	 */
	RtInt handleArrayEnd();

	//! Handles a string value parsed before
	/*! Terminates m_token, inserts a string into m_parameters
	 * \return RIBPARSER_STRING if a string is insertet into the m_parameters or RIBPARSER_NORMAL_COMMENT if an error occured (and should be ignored later)
	 */
	RtInt handleString();

	//! Handles a number (float or integer) value parsed before
	/*! Terminates m_token, calls the appropriate insertNumber() to
	 *  insert a number into m_parameters.
	 * \param isInteger true: M_token contains a number.
	 * \return RIBPARSER_STRING if a string is insertet into the m_parameters or RIBPARSER_NORMAL_COMMENT if an error occured (and should be ignored later)
	 */
	RtInt handleNumber(bool isInteger);

	//! Handles a binary: a byte > 127.
	/*! If a byte is greater than 127 it encodes a binary value. This function
	 *  handles such binary RIB data. Some code is taken from readfunc.c found
	 *  in the affine lib by Thomas E. Burge
	 *  \param c The first byte of binary data found
	 *  \return The token value for the binary
	 */
	RtInt handleBinary(unsigned char c);

	// bool readString();				 // Reads an encoded string to 

	//! Fills the token-value list
	/*! The members m_tokenList and m_valueList are filled by
	 * this function. It is called by handleCall(). The
	 * token-value list (parameter list) starts by the parameter at position start
	 * \param start Token-value list starts here
	 * \vertices Number of vertices (shared vertices count 1) of the primary to which the parameter list belongs
	 * \corners Number of the corners (shared corners count 1) of the primary to which the parameter list belongs
	 * \facets Number of the faces of the primary to which the parameter list belongs
	 * \faceVertices Number of the vertices of all faces of the primary to which the parameter list belongs
	 * \return The number of token-value pairs stored in m_tokenList and m_valueList, -1 if an error occured
	 */
	int getTokenList(
		size_t start,
		RtInt vertices=0, RtInt corners=0, RtInt facets=0,
		RtInt faceVertices=0, RtInt faceCorners=0
	);

	//! Gets the token-value list without checking, doesn't store values
	/*! \param start Token-value list starts here
	 *  \return The number of token-value pairs found
	 */
	inline int getTokenListNoCheck(size_t start) {
		bool check = m_checkParameters;
		m_checkParameters = false;
		int r = getTokenList(start);
		m_checkParameters = check;
		return r;
	}

	//! Gets the token list
	/*! Uses an instances TParameterClasses to get vertices, corners e.t.c.
	 *  \param start Token-value list starts here
	 *  \param p used to get vertices, corners e.t.c.
	 */
	inline int getTokenList(size_t start, const TParameterClasses &p) {
		return getTokenList(start, p.vertex(), p.varying(), p.uniform(), p.faceVertex(), p.faceVarying());
	}


	//! Maps number to object handle
	NUM2OBJECT m_mapObjectHandle;

	//! Maps name to object handle
	STR2OBJECT m_mapObjectStrHandle;

	//! Mapping of (RIB) object number to handle RtObjectHandle, stored in m_mapObjectHandle
	/*! \param handle The object handle.
	 *  \param number The number for an object found for the RIB call ObjectBegin.
	 *  \return true
	 */
	bool bindObjectHandle(RtObjectHandle handle, RtInt number);

	//! Mapping of (RIB) object name to handle RtObjectHandle, stored in m_mapObjectHandle
	/*! \param handle The object handle.
	 *  \param name The name for an object found for the RIB call ObjectBegin.
	 *  \return true
	 */
	bool bindObjectHandle(RtObjectHandle handle, const char *name);

	//! Gets the object handle for a (RIB) object number
	/*! \retval handle The object handle is stored here
	 *  \param number The number for which an object handle is searched for
	 *  \return true: handle found, false: otherwise
	 */
	bool getObjectHandle(RtObjectHandle &handle, RtInt number);

	//! Gets the object handle for a (RIB) object name
	/*! \retval handle The object handle is stored here
	 *  \param name The name for which an object handle is searched for
	 *  \return true: handle found, false: otherwise
	 */
	bool getObjectHandle(RtObjectHandle &handle, const char *name);

	//! Maps number to light handle
	NUM2LIGHT m_mapLightHandle;

	//! Maps name to light handle
	STR2LIGHT m_mapLightStrHandle;

	//! Mapping of (RIB) light number to handle RtLightHandle, stored in m_mapLightHandle
	/*! \param handle The light handle.
	 *  \param number The number for a light source found for the RIB call LightSource or AreaLightSource
	 *  \return true
	 */
	bool bindLightHandle(RtLightHandle handle, RtInt number);

	//! Mapping of (RIB) light name to handle RtLightHandle, stored in m_mapLightStrHandle
	/*! \param handle The light handle.
	 *  \param handleName The name for a light source found for the RIB call LightSource or AreaLightSource
	 *  \return true
	 */
	bool bindLightHandle(RtLightHandle handle, const char *handleName);

	//! Gets the light handle for a (RIB) light number
	/*! \retval handle The light handle is stored here
	 *  \param number The number for which a light handle is searched for
	 *  \return true: handle found, false: otherwise
	 */
	bool getLightHandle(RtObjectHandle &handle, RtInt number);

	//! Gets the light handle for a (RIB) light name
	/*! \retval handle The light handle is stored here
	 *  \param handleName The name for which a light handle is searched for
	 *  \return true: handle found, false: otherwise
	 */
	bool getLightHandle(RtObjectHandle &handle, const char *handleName);

	//! Find the number of vertices
	/*! The first occurence of RI_P, RI_PW, RI_Z is used to count number of vertices
	 *  by counting the points.
	 *  \param start start of the parameter list (token.value)
	 *  \param n size of list
	 *  \return The number of vertices
	 */
	RtInt numVertices(RtInt start, RtInt n);

	//! Returns a pointer to the filter function of 'name', eg. "Box" -> RiBoxFilter
	/*  \param name The name of a pixelfilter function
	 *  \return The pixel filter function, NULL if a function is not found
	 */
	RtFilterFunc getFilterFunc(const char *name)  const;

	//! Returns the token for the next interface call to handle (Scanner)
	/*! This function is called by to scan the next token. The token number
	 *  is return (either a RIB-Token, value (RIBPARSER_...), EOF, etc.)
	 *  \return Token number to handle next
	 */
	int nextToken();

	//! Parses the next interface call (Parser)
	/*! Called by parseFile() to parses a complete interface call and handles it
	 *  by calling handleCall(). Cares also for the values and comments.
	 *  \return The token of the call to handle next or RIBPARSER_EOF if the end of file is found
	 */
	RtInt parseNextCall();

public:
	//! Standard constructor (no file).
	/*! Only initializes. reset() can be called to set a file name.
	 */
	TRibParser();

	//! Constructor, sets a file for parsing.
	/*! \param filename Name of the file to parse.
	 *  \param ri Renderer which interface functions are called.
	 *  \param ricb Implementation of interface to set line numbers and filename etc. (see TRiCallbackInterface).
	 *  \param callback Callback function called if comments are found and there is no 'macro recording'.
	 *  \param n Number of token value pairs
	 *  \param tokens Tokens of TRi::readRecord()
	 *  \param params values of TRi::readRecords()
	 */
	TRibParser(const char *filename, TRi &ri, TRiCallbackInterface &ricb, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[]);

	//! Destructor, cleans up, frees memory.
	~TRibParser();

	//! Parses the file, returns true if file could get opened
	/*! \return true file could get opened and is parsed
	 */
	bool parseFile();

	//! Resets the parser with a new file to parse
	/*! Parsing is not done by this function, call parseFile() to do the parsing.
	 *  \param filename Name of the file to parse
	 *  \param ri Renderer which interface functions are called
	 *  \param ricb Implementation of interface to set line numbers and filename etc. (see TRiCallbackInterface)
	 *  \param callback Callback function called if comments are found and there is no 'macro recording'.
	 *  \param n Number of token value pairs
	 *  \param tokens Tokens of TRi::readRecord()
	 *  \param params values of TRi::readRecords()
	 */
	void reset(const char *filename, TRi &ri, TRiCallbackInterface &ricb, RtArchiveCallback callback, int n, RtToken tokens[], RtPointer params[]);

	//! Enables/disables parameter checking while parsing token-value pairs is done, the parsing is enabled by default
	/*! \param check Sets if parameter checking while parsing token-value pairs
	 *  \return Value previously set value for parsing.
	 */
	inline bool setCheckParameters(bool check=true) {bool old=m_checkParameters; m_checkParameters=check; return old;}

	//! Asks if parameters are checked
	/*! \return true: checks parameters while parsing, false: otherwise.
	 */
	inline bool getCheckParameters() const {return m_checkParameters;}
}; // TRibParser

#endif // _RICPP_RIBPARSER_RIBPARSER_H
