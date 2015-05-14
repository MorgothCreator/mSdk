/*
 * lwip_api.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */
#include "lwip_hardware_init.h"

#include "clk/clk_cpsw.h"
#include "pin_mux_cpsw.h"
#include "aintc/aintc_cpsw.h"
#include "driver/cpu.h"
#include "ports/include/lwiplib.h"
#include "lib/util//ascii.h"
#include "lib/lwip/simple_fs_httpd.h"

#ifdef UseLwip
extern new_uart* DebugCom;

#define MASK_LOWER_4BITS                   0x0F
#define MASK_BYTE_UPPER_4BITS              0xF0

#ifdef CPSW_SWITCH_CONFIG
#define CONFIG_SWITCH_EXIT_CMD             (21)
#else
#define CONFIG_SWITCH_EXIT_CMD             (2)
#endif

#define DEF_SLV_PORT                       (1)
#define DEF_AUTONEG                        (1)
#define DEF_PHY_CONFIG                     (0x3f)
#define DEF_SPEED                          (1)
#define DEF_DUPLEX                         (1)

#ifdef CPSW_SWITCH_CONFIG
#define NUM_BYTES_ALE_ENTRY_WIDTH          (24)
#define NUM_DWORDS_ALE_ENTRY               (3)
#define NUM_BYTES_ADDR                     (6)

#define MASK_ENTRY_TPYE_WORD               (0x30000000)
#define MASK_BROADCAST_ADDR                (0xFF)
#define MASK_MULTICAST_ADDR                (0x01)

#define MIN_ALE_ENTRY_IDX_START            (0)
#define MIN_ALE_ENTRY_IDX_END              (0)

#define MAX_ALE_ENTRY_IDX_START            (1023)
#define MAX_ALE_ENTRY_IDX_END              (1023)

#define DEF_VLANAWARE                      (0)
#define DEF_ALE_VLANAWARE                  (0)
#define DEF_ALE_ENTRY_IDX                  (0)
#define DEF_ALE_ENTRY_IDX_START            (0)
#define DEF_ALE_ENTRY_IDX_END              (1023)
#define DEF_VLAN_UNTAG                     (0)
#define DEF_VLAN_MCAST_UNREG               (0)
#define DEF_VLAN_MCAST_REG                 (0)
#define DEF_MCAST_FWD_STATE                (0)
#define DEF_SUPER                          (0)
#define DEF_PORT_MASK                      (0)
#define DEF_UCAST_FLAGS                    (0)
#define DEF_PORT                           (0)
#define DEF_VLANID                         (0)
#define DEF_CFI                            (0)
#define DEF_PRI                            (0)
#define DEF_PORT_STATE                     (3)
#define DEF_ADDR_TYPE                      (1)
#define DEF_DIRECTION                      (0)
#define DEF_LIMIT                          (0)
#define DEF_UNKNOWN_VLAN                   (0)
#define DEF_MAC_AUTH                       (0)
#define DEF_ENABLE                         (0)
#endif

static CPSW_PHY_PARAM_IF cpswPhyParam;
#ifdef CPSW_SWITCH_CONFIG
static CPSW_SW_PARAM_IF cpswSwitchParam;
#endif
static CPSW_CONF_IF cpswConfig;



/*
** This function takes user input for value and checks given values is the
** range of min and max values. If not default value is assiagned.
*/
static unsigned int UserValueInfoGet(unsigned int min, unsigned int max,
                                     unsigned int def, unsigned int hex,
                                     unsigned char *help)
{
    unsigned int value = 0;

    UARTprintf(DebugCom, "%s", help);

    if(FALSE == hex)
    {
    	UARTscanf(DebugCom, "%d", &value);
    }
    else
    {
    	UARTscanf(DebugCom, "%x", &value);
    }

    if((min > value) || (value > max))
    {
    	UARTprintf(DebugCom, "\n\rSetting Default Value: 0x%x\r\n", def);
        return def;
    }

    return value;
}


#ifdef CPSW_SWITCH_CONFIG
/*
** This function check for valid address for the given Unicast, Multicast
** and Broadcast type.
*/
unsigned int IsEthAddrValid(unsigned char *ethAddr, unsigned int addrTyp)
{
    unsigned int cnt = 0;

    if(!addrTyp)
        return TRUE;

    for(cnt = 0; cnt < NUM_BYTES_ADDR; cnt++)
        if((ethAddr[cnt] & MASK_BROADCAST_ADDR) != MASK_BROADCAST_ADDR)
            break;

    if((cnt == NUM_BYTES_ADDR) && (addrTyp == ADDR_TYPE_BROADCAST))
        return TRUE;
    else if(cnt == NUM_BYTES_ADDR)
        return FALSE;
    else if(addrTyp == ADDR_TYPE_BROADCAST)
        return FALSE;

    if(addrTyp == ADDR_TYPE_MULTICAST)
    {
        if((ethAddr[0] & MASK_MULTICAST_ADDR) == MASK_MULTICAST_ADDR)
            return TRUE;
        else
            return FALSE;
    }
    else if(addrTyp == ADDR_TYPE_UNICAST)
    {
        if((ethAddr[0] & MASK_MULTICAST_ADDR) != MASK_MULTICAST_ADDR)
            return TRUE;
        else
            return FALSE;
    }

    return FALSE;
}

