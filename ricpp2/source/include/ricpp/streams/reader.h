#ifndef _RICPP_STREAMS_READER_H
#define _RICPP_STREAMS_READER_H

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

/** @file reader.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Base class for input streams.
 */


#ifndef _RICPP_STREAMS_URI_H
#include "ricpp/streams/uri.h"
#endif // _RICPP_STREAMS_URI_H

#ifndef _RICPP_STREAMS_BUFFER_H
#include "ricpp/streams/buffer.h"
#endif // _RICPP_STREAMS_BUFFER_H

#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#include "ricpp/pluginhandler/pluginhandler.h"
#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

namespace RiCPP {

	class CReader : public IPlugin {
		CStringList m_schemes;

	protected:
		void addScheme(const char *scheme)
		{
			std::string str_scheme(nonullstr(scheme));
			if ( str_scheme.empty() )
				return;
			std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(toupper));
			m_schemes.push(str_scheme, false, false, false);
		}
	public:
		/** @brief Const iterator for the protocol identifiers.
		 */
		typedef CStringList::const_iterator const_iterator;

		/** @brief Size type for the number of stored protocol identifiers.
		 */
		typedef CStringList::size_type size_type;

		static const char *myName();
		static const char *myType();
		static unsigned long myMajorVersion();
		static unsigned long myMinorVersion();
		static unsigned long myRevision();

		inline virtual const char *name() const { return myName(); }
		inline virtual const char *type() const { return myType(); }
		inline virtual unsigned long majorVersion() const {return myMajorVersion(); }
		inline virtual unsigned long minorVersion() const {return myMinorVersion(); }
		inline virtual unsigned long revision() const { return myRevision(); }

		inline virtual void startup() {}
		inline virtual void shutdown() {}

		inline CReader() {}
		inline virtual ~CReader() {}

		inline virtual bool acceptsScheme(const char *scheme) const
		{
			std::string str_scheme(nonullstr(scheme));
			std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(asciiToUpper));
			return m_schemes.isMember(str_scheme.c_str());
		}

		inline virtual const_iterator begin() const { return m_schemes.begin(); }
		inline virtual const_iterator end() const { return m_schemes.end(); }
		inline virtual size_type size() const { return m_schemes.size(); }

		inline virtual bool open(const char *anURI) { return false; }
		inline virtual TBuffer<>::size_type fillBuffer(TBuffer<> &buffer) { return 0; }
		inline virtual bool close() { return false; }
	}; // CReader

} // namespace RiCPP

#endif // _RICPP_STREAMS_READER_H
