//
//  ExpSQL.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 3/29/23.
//

#ifndef ExpSQL_h
#define ExpSQL_h
#pragma mark - PJ_SQLWrapTransaction

/*
 PJ_SQLWrapTransaction

 Wrap Begin/End SQL transaction.

 OPTIMIZE: can result in 10x fast SQL transactions. Always wrap batches of SQL operations with one of these.

 */
class PJ_SQLWrapTransaction {
public:
    SQLDatabase* db;

    PJ_SQLWrapTransaction(SQLDatabase* db)
    :    db(db)
    {
        if (NULL == db) { return; }
        db->BeginTransaction();
    }

    virtual ~PJ_SQLWrapTransaction() {
        if (NULL == db) { return; }
        db->EndTransaction();
    }
};
#endif /* ExpSQL_h */
