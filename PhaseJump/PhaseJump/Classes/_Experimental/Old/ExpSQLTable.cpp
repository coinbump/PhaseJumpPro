// #include <stdio.h>

// void SQLTable::addBuildTableStr(String& params, bool& gotOne, String& name,
// String& sqlType, String& paramStr)
//{
//     if (gotOne) {
//         params.append(", ");
//     }
//     gotOne = true;
//     params.append(SQLValueString(name, true));
//     params.append(" ");
//     params.append(sqlType);
//
//     if (!paramStr.empty()) {
//         params.append(" ");
//         params.append(paramStr);
//     }
//
// }
//
// void SQLTable::RemoveAllSchemaColumns() {
//     schema->RemoveAllChildren();
//
// }
//
// PJ_FragmentPtr SQLTable::AddSchemaColumn(String& colName, PJ_TagPtr valueTag)
// {
//     PJ_FragmentPtr    result(new PJ_Fragment);
//     result->name = colName;
//
//     schema->AddChild(result);
//     if (NULL != valueTag.get()) {
//         result->AddTag(PJ::TagNames::Type, valueTag, true);
//     }
//
//     return result;
//
// }
//
// bool SQLTable::HasSchemaColumn(String& colName) const {
//     PJ_FragmentPtr    fragment;
//     return schema->GetNamedChild(colName, fragment);
// }
//
// String SQLTable::GetTagSQLDefault(PJ_Tag* tag)
//{
//     String    result = "";
//
//     {PJ_IntTag* it = dynamic_cast<PJ_IntTag*>(tag);
//     if (NULL != it) {
//         result.append("DEFAULT ");
//         result.append(String(it->mVal));
//         return result;
//     }}
//     {PJ_FloatTag* ft = dynamic_cast<PJ_FloatTag*>(tag);
//         if (NULL != ft) {
//             result.append("DEFAULT ");
//             result.append(String((float)ft->mVal, 4));
//
//             return result;
//         }
//     }
//     {PJ_StringTag    *st = dynamic_cast<PJ_StringTag*> (tag);
//     if (NULL != st) {
//         if (!st->mVal.empty()) {
//
//             result.append("DEFAULT ");
//             result.append(SQLValueString(st->mVal, false));
//         }
//         return result;
//     }}
//
//     return result;
//
// }
//
// String SQLTable::GetTagSQLType(PJ_Tag* tag)
//{
//     String    result = "";
//
//     {PJ_IntTag* it = dynamic_cast<PJ_IntTag*>(tag);
//         if (NULL != it) {
//             result = "INTEGER";    // For sqlite, not "INT"
//             return result;
//         }}
//     {PJ_FloatTag* ft = dynamic_cast<PJ_FloatTag*>(tag);
//         if (NULL != ft) {
//             result = "FLOAT";
//             return result;
//         }
//     }
//     {PJ_StringTag    *st = dynamic_cast<PJ_StringTag*> (tag);
//         if (NULL != st) {
//             //result = "varChar(10)";    // Length not enforced by sqlite.
//             result = "TEXT";
//             return result;
//         }}
//     {PJ_DataTag    *dt = dynamic_cast<PJ_DataTag*> (tag);
//         if (NULL != dt) {
//             result = "BLOB";
//             return result;
//         }}
//
//     return result;
//
// }
//
///*
//    BuildTable
//
//     This synchronizes the SQL table state, to ensure that the correct table
//    and columns exist for SQL operations.
//
//    Call this if you are unsure of an SQL table's state (only once).
//
// */
// bool SQLTable::BuildTable()
//{
//    if (NULL == schema.get()) {
//        PJLog("ERROR. Can't BuildTable with NULL schema");
//        return false;
//    }
//
//    bool    result = false;
//    if (NULL == db) { return false; }
//
//    if (!db->TableExists(name)) {
//
//        String    params;
//
//        // EXAMPLE: bank (game int, paid int)"
//        if (!schema->mChildren.empty()) {
//            params = "(";
//
//            bool    gotOne = false;
//            for (auto& _i : schema->mChildren) {
//                PJ_FragmentPtr child = _i;
//                PJ_TagPtr    typeTag;
//                String    colName = child->name;
//
//                if (child->GetTag(PJ::TagNames::Type, typeTag)) {
//                    String    sqlType = GetTagSQLType(typeTag.get());
//                    if (sqlType.empty()) { continue; }
//
//                    String    paramStr;
//                    if (child->BoolValue(SQL_PRIMARYKEY_PNAME, false)) {
//                        paramStr.append("PRIMARY KEY");
//                    }
//                    addBuildTableStr(params, gotOne, colName, sqlType,
//                    paramStr);
//
//                    // Add default after primary key.
//                    PJ_TagPtr    defaultTag;
//                    if (child->GetTag(SQL_DEFAULT_PNAME, defaultTag)) {
//                        String defaultStr =
//                        GetTagSQLDefault(defaultTag.get()); paramStr.append("
//                        "); paramStr.append(defaultStr);
//                    }
//                }
//            }
//
//            params.append(")");
//        }
//
// #ifdef __SQL_DEBUG__
//        PJLog("Build Table:\n%s", params.c_str());
// #endif
//
//        result = db->CreateTable(name, params);
//    }
//    else {
//        result = true;
//
//        for (auto& _i : schema->mChildren) {
//            PJ_FragmentPtr child = _i;
//
//            // TODO: shouldn't matter if column exists or not(?)
//            // FUTURE: support unique, primary key, default constraints here.
//            if (!db->ColumnExists(name, child->name)) {
//                PJ_TagPtr    typeTag;
//                if (child->GetTag(PJ::TagNames::Type, typeTag)) {
//
//                    result = result && db->AddColumn(name, child->name,
//                    GetTagSQLType(typeTag.get()));
//                }
//            }
//        }
//    }
//
//    return result;
//
//}
//
// void SQLTable::WriteStatementBlob(sqlite3_stmt* statement, int colIndex,
// String& path, bool writeEmpty) {
//
//    void const*    sqldata = sqlite3_column_blob(statement, colIndex);
//    int dataSize = sqlite3_column_bytes(statement, colIndex);
//    if (0 == dataSize && !writeEmpty) { return; }
//
//    PJ_FilePtr    file = PJ_File::NewFilePtr();
//    file->SetPath(path);
//    PJ_Data    *data = new PJ_Data((char*)sqldata, dataSize);
//    file->WriteSynch(data);
//
//    // IMPORTANT: sql is responsible for managing blob data.
//    data->Clear();
//    pjRelease(data);
//
//}
//
//// TODO: needs unit tests.
//
//// TODO: untested
// void SQLTable::CollectAllRows(PJ_TStdVector<PJ_FragmentPtr>& rows, DataType
// blobType) {
//     FileManager* fm = FileManager::GetInstance();
//
//     rows.clear();
//
//     String    cmd = "SELECT * FROM ";
//     cmd.append(SQLValueString(name, true));
//
//     sqlite3_stmt    *statement;
//     int    columnCount = -1;
//     if (SQLITE_OK == db->Prepare(cmd, &statement)) {
//
//         while (SQLITE_ROW == db->Step(command)) {
//             PJ_FragmentPtr    row(new PJ_Fragment);
//             rows.Add(row);
//
//             if (columnCount < 0) {
//                 columnCount = sqlite3_column_count(statement);
//             }
//
//             for (int i = 0; i < columnCount; i++) {
//                 String    colName(sqlite3_column_name(statement, i));
//                 int    columnType = sqlite3_column_type(statement, i);
//
//                 switch (columnType) {
//                     case SQLITE_INTEGER: {
//                         int    intVal = sqlite3_column_int(statement, i);
//                         row->SetIntValue(colName, intVal);
//                         break;
//                     }
//                     case SQLITE_FLOAT: {
//                         // FUTURE: evaluate: double vs float. Does it matter?
//                         float    floatVal =
//                         static_cast<float>(sqlite3_column_double(statement,
//                         i)); row->SetFloatValue(colName, floatVal); break;
//                     }
//                     case SQLITE_TEXT: {
//                         String    strVal((const
//                         char*)sqlite3_column_text(statement, i));
//                         row->SetStringValue(colName, strVal);
//                         break;
//                     }
//                     case SQLITE_BLOB:
//                         switch (blobType) {
//                             case DataType::XML: {
//
//                                 // Write blob.
//                                 // FUTURE: parse memory stream (faster).
//                                 String    blobPath =
//                                 fm->TempFilesPath("phasejump_blob");
//                                 WriteStatementBlob(statement, i, blobPath,
//                                 true);
//
//                                 // Parse blob.
//                                 PJ_XMLFragmentParser    parser;
//                                 parser.ParseFile(blobPath);
//
//                                 PJ_FragmentPtr blobFrag = parser.Root();
//                                 if (NULL != blobFrag.get()) {
//                                     blobFrag->name = colName;
//                                     row->AddChild(blobFrag);
//                                 }
//                                 break;
//                             }
//                             case DataType::Data:
//                                 // FUTURE: support data blobs.
//                                 PJLog("UNSUPPORTED. Can't convert data blogs
//                                 to fragments yet."); break;
//                             default:
//                                 break;
//                         }
//                         break;
//                     default:
//                         break;
//                 }
//             }
//         }
//     }
//     sqlite3_finalize(statement);
//
// }
//

