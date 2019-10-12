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
  return addr;
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
  /* 一旦、コマンドとステータスを読み出す */
  uint32_t conf_data = pci_conf_read(*pci, PCI_CONF_STATUS_COMMAND);
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
