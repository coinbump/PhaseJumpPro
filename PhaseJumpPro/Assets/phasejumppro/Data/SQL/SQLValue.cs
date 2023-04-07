/*
RATING: 5 stars
Simple type
CODE REVIEW: 4/4/23
PORTED TO: C++
*/
namespace PJ
{
#if _PJ_SQL_
    public struct SQLValue
    {
        public SQLValueType type;
        public string value;

        public SQLValue(SQLValueType type, string value)
        {
            this.type = type;
            this.value = value;
        }
    }
#endif
}