//
// int SQLTable::CachedIntValue(String& colName, String& whereColumn, String&
// whereMatch, int defaultValue) {
//    if (NULL == db) { return defaultValue; }
//
//    String    matchStr =
//    "colName:"+colName+"whereColumn:"+whereColumn+"whereMatch:"+whereMatch;
//    PJ_TagPtr    tag;
//    if (db->cache->GetTag(matchStr, tag)) {
//        PJ_IntTag*    trueTag = dynamic_cast<PJ_IntTag*>(tag.get());
//        if (NULL != trueTag) {
//            return trueTag->mVal;
//        }
//    }
//
//    int    result = IntValue(colName, whereColumn, whereMatch, defaultValue);
//    db->cache->SetIntValue(matchStr, result);
//    return result;
//
//}
//
// float SQLTable::CachedFloatValue(String& colName, String& whereColumn,
// String& whereMatch, float defaultValue) {
//    if (NULL == db) { return defaultValue; }
//
//    String    matchStr =
//    "colName:"+colName+"whereColumn:"+whereColumn+"whereMatch:"+whereMatch;
//    PJ_TagPtr    tag;
//    if (db->cache->GetTag(matchStr, tag)) {
//        PJ_FloatTag*    trueTag = dynamic_cast<PJ_FloatTag*>(tag.get());
//        if (NULL != trueTag) {
//            return trueTag->mVal;
//        }
//    }
//
//    float    result = FloatValue(colName, whereColumn, whereMatch,
//    defaultValue); db->cache->SetFloatValue(matchStr, result); return result;
//
//}
//
// String SQLTable::CachedStringValue(String& colName, String& whereColumn,
// String& whereMatch, String& defaultValue) {
//    if (NULL == db) { return defaultValue; }
//
//    String    matchStr =
//    "colName:"+colName+"whereColumn:"+whereColumn+"whereMatch:"+whereMatch;
//    PJ_TagPtr    tag;
//    if (db->cache->GetTag(matchStr, tag)) {
//        PJ_StringTag*    trueTag = dynamic_cast<PJ_StringTag*>(tag.get());
//        if (NULL != trueTag) {
//            return trueTag->mVal;
//        }
//    }
//
//    String    result = StringValue(colName, whereColumn, whereMatch,
//    defaultValue); db->cache->SetStringValue(matchStr, result); return result;
//
//}
//

