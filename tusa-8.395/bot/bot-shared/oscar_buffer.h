#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <set>
#include <deque>
#include <iostream>
#include <iomanip.h>
#include <string>
#include <iterator>
#include <algorithm>
#include <map>
#include <list>
#ifndef WIN32

#endif
#include "_mutex.h"
//#include "M_MAP.h"
#ifdef __OSCAR_SIGN_17
#	define OSCAR_START_BYTE 17
#else
#	ifdef __OSCAR_SIGN_42
#		define OSCAR_START_BYTE 42
#	else
#	error must define  __OSCAR_SIGN_42 or __OSCAR_SIGN_17
#	endif		
#endif

#ifdef __OSCAR_DATALEN_16
#	define OSCAR_EXTRA_DATA_LEN 6
	typedef unsigned short OSCAR_DATA_LEN_T;
#else
#	ifdef __OSCAR_DATALEN_32
#		define OSCAR_EXTRA_DATA_LEN 8
	typedef unsigned long OSCAR_DATA_LEN_T;
#	else
#	error must define  __OSCAR_DATALEN_16 or __OSCAR_DATALEN_32
#	endif		
#endif

using namespace std;
#ifdef WIN32
typedef __int64 datetime;
#else
typedef long long datetime;
#endif

class oscar_buffer;


class oscar_buffer {
 public:
  enum endian { BIG, LITTLE };


 private:
  endian endn;
  unsigned int out_pos;

 public:
  deque<unsigned char> data;
 ~oscar_buffer(){}
  oscar_buffer();
  oscar_buffer(const unsigned char *d, int size); // construct from an array
  oscar_buffer(oscar_buffer& b, unsigned int start, unsigned int data_len); // construct by copying from another oscar_buffer
//	deque<unsigned char>	getData() const {return data;}
//  const unsigned char* Data() { return data.begin(); }
  unsigned int size() const { return data.size(); }
  unsigned int pos() const { return out_pos; }
  int remains() const { return data.size() - out_pos; }
	void assign(const deque<unsigned char>& b){data=b;}
  void clear();
  bool empty();
  void advance(unsigned int ad) { out_pos += ad; }
  bool beforeEnd() const { return (out_pos < data.size()); }
  void setPos(unsigned int o) { out_pos = o; }
  void chopOffoscar_buffer(oscar_buffer& b, unsigned int sz);

  void setEndianness(endian e);


  oscar_buffer& operator<<(unsigned char);
  oscar_buffer& operator<<(char);
  oscar_buffer& operator<<(bool);
  oscar_buffer& operator<<(datetime);
  oscar_buffer& operator<<(unsigned short);
  oscar_buffer& operator<<(unsigned int);
  oscar_buffer& operator<<(unsigned long int);
  oscar_buffer& operator<<(long int);
  oscar_buffer& operator<<(signed char l) { return (*this) << (unsigned char)l; }
  oscar_buffer& operator<<(signed short l) { return (*this) << (unsigned short)l; }
  oscar_buffer& operator<<(signed int l) { return (*this) << (unsigned int)l; }

  oscar_buffer& operator>>(unsigned char&);
  oscar_buffer& operator>>(char&);
  oscar_buffer& operator>>(bool&);
  oscar_buffer& operator>>(datetime&);
  oscar_buffer& operator>>(unsigned short&);
  oscar_buffer& operator>>(unsigned int&);
  oscar_buffer& operator>>(signed int&);
  oscar_buffer& operator>>(unsigned long int&);
  oscar_buffer& operator>>(long int&);

  void Pack(const unsigned char *d, int size);
  void Pack(const string& s);
  void Unpack(string& s, unsigned int size) ;
  void Unpack_const(string& s, unsigned int size)  const;
  string as_string() const;

  unsigned char& operator[](unsigned int p);

  void dump(std::ostream& out);

	struct COLOR
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		COLOR(unsigned char r,unsigned char g,unsigned char b): R(r),G(g),B(b){}
		COLOR(){}
		void Output(oscar_buffer&b) const{b<<R<<G<<B;}
		void Input(oscar_buffer&b){b>>R>>G>>B;}
	};
	struct NTS
	{
		string container;	
		NTS(const string &s);
		NTS &operator=(const string&s);
		NTS();
		void Output(oscar_buffer&b) const;
		void Input(oscar_buffer&b);
		const char* c_str() const {return container.c_str();}
	};
