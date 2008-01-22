#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#define _RICPP_RIBPARSER_RIBPARSER_H

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
 *  @brief Parser for rib streams.
 */

#ifndef _RICPP_RICONTEXT_RENDERSTATE_H
#include "ricpp/ricontext/renderstate.h"
#endif // _RICPP_RICONTEXT_RENDERSTATE_H

#ifndef _RICPP_RIBPARSER_RIBPARSERCALLBACK_H
#include "ricpp/ribparser/ribparsercallback.h"
#endif // _RICPP_RIBPARSER_RIBPARSERCALLBACK_H

#include <deque>

namespace RiCPP {

	/** @brief Container class for one parameter (array) read by the parser (RIB)
	 *
	 *  The basic types float, int or string are supported.
	 */
	class CRibParameter {

		unsigned long m_lineNo;  //!< Start line of the parameter.

		EnumBasicTypes m_typeID; //!< Type of value.
		bool    m_isArray;       //!< Parameter is array yes/no.

		std::vector <RtFloat>     m_vFloat;  //!< Values are Floats.
		std::vector <RtInt>       m_vInt;    //!< Values are Integers.
		std::deque  <std::string> m_vString; //!< Values are Strings.

		/** @brief Value container, used if parameter is an array of string.
		 *
		 *  Pointers to the strings of m_vString, m_vString is not changed
		 *  after a value is read (so pointers stay valid).
		 */
		std::vector<const char *> *m_cstrVector;

		void freeValue();			//!< Clears m_vFloat, m_vInt, m_vString.

	public:	
		CRibParameter();			//!< Standard constructor, initializes the members as empty.
		~CRibParameter();			//!< Destructor, cleans up and frees all memory allocated.

		/** @brief Copy constructor for a CRibParameter.
		 *
		 *  @param p The CRibParameter to copy from.
		 */
		CRibParameter(const CRibParameter &p);
		// CRibParameter &duplicate() const;	// duplication is not used

		/** @brief Assignment.
		 *
		 *  @param p Assigns this parameter to *this.
		 *  @return *this.
		 */
		CRibParameter &operator=(const CRibParameter &p);

		/** @brief Called if parameter will be an array.
		 *
		 *  If the rib parser encounters a '[' it calls this
		 *  member function to indicate that an array of values
		 *  will follow.
		 */
		inline void startArray()
		{
			m_isArray = true;
		}

		/** @brief Asks if parameter is an array.
		 *
		 *  @return true, if parameter is an array or false, if parameter is a single value.
		 *  @see void CRibParameter::startArray().
		 */
		inline bool isArray() const
		{
			return m_isArray;
		}

		/** @brief Called to get the line counter.
		 *
		 *  Since each parameter can be written on a different line. The
		 *  starting line is stored and can be recalled for error
		 *  notification. The line counter can be set by calling
		 *  unsigned long CRibParameter::lineNo(unsigned long).
		 *
		 *  @return The line counter.
		 */
		inline unsigned long lineNo() const
		{
			return m_lineNo;
		}

		/** @brief Called to set the line counter.
		 *
		 *  @param count The new line counter value.
		 *  @return The old line counter value.
		 *  @see unsigned long CRibParameter::lineNo() const.
		 */
		inline unsigned long lineNo(unsigned long count)
		{
			unsigned long old = m_lineNo;
			m_lineNo = count;
			return old;
		}

		/** @brief Ask the type of the parameter.
		 *  @return id of the type of the contained values (BASICTYPE_INT, BASICTYPE_FLOAT, BASICTYPE_STRING).
		 */
		inline EnumBasicTypes typeID() const
		{
			return m_typeID;
		}

		/** @brief Sets single values (appends if isArray).
		 *
		 *  Converts array to floats if needed (if array contains integers,
		 *  strings are not converted, it's not possibe to mix strings with
		 *  floats).
		 *
		 *  @param v The float value to be set.
		 *  @return true, value could be set or false, otherwise.
		 */
		inline bool setFloat(RtFloat v)
		{
			if ( m_typeID == BASICTYPE_INTEGER ) {
				convertIntToFloat();
				m_typeID = BASICTYPE_FLOAT;
				// do not return
			}
			if ( m_typeID == BASICTYPE_UNKNOWN || m_typeID == BASICTYPE_FLOAT ) {
				m_vFloat.push_back(v);
				m_typeID = BASICTYPE_FLOAT;
				return true;
			}
			return false;
		}


