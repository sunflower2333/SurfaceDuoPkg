#ifndef __EARLY_QGIC_H__
#define __EARLY_QGIC_H__

#define GIC_DIST_REG(off) (FixedPcdGet64(PcdGicDistributorBase) + (off))

#define GICR_WAKER 0x0014
#define GICR_SIZE 0x20000
#define GICR_SGI 0x10000
#define GICR_ICENABLER0 0x0180
#define GICR_ICPENDR0 0x0280

#define GIC_DIST_CTRL GIC_DIST_REG(0x000)
#define GIC_DIST_CTR GIC_DIST_REG(0x004)
#define GIC_DIST_CGCR GIC_DIST_REG(0X024)
#define GIC_DIST_ENABLE_SET GIC_DIST_REG(0x100)
#define GIC_DIST_ENABLE_CLEAR GIC_DIST_REG(0x180)
#define GIC_DIST_PENDING_SET GIC_DIST_REG(0x200)
#define GIC_DIST_PENDING_CLEAR GIC_DIST_REG(0x280)
#define GIC_DIST_ACTIVE_BIT GIC_DIST_REG(0x300)
#define GIC_DIST_PRI GIC_DIST_REG(0x400)
#define GIC_DIST_TARGET GIC_DIST_REG(0x800)
#define GIC_DIST_CONFIG GIC_DIST_REG(0xc00)
#define GIC_DIST_SOFTINT GIC_DIST_REG(0xf00)

#define GICD_IROUTER GIC_DIST_REG(0x6000)
#define GIC_REDIST_CPU(cpuid) (FixedPcdGet64(PcdGicRedistributorsBase) + (cpuid * GICR_SIZE))
#define GICR_WAKER_CPU0 GIC_REDIST_CPU(0) + GICR_WAKER

#define ENABLE_GRP0_SEC 1
#define ENABLE_GRP1_NS 2
#define ENABLE_ARE 16

EFI_STATUS
EFIAPI
QGicPeim(VOID);

#endif