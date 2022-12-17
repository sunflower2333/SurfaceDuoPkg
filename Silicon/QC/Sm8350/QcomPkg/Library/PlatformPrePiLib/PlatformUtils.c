#include <PiPei.h>
#include <IndustryStandard/ArmStdSmc.h>
#include <Library/ArmLib.h>
#include <Library/ArmGicLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include "PlatformUtils.h"

BOOLEAN IsLinuxBootRequested(VOID)
{
  return (MmioRead32(LID0_GPIO38_STATUS_ADDR) & 1) == 0;
}

VOID DisableAllInterrupts ()
{
  // Initialize GIC
  MmioWrite32(
      PcdGet64(PcdGicRedistributorsBase) + 0x20000 + 0x0014,
      (MmioRead32(PcdGet64(PcdGicRedistributorsBase) + 0x20000 + 0x0014) & ~2));

  UINTN GicNumInterrupts = ArmGicGetMaxNumInterrupts ((UINT32)PcdGet64 (PcdGicDistributorBase));

  for (UINTN Index = 0; Index < GicNumInterrupts; Index++) {
    if (ArmGicIsInterruptEnabled (PcdGet64 (PcdGicDistributorBase), PcdGet64 (PcdGicRedistributorsBase), Index)) {
      ArmGicDisableInterrupt (PcdGet64 (PcdGicDistributorBase), PcdGet64 (PcdGicRedistributorsBase), Index);
    }
  }

  UINTN InterruptId;
  UINTN IntValue = ArmGicAcknowledgeInterrupt (PcdGet64 (PcdGicInterruptInterfaceBase), &InterruptId);
  ArmGicEndOfInterrupt (PcdGet64 (PcdGicInterruptInterfaceBase), IntValue);
}

UINTN
PSCI_CPU_ON(UINTN target_cpu, UINTN entry_point_address, UINTN context_id)
{
  ARM_SMC_ARGS ArmSmcArgs;
  ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_CPU_ON_AARCH64;
  ArmSmcArgs.Arg1 = target_cpu;
  ArmSmcArgs.Arg2 = entry_point_address;
  ArmSmcArgs.Arg3 = context_id;

  ArmCallSmc(&ArmSmcArgs);
  return ArmSmcArgs.Arg0;
}

UINTN
PSCI_CPU_OFF()
{
  ARM_SMC_ARGS ArmSmcArgs;
  ArmSmcArgs.Arg0 = ARM_SMC_ID_PSCI_CPU_OFF;

  ArmCallSmc(&ArmSmcArgs);
  return ArmSmcArgs.Arg0;
}

VOID Ap1EntryPoint ()
{
  DisableAllInterrupts ();
  PSCI_CPU_OFF ();
}

VOID PlatformInitialize(VOID)
{
  DisableAllInterrupts ();
  PSCI_CPU_ON (0x00000100, (UINTN)&Ap1EntryPoint, 0);
}