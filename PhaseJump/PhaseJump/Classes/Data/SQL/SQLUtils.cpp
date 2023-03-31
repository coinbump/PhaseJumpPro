#include "SQLUtils.h"
#include "SQLTypes.h"
#include "_String.h"
#include <sqlite3.h>

using namespace PJ;

// http://www.sqlite.org/lang_keywords.html
bool SQLUtils::IsReservedKeyword(String value) {
    return sqlite3_keyword_check(value.c_str(), (int)value.length()) != 0;
}
