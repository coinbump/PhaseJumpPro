/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/3/23
 PORTED TO: C++
 */
using System.Collections.Generic;

namespace PJ
{
#if _PJ_SQL_
    /// <summary>
    /// Arguments for a standard SQL table query
    /// </summary>
    public struct SQLTableQueryArguments
    {
        // Name of the column to fetch values from
        public List<string> columnNames;

        // Optional where clause
        public Optional<SQLWhereArguments> where;

        public SQLTableQueryArguments(string columnName, Optional<SQLWhereArguments> where)
        {
            this.columnNames = new();
            this.columnNames.Add(columnName);
            this.where = where;
        }

        public SQLTableQueryArguments(List<string> columnNames, Optional<SQLWhereArguments> where)
        {
            this.columnNames = columnNames;
            this.where = where;
        }
    }
#endif
}
