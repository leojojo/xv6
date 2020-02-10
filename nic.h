#ifndef __XV6_NIC_H__
#define __XV6_NIC_H__

#define PCI_CONF_STATUS_COMMAND 0x04
#define PCI_COM_INTR_DIS  (1U << 10)

#define NIC_REG_IMS     0x00d0      // irq mask set
#define NIC_REG_IMC    0x00d8       // irq mask clear
#define NIC_REG_EERD    0x0014       // EEPROM Read Register

#define NIC_EERD_START  (1U << 0)
#define NIC_EERD_DONE (1U << 4)
#define NIC_EERD_ADDRESS_SHIFT  8
#define NIC_EERD_DATA_SHIFT 16

void nicinit(struct pci_func *pci);
void dump_nic_ims();
uint32_t* get_mac_addr();

#endif