//
//
// void SQLTable::SelectStrCol(String& colName, vector<String>& cells)
//{
//    cells.clear();
//
//    String    cmd = "select ";
//    cmd.append(SQLValueString(colName, true));
//    cmd.append(" from ");
//    cmd.append(SQLValueString(name, true));
//
//    sqlite3_stmt    *statement;
//    if (SQLITE_OK == db->Prepare(cmd, &statement)) {
//        int    columnCount = -1;
//
//        while (SQLITE_ROW == db->Step(command)) {
//            if (columnCount < 0) {
//                columnCount = sqlite3_column_count(statement);
//            }
//            if (columnCount > 0) {
//                String    cell((const char*)sqlite3_column_text(statement,
//                0)); cells.push_back(cell);
//            }
//        }
//    }
//    sqlite3_finalize(statement);
//
//
//}
//
// void SQLTable::SelectStrColWhere(String& colName, String& whereColumn,
// String& whereMatch, vector<String>& cells) {
//
//    cells.clear();
//
//    String    cmd = "select ";
//    cmd.append(SQLValueString(colName, true));
//    cmd.append(" from ");
//    cmd.append(SQLValueString(name, true));
//
//    cmd.append(" where ");
//    cmd.append(SQLValueString(whereColumn, true));
//    cmd.append("=");
//    cmd.append(SQLValueString(whereMatch, false));
//    cmd.append("");
//
//    sqlite3_stmt    *statement;
//    if (SQLITE_OK == db->Prepare(cmd, &statement)) {
//        int    columnCount = -1;
//
//        while (SQLITE_ROW == db->Step(command)) {
//            if (columnCount < 0) {
//                columnCount = sqlite3_column_count(statement);
//            }
//            if (columnCount > 0) {
//                String    cell((const char*)sqlite3_column_text(statement,
//                0)); cells.push_back(cell);
//            }
//        }
//    }
//    sqlite3_finalize(statement);
//}
//
// void SQLTable::SelectStrColWhereSet(String& colName, String& whereColumn,
// String& whereMatch, set<String>& cells) {
//
//    cells.clear();
//
//    String    cmd = "select ";
//    cmd.append(SQLValueString(colName, true));
//    cmd.append(" from ");
//    cmd.append(SQLValueString(name, true));
//
//    cmd.append(" where ");
//    cmd.append(SQLValueString(whereColumn, true));
//    cmd.append("=");
//    cmd.append(SQLValueString(whereMatch, false));
//    cmd.append("");
//
//    sqlite3_stmt    *statement;
//    if (SQLITE_OK == db->Prepare(cmd, &statement)) {
//        int    columnCount = -1;
//
//        while (SQLITE_ROW == db->Step(command)) {
//            if (columnCount < 0) {
//                columnCount = sqlite3_column_count(statement);
//            }
//            if (columnCount > 0) {
//                String    cell((const char*)sqlite3_column_text(statement,
//                0)); cells.insert(cell);
//            }
//        }
//    }
//    sqlite3_finalize(statement);
//
//}

