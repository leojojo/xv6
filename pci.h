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

#define NETWORK_CONTROLLER_CLASS 2

#define PCI_BAR_REG 0x10
#define PCI_BAR_MASK_MEM_TYPE 0x00000006
#define PCI_BAR_MEM_TYPE_32BIT 0x00000000
#define PCI_BAR_MASK_MEM_ADDR 0xfffffff0

struct pci_func {
  int bus;
  int device;
  int function;

  int dev_id;
  int dev_class;

  int reg_base[6];
  int reg_size[6];
  int irq;
};

int pci_conf_read(struct pci_func pci, int offset);
void pci_conf_write(struct pci_func pci, int offset, int value);
int get_base_address(struct pci_func *pci);

#endif
