#include "types.h"
#include "defs.h"
#include "x86.h"

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

struct pci_func {
  int bus;
  int device;
  int function;
};

int
pci_conf_read(struct pci_func pci, int offset)
{
  int value;
  // 0x80000000: start of kernel space
  // 31     30-24    23-16(8bit) 15-11(5bit)  10-8(3bit)     7-0(8bit)
  // Enable Reserved Bus         Device       Function       Offset
  value = 0x80000000 | pci.bus << 16 | pci.device << 11 | pci.function << 8 | offset;
  outl(PCI_CONFIG_ADDRESS, value);
  return inl(PCI_CONFIG_DATA);
}

int
pciinit()
{
  int device_ctr = 0;
  struct pci_func pci;
  memset(&pci, 0, sizeof(struct pci_func));

  for (pci.device = 0; pci.device < 32; pci.device++) {      // loop over bus for devices
    device_ctr++;

    for (pci.function = 0; pci.function < 8; pci.function++) {      // loop over device for functions
      int vend_prod = pci_conf_read(pci, PCI_ID_REG);
      if (PCI_VENDOR(vend_prod) == 0xffff)
        continue;
      int irq = PCI_INTERRUPT_LINE(pci_conf_read(pci, PCI_INTERRUPT_REG));
      int class = PCI_CLASS(pci_conf_read(pci, PCI_CLASS_REG));
      static const char *pci_class[] = {"Unclassified device", "Mass storage controller", "Network controller", "Display controller", "Multimedia device", "Memory controller", "Bridge device"};

      cprintf("PCI\tbus: %x\t", pci.bus);
      cprintf("device: %x\t", pci.device);
      cprintf("function: %d\t", pci.function);
      cprintf("irq line: %d\t", irq);
      cprintf("vendor: %x\t", PCI_VENDOR(vend_prod));
      cprintf("product: %x\t", PCI_PRODUCT(vend_prod));
      cprintf("class: %s\n", pci_class[class]);
    }
  }

  return device_ctr;
}
