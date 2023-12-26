namespace PJ
{
#if _PJ_SQL_
    public struct SQLIdentifierFormatter
    {
        public string Formatted(string value)
        {
            string result = "";

            result += "[";
            result += value;
            result += "]";

            return result;
        }
    }
#endif
}
