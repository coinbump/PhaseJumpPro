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
    /// Selects values "WHERE columnName='value'"
    /// </summary>
    public struct SQLWhereArguments
    {
        public string columnName;
        public string value;

        public SQLWhereArguments(string columnName, string value)
        {
            this.columnName = columnName;
            this.value = value;
        }
    }
#endif
}
