#ifndef _RICPP_BASERENDERER_RIBPARSER_H
#define _RICPP_BASERENDERER_RIBPARSER_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
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

/** @file ribparser.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Parser for rib
 */

#ifndef _RICPP_RENDERSTATE_RENDERSTATE_H
#include "ricpp/renderstate/renderstate.h"
#endif // _RICPP_RENDERSTATE_RENDERSTATE_H

#ifndef _RICPP_STREAMS_BACKBUFFER_H
#include "ricpp/streams/backbuffer.h"
#endif // _RICPP_STREAMS_BACKBUFFER_H

#include <deque>

namespace RiCPP {
	//! Container class for one parameter (array) read by the parser (RIB)
	/*! Types float, int or string are supported.
	 */
	class CRibParameter {

		unsigned long m_lineCount; //!< Start line of the parameter.

		EnumBasicTypes m_typeID; //!< Type of value.
		bool    m_isArray;       //!< Parameter is array yes/no.

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
		CRibParameter();			//!< Standard constructor, initializes the members as empty.
		~CRibParameter();			//!< Destructor, cleans up and frees all memory allocated.

		//! Copy constructor for a CRibParameter.
		/*! \param p The CRibParameter to copy from
		 */
		CRibParameter(const CRibParameter &p);
		// CRibParameter &duplicate() const;	// duplication is not used

		//! assigns an instance to *this.
		CRibParameter &operator=(const CRibParameter &p);

		//*! Is called if parameter will be an array.
		inline void startArray() {
			m_isArray = true;
		}