struct B_UIN
{
	string  container;	
	B_UIN(unsigned int s);
	B_UIN &operator=(unsigned int s);
	B_UIN();
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
};

struct oscar_STRING
{
	string container;	
	oscar_STRING(const string &s);
	oscar_STRING &operator=(const string&s);
	oscar_STRING();
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
	
};
struct LNTS
{
	string container;	
	LNTS(const string &s);
	LNTS(){}
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
};
struct L_STR
{
	string container;	
	L_STR(const string &s);
	L_STR(const char* s);
	L_STR(){}
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
};
struct L_L_STR
{
	string container;	
	L_L_STR(const string &s);
	L_L_STR(const char* s);
	L_L_STR(){}
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
};
struct BNTS
{
	string container;	
	BNTS(const string &s);
	BNTS(){}
	void Output(oscar_buffer&b) const;
	void Input(oscar_buffer&b);
	const char* c_str() const {return container.c_str();}
};

struct FAM_T
{
  enum fam_t
  {
    Undef=0,SIRCDComp = 100, SIRCDComps, SIRCDCustomer, SIRCDCustomers, SIRCDDocument,
    SIRCDDocuments, SIRCDDomain, SIRCDDomains, SIRCDEvent, SIRCDEvents, SIRCDFile,
    SIRCDFolder, SIRCDFolders, SIRCDGroup, SIRCDGroups, SIRCDIndex, SIRCDIndexDef,
    SIRCDIndexDefs, SIRCDIndexes, SIRCDLayout, SIRCDLayouts, SIRCDManager,
    SIRCDNote, SIRCDNotes, SIRCDObject, SIRCDOptions, SIRCDSchema, SIRCDSchemas,
    SIRCDSession, SIRCDSource, SIRCDSources, SIRCDStencil, SIRCDStencils,
    SIRCDTextArchive, SIRCDTextArchives, SIRCDTransfer, SIRCDTransfers, SIRCDUnit,
    SIRCDUnits, SIRCDUser, SIRCDUsers, SIRCDView,
    SIRCDViews,SIRCD_base
  };
};
struct SUBT_T
{
  enum subt_t
  {
    Undef=0,add =1,
	addComp, addLayout, addSchema, addView, checkDuplicate, clone, close,
    count, createDocument, createTable, deleteComp, deleteDoc, deleteEvents,
    deleteLayout, deleteSchema, deleteStencil, deleteView, deregisterCD,
    distributeCD, excludeUser, findFolder, findNote, findSource, findStencil,
    findTransfer, findUser, getAllDocuments, getAllDomainGroups,
    getAllGroupUsers, getAllGroups, getAllGroupsByRole, getAllUsers,
    getArchive, getArchiveID, getArchives, getBDID, getBottomMargin, getCDID,
    getCDPathText, getCDUnits, getCDVersion, getCRC, getClass, getClickMode,
    getClientCode, getCode, getComp, getCompID, getComps, getCreated,
    getCreator, getCreatorID, getCriteria, getCustID, getCustomer,
    getCustomerByCode, getCustomerCode, getCustomerID, getCustomers, getDate,
    getDateArchive, getDeadline, getDesc, getDescription, getDetails,
    getDisplayName, getDocID, getDocName, getDocViewID, getDocument,
    getDocumentID, getDocuments, getDomID, getDomain, getDomainByCode,
    getDomainCode, getDomainID, getDomains, getDoubleClickMode, getEdition,
    getEditor, getErrMsg, getErrorCode, getEvent, getEvents, getFirstName,
    getFolder, getFolderID, getFolderLevel, getFolders, getFontBlueColor,
    getFontEffect, getFontFace, getFontGreenColor, getFontRedColor,
    getFontSize, getFontStyle, getGroup, getGroupID, getGroupRecipient,
    getHost, getID, getId, getInboxFolders, getIndex, getIndexDef,
    getIndexDefs, getIndexes, getInfo, getKindDocument, getKindView,
    getLandscapeCols, getLandscapeRows, getLastChanged, getLastLogin,
    getLastName, getLayout, getLayouts, getLeftMargin, getLikeValue,
    getLoggedUser, getLogicalName, getLogin, getMarkFindText, getMethod,
    getName, getNextID, getNote, getNoteBackColor, getNoteForeColor,
    getNoteID, getNoteTransparant, getNumberPages, getObj, getObjID,
    getObjName, getOffset, getOffsetX, getOffsetY, getOptions, getOrderField,
    getOwnerID, getPage, getPageNotes, getPageOffset, getPageOrientation,
    getPageSVG, getParentFolderID, getPath, getPerformed, getPerformer,
    getPhysicalName, getPortraitCols, getPortraitRows, getPrintStripe,
    getRightMargin, getRole, getSVGStencil, getSVGTag, getSchema, getSchemas,
    getSelectSQL, getSession, getSign, getSize, getSourceDocID, getSourceID,
    getSourceMD, getSourceName, getSources, getStageID, getStatus, getStencil,
    getStencilID, getStencils, getSubsystem, getTargetID, getText,
    getTextValue, getTime, getTopMargin, getTransfer, getTransfers, getType,
    getURL, getUnit, getUser, getUserByLogon, getUserGroups, getUserID,
    getUserLayouts, getUserNotes, getUserOptions, getUserRecipient,
    getValidation, getValue, getVersion, getView, getViewByID, getViewID,
    getViewMode, getViews, getWidthStripe, getWindowsMode, getZoomMode,
    getZoomScaleStep, hasRight, includeUser, init, initArchive,
    initRootFolder, initSnapshot, initSource, initSubfolder, initTransfer,
    insertStencil, isAdmin, isConfidential, isCustomerOfDomain, isIndexed,
    isMatchCase, isPublicRecipient, isQuery, isSnapshot, isSupervisorOfDomain,
    isTemplate, isVisible, isWholeWord, logoff, move, newComp, newComps,
    newCustomer, newDocument, newDocuments, newDomain, newFile, newGroup,
    newIndex, newIndexDef, newIndexDefs, newInitArchive, newInitDocument,
    newInitFolder, newInitSchema, newInitSource, newInitView, newLayout,
    newNote, newStencil, newTransfer, newUser, open, openSession,
    registerEvent, registerUnit, remove, rename, reorganize, save,
    searchDocuments, searchFolders, setArchiveID, setClickMode, setCode,
    setConfidential, setCreated, setCreator, setCustID, setCustomerCode,
    setCustomerID, setDBParam, setDeadline, setDescription, setDisplayName,
    setDocName, setDomID, setDomainID, setDoubleClickMode, setEditor,
    setFirstName, setFontFace, setHost, setIndexDefs, setIndexed, setInfo,
    setKind, setLastChanged, setLastLogin, setLastName, setLikeValue,
    setLogicalName, setLogin, setMarkFind, setName, setNoteColors, setObjName,
    setOffset, setOrderField, setOwnerID, setPageID, setPageMargins,
    setPageOrientation, setPassword, setPerformed, setPerformer,
    setPhysicalName, setPrintStripe, setRecipient, setRights, setRole,
    setSign, setSize, setSourceMD, setSourceName, setStatus, setTargetID,
    setText, setTextValue, setType, setViewID, setViewMode, setVisible,
    setWidthStripe, setWindowsMode, setZoomMode, setZoomScaleStep, size,
    start, stop, translateFSLtoSVG, update, updateURL, validatePassword,
	AddRef,Release, getFieldValues
  };

  
};

