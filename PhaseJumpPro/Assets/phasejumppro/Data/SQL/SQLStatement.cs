/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 4/2/23
PORTED TO: C++
*/
using UnityEngine;

namespace PJ
{
    // Install SQL and add the directive in Player settings
    // Install instructions: https://www.mongodb.com/developer/code-examples/csharp/saving-data-in-unity3d-using-sqlite/
#if _PJ_SQL_
    public class SQLStatement
    {
        public string value = "";

        public SQLStatement()
        {
        }

        public SQLStatement(string value)
        {
            this.value = value;
        }

        public void AppendString(string _string)
        {
            value += _string;
        }

        public void AppendIdentifier(string identifier, bool isEscaped = false)
        {
            if (new SQLUtils().IsReservedKeyword(identifier))
            {
                Debug.Log(string.Format("WARNING. %s is reserved by SQL.", identifier));
            }

            if (isEscaped)
            {
                value += identifier;
            }
            else
            {
                // SQL identifier strings are wrapped in double quotes.
                // Using brackets allows us to use reserved keywords as identifiers
                value += "[";
                value += identifier;
                value += "]";
            }
        }

        public void Append(SQLValue value)
        {
            switch (value.type)
            {
                case SQLValueType.Text:
                case SQLValueType.Any:
                case SQLValueType.Blob:
                    // SQL value strings are wrapped in single quotes.
                    // Example: name='myName'
                    this.value += "'";
                    this.value += value.value;
                    this.value += "'";
                    break;
                case SQLValueType.Int:
                case SQLValueType.Real:
                    this.value += value.value;
                    break;
            }
        }

    }
#endif
}
