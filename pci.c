#include "types.h"
#include "defs.h"
#include "x86.h"
#include "pci.h"

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

void
print_pci_function(struct pci_func *pci)
{
  static const char *pci_class[] = {
    "Unclassified device",
    "Mass storage controller",
    "Network controller",
    "Display controller",
    "Multimedia device",
    "Memory controller",
    "Bridge device",
  };

  cprintf("PCI\tbus: %x\t", pci->bus);
  cprintf("device: %x\t", pci->device);
  cprintf("function: %d\t", pci->function);
  cprintf("irq line: %d\t", pci->irq);
  cprintf("vendor: %x\t", PCI_VENDOR(pci->dev_id));
  cprintf("product: %x\t", PCI_PRODUCT(pci->dev_id));
  cprintf("class: %s\n", pci_class[PCI_CLASS(pci->dev_class)]);
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
      pci.dev_id = pci_conf_read(pci, PCI_ID_REG);
      if (PCI_VENDOR(pci.dev_id) == 0xffff)
        continue;
      pci.irq = PCI_INTERRUPT_LINE(pci_conf_read(pci, PCI_INTERRUPT_REG));
      pci.dev_class = pci_conf_read(pci, PCI_CLASS_REG);

      print_pci_function(&pci);
    }
  }

  return device_ctr;
}
