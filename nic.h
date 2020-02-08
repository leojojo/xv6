#ifndef __XV6_NIC_H__
#define __XV6_NIC_H__

#define PCI_CONF_STATUS_COMMAND 0x04
#define PCI_COM_INTR_DIS  (1U << 10)

#define NIC_REG_IMS     0x00d0      // irq mask set
#define NIC_REG_IMC    0x00d8       // irq mask clear

void nicinit(struct pci_func *pci);
void dump_nic_ims();

#endif
