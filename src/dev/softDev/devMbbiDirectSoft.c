/* devMbbiDirectSoft.c */
/* base/src/dev $Id$ */
/*
 *      Original Author: Bob Dalesio
 *      Current Author:  Matthew Needes
 *      Date:            10-08-93
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 *
 * Modification Log:
 * -----------------
 *    (Modification log in devMbbiSoft.c applies)
 *  .01  10-08-93    mcn (created)  support for direct mbbi records
 */
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#include	"alarm.h"
#include	"dbDefs.h"
#include	"dbAccess.h"
#include	"recGbl.h"
#include        "recSup.h"
#include	"devSup.h"
#include	"mbbiDirectRecord.h"
/* Create the dset for devMbbiSoft */
static long init_record();
static long read_mbbi();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_mbbi;
}devMbbiDirectSoft={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_mbbi
};

static long init_record(mbbiDirectRecord *pmbbi)
{

    /* mbbi.inp must be a CONSTANT or a PV_LINK or a DB_LINK or a CA_LINK*/
    switch (pmbbi->inp.type) {
    case (CONSTANT) :
        if(recGblInitConstantLink(&pmbbi->inp,DBF_ENUM,&pmbbi->val))
            pmbbi->udf = FALSE;
        break;
    case (DB_LINK) :
    case (PV_LINK) :
    case (CA_LINK) :
        break;
    default :
	recGblRecordError(S_db_badField,(void *)pmbbi,
		"devMbbiDirectSoft (init_record) Illegal INP field");
	return(S_db_badField);
    }
    return(0);
}

static long read_mbbi(mbbiDirectRecord	*pmbbi)
{
    long status;

    status = dbGetLink(&pmbbi->inp,DBR_USHORT,&pmbbi->val,0,0);
    if (pmbbi->inp.type!=CONSTANT && RTN_SUCCESS(status)) pmbbi->udf=FALSE;
    return(2);
}