		/** @brief Sets a single integer value (appends if isArray()).
		 *
		 *  Converts v to a float if needed (array contains floats,
		 *  i's not possible to mix ints with strings).
		 *
		 *  @param v The integer value to be set.
		 *  @return true, value could be set or false, otherwise.
		 */
		inline bool setInt(RtInt v)
		{
			if ( m_typeID == BASICTYPE_FLOAT ) {
				m_vFloat.push_back((RtFloat)v);
				return true;
			}
			if ( m_typeID == BASICTYPE_UNKNOWN || m_typeID == BASICTYPE_INTEGER ) {
				m_vInt.push_back(v);
				m_typeID = BASICTYPE_INTEGER;
				return true;
			}
			return false;
		}

		/** @brief Sets a single string value (appends if isArray()).
		 *
		 *  Strings cannot be mixed with any other type.
		 *
		 *  @param v The string value to be set.
		 *  @return true, value could be set or false, otherwise.
		 */
		inline bool setString(const char *v)
		{
			if ( m_typeID == BASICTYPE_UNKNOWN || m_typeID == BASICTYPE_STRING ) {
				m_vString.push_back(v);
				m_typeID = BASICTYPE_STRING;
				return true;
			}
			return false;
		}

		/** @brief Gets single value of arbitrary type.
		 *
		 *  @return Pointer to value.
		 */
		void *getValue();

		/** @brief Gets a single array value of arbitrary type.
		 *  @param i Index of the value.
		 *  @return Pointer to the value, 0 if it is out of range.
		 */
		void *getValue(size_t i) const;

		// The following member routines get single values.

