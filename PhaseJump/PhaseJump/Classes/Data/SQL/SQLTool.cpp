#include "SQLTool.h"
#include "SQLTypes.h"
#include "StringUtils.h"
#include <sqlite3.h>

using namespace PJ;

// http://www.sqlite.org/lang_keywords.html
bool SQLTool::IsReservedKeyword(String value) {
    return sqlite3_keyword_check(value.c_str(), (int)value.length()) != 0;
}
