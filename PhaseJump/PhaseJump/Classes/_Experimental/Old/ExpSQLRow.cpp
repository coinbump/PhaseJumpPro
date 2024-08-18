
// #include "pch.h"
//
// #include "SQLRow.h"
// #include "_StringList.h"
// #include "SQLTable.h"
// #include "PJ_CoreTags.h"
//
// SQLRow::SQLRow(SQLTable* table, String& keyColName, String& keyColValue) {
//	mTable = table;
//	mKeyColName = keyColName;
//	mKeyColValue = keyColValue;
// }
//
// PJ_Vector2 SQLRow::GetVec2Val(String& key, PJ_Vector2 defaultValue) const {
//	PJ_Vector2	result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	String strVal = StringValue(key, "");
//	if (!strVal.empty()) {
//		PJ_StringList	strList;
//		strList.BuildFromDelimStr(strVal, ',');
//
//		if (strList.size() > 1) {
//			PJ_StringList::iterator	i = strList.begin();
//			result[0] = (*i).FloatValue();
//
//			i++;
//			result[1] = (*i).FloatValue();
//		}
//	}
//
//	return result;
// }
//
// int SQLRow::IntValue(String& key, int defaultValue) const {
//	int	result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	return mTable->IntValue(key, mKeyColName, mKeyColValue, defaultValue);
// }
//
// float SQLRow::FloatValue(String& key, float defaultValue) const {
//	float result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	return mTable->FloatValue(key, mKeyColName, mKeyColValue, defaultValue);
// }
//
// bool SQLRow::BoolValue(String& key, bool defaultValue) const {
//	bool result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	String	strVal = StringValue(key, "");
//	result = strVal.BoolValue();
//	return result;
// }
//
// String SQLRow::StringValue(String& key, String& defaultValue) const {
//	String result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	return mTable->StringValue(key, mKeyColName, mKeyColValue,
// defaultValue);
// }
//
// PJ_Color SQLRow::GetColorVal(String& key, PJ_Color defaultValue) const {
//	PJ_Color result = defaultValue;
//	if (NULL == mTable) { return result; }
//
//	String strVal = StringValue(key, "");
//	if (!strVal.empty()) {
//		// FUTURE: support this.
//		PJLog("UNIMPLEMENTED. SQLRow::GetColorVal");
//	}
//
//	return result;
// }
//
// PJ_FColor SQLRow::GetFColorVal(String& key, PJ_FColor const& defaultValue)
// const { 	PJ_FColor result = defaultValue; 	if (NULL == mTable) {
// return result;
//}
//
//	String	strVal = StringValue(key, "");
//	if (!strVal.empty()) {
//		// FUTURE: support this.
//		PJLog("UNIMPLEMENTED. SQLRow::GetFColorVal");
//	}
//
//	return result;
// }
//
// void SQLRow::SetVec2Val(String& key, PJ_Vector2 val) {
//	String xStr((float)val.x, 4);
//	String yStr((float)val.y, 4);
//	String ptStr = xStr + "," + yStr;
//	SetStringValue(key, ptStr);
// }
//
// void SQLRow::SetIntValue(String& key, int val) {
//	if (NULL == mTable) { return; }
//	mTable->SetIntValue(key, mKeyColName, mKeyColValue, val, true);
// }
//
// void SQLRow::SetFloatValue(String& key, float val) {
//	if (NULL == mTable) { return; }
//	mTable->SetFloatValue(key, mKeyColName, mKeyColValue, val, true);
// }
//
// void SQLRow::SetBoolVal(String& key, bool val) {
//	String boolStr("0");
//	if (val) {
//		boolStr = "1";
//	}
//
//	SetStringValue(key, boolStr);
// }
//
// void SQLRow::SetStringValue(String& key, String& val) {
//	if (NULL == mTable) { return; }
//
//	mTable->SetStringValue(key, mKeyColName, mKeyColValue, val, true);
// }
//
// #pragma mark - UNIT TESTS
//
// #ifdef __UNIT_TESTS__
//
// #include "gtest.h"
// #include "_String.h"
// #include "FileManager.h"
// #include "PJ_CoreTags.h"
// #include "SQLDatabase.h"
//
// TEST(SQLRow, Unit_Tests) {
//
//	SQLDatabase	db;
//	FileManager* fm = FileManager::GetInstance();
//	String	path = fm->ApplicationDataPath("PJ_SQLRow_unit_test.sql",
// FileManager::PathDomain::Default); 	fm->DeleteFile(path); 	db.Open(path);
//	EXPECT_FALSE(db.TableExists("unit_test")) << "empty database shouldn't
// have tables"; 	SQLTable	table("unit_test", &db);
//	table.AddSchemaColumn("key_col", PJ_TagPtr(new
// PJ_IntTag(0)))->SetBoolVal(SQL_PRIMARYKEY_PNAME, true);
// table.BuildTable();
//
//	PJ_SQLRowPtr	row(new SQLRow(&table, "key_col", "1"));
//	EXPECT_STREQ("", row->StringValue("test_str", "").c_str());
//	row->SetStringValue("test_str", "hello");
//	EXPECT_STREQ("hello", row->StringValue("test_str", "").c_str());
//
//	// After value is set, check primary key.
//	EXPECT_EQ(1, row->IntValue("key_col", 0));
//
//	EXPECT_EQ(-1, row->IntValue("test_int", -1));
//	row->SetIntValue("test_int", 27);
//	EXPECT_EQ(27, row->IntValue("test_int", -1));
//
//	EXPECT_FALSE(row->BoolValue("test_bool", false));
//	row->SetBoolVal("test_bool", true);
//	EXPECT_TRUE(row->BoolValue("test_bool", false));
//
//	EXPECT_EQ(-1, row->FloatValue("test_float", -1));
//	row->SetFloatValue("test_float", 27.7f);
//	EXPECT_NEAR(27.7, row->FloatValue("test_float", -1), .001);
//
// }
//
// #endif