		/** @brief Gets a single float value, integers are converted.
		 *
		 *  @retval v Float value is stored here.
		 *  @return true, if value has been set.
		 */
		inline bool getFloat(RtFloat &v) const
		{
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

		/** @brief Gets a single integer value, floats are converted.
		 *
		 *  @retval v Integer value is stored here.
		 *  @return true, if value has been set.
		 */
		inline bool getInt(RtInt &v) const
		{
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

		/** @brief Gets a single string value pointer
		 *
		 *  @retval v String value pointer is stored here.
		 *  @return true, if value has been set.
		 */
		inline bool getString(const char * &v) const
		{
			if ( m_typeID != BASICTYPE_STRING )
				return false;
			v = m_vString.empty() ? NULL : m_vString[0].c_str();
			return true;
		}

		/** @briefs Get number of values contained (1 or size of array).
		 *
		 *  @return Number of single parameters.
		 */
		size_t getCard() const;

		/** @brief Converts float values to integer values.
		 *
		 *  Sometimes integers are needed,
		 *  since numbers are always stored as float at first.
		 *  they need to be converted in this case.
		 *
		 *  @return true, if parameters have been converted.
		 */
		bool convertFloatToInt();

		/** @brief Converts integer values to float values.
		 *
		 *  @return true, if parameters have been be converted.
		 */
		bool convertIntToFloat();
	}; // CRibParameter


	/** @brief Interface for the state reader/manipulator of the parser CRibParser
	 */
	class IRibParserState {
	public:
		inline virtual ~IRibParserState() {}
		virtual const CUri &baseUri() const = 0;
		virtual const CUri &absUri() const = 0;
		virtual long lineNo() const = 0;
		virtual const char *resourceName() const = 0;
		virtual const IArchiveCallback *callback() const = 0;
		virtual CRenderState &renderState() const = 0;
		virtual IRiRoot &ribFilter() = 0;
		virtual IRiCPPErrorHandler &errHandler() = 0;
		virtual CParameterList &params() = 0;

		virtual bool bindObjectHandle(RtObjectHandle handle, RtInt number) = 0;
		virtual bool bindObjectHandle(RtObjectHandle handle, const char *name) = 0;
		virtual bool getObjectHandle(RtObjectHandle &handle, RtInt number) const = 0;
		virtual bool getObjectHandle(RtObjectHandle &handle, const char *name) const = 0;

		virtual bool bindLightHandle(RtLightHandle handle, RtInt number) = 0;
		virtual bool bindLightHandle(RtLightHandle handle, const char *handleName) = 0;
		virtual bool getLightHandle(RtLightHandle &handle, RtInt number) const = 0;
		virtual bool getLightHandle(RtLightHandle &handle, const char *handleName) const = 0;

		// virtual bool bindArchiveHandle(RtArchiveHandle handle, RtInt number) = 0;
		virtual bool bindArchiveHandle(RtArchiveHandle handle, const char *handleName) = 0;
		// virtual bool getArchiveHandle(RtArchiveHandle &handle, RtInt number) const = 0;
		virtual bool getArchiveHandle(RtArchiveHandle &handle, const char *handleName) const = 0;
	}; // IRibParserState


	/** @brief Used to store the content of a rib request
	 *
	 *  The parser uses one instance of this class.
	 */
	class CRibRequestData {
		IRibParserState *m_parserState;              //!< The state of the parser (and front end callbacks).
		std::vector<CRibParameter> m_parameters;     //!< All parameters parsed within one interface call.
		std::vector<const char *> m_tokenList;       //!< Tokens of the token-value parameterlist of an interface call, inserted by getTokenList().
		std::vector<void *> m_valueList;             //!< Values of the token-value parameterlist of an interface call, inserted by getTokenList().
		bool m_checkParameters;                      //!< Indicator to check the size and types of the parameter list while parsing.
		std::string m_curRequest;                    //!< Current request as string (e.g. "Sphere", "BeginWorld").

		inline IRiCPPErrorHandler &errHandler()
		{
			assert(m_parserState != 0);
			return m_parserState->errHandler();
		}
		inline const char *resourceName() const
		{
			assert(m_parserState != 0);
			return m_parserState->resourceName();
		}

		inline CRenderState &renderState() const
		{
			assert(m_parserState != 0);
			return m_parserState->renderState();
		}
		
		inline IRibParserState &parserState()
		{
			assert(m_parserState != 0);
			return *m_parserState;
		}
	public:
		inline CRibRequestData()
		{
			m_parserState = 0;
			m_checkParameters = true;
		}

		inline void init(IRibParserState &aParserState)
		{
			m_parserState = &aParserState;
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

		/** @brief Fills the token-value list.
		 *
		 * The members m_tokenList and m_valueList are filled by
		 * this function. The token-value list (parameter list) starts by
		 * the parameter at position start.
		 *
		 * @param start Token-value list starts here.
		 * @param vertices Number of vertices (shared vertices count 1) of the primary to which the parameter list belongs.
		 * @param corners Number of the corners (shared corners count 1) of the primary to which the parameter list belongs.
		 * @param facets Number of the faces of the primary to which the parameter list belongs.
		 * @param faceVertices Number of the vertices of all faces of the primary to which the parameter list belongs.
		 * @return The number of token-value pairs stored in m_tokenList and m_valueList, -1 if an error occured.
		 */
		int getTokenList(
			size_t start,
			RtInt vertices=0, RtInt corners=0, RtInt facets=0,
			RtInt faceVertices=0, RtInt faceCorners=0
		);

		/** @brief Gets the token-value list without checking, doesn't store values.
		 *
		 *  @param start Token-value list starts here.
		 *  @return The number of token-value pairs found.
		 */
		inline int getTokenListNoCheck(size_t start)
		{
			bool check = m_checkParameters;
			m_checkParameters = false;
			int r = getTokenList(start);
			m_checkParameters = check;
			return r;
		}

		/** @brief Gets the token list.
		 *
		 *  Uses an instances TParameterClasses to get vertices, corners etc.
		 *
		 *  @param start Token-value list starts here.
		 *  @param p used to get vertices, corners etc.
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

	RtInt numVertices(RtInt start, RtInt n);

	void curRequest(const char *req)
		{
			m_curRequest = req ? req : "";
		}

		const std::string &curRequest() const
		{
			return m_curRequest;
		}
	}; // CRibRequestData


	/** @brief Base class for the request handlers.
	 *
	 *  Every interface call has its own class (eg. CWorldBeginRibRequest, CSphereRibRequest=
	 */
	class CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const = 0;
		virtual EnumRequests interfaceIdx() const {return REQ_UNKNOWN;}
		inline virtual RtToken requestName() const { return CRequestInfo::requestName(interfaceIdx()); }
	}; // CRibRequest


	/** @brief The Rib parser object.
	 *
	 *  This class implements the parsing part of the RIB binding. RIB
	 *  can be written using the CRibWriter 'renderer'.
	 */
	class CRibParser : public IRibParserState {
	protected:
		typedef long RibHandleNumber; //!< Representation of a handle number in a RIB file.
		
	private:
		typedef std::map<RibHandleNumber, RtLightHandle> NUM2LIGHT;	  //!< Maps a long to a light handle.
		typedef std::map<std::string, RtLightHandle> STR2LIGHT;       //!< Maps a string to a light handle.
		typedef std::map<RibHandleNumber, RtObjectHandle> NUM2OBJECT; //!< Maps a long to an object handle.
		typedef std::map<std::string, RtObjectHandle> STR2OBJECT;     //!< Maps a string to an object handle.
		// typedef std::map<RibHandleNumber, RtArchiveHandle> NUM2ARCHIVE; // Maps a long to an archive handle.
		typedef std::map<std::string, RtArchiveHandle> STR2ARCHIVE;     //!< Maps a string to an archive handle.

		typedef std::map<RibHandleNumber, std::string> NUM2STRING;    //!< Maps an integer to a string to encode string tokens.

		//! Maps number to object handle
		NUM2OBJECT m_mapObjectHandle;

		//! Maps name to object handle
		STR2OBJECT m_mapObjectStrHandle;

		//! Maps number to light handle
		NUM2LIGHT m_mapLightHandle;

		//! Maps name to light handle
		STR2LIGHT m_mapLightStrHandle;

		// Maps number to archive handle
		// NUM2ARCHIVE m_mapArchiveHandle;

		//! Maps name to archive handle
		STR2ARCHIVE m_mapArchiveStrHandle;

		//! @brief Frontnd values the parser needs to know.
		IRibParserCallback *m_parserCallback;
		
		//! @brief State of the backend renderer.
		CRenderState *m_renderState;
		
		//! @brief The callback for comments (@see IRiCPP::readArchiveV())
		const IArchiveCallback *m_callback;

		CUri m_baseUri; //!< The base URI for the rib resource.
		CUri m_absUri;  //!< The absolute URI of the rib resource.
		long m_lineNo;  //!< Current line number.

		bool m_hasPutBack;       //!< One character put back.
		unsigned char m_putBack; //!< The character has been put back (if m_hasOutBack == true).

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

		/** @brief Request handler for each token (e.g. "Sphere")
		 */
		static std::map<std::string, CRibRequest *> s_requestMap;

		/** @brief Calls the handler routine for a request token using s_requestMap.
		 *
		 *  @param request The string of request token
		 *  @return False, no CRibRequest handler found for \a request. True, handler found and called.
		 */
		bool call(const std::string &request);

		/** @brief Initializes s_requestMap.
		 */
		void initRequestMap();

		/** @brief Asks if token stands for a comment.
		 *
		 *  @return true, token stands for a comment (structured or normal) or false, otherwise.
		 */
		inline static bool isCommentToken(int token)
		{
			return token != RIBPARSER_NORMAL_COMMENT || token != RIBPARSER_STRUCTURED_COMMENT;
		}

		/** @brief Token is a Ri token (Sphere, Patch, Format etc.).
		 *
		 *  @return true, token stands for a Ri token, false: otherwise.
		 */
		inline static bool isRequestToken(int token)
		{
			return token == RIBPARSER_REQUEST;
		}

		/** @brief Test if token introduces a valid request or comment.
		 *
		 *  @return true, token is a request or a comment.
		 */
		inline static bool isValidToken(int token)
		{
			return isRequestToken(token) || isCommentToken(token);
		}

		/** @brief Stores a comment.
		 *
		 *  Comments found within an interface call are handled after the interface
		 *  call itself is handled, so the comments are stored until this is happend.
		 */
		class CComment {
		public:
			std::vector<char> m_comment; //!< Storage for a comment.
			bool m_isStructured;         //!< Comment is structured comment (##).
			long m_lineNo;               //!< Line count where the comment was found.
			
			/** @brief Assignes another comment to *this.
			 *  @return A reference to this.
			 */
			inline CComment &operator=(const CComment &c)
			{
				m_comment = c.m_comment;
				m_isStructured = c.m_isStructured;
				m_lineNo = c.m_lineNo;
				return *this;
			}
		};

		std::vector<CComment> m_deferedCommentList; //!< Comments, that are defered in this interface call.

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

		/** @brief list of parameters currently parsed.
		 */
		CParameterList m_parameterList;

		/** @brief RIB input stream.
		 */
		std::istream m_istream;

		/** @brief RIB input stream back buffer for m_istream.
		 */
		TemplFrontStreambuf<char> m_ob;

		/** @brief Put back one character to the stream.
		 *
		 *  Only one character can be put back.
		 *
		 *  @param c Character to put back.
		 */
		void putback(unsigned char c);

		/** @brief Gets the next character from input stream.
		 *
		 *  Get the nekt character or the put back character from the rib input stream.
		 *
		 *  @retrun Next character to examine.
		 */
		unsigned char getchar();
		
		/** @brief Clears the handle maps at the start of the parsing.
		 */
		inline void clearHandleMaps()
		{
			m_mapLightHandle.clear();
			m_mapLightStrHandle.clear();
			m_mapObjectHandle.clear();
			m_mapObjectStrHandle.clear();
			// m_mapArchiveHandle.clear();
			m_mapArchiveStrHandle.clear();
		}

		virtual bool bindObjectHandle(RtObjectHandle handle, RtInt number);
		virtual bool bindObjectHandle(RtObjectHandle handle, const char *name);
		virtual bool getObjectHandle(RtObjectHandle &handle, RtInt number) const;
		virtual bool getObjectHandle(RtObjectHandle &handle, const char *name) const;

		virtual bool bindLightHandle(RtLightHandle handle, RtInt number);
		virtual bool bindLightHandle(RtLightHandle handle, const char *handleName);
		virtual bool getLightHandle(RtLightHandle &handle, RtInt number) const;
		virtual bool getLightHandle(RtLightHandle &handle, const char *handleName) const;

		// virtual bool bindArchiveHandle(RtArchiveHandle handle, RtInt number);
		virtual bool bindArchiveHandle(RtArchiveHandle handle, const char *handleName);
		// virtual bool getArchiveHandle(RtArchiveHandle &handle, RtInt number) const;
		virtual bool getArchiveHandle(RtArchiveHandle &handle, const char *handleName) const;

	public:
		/** @brief Constructor
		 *
		 *  Initializes a new parser with the current render state
		 *
		 *  @brief aParserCallback Callbacks of the frontend.
		 *  @brief aRenderState The state object of the backend. 
		 *  @brief aBaseUri The base URI of the rib resources
		 *
		 */
		inline CRibParser(
			IRibParserCallback &aParserCallback,
			CRenderState &aRenderState,
			const CUri &aBaseUri) :
			m_ob(aParserCallback.protocolHandlers()),
			m_istream(&m_ob)
		{
			m_parserCallback = &aParserCallback;
			m_renderState = &aRenderState;
			m_baseUri = aBaseUri;
			m_lineNo = 0;
			m_hasPutBack = false;
			m_putBack = 0;
			m_request.init(*this);
			initRequestMap();
		}

		/** @brief Destructor.
		 *
		 *  Closes opened resources.
		 */
		inline virtual ~CRibParser()
		{
			close();
		}

		/** @brief Gets the base URI of the rib resources.
		 *
		 *  @return The base URI of the rib resources.
		 */
		inline virtual const CUri &baseUri() const
		{
			return m_baseUri;
		}

		/** @brief Gets the absoulte URI of the current rib resource.
		 *
		 *  @return The absoulte URI of the current rib resource.
		 */
		inline virtual const CUri &absUri() const
		{
			return m_absUri;
		}

		/** @brief Gets the current line number.
		 *
		 *  @return The current line number.
		 */
		inline virtual long lineNo() const
		{
			return m_lineNo;
		}

		/** @brief Gets the name of the current rib resource.
		 *
		 *  @return The name of the current rib resource.
		 */
		inline virtual const char *resourceName() const
		{
			return absUri().toString().c_str();
		}

		/** @brief Gets the archive callback for rib comments.
		 *
		 *  @return The archive callback for rib comments.
		 */
		inline virtual const IArchiveCallback *callback() const
		{
			return m_callback;
		}

		/** @brief Gets the current render state.
		 *
		 *  @return The current render state.
		 */
		inline virtual CRenderState &renderState() const
		{
			return *m_renderState;
		}

		/** @brief Gets the error handler of the frontend.
		 *
		 *  @return The error handler of the frontend.
		 */
		inline virtual IRiCPPErrorHandler &errHandler()
		{
			return m_parserCallback->ricppErrHandler();
		}

		/** @brief Gets the rib filter of the frontend.
		 *
		 *  Used to call the parsed rib requests.
		 *
		 *  @return The rib filter of the frontend.
		 */
		inline virtual IRiRoot &ribFilter()
		{
			return m_parserCallback->ribFilter();
		}

		/** @brief Closes a resource.
		 *
		 *  @return true, resource was closed.
		 */
		virtual bool close();

		/** @brief Find out if the file can be parsed by this type of parser and opens it.
		 *
		 *  @param name Resource name.
		 *  @return true, file has been opened and is parseable.
		 */
		virtual bool canParse(RtString name);

		/** @brief Parses a resource (already opend by canParse())
		 *
		 *  @param callback The callback for comments (@see IRiCPP::readArchiveV())
		 *  @param params   Parameters of IRiCPP::readArchiveV()
		 */
		virtual void parse(
			const IArchiveCallback *callback,
			const CParameterList &params);

		inline virtual CParameterList &params()
		{
			return m_parameterList;
		}
	}; // CRibParser
}

#endif // _RICPP_RIBPARSER_RIBPARSER_H
