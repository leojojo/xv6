#ifndef __XV6_PCI_H__
#define __XV6_PCI_H__

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_INTERRUPT_REG 0x3c
#define PCI_INTERRUPT_LINE_SHIFT    0
#define PCI_INTERRUPT_LINE_MASK     0xff
#define PCI_INTERRUPT_LINE(icr) \
        (((icr) >> PCI_INTERRUPT_LINE_SHIFT) & PCI_INTERRUPT_LINE_MASK)

#define PCI_ID_REG 0x00
#define PCI_VENDOR_SHIFT      0
#define PCI_VENDOR_MASK       0xffff
#define PCI_VENDOR(id) \
        (((id) >> PCI_VENDOR_SHIFT) & PCI_VENDOR_MASK)

#define PCI_PRODUCT_SHIFT     16
#define PCI_PRODUCT_MASK      0xffff
#define PCI_PRODUCT(id) \
        (((id) >> PCI_PRODUCT_SHIFT) & PCI_PRODUCT_MASK)

#define PCI_CLASS_REG 0x08
#define PCI_CLASS_SHIFT       24
#define PCI_CLASS_MASK        0xff
#define PCI_CLASS(cr) \
        (((cr) >> PCI_CLASS_SHIFT) & PCI_CLASS_MASK)

#endif
