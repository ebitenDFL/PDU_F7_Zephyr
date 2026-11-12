/*
 * Copyright (c) 2019 Vestas Wind Systems A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*******************************************************************************

   File - CO_OD.c/CO_OD.h
   CANopen Object Dictionary.

   This file was automatically generated with libedssharp Object
   Dictionary Editor v0.8-7-g2e53bbc   DON'T EDIT THIS FILE MANUALLY !!!!
*******************************************************************************/


#include "CO_driver.h"
#include "CO_OD.h"
#include "CO_SDO.h"

/*******************************************************************************
   DEFINITION AND INITIALIZATION OF OBJECT DICTIONARY VARIABLES
*******************************************************************************/


/***** Definition for ROM variables ********************************************/
struct sCO_OD_ROM CO_OD_ROM = {
	CO_OD_FIRST_LAST_WORD,

/*1000*/ 0x0000L,
/*1005*/ 0x0080L,
/*1006*/ 0x0000L,
/*1007*/ 0x0000L,
/*1008*/ { 'V', 'I', 'I', 'A', 'L', 'A', '_', 'P', 'D', 'U', '1', '_', 'V', '0' },
/*1009*/ { 'H', 'W', '2', '0' },
#if defined(CONFIG_PDU_IMAGE_VARIANT_A)
/*100a*/ { 'A', '0', '0', '1' },
#else
/*100a*/ { 'B', '0', '0', '1' },
#endif
/*1012*/ 0x0000L,
/*1014*/ 0x0080L,
/*1015*/ 0x64,
/*1016*/ { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
/*1017*/ 0x3e8,
/*1018*/ { 0x4L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1019*/ 0x0L,
/*1029*/ { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 },
/*1200*/ { { 0x2L, 0x0600L, 0x0580L } },
/*1400*/ { { 0x2L, 0x0200L, 0xfeL },
/*1401*/ { 0x2L, 0x0300L, 0xfeL },
/*1402*/ { 0x2L, 0x0400L, 0xfeL },
/*1403*/ { 0x2L, 0x0500L, 0xfeL } },
/*1600*/ { { 0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1601*/ { 0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1602*/ { 0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1603*/ { 0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L } },
/*1800*/ { { 0x6L, 0x0180L, 0xfeL, 0x03e8L, 0x0L, 0x0064L, 0x0L },
/*1801*/ { 0x6L, 0x0280L, 0xfeL, 0x03e8L, 0x0L, 0x0064L, 0x0L },
/*1802*/ { 0x6L, 0x0380L, 0xfeL, 0x03e8L, 0x0L, 0x0064L, 0x0L },
/*1803*/ { 0x6L, 0x0480L, 0xfeL, 0x03e8L, 0x0L, 0x0064L, 0x0L } },
/*1a00*/ { { 0x4L, 0x30000110L, 0x30000210L, 0x30000310L, 0x30000410L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1a01*/ { 0x4L, 0x30000510L, 0x30000610L, 0x30000710L, 0x30000810L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1a02*/ { 0x4L, 0x30000910L, 0x30000a10L, 0x30000b10L, 0x30000c10L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*1a03*/ { 0x4L, 0x30000d10L, 0x30000e10L, 0x30000f10L, 0x20000010L, 0x0000L, 0x0000L, 0x0000L, 0x0000L } },
/*1f80*/ 0x0000L,

	CO_OD_FIRST_LAST_WORD,
};


/***** Definition for RAM variables ********************************************/
struct sCO_OD_RAM CO_OD_RAM = {
	CO_OD_FIRST_LAST_WORD,

/*1001*/ 0x0L,
/*1002*/ 0x0000L,
/*1003*/ { 0, 0, 0, 0, 0, 0, 0, 0 },
/*1010*/ { 0x00000003 },
/*1011*/ { 0x00000001 },
/*1f50*/ { 0 },
/*1f51*/ { 0x0L },
/*1f56*/ { 0x0000L },
/*1f57*/ { 0x0000L },
/*2000*/ 0x0000L,
/*2001*/ 0x0000L,
/*2002*/ 0x0000L,
/*2003*/ 0x0000L,
/*2004*/ 0x0000L,
/*2005*/ 0x0000L,
/*2006*/ 0xffffffffL,
/*2007*/ 0xffffL,
/*2008*/ 0x7fffL,
/*2009*/ 0x7fffL,
/*200a*/ 0x00000000L,
/*200b*/ 0xffffL,
/*200c*/ 0x0000L,
/*200d*/ 0x0000L,
/*200e*/ 0x0000L,
/*200f*/ 0x00000001L,
/*2100*/ { 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L },
/*2102*/ 0x0000L,
/*2103*/ 0x0000L,
/*2104*/ 0x0000L,
/*2105*/ 0x0aL,
/*2106*/ 0x01f4L,
/*2107*/ 0x00L,
/*3000*/ { 0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL },
/*3001*/ { 0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL },
/*3002*/ { 0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL,
	     0xffffL, 0xffffL, 0xffffL, 0xffffL, 0xffffL },
/*3003*/ { 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L,
	     0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L,
	     0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L },
/*3100*/ { 5000, 5000, 5000, 5000, 5000,
	     5000, 5000, 5000, 5000, 5000,
	     5000, 5000, 5000, 5000, 5000 },
/*3101*/ { 100, 100, 100, 100, 100,
	     100, 100, 100, 100, 100,
	     100, 100, 100, 100, 100 },
/*3102*/ { 8000, 8000, 8000, 8000, 8000,
	     8000, 8000, 8000, 8000, 8000,
	     8000, 8000, 8000, 8000, 8000 },
/*3103*/ { 500, 500, 500, 500, 500,
	     500, 500, 500, 500, 500,
	     500, 500, 500, 500, 500 },

	CO_OD_FIRST_LAST_WORD,
};


/***** Definition for EEPROM variables ********************************************/
struct sCO_OD_EEPROM CO_OD_EEPROM = {
	CO_OD_FIRST_LAST_WORD,

/*2101*/ 0x0000L,

	CO_OD_FIRST_LAST_WORD,
};




/*******************************************************************************
   STRUCTURES FOR RECORD TYPE OBJECTS
*******************************************************************************/


/*0x1018*/ const CO_OD_entryRecord_t OD_record1018[5] = {
	{ (void *)&CO_OD_ROM.identity.maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.identity.vendorID, 0x85, 0x4 },
	{ (void *)&CO_OD_ROM.identity.productCode, 0x85, 0x4 },
	{ (void *)&CO_OD_ROM.identity.revisionNumber, 0x85, 0x4 },
	{ (void *)&CO_OD_ROM.identity.serialNumber, 0x85, 0x4 },
};

/*0x1200*/ const CO_OD_entryRecord_t OD_record1200[3] = {
	{ (void *)&CO_OD_ROM.SDOServerParameter[0].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.SDOServerParameter[0].COB_IDClientToServer, 0x85, 0x4 },
	{ (void *)&CO_OD_ROM.SDOServerParameter[0].COB_IDServerToClient, 0x85, 0x4 },
};

/*0x1400*/ const CO_OD_entryRecord_t OD_record1400[3] = {
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[0].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[0].COB_IDUsedByRPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[0].transmissionType, 0x0d, 0x1 },
};

/*0x1401*/ const CO_OD_entryRecord_t OD_record1401[3] = {
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[1].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[1].COB_IDUsedByRPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[1].transmissionType, 0x0d, 0x1 },
};

/*0x1402*/ const CO_OD_entryRecord_t OD_record1402[3] = {
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[2].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[2].COB_IDUsedByRPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[2].transmissionType, 0x0d, 0x1 },
};

/*0x1403*/ const CO_OD_entryRecord_t OD_record1403[3] = {
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[3].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[3].COB_IDUsedByRPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOCommunicationParameter[3].transmissionType, 0x0d, 0x1 },
};

/*0x1600*/ const CO_OD_entryRecord_t OD_record1600[9] = {
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[0].mappedObject8, 0x8d, 0x4 },
};

/*0x1601*/ const CO_OD_entryRecord_t OD_record1601[9] = {
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[1].mappedObject8, 0x8d, 0x4 },
};

/*0x1602*/ const CO_OD_entryRecord_t OD_record1602[9] = {
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[2].mappedObject8, 0x8d, 0x4 },
};

/*0x1603*/ const CO_OD_entryRecord_t OD_record1603[9] = {
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.RPDOMappingParameter[3].mappedObject8, 0x8d, 0x4 },
};