/*
** This function takes user input and checks for valid address for the given
** Unicast, Multicast and Broadcast type.
*/
unsigned int UserEthAddrInfoGet(unsigned char *ethAddr, unsigned int typ)
{
    unsigned char ethAddrStr[3 * NUM_BYTES_ADDR] = {0};
    unsigned char ethAddrMulticast[NUM_BYTES_ADDR] =
                                           {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
    unsigned char ethAddrUnicast[NUM_BYTES_ADDR] =
                                           {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54};
    unsigned char ethAddrBroadcast[NUM_BYTES_ADDR] =
                                           {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned int status = 0;
    unsigned int index = 0;

    UARTPuts(DebugCom, "\nEnter the Ethernet Address.\r\n", -1);
    UARTPuts(DebugCom, "Example (ZY-XW-VU-TS-RQ-PO) 00-13-72-8C-F6-EF", -1);

    UARTPuts(DebugCom, "\r\nEthernet Address: ", -1);

    UARTscanf(DebugCom, "%s", ethAddrStr);

    status = StrToEthrAddr(ethAddrStr, ethAddr);

    if(!(IsEthAddrValid(ethAddr, typ) && (status == 3*NUM_BYTES_ADDR)))
    {
        if(typ == ADDR_TYPE_MULTICAST)
        {
            memcpy(ethAddr, ethAddrMulticast, NUM_BYTES_ADDR);
        }
        else if(typ == ADDR_TYPE_UNICAST)
        {
            memcpy(ethAddr, ethAddrUnicast, NUM_BYTES_ADDR);
        }
        else if(typ == ADDR_TYPE_BROADCAST)
        {
            memcpy(ethAddr, ethAddrBroadcast, NUM_BYTES_ADDR);
        }

        status = EthrAddrToStr(ethAddr, ethAddrStr);
        UARTprintf(DebugCom, "\nSetting Default Value : %s", ethAddrStr);
        return FALSE;
    }

    return TRUE;
}

/*
** This function prints the hex code data of ALE Entry onto user console.
*/
void UserALEEntryIndexInfoPrint(unsigned int *aleEntry)
{
    unsigned char aleEntryOutput[NUM_BYTES_ALE_ENTRY_WIDTH + 1] = {0};
    unsigned int index = 0;
    unsigned char upper = 0;
    unsigned char lower = 0;

    for(index = 0; index < NUM_BYTES_ALE_ENTRY_WIDTH/2; index++)
    {
        lower = DigitToASCII(*(((unsigned char *)aleEntry) + index) &
                              MASK_LOWER_4BITS, BASE_HEXADECIMAL);
        upper = DigitToASCII((*(((unsigned char *)aleEntry) + index) &
                              MASK_BYTE_UPPER_4BITS) >> 4, BASE_HEXADECIMAL);

        if((lower != 0xFF) && (upper != 0xFF))
        {
            aleEntryOutput[NUM_BYTES_ALE_ENTRY_WIDTH - 2*index - 1] = lower;
            aleEntryOutput[NUM_BYTES_ALE_ENTRY_WIDTH - 2*index - 2] = upper;
        }
        else
        {
            break;
        }
    }

    aleEntryOutput[NUM_BYTES_ALE_ENTRY_WIDTH] = '\0';
    UARTprintf(DebugCom, "%s", aleEntryOutput);
}
#endif

/*
** This is the CPSW Switch Configuration interface to
**   1. Take User Input
**   2. Configure the CPSW
**   3. Print the status onto user console
**
** Available Switch Configutarions
**  1 - Add a multicast entry
**  2 - Add a unicast entry
**  3 - Add a OUI entry
**  4 - Search address in entry list
**  5 - Delete a multicast entry
**  6 - Delete a unicast entry
**  7 - Adds a vlan entry
**  8 - Search vlan in entry list
**  9 - Delete vlan
** 10 - Configure Port Vlan (ID, CFI, PRI)
** 11 - Age Out the Untouched entries of ALE Table
** 12 - Print Dump of all valid ALE Table Entries in given range
** 13 - Print Dump of an ALE Entry
** 14 - ALE VLAN Aware Config
** 15 - Configure Rate Limit for TX or RX
** 16 - Enable Engress Check
** 17 - Set port unknown VLAN info
** 18 - Enable MAC Auth
** 19 - Configure Port State
**
** Available PHY Configuration
**  1 - Configure Phy of a port
**
*/
static unsigned int EnetLWIPSwitchConfiguration(unsigned int switchConfig)
{
#ifdef CPSW_SWITCH_CONFIG
    unsigned int aleEntryIndex = 0;
    unsigned int aleEntryStartIndex = 0;
    unsigned int aleEntryEndIndex = 0;
    signed int ret_cpsw = 0;
    unsigned char vlanInputHelp[] =
     "\nVLAN ID (0x00-0xFF) [(e.g. 127 or 0xFF) - 0xFF] : 0x";
    unsigned char portInputHelp[] =
     "\nPort (0x0-0x2) [(e.g. 2 or 0x2) - 0x2] : 0x";
    unsigned char ucastFlagsInputHelp[] =
     "\nUnicast Flags (0x0-0x3) [(e.g. 3 or 0x3) - 0x3] : 0x";
    unsigned char ucastTypeInputHelp[] =
     "\nUnicast Type (0x0-Persistant, 0x1-Untouched, 0x2-OUI, 0x3-Touched): 0x";
    unsigned char portMaskInputHelp[] =
     "\nPortmask (0x0-0x7) [(e.g. 7 or 0x7) - 0x7] : 0x";
    unsigned char superInputHelp[] =
     "\nVLAN Super (0x0-0x1) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char mcastFwdStateInputHelp[] =
     "\nMulticast Fwd State (0x0-0x3) [(e.g. 3 or 0x3) - 0x3] : 0x";
    unsigned char vlanMcastRegInputHelp[] =
     "\nVLAN reg multicast Flood Mask (0x0-0x7) [(e.g. 7 or 0x7) - 0x7] : 0x";
    unsigned char vlanMcastUnregInputHelp[] =
     "\nVLAN Unreg Multicast Flood Mask (0x0-0x7) [(e.g. 7 or 0x7) - 0x7] : 0x";
    unsigned char vlanUntagInputHelp[] =
     "\nForce Untagged Egress of vlan (0x0-0x7) [(e.g. 7 or 0x7) - 0x7] : 0x";
    unsigned char aleEntryIndexInputHelp[] =
     "\nALE Table Entry Index (0x000-0x3FF) [(e.g. 127 or 0x0FF) - 0x0FF] : 0x";
    unsigned char aleEntryStartIndexInputHelp[] =
     "\nALE Start Entry Index (0x000-0x3FF) [(e.g. 127 or 0x0FF) - 0x0FF] : 0x";
    unsigned char aleEntryEndIndexInputHelp[] =
     "\nALE End Entry Index (START-0x3FF) [(e.g. 127 or 0x0FF) - 0x0FF] : 0x";
    unsigned char vlanAwareInputHelp[] =
     "\nCPSW VLAN Aware (0x0- Disable; 0x1- Enable) [(e.g. 1 or 0x1)-0x1] : 0x";
    unsigned char aleVlanAwareInputHelp[] =
     "\nALE VLAN Aware (0x0 - Flood; 0x1 - Drop) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char vlanPortCFIInputHelp[] =
     "\nCFI (0x0 - 0x1) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char vlanPortPriInputHelp[] =
     "\nPriority (0x0 - 0x7) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char enableInputHelp[] =
     "\nEnable (0x0 - 0x1) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char directionInputHelp[] =
     "\nDirection (0x0 - 0x1) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char addrTypeInputHelp[] =
     "\nAddress Type (0x1 - BROD, 0x2 - MULTI, 0x3- UNI) : 0x";
    unsigned char limitInputHelp[] =
     "\nLimit (0x00 - 0xFF) [(e.g. 511 or 0x1F) - 0x1F] : 0x";
    unsigned char unknownVlanInputHelp[] =
     "\nPort Unknown (0x1-UNTAG,0x1-Reg Multi,0x2-Unreg Multi,0x3-Vlan Mem):0x";
    unsigned char macAuthInputHelp[] =
     "\nMAC Auth (0x0 - Disable, 0x1 - Enable) [(e.g. 1 or 0x1) - 0x1] : 0x";
    unsigned char portStateInputHelp[] =
     "\nPort State (0x3 - Fwd, 0x2 - Learn, 0x1 - Blocked, 0x0 - Disable) : 0x";
#endif
    unsigned char slavePortInputHelp[] =
     "\nSlave Port (0x1-0x2):0x";
    unsigned char autonegInputHelp[] =
     "\nSelect Configuration (0x0 - Manual, 0x1 - Autonegotiation) : 0x";
    unsigned char configSpeedDuplexInputHelp1[] =
     "\nSelect Speed-Duplex Modes\n BIT0 - 10 Base Half Duplex\n BIT1 - 10 Bas";
    unsigned char configSpeedDuplexInputHelp2[] =
     "e FULL Duplex\n BIT2 - 100 Base Half Duplex\n BIT3 - 100 Base Full Duple";
    unsigned char configSpeedDuplexInputHelp3[] =
     "x\n BIT4 - 1000 Base Half Duplex\n BIT5 - 1000 Base Full Duplex\n : 0x";
    unsigned char speedInputHelp[] =
     "\nSpeed (0x0 - 10 Base, 0x1 - 100 Base, 0x2 - 1000 Base) : 0x";
    unsigned char duplexInputHelp[] =
     "\nDuplex (0x0 - Half Duplex, 0x1 - Full Duplex) : 0x";

    cpswConfig.cmd = CONFIG_SWITCH_INVALID;
    cpswConfig.cpsw_inst = 0;
    cpswConfig.ret = ERR_INVAL;
#ifdef CPSW_SWITCH_CONFIG
    cpswConfig.switch_param->port_num = 0;
    cpswConfig.switch_param->port_mask = 0;
    cpswConfig.switch_param->vid = 0;
    cpswConfig.switch_param->prio_port = 0;
    cpswConfig.switch_param->cfi_port = 0;
    cpswConfig.switch_param->unreg_multi = 0;
    cpswConfig.switch_param->reg_multi = 0;
    cpswConfig.switch_param->untag_port = 0;
    cpswConfig.switch_param->super = 0;
    cpswConfig.switch_param->fwd_state = 0;
    cpswConfig.switch_param->ucast_flags = 0;
    cpswConfig.switch_param->ucast_type = 0;
    cpswConfig.switch_param->blocked = 0;
    cpswConfig.switch_param->secure = 0;
    cpswConfig.switch_param->ageable = 0;
    cpswConfig.switch_param->ale_tbl_index = 0;
    cpswConfig.switch_param->vlan_aware = 0;
    cpswConfig.switch_param->drop_packet = 0;
    cpswConfig.switch_param->direction = 0;
    cpswConfig.switch_param->addr_type = 0;
    cpswConfig.switch_param->limit = 0;
    cpswConfig.switch_param->vlan_ingress_check = 0;
    cpswConfig.switch_param->port_state = 0;
    cpswConfig.switch_param->drop_untagged = 0;
    cpswConfig.switch_param->enable = 0;
    cpswConfig.switch_param->unknown_vlan = 0;
    cpswConfig.switch_param->mac_auth = 0;
#endif
    cpswConfig.phy_param->slv_port_num = 0;
    cpswConfig.phy_param->autoneg = 0;
    cpswConfig.phy_param->config = 0;
    cpswConfig.phy_param->speed = 0;
    cpswConfig.phy_param->duplex = 0;

    switch(switchConfig)
    {
#ifdef CPSW_SWITCH_CONFIG
        case CONFIG_SWITCH_ADD_MULTICAST:
        {
            cpswConfig.cmd = CONFIG_SWITCH_ADD_MULTICAST;
            UserEthAddrInfoGet(cpswSwitchParam.addr, ADDR_TYPE_MULTICAST);
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_mask = UserValueInfoGet(MIN_PORT_MASK,
                                                         MAX_PORT_MASK,
                                                         DEF_PORT_MASK, TRUE,
                                                         portMaskInputHelp);
            cpswSwitchParam.super = UserValueInfoGet(MIN_SUPER, MAX_SUPER,
                                                     DEF_SUPER, TRUE,
                                                     superInputHelp);
            cpswSwitchParam.fwd_state =
                                     UserValueInfoGet(MIN_MCAST_FWD_STATE,
                                                      MAX_MCAST_FWD_STATE,
                                                      DEF_MCAST_FWD_STATE, TRUE,
                                                      mcastFwdStateInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to add Multicast Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nMulticast entry added at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_ADD_UNICAST:
        {
            cpswConfig.cmd = CONFIG_SWITCH_ADD_UNICAST;
            UserEthAddrInfoGet(cpswSwitchParam.addr, ADDR_TYPE_UNICAST);
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpswSwitchParam.ucast_flags = UserValueInfoGet(MIN_UCAST_FLAGS,
                                                           MAX_UCAST_FLAGS,
                                                           DEF_UCAST_FLAGS,
                                                           TRUE,
                                                           ucastFlagsInputHelp);
            cpswSwitchParam.ucast_type = UserValueInfoGet(MIN_UCAST_FLAGS,
                                                          MAX_UCAST_FLAGS,
                                                          DEF_UCAST_FLAGS, TRUE,
                                                          ucastTypeInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to add Unicast Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nUnicast entry added at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_ADD_OUI:
        {
            cpswConfig.cmd = CONFIG_SWITCH_ADD_OUI;
            UserEthAddrInfoGet(cpswSwitchParam.addr, ADDR_TYPE_UNICAST);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to add OUI Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nOUI entry added at index  0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_FIND_ADDR:
        {
            cpswConfig.cmd = CONFIG_SWITCH_FIND_ADDR;
            UserEthAddrInfoGet(cpswSwitchParam.addr, 0);
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nNo match for given addr found !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nALE table index match for addr is 0x%x!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_DEL_MULTICAST:
        {
            cpswConfig.cmd = CONFIG_SWITCH_DEL_MULTICAST;
            UserEthAddrInfoGet(cpswSwitchParam.addr, ADDR_TYPE_MULTICAST);
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_mask = UserValueInfoGet(MIN_PORT_MASK,
                                                         MAX_PORT_MASK,
                                                         DEF_PORT_MASK, TRUE,
                                                         portMaskInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to delete Multicast Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nMulticast entry deleted at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_DEL_UNICAST:
        {
            cpswConfig.cmd = CONFIG_SWITCH_DEL_UNICAST;
            UserEthAddrInfoGet(cpswSwitchParam.addr, ADDR_TYPE_UNICAST);
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to delete Unicast Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nUnicast entry deleted at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_ADD_VLAN:
        {
            cpswConfig.cmd = CONFIG_SWITCH_ADD_VLAN;
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpswSwitchParam.reg_multi = UserValueInfoGet(MIN_VLAN_MCAST_REG,
                                                         MAX_VLAN_MCAST_REG,
                                                         DEF_VLAN_MCAST_REG,
                                                         TRUE,
                                                         vlanMcastRegInputHelp);
            cpswSwitchParam.unreg_multi = UserValueInfoGet(MIN_VLAN_MCAST_UNREG,
                                                           MAX_VLAN_MCAST_UNREG,
                                                           DEF_VLAN_MCAST_UNREG,
                                                           TRUE,
                                                       vlanMcastUnregInputHelp);
            cpswSwitchParam.untag_port = UserValueInfoGet(MIN_VLAN_UNTAG,
                                                          MAX_VLAN_UNTAG,
                                                          DEF_VLAN_UNTAG, TRUE,
                                                          vlanUntagInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to add VLAN Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nVLAN entry added at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_FIND_VLAN:
        {
            cpswConfig.cmd = CONFIG_SWITCH_FIND_VLAN;
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nNo match for given vlan is found !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nALE Table Index match for VLAN is 0x%x!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_DEL_VLAN:
        {
            cpswConfig.cmd = CONFIG_SWITCH_DEL_VLAN;
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            if((MIN_ALE_ENTRY_IDX > ret_cpsw) || (ret_cpsw > MAX_ALE_ENTRY_IDX))
                UARTprintf(DebugCom, "\r\nFailed to delete VLAN Entry !!!\r\n");
            else
            {
                UARTprintf(DebugCom, "\r\nVLAN entry deleted at index 0x%x!!!"
                                   "\r\n0x%x - ", ret_cpsw, ret_cpsw);
                cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
                cpswSwitchParam.ale_tbl_index = ret_cpsw;
                cpsw_switch_configuration(&cpswConfig);
                UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
                UARTprintf(DebugCom, "\r\n");
            }

            return TRUE;
        }

        case CONFIG_SWITCH_PORT_VLAN_CONFIG:
        {
            cpswConfig.cmd = CONFIG_SWITCH_PORT_VLAN_CONFIG;
            cpswSwitchParam.vid = UserValueInfoGet(MIN_VLANID, MAX_VLANID,
                                                   DEF_VLANID, TRUE,
                                                   vlanInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpswSwitchParam.cfi_port = UserValueInfoGet(MIN_CFI, MAX_CFI,
                                                        DEF_CFI, TRUE,
                                                        vlanPortCFIInputHelp);
            cpswSwitchParam.prio_port = UserValueInfoGet(MIN_PRI, MAX_PRI,
                                                         DEF_PRI, TRUE,
                                                         vlanPortPriInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_AGEOUT:
        {
            cpswConfig.cmd = CONFIG_SWITCH_AGEOUT;
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_DUMP:
        {
            unsigned int cnt = 0;

            cpswConfig.cmd = CONFIG_SWITCH_DUMP;
            aleEntryStartIndex = UserValueInfoGet(MIN_ALE_ENTRY_IDX_START,
                                                  MAX_ALE_ENTRY_IDX_START,
                                                  DEF_ALE_ENTRY_IDX_START, TRUE,
                                                  aleEntryStartIndexInputHelp);
            aleEntryEndIndex = UserValueInfoGet(aleEntryStartIndex,
                                                MAX_ALE_ENTRY_IDX_END,
                                                DEF_ALE_ENTRY_IDX_END, TRUE,
                                                aleEntryEndIndexInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            UARTprintf(DebugCom, "\r\n");

            for(cnt = aleEntryStartIndex; cnt <= aleEntryEndIndex; cnt++)
            {
                if(cpswConfig.buf[cnt][1] & MASK_ENTRY_TPYE_WORD)
                {
                    UARTprintf(DebugCom, "0x%x - ", cnt);
                    UserALEEntryIndexInfoPrint(cpswConfig.buf[cnt]);
                    UARTprintf(DebugCom, "\r\n");
                }
            }

            return TRUE;
        }

        case CONFIG_SWITCH_CONFIG_DUMP:
        {
            cpswConfig.cmd = CONFIG_SWITCH_CONFIG_DUMP;
            cpswSwitchParam.ale_tbl_index = UserValueInfoGet(MIN_ALE_ENTRY_IDX,
                                                             MAX_ALE_ENTRY_IDX,
                                                             DEF_ALE_ENTRY_IDX,
                                                             TRUE,
                                                        aleEntryIndexInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            ret_cpsw = cpswConfig.ret;

            UARTprintf(DebugCom, "\r\n");
            UARTprintf(DebugCom, "0x%x - ", aleEntryIndex);
            UserALEEntryIndexInfoPrint(cpswConfig.ale_entry);
            UARTprintf(DebugCom, "\r\n");

            return TRUE;
        }

        case CONFIG_SWITCH_VLANAWARE:
        {
            cpswConfig.cmd = CONFIG_SWITCH_VLANAWARE;
            cpswSwitchParam.vlan_aware = UserValueInfoGet(MIN_VLANAWARE,
                                                          MAX_VLANAWARE,
                                                          DEF_VLANAWARE, TRUE,
                                                          vlanAwareInputHelp);
            cpswSwitchParam.drop_packet = UserValueInfoGet(MIN_ALE_VLANAWARE,
                                                           MAX_ALE_VLANAWARE,
                                                           DEF_ALE_VLANAWARE,
                                                           TRUE,
                                                        aleVlanAwareInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_RATELIMIT:
        {
            cpswConfig.cmd = CONFIG_SWITCH_RATELIMIT;
            cpswSwitchParam.enable = UserValueInfoGet(MIN_ENABLE, MAX_ENABLE,
                                                      DEF_ENABLE, TRUE,
                                                      enableInputHelp);
            cpswSwitchParam.direction = UserValueInfoGet(MIN_DIRECTION,
                                                         MAX_DIRECTION,
                                                         DEF_DIRECTION, TRUE,
                                                         directionInputHelp);
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpswSwitchParam.addr_type = UserValueInfoGet(MIN_ADDR_TYPE,
                                                         MAX_ADDR_TYPE,
                                                         DEF_ADDR_TYPE, TRUE,
                                                         addrTypeInputHelp);
            cpswSwitchParam.limit = UserValueInfoGet(MIN_LIMIT, MAX_LIMIT,
                                                     DEF_LIMIT, TRUE,
                                                     limitInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_VID_INGRESS_CHECK:
        {
            cpswConfig.cmd = CONFIG_SWITCH_VID_INGRESS_CHECK;
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }


        case CONFIG_SWITCH_ADD_UNKNOWN_VLAN_INFO:
        {
            cpswConfig.cmd = CONFIG_SWITCH_ADD_UNKNOWN_VLAN_INFO;
            cpswSwitchParam.unknown_vlan = UserValueInfoGet(MIN_UNKNOWN_VLAN,
                                                            MAX_UNKNOWN_VLAN,
                                                            DEF_UNKNOWN_VLAN,
                                                            TRUE,
                                                         unknownVlanInputHelp);
            if(cpswSwitchParam.unknown_vlan == ALE_PORT_UNKNOWN_VLAN_MEMBER)
                cpswSwitchParam.port_mask = UserValueInfoGet(MIN_PORT_MASK,
                                                             MAX_PORT_MASK,
                                                             DEF_PORT_MASK,
                                                             TRUE,
                                                             portMaskInputHelp);
            if(cpswSwitchParam.unknown_vlan == ALE_PORT_UNKNOWN_REG_MCAST_FLOOD)
                cpswSwitchParam.reg_multi = UserValueInfoGet(MIN_VLAN_MCAST_REG,
                                                             MAX_VLAN_MCAST_REG,
                                                             DEF_VLAN_MCAST_REG,
                                                             TRUE,
                                                         vlanMcastRegInputHelp);
            if(cpswSwitchParam.unknown_vlan ==
                                             ALE_PORT_UNKNOWN_UNREG_MCAST_FLOOD)
                cpswSwitchParam.unreg_multi =
                                    UserValueInfoGet(MIN_VLAN_MCAST_UNREG,
                                                     MAX_VLAN_MCAST_UNREG,
                                                     DEF_VLAN_MCAST_UNREG, TRUE,
                                                     vlanMcastUnregInputHelp);
            if(cpswSwitchParam.unknown_vlan == ALE_PORT_UNTAGGED_EGRESS)
                cpswSwitchParam.untag_port = UserValueInfoGet(MIN_VLAN_UNTAG,
                                                              MAX_VLAN_UNTAG,
                                                              DEF_VLAN_UNTAG,
                                                              TRUE,
                                                           vlanUntagInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_MACAUTH:
        {
            cpswConfig.cmd = CONFIG_SWITCH_MACAUTH;
            cpswSwitchParam.mac_auth = UserValueInfoGet(MIN_MAC_AUTH,
                                                        MAX_MAC_AUTH,
                                                        DEF_MAC_AUTH, TRUE,
                                                        macAuthInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        case CONFIG_SWITCH_PORT_STATE:
        {
            cpswConfig.cmd = CONFIG_SWITCH_PORT_STATE;
            cpswSwitchParam.port_num = UserValueInfoGet(MIN_PORT, MAX_PORT,
                                                        DEF_PORT, TRUE,
                                                        portInputHelp);
            cpswSwitchParam.port_state = UserValueInfoGet(MIN_PORT_STATE,
                                                          MAX_PORT_STATE,
                                                          DEF_PORT_STATE, TRUE,
                                                          portStateInputHelp);
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }
#endif

        case CONFIG_SWITCH_SET_PORT_CONFIG:
        {
            cpswConfig.cmd = CONFIG_SWITCH_SET_PORT_CONFIG;
            cpswPhyParam.slv_port_num = UserValueInfoGet(MIN_SLV_PORT,
                                                         MAX_SLV_PORT,
                                                         DEF_SLV_PORT, TRUE,
                                                         slavePortInputHelp);
            cpswPhyParam.autoneg = UserValueInfoGet(MIN_AUTONEG,
                                                    MAX_AUTONEG,
                                                    DEF_AUTONEG, TRUE,
                                                    autonegInputHelp);
            if(TRUE == cpswPhyParam.autoneg)
            {
                UARTprintf(DebugCom, "%s", configSpeedDuplexInputHelp1);
                UARTprintf(DebugCom, "%s", configSpeedDuplexInputHelp2);
                cpswPhyParam.config = UserValueInfoGet(MIN_PHY_CONFIG,
                                                       MAX_PHY_CONFIG,
                                                       DEF_PHY_CONFIG, TRUE,
                                                   configSpeedDuplexInputHelp3);
            }
            else
            {
                cpswPhyParam.speed = UserValueInfoGet(MIN_SPEED,
                                                      MAX_SPEED,
                                                      DEF_SPEED, TRUE,
                                                      speedInputHelp);
                cpswPhyParam.duplex = UserValueInfoGet(MIN_DUPLEX,
                                                       MAX_DUPLEX,
                                                       DEF_DUPLEX, TRUE,
                                                       duplexInputHelp);
            }
            cpsw_switch_configuration(&cpswConfig);
            return TRUE;
        }

        default:
            break;
    }

    return FALSE;
}

/*
** Displays the IP addrss on the UART Console
*/
static void IpAddrDisplay(unsigned int ipAddr)
{
    unsigned char byte;
    int cnt;

    for(cnt = 0; cnt <= LEN_IP_ADDR - 1; cnt++)
    {
        byte = (ipAddr >> (cnt * 8)) & 0xFF;

        if(cnt)
        {
            UARTPuts(DebugCom, ".", -1);
        }

        UARTPutNum(DebugCom, (int)byte);
    }
}

/*
** Interrupt Handler for Core 0 Receive interrupt
*/
void CPSWCore0RxIsr(void)
{
	//CPUirqd();
	lwIPRxIntHandler(0);
	//CPUirqe();
}

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
void CPSWCore0TxIsr(void)
{
	//CPUirqd();
    lwIPTxIntHandler(0);
    //CPUirqe();
}


/**
 * \brief   This function returns the MAC address for the EVM
 *
 * \param   addrIdx    the MAC address index.
 * \param   macAddr    the Pointer where the MAC address shall be stored
 *     'addrIdx' can be either 0 or 1
 *
 * \return  None.
 */
void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr)
{
    macAddr[0] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx))
                   >> 8) & 0xFF;
    macAddr[1] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx)))
                  & 0xFF;
    macAddr[2] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 24) & 0xFF;
    macAddr[3] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 16) & 0xFF;
    macAddr[4] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 8) & 0xFF;
    macAddr[5] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx)))
                  & 0xFF;
}


void lan_interface_init(void)
{
	UARTPuts(DebugCom, "Initialize the LWIP library.\n\r" , -1);
    unsigned int ipAddr;
#ifdef CPSW_DUAL_MAC_MODE
    LWIP_IF lwipIfPort1, lwipIfPort2;
#else
    LWIP_IF lwipIfPort;
#endif
    CPSWPinMuxSetup(1);
    CPSWClkEnable();
    /* Chip configuration RGMII selection */
    EVMPortMIIModeSelect();
    AintcCPSWIntrSetUp();


    //#define STATIC_IP_ADDRESS_PORT1 192<<24 | 168<<16 | 0<<8 | 10
    //#define STATIC_IP_ADDRESS_PORT1  0


#ifdef CPSW_DUAL_MAC_MODE
    /* Get the MAC address */
    EVMMACAddrGet(0, lwipIfPort1.macArray);
    EVMMACAddrGet(1, lwipIfPort2.macArray);

    UARTprintf(DebugCom, "Acquiring IP Address for Port 1... \n\r" );

#if STATIC_IP_ADDRESS_PORT1

    lwipIfPort1.instNum = 0;
    lwipIfPort1.slvPortNum = 1;
    lwipIfPort1.ipAddr = STATIC_IP_ADDRESS_PORT1;
    lwipIfPort1.netMask = 0;
    lwipIfPort1.gwAddr = 0;
    lwipIfPort1.ipMode = IPADDR_USE_STATIC;

    ipAddr = lwIPInit(&lwipIfPort1);

#else

    lwipIfPort1.instNum = 0;
    lwipIfPort1.slvPortNum = 1;
    lwipIfPort1.ipAddr = 0;
    lwipIfPort1.netMask = 0;
    lwipIfPort1.gwAddr = 0;
    lwipIfPort1.ipMode = IPADDR_USE_DHCP;

    ipAddr = lwIPInit(&lwipIfPort1);

#endif
    if(ipAddr)
    {
        UARTprintf(DebugCom, "\n\r\n\rPort 1 IP Address Assigned: ");
        IpAddrDisplay(ipAddr);
    }
    else
    {
        UARTprintf(DebugCom, "\n\r\n\rPort 1 IP Address Acquisition Failed.");
    }

    UARTprintf(DebugCom, "\n\r\n\rAcquiring IP Address for Port 2... \n\r" );

#if STATIC_IP_ADDRESS_PORT2

    lwipIfPort2.instNum = 0;
    lwipIfPort2.slvPortNum = 2;
    lwipIfPort2.ipAddr = STATIC_IP_ADDRESS_PORT2;
    lwipIfPort2.netMask = 0;
    lwipIfPort2.gwAddr = 0;
    lwipIfPort2.ipMode = IPADDR_USE_STATIC;

    ipAddr = lwIPInit(&lwipIfPort2);

#else

    lwipIfPort2.instNum = 0;
    lwipIfPort2.slvPortNum = 2;
    lwipIfPort2.ipAddr = 0;
    lwipIfPort2.netMask = 0;
    lwipIfPort2.gwAddr = 0;
    lwipIfPort2.ipMode = IPADDR_USE_DHCP;

    ipAddr = lwIPInit(&lwipIfPort2);

#endif
    if(ipAddr)
    {
        UARTprintf(DebugCom, "\n\r\n\rPort 2 IP Address Assigned: ");
        IpAddrDisplay(ipAddr);
    }
    else
    {
        UARTprintf(DebugCom, "\n\r\n\rPort 2 IP Address Acquisition Failed.");
    }

#else /* CPSW_DUAL_MAC_MODE */


    /* Get the MAC address */
    EVMMACAddrGet(0, lwipIfPort.macArray);

    UARTprintf(DebugCom, "Acquiring IP Address... \n\r" );

#if STATIC_IP_ADDRESS

    lwipIfPort.instNum = 0;
    lwipIfPort.ipAddr = STATIC_IP_ADDRESS;
    lwipIfPort.netMask = 0;
    lwipIfPort.gwAddr = 0;
    lwipIfPort.ipMode = IPADDR_USE_STATIC;

    ipAddr = lwIPInit(&lwipIfPort);

#else

    lwipIfPort.instNum = 0;
    lwipIfPort.ipAddr = 0;
    lwipIfPort.netMask = 0;
    lwipIfPort.gwAddr = 0;
    lwipIfPort.ipMode = IPADDR_USE_DHCP;

    ipAddr = lwIPInit(&lwipIfPort);

#endif
    if(ipAddr)
    {
        UARTprintf(DebugCom, "\n\r\n\rEVM IP Address Assigned: ");
        IpAddrDisplay(ipAddr);
    }
    else
    {
        UARTprintf(DebugCom, "\n\r\n\rEVM IP Address Acquisition Failed.");
    }

#endif

    /* Initialize the sample httpd server. */
    httpd_init();

    UARTPuts(DebugCom, "StarterWare Ethernet Application. Access the embedded"
             " web page using http://<ip address assigned>/index.html"
             " via a web browser. \n\r\n\r" , -1);



    cpswConfig.phy_param = &cpswPhyParam;
#ifdef CPSW_SWITCH_CONFIG
    cpswConfig.switch_param = &cpswSwitchParam;
#endif
    unsigned int switchConfig = 0;
    unsigned char switchConfigInputHelp[] = "):  ";


    UARTprintf(DebugCom, "\n\r\n\r === CPSW Configurations === ");
#ifdef CPSW_SWITCH_CONFIG
    UARTprintf(DebugCom, "\n\r\n\r === Available Switch Configurations === ");
    UARTprintf(DebugCom, "\n\r\n\r 1 - Add a multicast entry ");
    UARTprintf(DebugCom, "\n\r\n\r 2 - Add a unicast entry ");
    UARTprintf(DebugCom, "\n\r\n\r 3 - Add a OUI entry ");
    UARTprintf(DebugCom, "\n\r\n\r 4 - Search address in entry list ");
    UARTprintf(DebugCom, "\n\r\n\r 5 - Delete a multicast entry ");
    UARTprintf(DebugCom, "\n\r\n\r 6 - Delete a unicast entry ");
    UARTprintf(DebugCom, "\n\r\n\r 7 - Adds a vlan entry ");
    UARTprintf(DebugCom, "\n\r\n\r 8 - Search vlan in entry list ");
    UARTprintf(DebugCom, "\n\r\n\r 9 - Delete vlan ");
    UARTprintf(DebugCom, "\n\r\n\r10 - Configure Port Vlan (ID, CFI, PRI) ");
    UARTprintf(DebugCom, "\n\r\n\r11 - Age Out Untouch entries of ALE Table ");
    UARTprintf(DebugCom, "\n\r\n\r12 - Print valid ALE Entries in the range ");
    UARTprintf(DebugCom, "\n\r\n\r13 - Print an ALE Entry ");
    UARTprintf(DebugCom, "\n\r\n\r14 - ALE VLAN Aware Config ");
    UARTprintf(DebugCom, "\n\r\n\r15 - Configure Rate Limit for TX or RX ");
    UARTprintf(DebugCom, "\n\r\n\r16 - Enable Engress Check ");
    UARTprintf(DebugCom, "\n\r\n\r17 - Set port unknown VLAN info ");
    UARTprintf(DebugCom, "\n\r\n\r18 - Enable MAC Auth ");
    UARTprintf(DebugCom, "\n\r\n\r19 - Configure Port State ");
    UARTprintf(DebugCom, "\n\r\n\r === Available PHY Configurations ===");
    UARTprintf(DebugCom, "\n\r\n\r20 - Configure Phy of a Port ");
    UARTprintf(DebugCom, "\n\r\n\r21 - Exit ");
#else
    UARTprintf(DebugCom, "\n\r\n\r === Available PHY Configurations ===");
    UARTprintf(DebugCom, "\n\r\n\r1 - Configure Phy of a Port ");
    UARTprintf(DebugCom, "\n\r\n\r2 - Exit ");
#endif
}
#endif