template <class Type>
class ICQOBJ
{
	public:
	Type obj;
	ICQOBJ(){obj=0;}
	ICQOBJ(const Type& v){obj=v;}
	ICQOBJ &operator=(const Type& v){obj=v; return *this;}
	void Output(oscar_buffer&b) const{	b<< obj;}
	void Input(oscar_buffer&b){b>>obj;}
};
template <class Type>
class ICQOBJ_LE
{
	public:
	Type obj;
	ICQOBJ_LE(){obj=0;}
	ICQOBJ_LE(const Type& v){obj=v;}
	ICQOBJ_LE &operator=(const Type& v){obj=v; return *this;}
	void Output(oscar_buffer&b)const {b.setEndianness(oscar_buffer::LITTLE);	b<< obj;b.setEndianness(oscar_buffer::BIG);}
	void Input(oscar_buffer&b){b.setEndianness(oscar_buffer::LITTLE);b>>obj;b.setEndianness(oscar_buffer::BIG);}
};

struct SNAC_head
{
	FAM_T::fam_t family;
	SUBT_T::subt_t subtype;
	unsigned char flagA; // not used
	unsigned char flagB; // not used
	unsigned int request_id; // not used
	void read(oscar_buffer &b)
	{
		unsigned short f,t;
		b>>f>>t>>flagA>>flagB>>request_id;
		family=(FAM_T::fam_t)f;
		subtype=(SUBT_T::subt_t)t;
	}
	SNAC_head():family(FAM_T::Undef),subtype(SUBT_T::Undef),flagA(0),flagB(0),request_id(0){}
};
struct FLAP_head
{
	unsigned char start_byte;
	unsigned char channel;
	unsigned short seq_num;
	unsigned short size;
	SNAC_head snac;
	void read(oscar_buffer &b)
	{
		b>>start_byte>>channel>>seq_num>>size;
		snac.read(b);
	}
	FLAP_head(): start_byte(0),channel(0),seq_num(0),size(0){}
	
};


};
class out_oscar_buffer:public oscar_buffer
{
	public:
	bool in_snac;
	bool in_flap;
	out_oscar_buffer(): in_snac(false),in_flap(false){}
	
};
std::ostream& operator<<(std::ostream&,oscar_buffer&);