/*0x1800*/ const CO_OD_entryRecord_t OD_record1800[7] = {
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].COB_IDUsedByTPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].transmissionType, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].inhibitTime, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].compatibilityEntry, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].eventTimer, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[0].SYNCStartValue, 0x0d, 0x1 },
};

/*0x1801*/ const CO_OD_entryRecord_t OD_record1801[7] = {
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].COB_IDUsedByTPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].transmissionType, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].inhibitTime, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].compatibilityEntry, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].eventTimer, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[1].SYNCStartValue, 0x0d, 0x1 },
};

/*0x1802*/ const CO_OD_entryRecord_t OD_record1802[7] = {
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].COB_IDUsedByTPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].transmissionType, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].inhibitTime, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].compatibilityEntry, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].eventTimer, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[2].SYNCStartValue, 0x0d, 0x1 },
};

/*0x1803*/ const CO_OD_entryRecord_t OD_record1803[7] = {
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].maxSubIndex, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].COB_IDUsedByTPDO, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].transmissionType, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].inhibitTime, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].compatibilityEntry, 0x05, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].eventTimer, 0x8d, 0x2 },
	{ (void *)&CO_OD_ROM.TPDOCommunicationParameter[3].SYNCStartValue, 0x0d, 0x1 },
};

/*0x1a00*/ const CO_OD_entryRecord_t OD_record1a00[9] = {
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[0].mappedObject8, 0x8d, 0x4 },
};

