#pragma once

// TODO: move this out of experimental
/*
 PJ_SQLWrapTransaction

 Wrap Begin/End SQL transaction.

 Optimize: can result in 10x fast SQL transactions. Always wrap batches of SQL
 operations with Begin/End transaction.

 */
class PJ_SQLWrapTransaction {
public:
    SQLDatabase* db;

    PJ_SQLWrapTransaction(SQLDatabase* db) :
        db(db) {
        GUARD(db)
        db->BeginTransaction();
    }

    virtual ~PJ_SQLWrapTransaction() {
        GUARD(db)
        db->EndTransaction();
    }
};
