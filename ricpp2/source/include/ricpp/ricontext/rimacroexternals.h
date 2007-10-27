#ifndef _RICPP_RICONTEXT_RIMACROEXTERNALS_H
#define _RICPP_RICONTEXT_RIMACROEXTERNALS_H

#ifndef _RICPP_RICONTEXT_RIMACROBASE_H
#include "ricpp/ricontext/rimacrobase.h"
#endif // _RICPP_RICONTEXT_RIMACROBASE_H

namespace RiCPP {


// ----------------------------------------------------------------------------
// External resources
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
class CRiMakeTexture : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeTexture"; }
	inline virtual const char *className() const { return CRiMakeTexture::myClassName(); }

	inline CRiMakeTexture(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeTexture, m_filterfunc", __LINE__, __FILE__);
		}
	}

	inline virtual ~CRiMakeTexture()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_TEXTURE; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb) {
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeTexture(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}
	inline CRiMakeTexture &operator=(const CRiMakeTexture &) {
		return *this;
	}
}; // CRiMakeTexture

///////////////////////////////////////////////////////////////////////////////
class CRiMakeBump : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex, m_swrap, m_twrap;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeBump"; }
	inline virtual const char *className() const { return CRiMakeBump::myClassName(); }

	inline CRiMakeBump(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex, RtToken swrap, RtToken twrap,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swrap(swrap), m_twrap(twrap), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeBump, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeBump()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_BUMP; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeBump(m_pic.c_str(), m_tex.c_str(), m_swrap.c_str(), m_twrap.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}
	inline CRiMakeBump &operator=(const CRiMakeBump &) {
		return *this;
	}
}; // CRiMakeBump

///////////////////////////////////////////////////////////////////////////////
class CRiMakeLatLongEnvironment : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeLatLongEnvironment"; }
	inline virtual const char *className() const { return CRiMakeLatLongEnvironment::myClassName(); }

	inline CRiMakeLatLongEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeLatLongEnvironment, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeLatLongEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_LAT_LONG_ENVIRONMENT; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeLatLongEnvironment(m_pic.c_str(), m_tex.c_str(), *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}
	inline CRiMakeLatLongEnvironment &operator=(const CRiMakeLatLongEnvironment &) {
		return *this;
	}
}; // CRiMakeLatLongEnvironment

///////////////////////////////////////////////////////////////////////////////
class CRiMakeCubeFaceEnvironment : public CVarParamRManInterfaceCall {
protected:
	std::string m_px, m_nx, m_py, m_ny, m_pz, m_nz, m_tex;
	RtFloat m_fov;
	IFilterFunc *m_filterfunc;
	RtFloat m_swidth, m_twidth;
public:
	inline static const char *myClassName(void) { return "CRiMakeCubeFaceEnvironment"; }
	inline virtual const char *className() const { return CRiMakeCubeFaceEnvironment::myClassName(); }

	inline CRiMakeCubeFaceEnvironment(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz,
		const char *tex, RtFloat fov,
		const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_px(px), m_nx(nx), m_py(py), m_ny(ny), m_pz(pz), m_nz(nz), m_tex(tex), m_fov(fov), m_swidth(swidth), m_twidth(twidth)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_filterfunc = filterfunc.duplicate();
		if ( !m_filterfunc ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiMakeCubeFaceEnvironment, m_filterfunc", __LINE__, __FILE__);
		}
	}
	inline virtual ~CRiMakeCubeFaceEnvironment()
	{
		if ( m_filterfunc )
			delete m_filterfunc;
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_CUBE_FACE_ENVIRONMENT; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_filterfunc != 0);
		if ( m_filterfunc ) {
			ri.preMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
			ri.doMakeCubeFaceEnvironment(m_px.c_str(), m_nx.c_str(), m_py.c_str(), m_ny.c_str(), m_pz.c_str(), m_nz.c_str(), m_tex.c_str(), m_fov, *m_filterfunc, m_swidth, m_twidth, parameters());
		}
	}
	inline CRiMakeCubeFaceEnvironment &operator=(const CRiMakeCubeFaceEnvironment &) {
		return *this;
	}
}; // CRiMakeCubeFaceEnvironment

///////////////////////////////////////////////////////////////////////////////
class CRiMakeShadow : public CVarParamRManInterfaceCall {
protected:
	std::string m_pic, m_tex;
public:
	inline static const char *myClassName(void) { return "CRiMakeShadow"; }
	inline virtual const char *className() const { return CRiMakeShadow::myClassName(); }

	inline CRiMakeShadow(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		const char *pic, const char *tex,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_pic(pic), m_tex(tex)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}
	inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_SHADOW; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
		ri.doMakeShadow(m_pic.c_str(), m_tex.c_str(), parameters());
	}
	inline CRiMakeShadow &operator=(const CRiMakeShadow &) {
		return *this;
	}
}; // CRiMakeShadow

///////////////////////////////////////////////////////////////////////////////
class CRiArchiveRecord : public CRManInterfaceCall {
protected:
	std::string m_type;
	std::string m_line;
public:
	inline static const char *myClassName(void) { return "CRiArchiveRecord"; }
	inline virtual const char *className() const { return CRiArchiveRecord::myClassName(); }

	inline CRiArchiveRecord(long aLineNo, RtToken type, const char *line) : CRManInterfaceCall(aLineNo), m_type(type), m_line(line) {}
	inline virtual EnumRequests interfaceIdx() const { return REQ_ARCHIVE_RECORD; }
	inline virtual void replay(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preArchiveRecord(m_type.c_str(), m_line.c_str());
		ri.doArchiveRecord(m_type.c_str(), m_line.c_str());
		if ( cb ) 
			(*cb)(ri, m_type.c_str(), m_line.c_str());
	}
	inline CRiArchiveRecord &operator=(const CRiArchiveRecord &) {
		return *this;
	}
}; // CRiArchiveRecord

///////////////////////////////////////////////////////////////////////////////
class CRiReadArchive : public CVarParamRManInterfaceCall {
protected:
	std::string m_filename;
	IArchiveCallback *m_callback;
public:
	inline static const char *myClassName(void) { return "CRiReadArchive"; }
	inline virtual const char *className() const { return CRiReadArchive::myClassName(); }

	inline CRiReadArchive(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aFilename, const IArchiveCallback *cb,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_filename(noNullStr(aFilename))
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);

		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline virtual ~CRiReadArchive()
	{
		if ( m_callback )
			delete m_callback;
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
	
	inline const IArchiveCallback *callback() const
	{
		return m_callback;
	}
	inline void callback(const IArchiveCallback *cb)
	{
		if ( m_callback )
			delete m_callback;
		m_callback = 0;
		if ( cb )
			m_callback = cb->duplicate();
	}

	inline RtString filename() const
	{
		return m_filename.c_str();
	}
	inline void filename(RtString aFilename)
	{
		m_filename = noNullStr(aFilename);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postReadArchive(m_filename.c_str(), cb, parameters());
	}

	inline virtual void preProcess(IDoRender &ri)
	{
		preProcess(ri, m_callback);
	}

	inline virtual void doProcess(IDoRender &ri)
	{
		doProcess(ri, m_callback);
	}

	inline virtual void postProcess(IDoRender &ri)
	{
		postProcess(ri, m_callback);
	}

	inline CRiReadArchive &operator=(const CRiReadArchive &c) {
		if ( this == &c )
			return *this;

		filename(c.filename());
		callback(c.callback());
		
		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiReadArchive

}

#endif // _RICPP_RICONTEXT_RIMACROEXTERNALS_H