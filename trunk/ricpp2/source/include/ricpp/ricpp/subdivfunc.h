#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#define _RICPP_RICPP_SUBDIVFUNC_H

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

/** @file subdivfunc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard subdivision functions and free function for IRi::procedural()
 *         not yet implemented
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#include <string>
#include <deque>
#include <vector>

namespace RiCPP {

/** @brief Data container for delayed read archive.
 */
class CDataDelayedReadArchive : public ISubdivData {
private:
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;
public:
	/** @brief Constructor, copies data
	 */
	inline CDataDelayedReadArchive(const RtPointer da=0)
	{
		data(da);
	}

	/** @brief Copy constructor
	 */
	inline CDataDelayedReadArchive(const CDataDelayedReadArchive &da)
	{
		*this = da;
	}

	inline virtual ISubdivData *duplicate() const
	{
		return new CDataDelayedReadArchive(*this);
	}

	/** @brief Assigns an ionstance of data to another.
	 *  @return *this.
	 */
	inline CDataDelayedReadArchive &operator=(const CDataDelayedReadArchive &da)
	{
		freeData();
		data(da.data());
		return *this;
	}

	inline virtual RtPointer data()
	{
		if ( !m_str.empty() )
			return (RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual const RtPointer data() const
	{
		if ( !m_str.empty() )
			return (const RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual void data(RtPointer data)
	{
		freeData();
		if ( !data )
			return;

		const char **cp = (const char **)data;
		m_strcontainer.push_back(std::string(cp && cp[0] ? cp[0] : ""));
		m_str.push_back(m_strcontainer[0].c_str());
	}

	inline virtual void freeData()
	{
		m_str.clear();
		m_strcontainer.clear();
	}
};

/** @brief Implements the DelayedReadArchive procedural.
 */
class CProcDelayedReadArchive : public ISubdivFunc {
public:
	static class CProcDelayedReadArchive func;
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcDelayedReadArchive(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Number of (string) arguments used by the function.
	 *  @return The number of (string) arguments used by the function.
	 */
	inline virtual RtInt numArgs() const { return 1; }

	/** @brief Delayed read of a RIB archive
	 * @param ri Interface to be used
	 * @param data Array of one string, the name of the archive as null terminated string
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivData *createSubdivData(RtPointer data) const
	{
		return new CDataDelayedReadArchive(data);
	}
	inline virtual ISubdivFunc &singleton()  { return func; }
};


/** @brief Data container for run program.
 */
class CDataRunProgram : public ISubdivData {
private:
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;
public:
	/** @brief Constructor, copies data
	 */
	inline CDataRunProgram(const RtPointer da=0)
	{
		data(da);
	}

	/** @brief Copy constructor
	 */
	inline CDataRunProgram(const CDataRunProgram &da)
	{
		*this = da;
	}

	inline virtual ISubdivData *duplicate() const
	{
		return new CDataRunProgram(*this);
	}

	/** @brief Assigns an ionstance of data to another.
	 *  @return *this.
	 */
	inline CDataRunProgram &operator=(const CDataRunProgram &da)
	{
		freeData();
		data(da.data());
		return *this;
	}

	inline virtual RtPointer data()
	{
		if ( !m_str.empty() )
			return (RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual const RtPointer data() const
	{
		if ( !m_str.empty() )
			return (const RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual void data(RtPointer data)
	{
		freeData();
		if ( !data )
			return;

		const char **cp = (const char **)data;

		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
		m_strcontainer.push_back(cp && cp[1] ? cp[1] : "");
		m_str.push_back(m_strcontainer[1].c_str());
	}

	inline virtual void freeData()
	{
		m_str.clear();
		m_strcontainer.clear();
	}
};

/** @brief Implements the RunProgram procedural.
 */
class CProcRunProgram : public ISubdivFunc {
public:
	static class CProcRunProgram func;
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcRunProgram(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Number of (string) arguments used by the function.
	 *  @return The number of (string) arguments used by the function.
	 */
	inline virtual RtInt numArgs() const { return 2; }

	/** @brief Run a helper program and capture output as RIB for ri
	 * @param ri Interface to be used
	 * @param data Array of two strings, the program name and its command line arguments
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivData *createSubdivData(RtPointer data) const
	{
		return new CDataRunProgram(data);
	}
	inline virtual ISubdivFunc &singleton()  { return func; }
};


/** @brief Data container for dynamic load.
 */
class CDataDynamicLoad : public ISubdivData {
private:
	std::deque<std::string> m_strcontainer;
	std::vector<RtString> m_str;
public:
	/** @brief Constructor, copies data
	 */
	inline CDataDynamicLoad(const RtPointer da=0)
	{
		data(da);
	}

	/** @brief Copy constructor
	 */
	inline CDataDynamicLoad(const CDataDynamicLoad &da)
	{
		*this = da;
	}

	inline virtual ISubdivData *duplicate() const
	{
		return new CDataDynamicLoad(*this);
	}

	/** @brief Assigns an ionstance of data to another.
	 *  @return *this.
	 */
	inline CDataDynamicLoad &operator=(const CDataDynamicLoad &da)
	{
		freeData();
		data(da.data());
		return *this;
	}

	inline virtual RtPointer data()
	{
		if ( !m_str.empty() )
			return (RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual const RtPointer data() const
	{
		if ( !m_str.empty() )
			return (const RtPointer)&(m_str[0]);
		else
			return 0;
	}

	inline virtual void data(RtPointer data)
	{
		freeData();
		if ( !data )
			return;

		const char **cp = (const char **)data;

		m_strcontainer.push_back(cp && cp[0] ? cp[0] : "");
		m_str.push_back(m_strcontainer[0].c_str());
		m_strcontainer.push_back(cp && cp[1] ? cp[1] : "");
		m_str.push_back(m_strcontainer[1].c_str());
	}

	inline virtual void freeData()
	{
		m_str.clear();
		m_strcontainer.clear();
	}
};


/** @brief Implements the DynamicLoad procedural.
 */
class CProcDynamicLoad : public ISubdivFunc {
public:
	static class CProcDynamicLoad func;
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual ISubdivFunc *duplicate() const { return new CProcDynamicLoad(*this); }

	/** @brief The name of the function
	 *  @return The name of the function as used in RIB
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief Number of (string) arguments used by the function.
	 *  @return The number of (string) arguments used by the function.
	 */
	inline virtual RtInt numArgs() const { return 2; }

	/** @brief Calls a dynamic library implementing: RtPointer ConvertParameters(IRi &ri, char *initial data), 
	 * void Subdivide(IRi &ri, RtPointer blinddata, RtFloat detailsize), void Free(IRi &ri, RtPointer blinddata)
	 * @param ri Interface to be used
	 * @param data Array of two strings, the library name and its parameters (converted to blinddata by ConvertParameters)
	 * @param detail level of detail of the bounding box of the procedural or RI_INFINITY
	 */
	virtual RtVoid operator()(IRi &ri, RtPointer data, RtFloat detail) const;

	inline virtual ISubdivData *createSubdivData(RtPointer data) const
	{
		return new CDataDynamicLoad(data);
	}
	inline virtual ISubdivFunc &singleton()  { return func; }
};

/** @brief Implements the free function that can be called by the interface to free the 'data'
 *  of own procedurals.
 */
class CProcFree : public IFreeFunc {
public:
	static class CProcFree func;
	static RtToken myName();

	/** @brief Clone the instance.
	 *  @return A new cloned instance of the function.
	 */
	inline virtual IFreeFunc *duplicate() const { return new CProcFree(*this); }

	/** @brief The name of the function
	 *  @return The name of the function (no RIB binding)
	 */
	inline virtual RtToken name() const {return myName();}

	/** @brief The implementation of the free function
	 *
	 * Just call free() for the data.
	 *
	 * @param data
	 */
	virtual RtVoid operator()(RtPointer data) const;

	inline virtual IFreeFunc &singleton()  { return func; }
};


/** @brief Factory for subdivision functions.
 */
class CSubdivFuncFactory {
public:
	inline virtual ~CSubdivFuncFactory() {}

	virtual ISubdivFunc *newFunc(RtToken name);
	virtual void deleteFunc(ISubdivFunc *f);

	virtual ISubdivFunc *singleton(RtToken name) const;
};


/* @brief Factory for free functions.
class CFreeFuncFactory {
public:
	inline virtual ~CFreeFuncFactory() {}

	virtual IFreeFunc *newFunc(RtToken name);
	virtual void deleteFunc(IFreeFunc *f);

	virtual IFreeFunc *singleton(RtToken name) const;
};
*/

}

#endif // _RICPP_RICPP_SUBDIVFUNC_H
