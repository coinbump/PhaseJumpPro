#ifndef SQLROW_H
#define SQLROW_H

//#pragma once
//
//#include "PJ_RetainObject.h"
//#include "PJ_ValueMap.h"
//#include "_String.h"
//
//class SQLRow;
//typedef std::shared_ptr<SQLRow>	PJ_SQLRowPtr;
//
//class SQLTable;
//
///*
//	SQLRow
// 
// */
//// FUTURE: support store/retrieve date info.
//// FUTURE: support store/retrieve color info (web hex)
//class SQLRow : public PJ_RetainObject,
//				  public PJ_TValueMap<String>
//{
//public:
//	SQLTable		*mTable;
//	String		mKeyColName;
//	String		mKeyColValue;
//	
//	SQLRow(SQLTable* table, String& keyColName, String& keyColValue);
//
//	// OVERRIDE:
//	virtual PJ_Vector2 GetVec2Val(String& key, PJ_Vector2 defaultValue) const;
//	virtual int IntValue(String& key, int defaultValue) const;
//	virtual float FloatValue(String& key, float defaultValue) const;
//	virtual bool BoolValue(String& key, bool defaultValue) const;
//	virtual String StringValue(String& key, String& defaultValue) const;
//	virtual PJ_Color GetColorVal(String& key, PJ_Color defaultValue) const;
//	virtual PJ_FColor GetFColorVal(String& key, PJ_FColor const& defaultValue) const;
//	virtual void SetVec2Val(String& key, PJ_Vector2 val);
//	virtual void SetIntValue(String& key, int val);
//	virtual void SetFloatValue(String& key, float val);
//	virtual void SetBoolVal(String& key, bool val);
//	virtual void SetStringValue(String& key, String& val);
//	
//};

#endif
