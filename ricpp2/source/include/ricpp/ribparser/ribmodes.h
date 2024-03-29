#ifndef _RICPP_RIBPARSER_RIBMODES_H
#define _RICPP_RIBPARSER_RIBMODES_H

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

/** @file ribmodes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for modes (WorldBegin, WorldEnd, FrameBegin, FrameEnd, etc.).
 *
 *  The objects are used only by the implementation of the rib parser.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

	/** @brief Handles RIB request ResourceBegin.
	 */
	class CResourceBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE_BEGIN; }
	}; // CResourceBeginRibRequest

	/** @brief Handles RIB request ResourceEnd.
	 */
	class CResourceEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE_END; }
	}; // CResourceEndRibRequest

	/** @brief Handles RIB request Resource name type ...
	 */
	class CResourceRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_RESOURCE; }
	}; // CResourceEndRibRequest

	/** @brief Handles RIB request FrameBegin frameno.
	 */
	class CFrameBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_BEGIN; }
	}; // CFrameBeginRibRequest

	/** @brief Handles RIB request FrameEnd.
	 */
	class CFrameEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_END; }
	}; // CFrameEndRibRequest

	/** @brief Handles RIB request WorldBegin.
	 */
	class CWorldBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_BEGIN; }
	}; // CWorldBeginRibRequest

	/** @brief Handles RIB request WorldEnd.
	 */
	class CWorldEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_WORLD_END; }
	}; // CWorldEndRibRequest

	/** @brief Handles RIB request AttributeBegin.
	 */
	class CAttributeBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_BEGIN; }
	}; // CAttributeBeginRibRequest

	/** @brief Handles RIB request AttributeEnd.
	 */
	class CAttributeEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE_END; }
	}; // CAttributeEndRibRequest

	/** @brief Handles RIB request TransformBegin.
	 */
	class CTransformBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_BEGIN; }
	}; // CTransformBeginRibRequest

	/** @brief Handles RIB request TransformEnd.
	 */
	class CTransformEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_END; }
	}; // CTransformEndRibRequest

	/** @brief Handles RIB request SolidBegin type.
	 */
	class CSolidBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_BEGIN; }
	}; // CSolidBeginRibRequest

	/** @brief Handles RIB request SolidEnd.
	 */
	class CSolidEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SOLID_END; }
	}; // CSolidEndRibRequest

	/** @brief Handles RIB request ObjectBegin objectid.
	 */
	class CObjectBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_BEGIN; }
	}; // CObjectBeginRibRequest

	/** @brief Handles RIB request ObjectEnd.
	 */
	class CObjectEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_END; }
	}; // CObjectEndRibRequest

	/** @brief Handles RIB request ObjectInstance.
	 */
	class CObjectInstanceRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_OBJECT_INSTANCE; }
	}; // CObjectInstanceRibRequest

	/** @brief Handles RIB request ArchiveBegin name ...
	 */
	class CArchiveBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_BEGIN; }
	}; // CArchivetBeginRibRequest

	/** @brief Handles RIB request ArchiveEnd.
	 */
	class CArchiveEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_END; }
	}; // CArchiveEndRibRequest

	/** @brief Handles RIB request MotionBegin [v1 v2 ... vn ]
	 */
	class CMotionBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_BEGIN; }
	}; // CArchivetBeginRibRequest

	/** @brief Handles RIB request MotionEnd.
	 */
	class CMotionEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MOTION_END; }
	}; // CArchiveEndRibRequest

	/** @brief Handles RIB request IfBegin condition.
	 */
	class CIfBeginRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_IF_BEGIN; }
	}; // CIfBeginRibRequest

	/** @brief Handles RIB request ElseIf condition.
	 */
	class CElseIfRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ELSE_IF; }
	}; // CIfBeginRibRequest

	/** @brief Handles RIB request Else.
	 */
	class CElseRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ELSE; }
	}; // CIfBeginRibRequest

	/** @brief Handles RIB request IfEnd.
	 */
	class CIfEndRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_IF_END; }
	}; // CIfBeginRibRequest
}

#endif // _RICPP_RIBPARSER_RIBMODES_H
