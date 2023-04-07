/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/3/23
 PORTED TO: C++
 */
namespace PJ
{
#if _PJ_SQL_
    /// <summary>
    /// Arguments for a standard SQL table mutation
    /// </summary>
    public struct SQLTableMutateArguments
    {
        // Name of the column to change values
        public string columnName;

        // Optional where clause
        public Optional<SQLWhereArguments> where;

        // Value to set for mutation
        public SQLValue value;

        public SQLTableMutateArguments(string columnName, Optional<SQLWhereArguments> where, SQLValue value)
        {
            this.columnName = columnName;
            this.where = where;
            this.value = value;
        }
    }
#endif
}
