#ifndef __FDP_H__
#define __FDP_H__

#include "FDP_enum.h"


#define FDP_EXPORTED   __declspec( dllexport ) 

#ifndef FDP_EXPORTED
#define FDP_EXPORTED
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef __cplusplus
    typedef struct _uint128_t
    {
        uint64_t high;
        uint64_t low;
    } uint128_t;
//#endif

    typedef struct _declspec(align(16)) FDP_XSAVE_FORMAT64_T_
    {
        uint16_t    ControlWord;
        uint16_t    StatusWord;
        uint8_t     TagWord;
        uint8_t     Reserved1;
        uint16_t    ErrorOpcode;
        uint32_t    ErrorOffset;
        uint16_t    ErrorSelector;
        uint16_t    Reserved2;
        uint32_t    DataOffset;
        uint16_t    DataSelector;
        uint16_t    Reserved3;
        uint32_t    MxCsr;
        uint32_t    MxCsr_Mask;
        uint128_t   FloatRegisters[8];

        uint128_t   XmmRegisters[16];
        uint8_t     Reserved4[96];
	} FDP_XSAVE_FORMAT64_T;

#define    FDP_MAX_BREAKPOINT 255


    typedef _declspec(align(1)) struct FDP_SHM_ FDP_SHM;

    typedef struct _FDP_SERVER_INTERFACE_T{
        bool bIsRunning;

        void *pUserHandle;

        //all function ptr!
        bool(*pfnGetState)				(void *, uint8_t*);
        bool(*pfnReadRegister)			(void *, uint32_t, FDP_Register, uint64_t*);
        bool(*pfnWriteRegister)			(void *, uint32_t, FDP_Register, uint64_t);
        bool(*pfnWritePhysicalMemory)	(void*, uint8_t*, uint64_t, uint32_t);
        bool(*pfnReadPhysicalMemory)	(void*, uint8_t*, uint64_t, uint32_t);
        bool(*pfnWriteVirtualMemory)	(void*, uint32_t, uint8_t*, uint64_t, uint32_t);
        bool(*pfnGetMemorySize)			(void *, uint64_t *);
        bool(*pfnResume)				(void*);
        bool(*pfnPause)					(void*);
        bool(*pfnSingleStep)			(void*, uint32_t);
        bool(*pfnWriteMsr)				(void*, uint32_t, uint64_t, uint64_t);
        bool(*pfnReadMsr)				(void*, uint32_t, uint64_t, uint64_t*);
        bool(*pfnGetCpuCount)			(void*, uint32_t*);
        bool(*pfnGetCpuState)			(void*, uint32_t, uint8_t*);
        bool(*pfnUnsetBreakpoint)		(void*, uint8_t);
        bool(*pfnVirtualToPhysical)		(void*, uint32_t, uint64_t, uint64_t*);
        bool(*pfnGetFxState64)			(void*, uint32_t, uint8_t*, uint32_t*);
        bool(*pfnSetFxState64)			(void*, uint32_t, uint8_t*, uint32_t);
        bool(*pfnReadVirtualMemory)		(void*, uint32_t, uint64_t, uint32_t, uint8_t*);
        int(*pfnSetBreakpoint)			(void *, uint32_t, FDP_BreakpointType, uint8_t, FDP_Access, FDP_AddressType, uint64_t, uint64_t);
        bool(*pfnSave)					(void*);
        bool(*pfnRestore)				(void*);
        bool(*pfnReboot)				(void*);
        bool(*pfnInjectInterrupt)       (void*, uint32_t, uint32_t, uint32_t, uint64_t);
    }FDP_SERVER_INTERFACE_T;

    // FDP API
