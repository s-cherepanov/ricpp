#include "ricpp/baserenderer/parameter.h"
#include "ricpp/ricpp/ricpperror.h"

using namespace RiCPP;

void CParameter::copyStringPtr()
{
	m_stringPtrs.reserve(m_strings.size());
	for ( std::vector<std::string>::iterator i = m_strings.begin(); i != m_strings.end(); i++ ) {
		m_stringPtrs.push_back( (*i).c_str() );
	}
}

void CParameter::clear()
{
	if ( m_declaration && m_declaration->isInline() )
		delete m_declaration;

	m_declaration = 0;
	m_position = 0;

	m_ints.clear();
	m_floats.clear();
	m_stringPtrs.clear();
	m_strings.clear();
}

CParameter &CParameter::operator=(const CParameter &param)
{
	if ( this == &param )
		return *this;

	clear();

	if ( param.m_declaration && param.m_declaration->isInline() ) {
		m_declaration = new CDeclaration(*param.m_declaration);
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "While assigning parameter %s", param.name());
	} else {
		m_declaration = param.m_declaration;
	}

	m_position = param.m_position;

	m_ints = param.m_ints;
	m_floats = param.m_floats;
	m_strings = param.m_strings;

	copyStringPtr();
	return *this;
}

void CParameter::set(
	RtToken theName,
	RtPointer theData,
	unsigned int thePosition,
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	unsigned int curColorSize)
{
	clear();
	if ( !theName )
		return;

	const CDeclaration *decl = dict.findAndUpdate(theName, curColorSize);
	if ( !decl ) {
		// inline
		decl = new CDeclaration(theName, curColorSize);
		if ( !decl ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Parameter of %s", theName);
		}
		if ( decl->isInline() ) {
			delete decl;
			throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR, __LINE__, __FILE__, "Parameter of %s, no declaration or illegal inline declaration", theName);
		}
	}

	unsigned long elems = decl->selectNumber(counts) * decl->elemSize();

	if ( elems > 0 && !theData ) {
		clear();
		throw ExceptRiCPPError(RIE_MISSINGDATA, RIE_ERROR, __LINE__, __FILE__, "Parameter of %s", theName);
	}

	m_position = thePosition;

	if ( theData ) {
		switch ( decl->basicType() ) {
			case BASICTYPE_INTEGER:
				m_ints.reserve(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_ints.push_back(((RtInt *)theData)[cnt]);
				}
				break;
			case BASICTYPE_FLOAT:
				m_floats.reserve(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_floats.push_back(((RtFloat *)theData)[cnt]);
				}
				break;
			case BASICTYPE_STRING:
				m_strings.resize(elems);
				for (unsigned long cnt = 0; cnt < elems; ++cnt ) {
					m_strings[cnt] = ((const char *)theData)[cnt];
				}
				copyStringPtr();
				break;
			default:
				throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR, __LINE__, __FILE__, "Parameter of %s, no type info found", theName);
				break;
		}
	}
}


RtPointer CParameter::valptr()
{
	switch ( basicType() ) {
		case BASICTYPE_INTEGER:
			if ( m_ints.empty() )
				return 0;
			return static_cast<RtPointer>(&m_ints[0]);
		case BASICTYPE_FLOAT:
			if ( m_floats.empty() )
				return 0;
			return static_cast<RtPointer>(&m_floats[0]);
		case BASICTYPE_STRING:
			if ( m_stringPtrs.empty() )
				return 0;
			return static_cast<RtPointer>(&m_stringPtrs[0]);
		default:
			return 0;
	}
}

CParameterList::CParameterList(
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	unsigned int curColorSize,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	set(counts, dict, curColorSize, n, tokens, params);
}


void CParameterList::rebuild()
{
	m_tokenPtr.clear();
	m_paramPtr.clear();

	for (
		std::list<CParameter>::iterator i = m_params.begin();
		i != m_params.end();
		++i )
	{
		CParameter &p = (*i);
		const CDeclaration *d = p.declaration();
		if ( d && d->isInline() ) {
			m_tokenPtr.push_back(d->name());
		} else {
			m_tokenPtr.push_back(d->token());
		}
		m_paramPtr.push_back(p.valptr());
	}
}


CParameterList &CParameterList::operator=(const CParameterList &params)
{
	if ( this == &params )
		return *this;
	clear();

	for (
		const_iterator i = params.begin();
		i != params.end();
		++i )
	{
		m_params.push_back(*i);
		m_paramMap[m_params.back().name()] = &m_params.back();
	}

	rebuild();

	return *this;
}

void CParameterList::set(
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	unsigned int curColorSize,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	clear();
	add(counts, dict, curColorSize, n, tokens, params);
}

void CParameterList::add(
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	unsigned int curColorSize,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	for ( RtInt i = 0; i < n; ++n ) {
		const CParameter *param = get(tokens[i]);
		if ( param ) {
			erase(param);
		}
		m_params.push_back(CParameter());
		m_params.back().set(tokens[i], params[i], i, counts, dict, curColorSize);
		m_paramMap[m_params.back().token()] = &m_params.back();
	}
	rebuild();
}

const CParameter *CParameterList::get(const char *name) const
{
	Map_type::const_iterator i = m_paramMap.find(name);
	if ( i != m_paramMap.end() ) {
		return i->second;
	}
	return 0;
}

bool CParameterList::erase(const char *name)
{
	const CParameter *param = get(name);
	if ( !param )
		return false;

	std::list<CParameter>::iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( param == &(*i) ) {
			m_paramMap.erase(param->name());
			m_params.erase(i);
			rebuild();
			return true;
		}
	}

	return false;
}

bool CParameterList::erase(const CParameter *param)
{
	if (!param )
		return false;

	std::list<CParameter>::iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( param == &(*i) ) {
			m_paramMap.erase(param->name());
			m_params.erase(i);
			rebuild();
			return true;
		}
	}

	return false;
}

bool CParameterList::hasColor() const
{
	std::list<CParameter>::const_iterator i;
	for ( i = m_params.begin(); i != m_params.end(); ++i ) {
		if ( (*i).type() == TYPE_COLOR ) {
			return true;
		}
	}
	return false;
}


CNamedParameterList &CNamedParameterList::operator=(const CNamedParameterList &params)
{
	if ( this == &params )
		return *this;

	m_name = params.m_name;
	m_curColorDescr = params.m_curColorDescr;

	CParameterList::operator=(params);
	return *this;
}

void CNamedParameterList::set(
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &colorDescr,
	const char *aName,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	name(aName);
	m_curColorDescr = colorDescr;
	CParameterList::set(counts, dict, m_curColorDescr.colorSamples(), n, tokens, params);
}

void CNamedParameterList::add(
	const CValueCounts &counts,
	CDeclarationDictionary &dict,
	const CColorDescr &colorDescr,
	RtInt n, RtToken tokens[], RtPointer params[])
{
	if ( hasColor() && m_curColorDescr != colorDescr ) {
		throw(ExceptRiCPPError(RIE_CONSISTENCY, RIE_ERROR, "color descriptors of parameters differs to others of the same type"));
	}

	CParameterList::add(counts, dict, colorDescr.colorSamples(), n, tokens, params);

	if ( hasColor() ) {
		m_curColorDescr = colorDescr;
	}
}
