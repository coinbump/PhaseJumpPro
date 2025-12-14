/*
RATING: 5 stars
Simple types
CODE REVIEW: 4/2/23
*/
namespace PJ
{
    // Install SQL and add the directive in Player settings
    // Install instructions: https://www.mongodb.com/developer/code-examples/csharp/saving-data-in-unity3d-using-sqlite/
#if _PJ_SQL_
    public enum DataType
    {
        Data,
        XML
    }

    public enum SQLValueType
    {
        Int,
        Real,
        Text,
        Blob,
        Any
    }

    /// Specifies how the database should be opened
    /// https://www.sqlite.org/c3ref/open.html
    public enum SQLDatabaseOpenType
    {
        // Open database at path
        AtPath,

        // Create database in memory
        // NOTE: This is supposed to work according to the documentation, but in practice it does not
        // Use an on-disk database instead
        // InMemory

        // Create temporary database on disk
        // NOT YET SUPPORTED: TemporaryOnDisk
    }
#endif
}
