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

void nicinit(struct pci_func *pci);

int
pci_conf_read(struct pci_func pci, int offset)
{
  // 0x80000000: start of kernel space
  // 31     30-24    23-16(8bit) 15-11(5bit) 10-8(3bit) 7-0(8bit)
  // Enable Reserved Bus         Device      Function   Offset
  int address = 0x80000000 | pci.bus << 16 | pci.device << 11 | pci.function << 8 | offset;
  outl(PCI_CONFIG_ADDRESS, address);
  return inl(PCI_CONFIG_DATA);
}

void
pci_conf_write(struct pci_func pci, int offset, int value)
{
  int address = 0x80000000 | pci.bus << 16 | pci.device << 11 | pci.function << 8 | offset;
  outl(PCI_CONFIG_ADDRESS, address);
  outl(PCI_CONFIG_ADDRESS, value);
}

void
print_pci_function(struct pci_func *pci)
{
  static const char *pci_class[] = {
    "Unclassified device",
    "Mass storage controller",
    "Network controller",         // 2
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
get_base_address(struct pci_func *pci)      // todo: currently only returns address for 32-bit memory region
{
  int bar = pci_conf_read(*pci, PCI_BAR_REG);

  if ((bar & PCI_BAR_MASK_MEM_TYPE) == PCI_BAR_MEM_TYPE_32BIT) {
      int base_addr = bar & PCI_BAR_MASK_MEM_ADDR;
      cprintf("Memory at: %x (32-bit)\n", base_addr);
      return base_addr;
  }
  return 0;
}

int
pci_attach_driver(struct pci_func *pci)      // todo: currently only looks for e1000
{
  if (PCI_VENDOR(pci->dev_id) == 0x8086 && PCI_PRODUCT(pci->dev_id) == 0x100e) {
    nicinit(pci);
  }
  return 0;
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

      if (PCI_CLASS(pci.dev_class) == NETWORK_CONTROLLER_CLASS)
        pci_attach_driver(&pci);
    }
  }

  return device_ctr;
}

// ==================== NIC ==================== //
static unsigned int nic_reg_base;

unsigned int
get_nic_reg(unsigned int reg)
{
  unsigned int addr = nic_reg_base + reg;
  return *(volatile unsigned int *)addr;
}

void
set_nic_reg(unsigned int reg, unsigned int val)
{
  unsigned int addr = nic_reg_base + reg;
  *(volatile unsigned int *)addr = val;
}

static void
disable_nic_interrupt(struct pci_func *pci)
{
  /* 一旦、コマンドとステータスを読み出す */
  unsigned int conf_data = pci_conf_read(*pci, PCI_CONF_STATUS_COMMAND);

  /* ステータス(上位16ビット)をクリア */
  conf_data &= 0x0000ffff;
  /* コマンドに割り込み無効設定 */
  conf_data |= PCI_COM_INTR_DIS;

  /* コマンドとステータスに書き戻す */
  pci_conf_write(*pci, PCI_CONF_STATUS_COMMAND, conf_data);

  /* NICの割り込みをIMC(Interrupt Mask Clear Register)で全て無効化 */
  set_nic_reg(NIC_REG_IMC, 0xffffffff);
}

void
nicinit(struct pci_func *pci)
{
  /* NICのレジスタのベースアドレスを取得しておく */
  nic_reg_base = get_base_address(pci);

  /* NICの割り込みを全て無効にする */
  disable_nic_interrupt(pci);
}