class tlv: public oscar_buffer
{
	protected:
	tlv(){}
	public:
	unsigned short number;
	tlv(unsigned short n): number(n){}
};
class in_tlv: public tlv
{
	public:
	in_tlv(){}
};
oscar_buffer& operator<<(oscar_buffer& b, const tlv& obj);
oscar_buffer& operator>>(oscar_buffer& b,tlv& obj); 

typedef oscar_buffer::ICQOBJ_LE<unsigned int> UIN;
typedef oscar_buffer::ICQOBJ_LE<unsigned int> DWORD_LE;
typedef oscar_buffer::ICQOBJ_LE<unsigned short> WORD_LE;
typedef oscar_buffer::ICQOBJ<unsigned int> DWORD_BE;
typedef oscar_buffer::ICQOBJ<unsigned short> WORD_BE;
typedef oscar_buffer::ICQOBJ<unsigned char> __BYTE;
typedef oscar_buffer::ICQOBJ<oscar_buffer> TLV;
typedef oscar_buffer::ICQOBJ<unsigned int> IPADDR;

template <class Type>
	oscar_buffer& operator<<(oscar_buffer& b, const oscar_buffer::ICQOBJ<Type>& obj) { obj.Output(b); return b; }
template <class Type>
	oscar_buffer& operator<<(oscar_buffer& b, const oscar_buffer::ICQOBJ_LE<Type>& obj) { obj.Output(b); return b; }
template <class Type>
	oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::ICQOBJ<Type>& obj) { obj.Input(b); return b; }
template <class Type>
	oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::ICQOBJ_LE<Type>& obj) { obj.Input(b); return b; }


