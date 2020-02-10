#include "types.h"
#include "defs.h"
#include "x86.h"
#include "pci.h"
#include "nic.h"

uint32_t nic_reg_base;

uint32_t
get_nic_reg(uint32_t reg)
{
  uint32_t addr = nic_reg_base + reg;
  return *(uint32_t *)addr;
}

void
set_nic_reg(uint32_t reg, uint32_t val)
{
  uint32_t addr = nic_reg_base + reg;
  *(uint32_t *)addr = val;
}

void
disable_nic_interrupt(struct pci_func *pci)
{
  uint32_t conf_data = pci_conf_read(*pci, PCI_CONF_STATUS_COMMAND);
  conf_data &= 0x0000ffff;
  conf_data |= PCI_COM_INTR_DIS;
  pci_conf_write(*pci, PCI_CONF_STATUS_COMMAND, conf_data);
  set_nic_reg(NIC_REG_IMC, 0xffffffff);
}

void
dump_nic_ims()
{
  uint32_t ims = get_nic_reg(NIC_REG_IMS);
  cprintf("%x\n",ims);
  set_nic_reg(NIC_REG_IMS,0x0000beef);
  ims = get_nic_reg(NIC_REG_IMS);
  cprintf("%x\n",ims);
}

uint32_t
get_eeprom(uint16_t eeprom_addr)
{
  set_nic_reg(NIC_REG_EERD, (eeprom_addr << NIC_EERD_ADDRESS_SHIFT) | NIC_EERD_START);
  uint32_t eerd = get_nic_reg(NIC_REG_EERD);
  while (1) {
    if (eerd & NIC_EERD_DONE) {
      return eerd >> NIC_EERD_DATA_SHIFT;
    }
  }
}

uint32_t*
get_mac_addr(uint32_t* nic_mac_addr)
{
  uint32_t mac_1_0 = get_eeprom(0x00);
  uint32_t mac_3_2 = get_eeprom(0x01);
  uint32_t mac_5_4 = get_eeprom(0x02);

  nic_mac_addr[0] = mac_1_0 & 0x00ff;
  nic_mac_addr[1] = mac_1_0 >> 8;
  nic_mac_addr[2] = mac_3_2 & 0x00ff;
  nic_mac_addr[3] = mac_3_2 >> 8;
  nic_mac_addr[4] = mac_5_4 & 0x00ff;
  nic_mac_addr[5] = mac_5_4 >> 8;

  return nic_mac_addr;
}

void
print_mac_addr(uint32_t *nic_mac_addr)
{
  for(int i = 0; i < 6; i++) {
    cprintf("%d:",nic_mac_addr[i]);
  }
  cprintf("\n");
}

void
nicinit(struct pci_func *pci)
{
  uint32_t nic_mac_addr[6];
  nic_reg_base = get_base_address(pci);
  disable_nic_interrupt(pci);
  print_mac_addr(get_mac_addr(nic_mac_addr));
  //dump_nic_ims();
}