FDP_EXPORTED	FDP_SHM*    FDP_CreateSHM(char *shmName);
FDP_EXPORTED	FDP_SHM*    FDP_OpenSHM(const char *pShmName);
FDP_EXPORTED	bool        FDP_Init(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_Pause(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_Resume(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_ReadPhysicalMemory(FDP_SHM *pShm, uint8_t *pDstBuffer, uint32_t ReadSize, uint64_t PhysicalAddress);
FDP_EXPORTED	bool        FDP_WritePhysicalMemory(FDP_SHM *pShm, uint8_t *pSrcBuffer, uint32_t WriteSize, uint64_t PhysicalAddress);
FDP_EXPORTED	bool        FDP_ReadVirtualMemory(FDP_SHM *pShm, uint32_t CpuId, uint8_t *pDstBuffer, uint32_t ReadSize, uint64_t VirtualAddress);
FDP_EXPORTED	bool        FDP_WriteVirtualMemory(FDP_SHM *pShm, uint32_t CpuId, uint8_t *pSrcBuffer, uint32_t WriteSize, uint64_t VirtualAddress);
FDP_EXPORTED	uint64_t    FDP_SearchPhysicalMemory(FDP_SHM *pShm, const void *pPatternData, uint32_t PatternSize, uint64_t StartOffset);
FDP_EXPORTED	bool        FDP_SearchVirtualMemory(FDP_SHM *pFDP, uint32_t CpuId, const void *pPatternData, uint32_t PatternSize, uint64_t StartOffset);
FDP_EXPORTED	bool        FDP_ReadRegister(FDP_SHM *pShm, uint32_t CpuId, FDP_Register RegisterId, uint64_t *pRegisterValue);
FDP_EXPORTED	bool        FDP_WriteRegister(FDP_SHM *pShm, uint32_t CpuId, FDP_Register RegisterId, uint64_t RegisterValue);
FDP_EXPORTED	bool        FDP_ReadMsr(FDP_SHM *pShm, uint32_t CpuId, uint64_t MsrId, uint64_t *pMsrValue);
FDP_EXPORTED	bool        FDP_WriteMsr(FDP_SHM *pShm, uint32_t CpuId, uint64_t MsrId, uint64_t MsrValue);
FDP_EXPORTED	int         FDP_SetBreakpoint(FDP_SHM *pShm, uint32_t CpuId, FDP_BreakpointType BreakpointType, uint8_t BreakpointId, FDP_Access BreakpointAccessType, FDP_AddressType BreakpointAddressType, uint64_t BreakpointAddress, uint64_t BreakpointLength);
FDP_EXPORTED	bool        FDP_UnsetBreakpoint(FDP_SHM *pShm, uint8_t BreakpointId);
FDP_EXPORTED	bool        FDP_VirtualToPhysical(FDP_SHM *pShm, uint32_t CpuId, uint64_t VirtualAddress, uint64_t *pPhysicalAddress);
FDP_EXPORTED	bool        FDP_GetState(FDP_SHM *pShm, FDP_State *pState);
FDP_EXPORTED	bool        FDP_GetFxState64(FDP_SHM *pShm, uint32_t CpuId, FDP_XSAVE_FORMAT64_T *pFxState64);
FDP_EXPORTED	bool        FDP_SetFxState64(FDP_SHM *pFDP, uint32_t CpuId, FDP_XSAVE_FORMAT64_T* pFxState64);
FDP_EXPORTED	bool        FDP_SingleStep(FDP_SHM *pShm, uint32_t CpuId);
FDP_EXPORTED	bool        FDP_GetPhysicalMemorySize(FDP_SHM *pShm, uint64_t *pPhysicalMemorySize);
FDP_EXPORTED	bool        FDP_GetCpuCount(FDP_SHM *pShm, uint32_t *pCPUCount);
FDP_EXPORTED	bool        FDP_GetCpuState(FDP_SHM *pShm, uint32_t CpuId, FDP_State *pState);
FDP_EXPORTED	bool        FDP_Reboot(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_Save(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_Restore(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_GetStateChanged(FDP_SHM *pShm);
FDP_EXPORTED	void        FDP_SetStateChanged(FDP_SHM *pShm);
FDP_EXPORTED	bool        FDP_InjectInterrupt(FDP_SHM *pShm, uint32_t CpuId, uint32_t uInterruptionCode, uint32_t uErrorCode, uint64_t Cr2Value);


    uint8_t     FDP_Test(FDP_SHM *pShm);

    bool		FDP_UnitTest();

#ifdef __cplusplus
}
#endif

#endif //__FDP_H__