/*0x1a01*/ const CO_OD_entryRecord_t OD_record1a01[9] = {
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[1].mappedObject8, 0x8d, 0x4 },
};

/*0x1a02*/ const CO_OD_entryRecord_t OD_record1a02[9] = {
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[2].mappedObject8, 0x8d, 0x4 },
};

/*0x1a03*/ const CO_OD_entryRecord_t OD_record1a03[9] = {
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].numberOfMappedObjects, 0x0d, 0x1 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject1, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject2, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject3, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject4, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject5, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject6, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject7, 0x8d, 0x4 },
	{ (void *)&CO_OD_ROM.TPDOMappingParameter[3].mappedObject8, 0x8d, 0x4 },
};

/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
const CO_OD_entry_t CO_OD[74] = {

	{ 0x1000, 0x00, 0x85, 4, (void *)&CO_OD_ROM.deviceType },
	{ 0x1001, 0x00, 0x26, 1, (void *)&CO_OD_RAM.errorRegister },
	{ 0x1002, 0x00, 0xa6, 4, (void *)&CO_OD_RAM.manufacturerStatusRegister },
	{ 0x1003, 0x08, 0x8e, 4, (void *)&CO_OD_RAM.preDefinedErrorField[0] },
	{ 0x1005, 0x00, 0x8d, 4, (void *)&CO_OD_ROM.COB_ID_SYNCMessage },
	{ 0x1006, 0x00, 0x8d, 4, (void *)&CO_OD_ROM.communicationCyclePeriod },
	{ 0x1007, 0x00, 0x8d, 4, (void *)&CO_OD_ROM.synchronousWindowLength },
	{ 0x1008, 0x00, 0x05, 14, (void *)&CO_OD_ROM.manufacturerDeviceName },
	{ 0x1009, 0x00, 0x05, 4, (void *)&CO_OD_ROM.manufacturerHardwareVersion },
	{ 0x100a, 0x00, 0x05, 4, (void *)&CO_OD_ROM.manufacturerSoftwareVersion },
	{ 0x1010, 0x01, 0x8e, 4, (void *)&CO_OD_RAM.storeParameters[0] },
	{ 0x1011, 0x01, 0x8e, 4, (void *)&CO_OD_RAM.restoreDefaultParameters[0] },
	{ 0x1012, 0x00, 0x85, 4, (void *)&CO_OD_ROM.COB_ID_TIME },
	{ 0x1014, 0x00, 0x85, 4, (void *)&CO_OD_ROM.COB_ID_EMCY },
	{ 0x1015, 0x00, 0x8d, 2, (void *)&CO_OD_ROM.inhibitTimeEMCY },
	{ 0x1016, 0x04, 0x8d, 4, (void *)&CO_OD_ROM.consumerHeartbeatTime[0] },
	{ 0x1017, 0x00, 0x8d, 2, (void *)&CO_OD_ROM.producerHeartbeatTime },
	{ 0x1018, 0x04, 0x00, 0, (void *)&OD_record1018 },
	{ 0x1019, 0x00, 0x0d, 1, (void *)&CO_OD_ROM.synchronousCounterOverflowValue },
	{ 0x1029, 0x06, 0x0d, 1, (void *)&CO_OD_ROM.errorBehavior[0] },
	{ 0x1200, 0x02, 0x00, 0, (void *)&OD_record1200 },
	{ 0x1400, 0x02, 0x00, 0, (void *)&OD_record1400 },
	{ 0x1401, 0x02, 0x00, 0, (void *)&OD_record1401 },
	{ 0x1402, 0x02, 0x00, 0, (void *)&OD_record1402 },
	{ 0x1403, 0x02, 0x00, 0, (void *)&OD_record1403 },
	{ 0x1600, 0x08, 0x00, 0, (void *)&OD_record1600 },
	{ 0x1601, 0x08, 0x00, 4, (void *)&OD_record1601 },
	{ 0x1602, 0x08, 0x00, 4, (void *)&OD_record1602 },
	{ 0x1603, 0x08, 0x00, 4, (void *)&OD_record1603 },
	{ 0x1800, 0x06, 0x00, 0, (void *)&OD_record1800 },
	{ 0x1801, 0x06, 0x00, 4, (void *)&OD_record1801 },
	{ 0x1802, 0x06, 0x00, 4, (void *)&OD_record1802 },
	{ 0x1803, 0x06, 0x00, 4, (void *)&OD_record1803 },
	{ 0x1a00, 0x08, 0x00, 0, (void *)&OD_record1a00 },
	{ 0x1a01, 0x08, 0x00, 4, (void *)&OD_record1a01 },
	{ 0x1a02, 0x08, 0x00, 0, (void *)&OD_record1a02 },
	{ 0x1a03, 0x08, 0x00, 0, (void *)&OD_record1a03 },
	{ 0x1f50, 0x01, 0x0a, 0, (void *)0 },
	{ 0x1f51, 0x01, 0x0e, 1, (void *)&CO_OD_RAM.programControl[0] },
	{ 0x1f56, 0x01, 0x86, 4, (void *)&CO_OD_RAM.programSoftwareIdentification[0] },
	{ 0x1f57, 0x01, 0x86, 4, (void *)&CO_OD_RAM.flashStatusIdentification[0] },
	{ 0x1f80, 0x00, 0x8d, 4, (void *)&CO_OD_ROM.NMTStartup },
	{ 0x2000, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.outputStates },
	{ 0x2001, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.outputCommanded },
	{ 0x2002, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.outputFaults },
	{ 0x2003, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.overCurrentFaults },
	{ 0x2004, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.openLoadFaults },
	{ 0x2005, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.outputDisabled },
	{ 0x2006, 0x00, 0xa6, 4, (void *)&CO_OD_RAM.totalCurrent_mA },
	{ 0x2007, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.supplyVoltage_mV },
	{ 0x2008, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.boardTemperature_dC },
	{ 0x2009, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.mcuTemperature_dC },
	{ 0x200a, 0x00, 0xa6, 4, (void *)&CO_OD_RAM.uptime_s },
	{ 0x200b, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.resetReason },
	{ 0x200c, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.brownoutCount },
	{ 0x200d, 0x00, 0xa6, 2, (void *)&CO_OD_RAM.canBusOffCount },
	{ 0x200e, 0x00, 0xe6, 2, (void *)&CO_OD_RAM.warningFlags },
	{ 0x200f, 0x00, 0xe6, 4, (void *)&CO_OD_RAM.telemetryValidity },
	{ 0x2100, 0x00, 0x26, 10, (void *)&CO_OD_RAM.errorStatusBits },
	{ 0x2101, 0x00, 0xa7, 4, (void *)&CO_OD_EEPROM.powerOnCounter },
	{ 0x2102, 0x00, 0xfe, 4, (void *)&CO_OD_RAM.buttonPressCounter },
	{ 0x2103, 0x00, 0x8e, 2, (void *)&CO_OD_RAM.outputRequest },
	{ 0x2104, 0x00, 0x8e, 2, (void *)&CO_OD_RAM.faultClearRequest },
	{ 0x2105, 0x00, 0x0e, 1, (void *)&CO_OD_RAM.pendingNodeID },
	{ 0x2106, 0x00, 0x8e, 2, (void *)&CO_OD_RAM.pendingBitrate_kbps },
	{ 0x2107, 0x00, 0x0e, 1, (void *)&CO_OD_RAM.commApply },
	{ 0x3000, 0x0f, 0xe6, 2, (void *)&CO_OD_RAM.outputCurrent_mA[0] },
	{ 0x3001, 0x0f, 0xa6, 2, (void *)&CO_OD_RAM.currentSenseRaw[0] },
	{ 0x3002, 0x0f, 0xa6, 2, (void *)&CO_OD_RAM.peakCurrent_mA[0] },
	{ 0x3003, 0x0f, 0xa6, 4, (void *)&CO_OD_RAM.overCurrentTripCount[0] },
	{ 0x3100, 0x0f, 0x8e, 2, (void *)&CO_OD_RAM.currentLimit_mA[0] },
	{ 0x3101, 0x0f, 0x8e, 2, (void *)&CO_OD_RAM.currentTripTime_ms[0] },
	{ 0x3102, 0x0f, 0x8e, 2, (void *)&CO_OD_RAM.inrushLimit_mA[0] },
	{ 0x3103, 0x0f, 0x8e, 2, (void *)&CO_OD_RAM.inrushTime_ms[0] },
};