		//! Called to get the line counter, set by lineCount(long count).
		/*! \return The line count.
		 */
		inline unsigned long lineCount() const {
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
		inline unsigned long lineCount(unsigned long count) {
			unsigned long old = m_lineCount;
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
			if ( m_typeID == BASICTYPE_INTEGER ) {
				convertIntToFloat();
				m_typeID = BASICTYPE_FLOAT;
				// do not return
			}
			if ( m_typeID == -1 || m_typeID == BASICTYPE_FLOAT ) {
				m_vFloat.push_back(v);
				m_typeID = BASICTYPE_FLOAT;
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
			if ( m_typeID == BASICTYPE_FLOAT ) {
				m_vFloat.push_back((RtFloat)v);
				return true;
			}
			if ( m_typeID == -1 || m_typeID == BASICTYPE_INTEGER ) {
				m_vInt.push_back(v);
				m_typeID = BASICTYPE_INTEGER;
				return true;
			}
			return false;
		}

		//! Sets a single string value (appends if isArray()).
		/*! \param v The string value to be set
		 *  \return true: value could be set, false: otherwise
		 */
		inline bool setString(const char *v) {
			if ( m_typeID == -1 || m_typeID == BASICTYPE_STRING ) {
				m_vString.push_back(v);
				m_typeID = BASICTYPE_STRING;
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
			if ( m_typeID == BASICTYPE_INTEGER ) {
				if ( m_vInt.empty() )
					return false;
				v = (RtFloat)m_vInt[0];
				return true;
			}
			if ( m_typeID == BASICTYPE_FLOAT ) {
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
			if ( m_typeID == BASICTYPE_INTEGER ) {
				if ( m_vInt.empty() )
					return false;
				v = m_vInt[0];
				return true;
			}
			if ( m_typeID == BASICTYPE_FLOAT ) {
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
			if ( m_typeID != BASICTYPE_STRING )
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
	}; // CRibParameter


	/** @brief Interface for the state reader of the parser CRibParser
	 */
	class IRibParserState {
	public:
		inline virtual ~IRibParserState() {}
		virtual const CUri &baseUri() const = 0;
		virtual const CUri &absUri() const = 0;
		virtual long lineno() const = 0;
		virtual const IArchiveCallback *callback() const = 0;
		virtual const IRenderStateReader &stateReader() const = 0;
		virtual IRiRoot &ribFilter() = 0;
	}; // IRibParserState


	class CRibRequestData {
		IRibParserState *m_parserState;
		IRiCPPErrorHandler *m_errHandler;
		std::vector<CRibParameter> m_parameters;     //!< All parameteres parsed within one interface call
		std::vector<const char *> m_tokenList;       //!< Tokens of the token-value parameterlist of an interface call, inserted by getTokenList()
		std::vector<void *> m_valueList;             //!< Values of the token-value parameterlist of an interface call, inserted by getTokenList()
		bool m_checkParameters;                      //!< Check the size and types of the parameter list while parsing
		std::string m_curRequest;                    //!< Current request as string (e.g. "Sphere", "BeginWorld")

		inline IRiCPPErrorHandler &errHandler()
		{
			return *m_errHandler;
		}
		inline const char *resourceName()
		{
			return m_parserState->absUri().toString().c_str();
		}

		inline const IRenderStateReader &stateReader() const
		{
			return m_parserState->stateReader();
		}
	public:
		inline CRibRequestData()
		{
			m_parserState = 0;
			m_errHandler = 0;
			m_checkParameters = true;
		}

		inline void init(
			IRibParserState &aParserState,
			IRiCPPErrorHandler &anErrHandler
		)
		{
			m_parserState = &aParserState;
			m_errHandler = &anErrHandler;
		}

		inline void clear()
		{
			m_parameters.clear();
			m_tokenList.clear();
			m_valueList.clear();
			m_curRequest.clear();
		}

		void push_back(CRibParameter &p)
		{
			m_parameters.push_back(p);
		}

		inline CRibParameter &back()
		{
			return m_parameters.back();
		}

		inline size_t size()
		{
			return m_parameters.size();
		}

		inline RtToken *tokenList()
		{
			return (RtToken *)&m_tokenList[0];
		}

		inline RtPointer *valueList()
		{
			return (RtPointer *)&m_valueList[0];
		}

		inline CRibParameter &operator[](int idx)
		{
			return m_parameters[idx];
		}

		//! Fills the token-value list
		/*! The members m_tokenList and m_valueList are filled by
		 * this function. The token-value list (parameter list) starts by
		 * the parameter at position start.
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
		inline int getTokenListNoCheck(size_t start)
		{
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
		inline int getTokenList(size_t start, const CParameterClasses &p)
		{
			return getTokenList(
				start,
				p.vertices(),
				p.corners(),
				p.facets(),
				p.faceVertices(),
				p.faceCorners());
		}

		void curRequest(const char *req)
		{
			m_curRequest = req ? req : "";
		}

		const char *curRequest() const
		{
			return m_curRequest.c_str();
		}
	}; // CRibRequestData

	/** @brief Root for the request handlers
	 */
	class CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const {}
		inline virtual CRibRequest &operator=(const IRibParserState &req)
		{
			return *this;
		}
		virtual EnumRequests getId() const {return REQ_UNKNOWN;}
	}; // CRibRequest


	/** @brief Generic parser object
	 */
	class CArchiveParser : public IRibParserState {
		CBackBufferProtocolHandlers *m_backBufferReg;
		IRiRoot *m_ribFilter;
		const IRenderStateReader *m_stateReader;
		const IArchiveCallback *m_callback;
		IRi *m_ri;
		IRiCPPErrorHandler *m_errHandler;

		CUri m_baseUri;
		CUri m_absUri;
		long m_lineno;

		bool m_hasPutBack;
		unsigned char m_putback;

		inline CArchiveParser &operator=(const CArchiveParser &) { return *this; }

	protected:
		std::istream m_istream;
		TemplFrontStreambuf<char> m_ob;

		void putback(unsigned char c);
		unsigned char getchar();

	public:
		inline CArchiveParser(
			IRi &aFrontend,
			IRiCPPErrorHandler &anErrHandler,
			CBackBufferProtocolHandlers &backBufferReg,
			IRiRoot &ribFilter,
			const IRenderStateReader &aStateReader,
			const CUri &baseUri) :
			m_ob(backBufferReg),
			m_istream(&m_ob)
		{
			m_ri = &aFrontend;
			m_errHandler = &anErrHandler;
			m_backBufferReg = &backBufferReg;
			m_ribFilter = &ribFilter;
			m_stateReader = &aStateReader;
			m_baseUri = baseUri;
			m_lineno = 0;
			m_hasPutBack = false;
			m_putback = 0;
		}

		inline virtual ~CArchiveParser()
		{
			close();
		}

		inline virtual const CUri &baseUri() const
		{
			return m_baseUri;
		}

		inline virtual const CUri &absUri() const
		{
			return m_absUri;
		}

		inline virtual long lineno() const
		{
			return m_lineno;
		}

		inline virtual const IArchiveCallback *callback() const
		{
			return m_callback;
		}

		inline virtual const IRenderStateReader &stateReader() const
		{
			return *m_stateReader;
		}

		inline virtual IRi &frontend() const
		{
			return *m_ri;
		}

		inline virtual IRiCPPErrorHandler &errHandler()
		{
			return *m_errHandler;
		}

		inline virtual IRiRoot &ribFilter()
		{
			return *m_ribFilter;
		}

		virtual bool canParse(RtString name);
		bool close();

		inline virtual void parse(
			const IArchiveCallback *callback,
			RtInt n,
			RtToken tokens[],
			RtPointer params[])
		{
			m_callback = callback;
		}

	}; // CArchiveParser

	/** @brief Rib parser object
	 */
	class CRibParser : public CArchiveParser {
		typedef long RibHandelNumber; //! Representation of a handle number in a RIB file.

		typedef std::map<RibHandelNumber, RtLightHandle> NUM2LIGHT;	  //!< Maps long to light handle.
		typedef std::map<std::string, RtLightHandle> STR2LIGHT;       //!< Maps a string to a light handle.
		typedef std::map<RibHandelNumber, RtObjectHandle> NUM2OBJECT; //!< Maps long to object handle.
		typedef std::map<std::string, RtObjectHandle> STR2OBJECT;     //!< Maps a string to an object handle.
		typedef std::map<RibHandelNumber, std::string> NUM2STRING;    //!< Maps an integer to a string to encode string tokens.

		static const int RIBPARSER_EOF;                //!< Used as token for end of file

		static const int RIBPARSER_NORMAL_COMMENT;     //!< Used as token for a normal comment, return code for handleComment()
		static const int RIBPARSER_STRUCTURED_COMMENT; //!< Used as token for a structured comment, return code for handleComment()

		static const int RIBPARSER_NUMBER;             //!< Used as token for a number, return code for insertNumber(), handleNumber()
		static const int RIBPARSER_STRING;             //!< Used as token for a string, return code for handleString()
		static const int RIBPARSER_ARRAY_START;        //!< Used as token for a '[', return code for handleArrayStart()
		static const int RIBPARSER_ARRAY_END;          //!< Used as token for a ']', return code for handleArrayEnd()


		static const int RIBPARSER_NOT_A_TOKEN;        //!< Used if a token is not found in token list.
		static const int RIBPARSER_REQUEST;            //!< Token is a request string stored in m_request

		CRibRequestData m_request;                   //!< Data of a parsed line.

		std::vector<char> m_token;                   //!< Current token to be handled as string
		int m_lookahead;                             //!< One character look ahead
		int m_braketDepth;                           //!< Braket [] nesting depth

		int m_code;                                  //!< If >= 0 Code of a RIB call to encode (next String) (used by binary encoding)
		EnumRequests m_ribEncode[256];               //!< 256 (0-255) Rib-Codes can be defined (used by binary encoding)

		long m_defineString;                         //!< If >= 0 encode a string token
		NUM2STRING m_stringMap;                      //!< Map of string tokens

		RtInt m_n;                                   //!< Number of token value pairs (copy of TRi::readArchive())
		RtToken *m_tokens;                           //!< Tokens (copy of TRi::readArchive())
		RtPointer *m_params;                         //!< Values (copy of TRi::readArchive())

		//! Maps number to object handle
		NUM2OBJECT m_mapObjectHandle;

		//! Maps name to object handle
		STR2OBJECT m_mapObjectStrHandle;

		//! Maps number to light handle
		NUM2LIGHT m_mapLightHandle;

		//! Maps name to light handle
		STR2LIGHT m_mapLightStrHandle;

		/** @brief Request handler for each token (e.g. "Sphere")
		 */
		std::map<std::string, CRibRequest> m_requestMap;

		/** @brief Calls the handler routine for a request token using m_requestMap.
		 * @param request The string of request token
		 * @return False, no CRibRequest handler found for \a request. True, handler found and called.
		 */
		bool call(const std::string &request);

		/** @brief Initializes m_requestMap.
		 */
		void initRequestMap();

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
			return (token > RIBPARSER_NOT_A_TOKEN) && (token <= RIBPARSER_REQUEST);
		}

		//! Token is a Ri token (Sphere, Patch, Format etc.)
		/*! \return true: token stands for a Ri token, false: otherwise
		 */
		inline static bool isRequestToken(int token) {
			return token == RIBPARSER_REQUEST;
		}


		//! Stores a comment
		/*! Comments found within an interface call are handled after the interface
		 *  call itself is handled, so the comments are stored until this is happend.
		 */
		class CComment {
		public:
			std::vector<char> m_comment; //!< Storage for a comment.
			bool m_isStructured;         //!< Comment is structured comment (##).
			long m_lineCount;            //!< Line count where the comment was found.
			//! Assignes another comment to *this.
			//! \return A reference to this.
			inline CComment &operator=(const CComment &c) {
				m_comment = c.m_comment;
				m_isStructured = c.m_isStructured;
				m_lineCount = c.m_lineCount;
				return *this;
			}
		};

		std::vector<CComment> m_deferedCommentList; //!< Comments, that are defered in this interface call

		EnumRequests findIdentifier();

		int handleBinary(unsigned char c);
		int handleComment(std::vector<char> &token, bool isStructured);
		void handleDeferedComments();
		int handleString();
		int handleArrayStart();
		int handleArrayEnd();
		int insertNumber(RtFloat flt);
		int insertNumber(RtInt num);
		int handleNumber(bool isInteger);

		int nextToken();
		int parseNextCall();
		void parseFile();

		inline const char *resourceName()
		{
			return absUri().toString().c_str();
		}
	public:
		inline CRibParser(
			IRi &aFrontend,
			IRiCPPErrorHandler &anErrHandler,
			CBackBufferProtocolHandlers &backBufferReg,
			IRiRoot &ribFilter,
			const IRenderStateReader &aStateReader,
			const CUri &baseUri)
			: CArchiveParser(aFrontend, anErrHandler, backBufferReg, ribFilter, aStateReader, baseUri)
		{
			m_request.init(*this, anErrHandler);
			initRequestMap();
		}

		inline virtual ~CRibParser() {}

		virtual bool canParse(RtString name);

		virtual void parse(
			const IArchiveCallback *callback,
			RtInt n,
			RtToken tokens[],
			RtPointer params[]);
	}; // CRibParser
}

#endif // _RICPP_BASERENDERER_RIBPARSER_H