// bool SQLTable::LoadFileBlob(String& colName, String& whereColumn, String&
// whereMatch, String& path, bool addToTable)
//{
//     FileManager *fm = FileManager::GetInstance();
//     if (!fm->FileExistsAtPath(path)) {
//         PJLog("ERROR. LoadFileBlob couldn't find file %s", path.c_str());
//         return false;
//     }
//
//     bool result = false;
//
//     if (!addToTable) {
//
//         if (!DoesCellExist(colName, whereColumn, whereMatch)) {
//             return false;    // Don't add to table.
//         }
//
//         String    cmd;
//         cmd.append("update ");
//         cmd.append(SQLValueString(name, true));
//         cmd.append(" set ");
//         cmd.append(SQLValueString(colName, true));
//         cmd.append("= ? where ");
//         cmd.append(SQLValueString(whereColumn, true));
//         cmd.append("=");
//         cmd.append(SQLValueString(whereMatch, false));
//         cmd.append("");
//
//         PJ_Data    *data = new PJ_Data;
//         data->LoadFile(path);
//         if (NULL != data->GetData()) {
//
//             {sqlite3_stmt    *statement;
//                 PJ_SQLPrepareStatement    preparer(db, cmd, &statement);
//             if (SQLITE_OK == preparer.GetPrepareResult()) {
//                 sqlite3_bind_blob(statement, 1, data->GetData(),
//                 static_cast<int>(data->GetDataSize()), NULL);
//
//                 if (SQLITE_DONE == db->Step(command)) {
//                     result = true;
//                 }}
//             }
//         }
//         pjRelease(data);
//     }
//     else if (addToTable) {
//         // Column does not exist.
//         if (!db->ColumnExists(name, colName)) {
//             db->AddColumn(name, colName, "blob");
//         }
//
//         // Row exists.
//         if (DoesCellExist(whereColumn, whereColumn, whereMatch)) {
//
//             // We now have a row and column. Update.
//             return LoadFileBlob(colName, whereColumn, whereMatch, path,
//             false);
//         }
//
//         // SYNTAX: INSERT INTO table_name (column1, column2, column3,...)
//         VALUES (value1, value2, value3,...) String    cmd; cmd.append("insert
//         into "); cmd.append(SQLValueString(name, true)); cmd.append(" (");
//         cmd.append(SQLValueString(colName, true));
//         cmd.append(", ");
//         cmd.append(SQLValueString(whereColumn, true));
//         cmd.append(") values (?, ");
//         cmd.append(SQLValueString(whereMatch, false));
//         cmd.append(")");
//
//         PJ_Data    *data = new PJ_Data;
//         data->LoadFile(path);
//         if (NULL != data->GetData()) {
//
//             {sqlite3_stmt    *statement;
//                 PJ_SQLPrepareStatement    preparer(db, cmd, &statement);
//
//             if (SQLITE_OK == preparer.GetPrepareResult()) {
//                 sqlite3_bind_blob(statement, 1, data->GetData(),
//                 static_cast<int>(data->GetDataSize()), NULL);
//
//                 if (SQLITE_DONE == db->Step(command)) {
//                     result = true;
//                 }}
//             }
//         }
//
//         pjRelease(data);
//     }
//
//     return result;
//
// }
//
///*
//    WriteBlobToFile
//
//    CAREFUL:
//    SELECT "column_name" from table_name where x=y produces a strange result.
//    It returns the actual string "column_name", which doesn't make sense.
//    SELECT [column_name] produces the correct results, but this isn't standard
//    SQL?
//
//    OPTIMIZE: This can be very slow. Be careful about overusing it.
//
//    MUCH FASTER:
//    void const*    sqldata = sqlite3_column_blob(statement, N);
//    int dataSize = sqlite3_column_bytes(statement, N);
//
//    if (dataSize > 0) {
//        char*    readData = new char[dataSize];
//        memcpy(readData, sqldata, dataSize);
//        std::stringstream    _stream;
//        _stream.rdbuf()->pubsetbuf(readData, dataSize);
//        parser.ParseStream(_stream);
//        free(readData);
//    }
//
// */
// bool SQLTable::WriteBlobToFile(String& colName, String& whereColumn, String&
// whereMatch, String& path, bool writeEmpty)
//{
//    bool    result = false;
//
//    // Ensure we don't result in old data.
//    FileManager    *fm = FileManager::GetInstance();
//    fm->DeleteFile(path);
//
//    // EXAMPLE: SELECT * FROM Persons WHERE City='Sandnes'
//    String    cmd = "select ";
//    cmd.append(SQLValueString(colName, true));
//    cmd.append(" from ");
//    cmd.append(SQLValueString(name, true));
//
//    cmd.append(" where ");
//    cmd.append(SQLValueString(whereColumn, true));
//    cmd.append("=");
//    cmd.append(SQLValueString(whereMatch, false));
//    cmd.append("");
//
//    sqlite3_stmt    *statement;
//    if (SQLITE_OK == db->Prepare(cmd, &statement)) {
//        if (SQLITE_ROW == db->Step(command)) {
//            int    columnCount = sqlite3_column_count(statement);
//            if (columnCount > 0) {
//
//                void const*    sqldata = sqlite3_column_blob(statement, 0);
//                int dataSize = sqlite3_column_bytes(statement, 0);
//
//                if (!writeEmpty && 0 == dataSize) {
////                    PJLog("SQL. Didn't write empty blob.");
//                    sqlite3_finalize(statement);    // FUTURE: use
//                    PJ_SQLPrepareStatement to prevent accidental failure to
//                    finalize. return false;
//                }
//
//                PJ_FilePtr    file = PJ_File::NewFilePtr();
//                file->SetPath(path);
//                PJ_Data    *data = new PJ_Data((char*)sqldata, dataSize);
//                file->WriteSynch(data);
//
//                // IMPORTANT: sql is responsible for managing blob data.
//                data->Clear();
//                pjRelease(data);
//
//                result = true;
//            }
//        }
//    }
//    sqlite3_finalize(statement);
//
//    return result;
//
//}
//
//
// void SQLTable::InsertInto(PJ_StringList const& names, PJ_StringList const&
// values) { #ifdef __DEBUG__
//    if (names.size() > values.size()) {
//        PJLog("WARNING. InsertInto. Names size %lu doesn't match Values size
//        %lu", names.size(), values.size());
//    }
// #endif
//
//    String cmd;
//    cmd.append("insert into ");
//    cmd.append(GetName());
//    cmd.append(" (");
//
//    String namesListStr = BuildSQLDelimStr(names);
//    cmd.append(namesListStr);
//    cmd.append(") VALUES");
//
//    /*
//        SPECIAL CASE: Insert multiple value sets into the columns.
//     */
//    if (values.size() > names.size() && values.size() % names.size() == 0) {
//
//        size_t numNames = names.size();
//        size_t numValues = 0;
//
//        PJ_StringList _values;
//        for (const auto& _i : values) {
//            numValues++;
//
//            String value = _i;
//            _values.push_back(value);
//
//            if (numValues == numNames) {
//
//                String valuesListStr = BuildSQLDelimStr(_values);
//
//                cmd.append(" (");
//                cmd.append(valuesListStr);
//                cmd.append("),");
//
//                _values.clear();
//                numValues = 0;
//            }
//        }
//        cmd.erase(cmd.size()-1, 1);    // Get rid of final ','
////        PJLog("Multiple insert %s", cmd.c_str());
//    }
//    else {
//        String valuesListStr = BuildSQLDelimStr(values);
//
//        cmd.append(" (");
//        cmd.append(valuesListStr);
//        cmd.append(")");
//    }
//
//    {sqlite3_stmt* statement;
//        PJ_SQLPrepareStatement prepareStatement(db, cmd, &statement);
//        if (SQLITE_OK == prepareStatement.GetPrepareResult()) {
//            if (SQLITE_DONE == db->Step(command)) {
//            }
//        }
//    }
//}
//
