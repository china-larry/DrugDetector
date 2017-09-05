#ifndef HELPER_H
#define HELPER_H

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#ifdef _LINUX_PLATFORM_
#include "stddef.h"
#include <stdio.h>
#endif

#include "utils.h"
#include "IObjectBase.h"
#include "IMSH.h"
#include "IHL7Message.h"
#include "IHL7Segment.h"

#include "IDSC.h"
#include "IDSP.h"
#include "IECD.h"
#include "IECR.h"
#include "IEQP.h"
#include "IEQU.h"
#include "IERR.h"
#include "IINV.h"
#include "IISD.h"
#include "IMSA.h"
#include "IMSH.h"
#include "IOBR.h"
#include "IOBX.h"
#include "IPID.h"
#include "IQAK.h"
#include "IQRD.h"
#include "IQRF.h"
#include "IROL.h"

#include "IACK.h"
#include "IDSR_Q03.h"
#include "IEAC_U07.h"
#include "IEAR_U08.h"
#include "IESR_U02.h"
#include "IESU_U01.h"
#include "IINR_U06.h"
#include "IINU_U05.h"
#include "ILSR_U13.h"
#include "ILSU_U12.h"
#include "IORF_R04.h"
#include "IQCK_Q02.h"
#include "IQRY_Q02.h"
#include "IQRY_R02.h"
#include "IORU_R01.h"

#include "IORU_R01_PATITENT_RESULT.h"
#include "IORU_R01_ORDER_OBSERVATION.h"
#include "IORF_R04_PATIENT_RESULT.h"
#include "IORF_R04_ORDER_OBSERVATION.h"
#include "IEAR_U08_COMMAND_RESPONSE.h"

//#define NULL ((void *)0)
template<typename T>
struct HmiObjectDeleter
{
	inline void operator()(T*  t) const
	{
		if (t != NULL)
			t->Release();
	}
};

template<typename T, typename D>
T* QueryInterface(D* p, IFID ifid)
{
	if (p == NULL)
		return NULL;

	T* pv = NULL;
	if (p->QueryIF(ifid, (void **)&pv) == N_OK)
	{
		assert(pv);
		return pv;
	}

	return NULL;
}


typedef ScopedPtrT<IObjectBase, HmiObjectDeleter<IObjectBase> > ObjectScopedPtr;
typedef ScopedPtrT<IHL7Message, HmiObjectDeleter<IHL7Message> > MessageScopedPtr;
typedef ScopedPtrT<IHL7Segment, HmiObjectDeleter<IHL7Segment> > SegmentScopedPtr;

typedef ScopedPtrT<IDSC, HmiObjectDeleter<IDSC> > DSCScopedPtr;
typedef ScopedPtrT<IDSP, HmiObjectDeleter<IDSP> > DSPScopedPtr;
typedef ScopedPtrT<IECD, HmiObjectDeleter<IECD> > ECDScopedPtr;
typedef ScopedPtrT<IECR, HmiObjectDeleter<IECR> > ECRScopedPtr;
typedef ScopedPtrT<IEQP, HmiObjectDeleter<IEQP> > EQPScopedPtr;
typedef ScopedPtrT<IEQU, HmiObjectDeleter<IEQU> > EQUScopedPtr;
typedef ScopedPtrT<IERR, HmiObjectDeleter<IERR> > ERRScopedPtr;
typedef ScopedPtrT<IINV, HmiObjectDeleter<IINV> > INVScopedPtr;
typedef ScopedPtrT<IISD, HmiObjectDeleter<IISD> > ISDScopedPtr;
typedef ScopedPtrT<IMSA, HmiObjectDeleter<IMSA> > MSAScopedPtr;
typedef ScopedPtrT<IMSH, HmiObjectDeleter<IMSH> > MSHScopedPtr;
typedef ScopedPtrT<IOBR, HmiObjectDeleter<IOBR> > OBRScopedPtr;
typedef ScopedPtrT<IOBX, HmiObjectDeleter<IOBX> > OBXScopedPtr;
typedef ScopedPtrT<IPID, HmiObjectDeleter<IPID> > PIDScopedPtr;
typedef ScopedPtrT<IQAK, HmiObjectDeleter<IQAK> > QAKScopedPtr;
typedef ScopedPtrT<IQRD, HmiObjectDeleter<IQRD> > QRDScopedPtr;
typedef ScopedPtrT<IQRF, HmiObjectDeleter<IQRF> > QRFScopedPtr;
typedef ScopedPtrT<IROL, HmiObjectDeleter<IROL> > ROLScopedPtr;


typedef ScopedPtrT<IORU_R01_PATIENT_RESULT, HmiObjectDeleter<IORU_R01_PATIENT_RESULT> > ORUR01PatientResultScopedPtr;
typedef ScopedPtrT<IORU_R01_ORDER_OBSERVATION, HmiObjectDeleter<IORU_R01_ORDER_OBSERVATION> > ORUR01OrderObrScopedPtr;
typedef ScopedPtrT<IORF_R04_PATIENT_RESULT, HmiObjectDeleter<IORF_R04_PATIENT_RESULT> > ORFR04PatientResultScopedPtr;
typedef ScopedPtrT<IORF_R04_ORDER_OBSERVATION, HmiObjectDeleter<IORF_R04_ORDER_OBSERVATION> > ORFR04OrderObrScopedPtr;
typedef ScopedPtrT<IEAR_U08_COMMAND_RESPONSE, HmiObjectDeleter<IEAR_U08_COMMAND_RESPONSE> > EARU08CommandResponseScopedPtr;

typedef ScopedPtrT<IACK, HmiObjectDeleter<IACK> > ACKScopedPtr;
typedef ScopedPtrT<IDSR_Q03, HmiObjectDeleter<IDSR_Q03> > DSRQ03ScopedPtr;
typedef ScopedPtrT<IEAC_U07, HmiObjectDeleter<IEAC_U07> > EACU07ScopedPtr;
typedef ScopedPtrT<IEAR_U08, HmiObjectDeleter<IEAR_U08> > EARU08ScopedPtr;
typedef ScopedPtrT<IESR_U02, HmiObjectDeleter<IESR_U02> > ESRU02ScopedPtr;
typedef ScopedPtrT<IESU_U01, HmiObjectDeleter<IESU_U01> > ESUU01ScopedPtr;
typedef ScopedPtrT<IINR_U06, HmiObjectDeleter<IINR_U06> > INRU06ScopedPtr;
typedef ScopedPtrT<IINU_U05, HmiObjectDeleter<IINU_U05> > INUU05ScopedPtr;
typedef ScopedPtrT<ILSR_U13, HmiObjectDeleter<ILSR_U13> > LSRU13ScopedPtr;
typedef ScopedPtrT<ILSU_U12, HmiObjectDeleter<ILSU_U12> > LSUU12ScopedPtr;
typedef ScopedPtrT<IORF_R04, HmiObjectDeleter<IORF_R04> > ORFR04ScopedPtr;
typedef ScopedPtrT<IORU_R01, HmiObjectDeleter<IORU_R01> > ORUR01ScopedPtr;
typedef ScopedPtrT<IQCK_Q02, HmiObjectDeleter<IQCK_Q02> > QCKQ02ScopedPtr;
typedef ScopedPtrT<IQRY_Q02, HmiObjectDeleter<IQRY_Q02> > QRYQ02ScopedPtr;
typedef ScopedPtrT<IQRY_R02, HmiObjectDeleter<IQRY_R02> > QRYR02ScopedPtr;

#endif