template < class T1, class T2 > oscar_buffer & operator << (oscar_buffer& b,const pair < T1, T2 > &v) {
	b << v.first << v.second;
	return b;
}
template < class T > oscar_buffer & operator << (oscar_buffer& b,const set < T > &v) {
	unsigned int sz = v.size();
	b << sz;
	for (typeof(v.begin()) j = v.begin(); j != v.end(); j++) {
		b << *j;
	}
	return b;
}
template < class T > oscar_buffer & operator << (oscar_buffer& b,const vector < T > &v) {
	unsigned int sz = v.size();
	b << sz;
	for (unsigned i=0;i<v.size();i++) {
		b << v[i];
	}
	return b;
}
template < class T > oscar_buffer & operator << (oscar_buffer& b,const list < T > &v) {
	unsigned int sz = v.size();
	b << sz;
	for (typeof(v.begin()) j = v.begin(); j < v.end(); j++) {
		b << *j;
	}
	return b;
}
template < class T > oscar_buffer & operator << (oscar_buffer& b,const deque < T > &v) {
	unsigned int sz = v.size();
	b << sz;
	for (typeof(v.begin()) j = v.begin(); j < v.end(); j++) {
		b << *j;
	}
	return b;
}
template < class T1, class T2 > oscar_buffer & operator << (oscar_buffer& b,const map < T1, T2 > &v) {
	unsigned int sz = v.size();
	b << sz;
	for (typeof(v.begin()) j = v.begin(); j != v.end(); j++) {
		b << j->first << j->second;
	}
	return b;
}
	  template < class T > oscar_buffer & operator >> (oscar_buffer& b,set < T > &v)
	{
		unsigned int n;
		 b >> n;
		for (unsigned int j = 0; j < n; j++)
		{
			T t;
			 b >> t;
			  v.insert(t);
		}
		return b;
	}
	template < class T > oscar_buffer& operator >> (oscar_buffer& b,list < T > &v) {
		unsigned int n;
		b >> n;
		for (unsigned int j = 0; j < n; j++) {
			T t;
			b >> t;
			v.push_back(t);
		}
		return b;
	}
	template < class T1, class T2 > oscar_buffer & operator >> (oscar_buffer& b,map < T1, T2 > &v) {
		unsigned int n;
		b >> n;
		for (unsigned int j = 0; j < n; j++) {
			T1 t1;
			T2 t2;
			b >> t1;
			b >> t2;
			v.insert(pair<T1,T2>(t1,t2));
		}
		return b;
	}
	template < class T1, class T2 > oscar_buffer & operator >> (oscar_buffer& b,pair < T1, T2 > &v) {
			b >> v.first;
			b >> v.second;
		return b;
	}
	template < class T > oscar_buffer & operator >> (oscar_buffer& b,vector < T > &v) {
		unsigned int n;
		b >> n;
		for (unsigned int j = 0; j < n; j++) {
			T t;
			b >> t;
			v.push_back(t);
		}
		return b;
	}
	template < class T > oscar_buffer & operator >> (oscar_buffer& b,deque < T > &v) {
		unsigned int n;
		b >> n;
		for (unsigned int j = 0; j < n; j++) {
			T t;
			b >> t;
			v.push_back(t);
		}
		return b;
	}





oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::LNTS& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::L_STR& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::L_L_STR& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::B_UIN& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::BNTS& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::NTS& obj);
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::oscar_STRING& obj);
oscar_buffer& operator>>(oscar_buffer & b, map<unsigned short,in_tlv>   &nts); 
oscar_buffer& operator>>(oscar_buffer & b, oscar_buffer::COLOR   &c); 



oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::NTS & nts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::LNTS & lnts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::L_STR & lnts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::L_L_STR & lnts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::BNTS & lnts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::oscar_STRING & lnts); 
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::B_UIN & lnts);
oscar_buffer& operator<<(oscar_buffer & b, const oscar_buffer::COLOR  &nts); 


oscar_buffer & operator << (oscar_buffer & b,  const oscar_buffer  &n);

out_oscar_buffer snac(unsigned short fam,unsigned short stype,const out_oscar_buffer &b);
out_oscar_buffer snac(unsigned short fam, unsigned short stype,unsigned int request_id,const out_oscar_buffer &b);
//out_oscar_buffer snac(unsigned short fam, unsigned short stype,unsigned int request_id,const oscar_buffer &b);
//#ifndef __CAMFROG__
out_oscar_buffer snac(unsigned short fam,	unsigned short stype,unsigned char flagA,unsigned char flagB,unsigned int request_id,const out_oscar_buffer &b);
//#endif
out_oscar_buffer flap(unsigned char channel,unsigned short seq_num, const out_oscar_buffer &b);
out_oscar_buffer flap_seq0(unsigned char channel, const out_oscar_buffer &b);

#endif
