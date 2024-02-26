/********************************** (C) COPYRIGHT  *******************************
 * File Name          : app_mtp_ptp.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/09/01
 * Description        :
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


/********************************************************************************/
/* Header File */
#include "usb_host_config.h"

/*******************************************************************************/
/* Variable Definition */
uint8_t  DevDesc_Buf[ 18 ];                                                     // Device Descriptor Buffer
__attribute__ ((aligned(4))) uint8_t  Com_Buf[ DEF_COM_BUF_LEN ];               // General Buffer
ROOT_HUB_DEVICE RootHubDev;
HOST_CTL HostCtl[ DEF_TOTAL_ROOT_HUB * DEF_ONE_USB_SUP_DEV_TOTAL ];

uint8_t  *pMTP_Data_Buf;
PTP_ContainerTypeDef PTP_Container;
PTP_OpContainerTypeDef PTP_OpContainer;
PTP_RespContainerTypeDef PTP_RespContainer;
PTP_DataContainerTypeDef PTP_DataContainer;
PTP_HandleTypeDef PTP_Handle;

PTP_DeviceInfoTypeDef PTP_DeviceInfo;
PTP_StorageIDsTypeDef PTP_StorageIDs;
PTP_StorageInfoTypeDef PTP_StorageInfo;
PTP_ObjectInfoTypeDef PTP_ObjectInfo;
MTP_ParaTypeDef MTP_Para;
PTP_PropertyValueTypeDef PTP_PropertyValue;

/*********************************************************************
 * @fn      USBH_AnalyseType
 *
 * @brief   Simply analyze USB device type.
 *
* @para     pdev_buf: Device descriptor buffer
 *          pcfg_buf: Configuration descriptor buffer
 *          ptype: Device type.
 *
 * @return  none
 */
void USBH_AnalyseType( uint8_t *pdev_buf, uint8_t *pcfg_buf, uint8_t *ptype )
{
    uint8_t  dv_cls, if_cls;

    dv_cls = ( (PUSB_DEV_DESCR)pdev_buf )->bDeviceClass;
    if_cls = ( (PUSB_CFG_DESCR_LONG)pcfg_buf )->itf_descr.bInterfaceClass;
    if( ( dv_cls == USB_DEV_CLASS_HID ) || ( if_cls == USB_DEV_CLASS_HID ) )
    {
        *ptype = USB_DEV_CLASS_HID;
    }
    else if( ( dv_cls == USB_DEV_CLASS_IMAGE ) || ( if_cls == USB_DEV_CLASS_IMAGE ) )
    {
        *ptype = USB_DEV_CLASS_IMAGE;
    }
    else if( ( dv_cls == USB_DEV_CLASS_PRINTER ) || ( if_cls == USB_DEV_CLASS_PRINTER ) )
    {
        *ptype = USB_DEV_CLASS_PRINTER;
    }
    else if( ( dv_cls == USB_DEV_CLASS_STORAGE ) || ( if_cls == USB_DEV_CLASS_STORAGE ) )
    {
        *ptype = USB_DEV_CLASS_STORAGE;
    }
    else if( ( dv_cls == USB_DEV_CLASS_HUB ) || ( if_cls == USB_DEV_CLASS_HUB ) )
    {
        *ptype = USB_DEV_CLASS_HUB;
    }
    else
    {
        *ptype = DEF_DEV_TYPE_UNKNOWN;
    }
}

/*********************************************************************
 * @fn      USBH_EnumRootDevice
 *
 * @brief   Generally enumerate a device connected to host port.
 *
 * @para    index: USB host port
 *
 * @return  Enumeration result
 */
uint8_t USBH_EnumRootDevice( void )
{
    uint8_t  s;
    uint8_t  enum_cnt;
    uint8_t  cfg_val;
    uint16_t i;
    uint16_t len;

    DUG_PRINTF( "Enum:\r\n" );

    enum_cnt = 0;
ENUM_START:
    /* Delay and wait for the device to stabilize */
    Delay_Ms( 100 );
    enum_cnt++;
    Delay_Ms( 8 << enum_cnt );

    /* Reset the USB device and wait for the USB device to reconnect */
    USBFSH_ResetRootHubPort( 0 );
    for( i = 0, s = 0; i < DEF_RE_ATTACH_TIMEOUT; i++ )
    {
        if( USBFSH_EnableRootHubPort( &RootHubDev.bSpeed ) == ERR_SUCCESS )
        {
            i = 0;
            s++;
            if( s > 6 )
            {
                break;
            }
        }
        Delay_Ms( 1 );
    }
    if( i )
    {
        /* Determine whether the maximum number of retries has been reached, and retry if not reached */
        if( enum_cnt <= 5 )
        {
            goto ENUM_START;
        }
        return ERR_USB_DISCON;
    }

    /* Get USB device device descriptor */
    DUG_PRINTF("Get DevDesc: ");
    s = USBFSH_GetDeviceDescr( &RootHubDev.bEp0MaxPks, DevDesc_Buf );
    if( s == ERR_SUCCESS )
    {
        /* Print USB device device descriptor */
#if DEF_DEBUG_PRINTF
        for( i = 0; i < 18; i++ )
        {
            DUG_PRINTF( "%02x ", DevDesc_Buf[ i ] );
        }
        DUG_PRINTF("\r\n");
#endif
    }
    else
    {
        /* Determine whether the maximum number of retries has been reached, and retry if not reached */
        DUG_PRINTF( "Err(%02x)\r\n", s );
        if( enum_cnt <= 5 )
        {
            goto ENUM_START;
        }
        return DEF_DEV_DESCR_GETFAIL;
    }

    /* Set the USB device address */
    DUG_PRINTF("Set DevAddr: ");
    s = USBFSH_SetUsbAddress( RootHubDev.bEp0MaxPks, USB_DEVICE_ADDR );
    if( s == ERR_SUCCESS )
    {
        DUG_PRINTF( "OK\r\n" );
    }
    else
    {
        /* Determine whether the maximum number of retries has been reached, and retry if not reached */
        DUG_PRINTF( "Err(%02x)\r\n", s );
        if( enum_cnt <= 5 )
        {
            goto ENUM_START;
        }
        return DEF_DEV_ADDR_SETFAIL;
    }
    Delay_Ms( 5 );

    /* Get the USB device configuration descriptor */
    DUG_PRINTF("Get CfgDesc: ");
    s = USBFSH_GetConfigDescr( RootHubDev.bEp0MaxPks, Com_Buf, DEF_COM_BUF_LEN, &len );
    if( s == ERR_SUCCESS )
    {
        cfg_val = ( (PUSB_CFG_DESCR)Com_Buf )->bConfigurationValue;

        /* Print USB device configuration descriptor  */
#if DEF_DEBUG_PRINTF
        for( i = 0; i < len; i++ )
        {
            DUG_PRINTF( "%02x ", Com_Buf[ i ] );
        }
        DUG_PRINTF("\r\n");
#endif

        /* Simply analyze USB device type  */
        USBH_AnalyseType( DevDesc_Buf, Com_Buf, &RootHubDev.bType );
        DUG_PRINTF( "DevType: %02x\r\n", RootHubDev.bType );
    }
    else
    {
        /* Determine whether the maximum number of retries has been reached, and retry if not reached */
        DUG_PRINTF( "Err(%02x)\r\n", s );
        if( enum_cnt <= 5 )
        {
            goto ENUM_START;
        }
        return DEF_CFG_DESCR_GETFAIL;
    }

    /* Set USB device configuration value */
    DUG_PRINTF("Set Cfg: ");
    s = USBFSH_SetUsbConfig( RootHubDev.bEp0MaxPks, cfg_val );
    if( s == ERR_SUCCESS )
    {
        DUG_PRINTF( "OK\r\n" );
    }
    else
    {
        /* Determine whether the maximum number of retries has been reached, and retry if not reached */
        DUG_PRINTF( "Err(%02x)\r\n", s );
        if( enum_cnt <= 5 )
        {
            goto ENUM_START;
        }
        return ERR_USB_UNSUPPORT;
    }

    return ERR_SUCCESS;
}

/*********************************************************************
 * @fn      USBH_AnalyzeCfgDesc
 *
 * @brief   Generally analyze USB device configuration descriptor.
 *
 * @para    index: USB device number.
 *
 * @return  Result
 */
void IMAGE_AnalyzeCfgDesc( uint8_t index )
{
    uint8_t  intfacenum, innum, outnum;
    uint16_t i, j;
    uint16_t total_len;

    /* Save the number of interface of the USB device, only up to 4 */
    if( ( (PUSB_CFG_DESCR)Com_Buf )->bNumInterfaces > DEF_INTERFACE_NUM_MAX )
    {
        HostCtl[ index ].InterfaceNum = DEF_INTERFACE_NUM_MAX;
    }
    else
    {
        HostCtl[ index ].InterfaceNum = ( (PUSB_CFG_DESCR)Com_Buf )->bNumInterfaces;
    }

    /* Save device endpoint number */
    total_len = Com_Buf[ 2 ] | ((uint16_t)Com_Buf[ 3 ] << 8 );
    intfacenum = 0;
    for( i = 0; i < total_len; i++ )
    {
        if( ( Com_Buf[ i + 0 ] == 0x09 ) &&
            ( Com_Buf[ i + 1 ] == 0x04 ) &&
            ( Com_Buf[ i + 2 ] == intfacenum ) )
        {
            /* Analyze the current interface */
            innum = 0;
            outnum = 0;
            i += 9;
            for( j = 0; j < total_len - i; j++ )
            {
                if( ( Com_Buf[ i + j + 0 ] == 0x07 ) && ( Com_Buf[ i + j + 1 ] == 0x05 ) )
                {
                    if( ( Com_Buf[ i + j + 2 ] & 0x80 ) == 0x00 )
                    {
                        /* OUT */
                        if( outnum < 4 )
                        {
                            HostCtl[ index ].Interface[ intfacenum ].OutEndpAddr[ outnum ] = Com_Buf[ i + j + 2 ];
                            HostCtl[ index ].Interface[ intfacenum ].OutEndpType[ outnum ] = Com_Buf[ i + j + 3 ];
                            HostCtl[ index ].Interface[ intfacenum ].OutEndpSize[ outnum ] = Com_Buf[ i + j + 4 ] | ( (uint16_t)Com_Buf[ i + j + 5 ] << 8 );
                            HostCtl[ index ].Interface[ intfacenum ].OutEndpTog[ outnum ] = 0x00;
                            outnum++;
                            HostCtl[ index ].Interface[ intfacenum ].OutEndpNum = outnum;
                        }
                    }
                    else
                    {
                        /* IN */
                        if( Com_Buf[ i + j + 3 ] == 0x02 )
                        {
                            if( innum < 4 )
                            {
                                HostCtl[ index ].Interface[ intfacenum ].InEndpAddr[ innum ] = Com_Buf[ i + j + 2 ];
                                HostCtl[ index ].Interface[ intfacenum ].InEndpType[ innum ] = Com_Buf[ i + j + 3 ];
                                HostCtl[ index ].Interface[ intfacenum ].InEndpSize[ innum ] = Com_Buf[ i + j + 4 ] | ( (uint16_t)Com_Buf[ i + j + 5 ] << 8 );
                                HostCtl[ index ].Interface[ intfacenum ].InEndpInterval[ innum ] = Com_Buf[ i + j + 6 ];
                                HostCtl[ index ].Interface[ intfacenum ].InEndpTog[ innum ] = 0x00;
                                HostCtl[ index ].Interface[ intfacenum ].InEndpTimeCount[ innum ] = 0x00;
                                innum++;
                                HostCtl[ index ].Interface[ intfacenum ].InEndpNum = innum;
                            }
                        }
                    }
                }
                else if( ( Com_Buf[ i + j + 0 ] == 0x09 ) &&
                         ( Com_Buf[ i + j + 1 ] == 0x04 ) )
                {
                    intfacenum++;
                    break;
                }
            }
            if( intfacenum >= DEF_INTERFACE_NUM_MAX )
            {
                break;
            }
        }
    }
}

/*********************************************************************
 * @fn      USBH_EnumHidDevice
 *
 * @brief   Enumerate HID device.
 *
 * @para    index: USB host port
 *
 * @return  The result of the enumeration.
 */
uint8_t USBH_EnumImageDevice( uint8_t index )
{
    uint8_t  s;
#if DEF_DEBUG_PRINTF
    uint8_t  i;
#endif

    DUG_PRINTF( "Enum Image:\r\n" );

    /* Analyze image class device configuration descriptor and save relevant parameters */
    DUG_PRINTF("Analyze CfgDesc: ");
    IMAGE_AnalyzeCfgDesc( index );

    /* Get the string descriptor contained in the configuration descriptor if it exists */
    if( Com_Buf[ 6 ] )
    {
        DUG_PRINTF("Get StringDesc4: ");
        s = USBFSH_GetStrDescr( RootHubDev.bEp0MaxPks, Com_Buf[ 6 ], Com_Buf );
        if( s == ERR_SUCCESS )
        {
            /* Print the string descriptor contained in the configuration descriptor */
#if DEF_DEBUG_PRINTF
            for( i = 0; i < Com_Buf[ 0 ]; i++ )
            {
                DUG_PRINTF( "%02x ", Com_Buf[ i ] );
            }
            DUG_PRINTF("\r\n");
#endif
        }
        else
        {
            DUG_PRINTF( "Err(%02x)\r\n", s );
        }
    }

    /* Get USB vendor string descriptor  */
    if( DevDesc_Buf[ 14 ] )
    {
        DUG_PRINTF("Get StringDesc1: ");
        s = USBFSH_GetStrDescr( RootHubDev.bEp0MaxPks, DevDesc_Buf[ 14 ], Com_Buf );
        if( s == ERR_SUCCESS )
        {
            /* Print USB vendor string descriptor */
#if DEF_DEBUG_PRINTF
            for( i = 0; i < Com_Buf[ 0 ]; i++ )
            {
                DUG_PRINTF( "%02x ", Com_Buf[ i ]);
            }
            DUG_PRINTF("\r\n");
#endif
        }
        else
        {
            DUG_PRINTF( "Err(%02x)\r\n", s );
        }
    }

    /* Get USB product string descriptor */
    if( DevDesc_Buf[ 15 ] )
    {
        DUG_PRINTF("Get StringDesc2: ");
        s = USBFSH_GetStrDescr(  RootHubDev.bEp0MaxPks, DevDesc_Buf[ 15 ], Com_Buf );
        if( s == ERR_SUCCESS )
        {
            /* Print USB product string descriptor */
#if DEF_DEBUG_PRINTF
            for( i = 0; i < Com_Buf[ 0 ]; i++ )
            {
                DUG_PRINTF( "%02x ", Com_Buf[ i ] );
            }
            DUG_PRINTF("\r\n");
#endif
        }
        else
        {
            DUG_PRINTF( "Err(%02x)\r\n", s );
        }
    }

    /* Get USB serial number string descriptor */
    if( DevDesc_Buf[ 16 ] )
    {
        DUG_PRINTF("Get StringDesc3: ");
        s = USBFSH_GetStrDescr( RootHubDev.bEp0MaxPks, DevDesc_Buf[ 16 ], Com_Buf );
        if( s == ERR_SUCCESS )
        {
            /* Print USB serial number string descriptor */
#if DEF_DEBUG_PRINTF
            for( i = 0; i < Com_Buf[ 0 ]; i++ )
            {
                DUG_PRINTF( "%02x ", Com_Buf[ i ] );
            }
            DUG_PRINTF("\r\n");
#endif
        }
        else
        {
            DUG_PRINTF( "Err(%02x)\r\n", s );
        }
    }

    return ERR_SUCCESS;
}

/*********************************************************************
 * @fn      MTP_SendData
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t MTP_SendData( uint8_t *pbuf, uint32_t len )
{
    uint8_t  s;
    uint8_t  index;

    index = RootHubDev.DeviceIndex;
    while( len )
    {
        if( len > HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ] )
        {
            USBFSH->HOST_TX_LEN = HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ];
            memcpy( USBFS_TX_Buf, pbuf, HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ] );
        }
        else
        {
            USBFSH->HOST_TX_LEN = len;
            memcpy( USBFS_TX_Buf, pbuf, len );
        }

        s = USBFSH_Transact( ( USB_PID_OUT << 4 ) | HostCtl[ index ].Interface[ 0 ].OutEndpAddr[ 0 ],
                             HostCtl[ index ].Interface[ 0 ].OutEndpTog[ 0 ], 20000 );
        if( s == ERR_SUCCESS )
        {
            HostCtl[ index ].Interface[ 0 ].OutEndpTog[ 0 ] ^= USBFS_UH_T_TOG;

            if( len > HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ] )
            {
                pbuf += HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ];
                len -= HostCtl[ index ].Interface[ 0 ].OutEndpSize[ 0 ];
            }
            else
            {
                break;
            }
        }
        else
        {
            return s;
        }
    }

    return ERR_SUCCESS;
}

/*********************************************************************
 * @fn      MTP_RecvData
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t MTP_RecvData( uint8_t *pbuf, uint32_t *plen )
{
    uint8_t  s = ERR_SUCCESS;
    uint8_t  index;

    index = RootHubDev.DeviceIndex;

    s = USBFSH_Transact( ( USB_PID_IN << 4 ) | HostCtl[ index ].Interface[ 0 ].InEndpAddr[ 0 ],
                         HostCtl[ index ].Interface[ 0 ].InEndpTog[ 0 ], 20000 );
    if( s == ERR_SUCCESS )
    {
        HostCtl[ index ].Interface[ 0 ].InEndpTog[ 0 ] ^= USBFS_UH_R_TOG;

        *plen = USBFSH->RX_LEN;
        memcpy( pbuf, USBFS_RX_Buf, *plen );
    }
    
    return s;
}

/*********************************************************************
 * @fn      MTP_PTP_ParaInit
 *
 * @brief
 *
 * @return  None
 */
void MTP_PTP_ParaInit( void )
{
    pMTP_Data_Buf = Com_Buf;
    PTP_Handle.data_ptr = pMTP_Data_Buf;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_packet = HostCtl[ RootHubDev.DeviceIndex ].Interface[ 0 ].InEndpSize[ 0 ];
}

/*********************************************************************
 * @fn      USBH_PTP_SendRequest
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_SendRequest( void )
{
    PTP_OpContainer.length = PTP_USB_BULK_REQ_LEN - ( ( sizeof( uint32_t ) * ( 5 - (uint32_t)PTP_Container.Nparam ) ) );
    if( PTP_Container.Transaction_ID == 0XFFFFFFFF )
    {
        PTP_Container.Transaction_ID = 0x00000001;
    }
    PTP_OpContainer.type = PTP_USB_CONTAINER_COMMAND;
    PTP_OpContainer.code = PTP_Container.Code;
    PTP_OpContainer.trans_id = PTP_Container.Transaction_ID;
    PTP_OpContainer.param1 = PTP_Container.Param1;
    PTP_OpContainer.param2 = PTP_Container.Param2;
    PTP_OpContainer.param3 = PTP_Container.Param3;
    PTP_OpContainer.param4 = PTP_Container.Param4;
    PTP_OpContainer.param5 = PTP_Container.Param5;
    return MTP_SendData( (uint8_t *)&PTP_OpContainer, PTP_OpContainer.length );
}

/*********************************************************************
 * @fn      USBH_PTP_GetResponse
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetResponse( void )
{
    uint8_t  s;
    uint32_t len;

    s = MTP_RecvData( (uint8_t *)&PTP_RespContainer, &len );
    if( s == ERR_SUCCESS )
    {
        if( PTP_RespContainer.code == PTP_RC_OK )
        {
            s = PTP_OK;
        }
        else
        {
            s = PTP_FAIL;
        }
    }

    return s;
}

/*********************************************************************
 * @fn      USBH_PTP_OpenSession
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_OpenSession( uint32_t session )
{
    uint8_t  s;

    PTP_Container.Transaction_ID = 0; // This command is the first command, so the transfer ID is 0.
    PTP_Container.Param1 = session;
    PTP_Container.Code = PTP_OC_OpenSession;
    PTP_Container.Nparam = 1;

    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }
    PTP_Container.Transaction_ID++; // The transfer ID is incremented after the transfer is complete.
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      USBH_PTP_GetDeviceInfo
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetDeviceInfo( void )
{
    uint8_t  s, flag;
    uint32_t len, temp;

    /* Command Phase */
    PTP_Container.Code = PTP_OC_GetDeviceInfo;
    PTP_Container.Nparam = 0;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }
    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;

    /* Data Phase */
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 ) // First Packet of Data
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;

            DUG_PRINTF( "temp: %04x\r\n", temp );
        }
        if( ( len < PTP_Handle.data_packet ) || ( temp == len ) ) // End of File
        {
            PTP_Handle.data_length += len;
            break;
        }
        PTP_Handle.data_ptr += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }
    PTP_Handle.data_ptr = pMTP_Data_Buf + 12;

    /* Data Processing  */
    PTP_DeviceInfo.StandardVersion = LE16( PTP_Handle.data_ptr + PTP_di_StandardVersion );
    PTP_DeviceInfo.VendorExtensionID = LE32( PTP_Handle.data_ptr + PTP_di_VendorExtensionID );
    DUG_PRINTF( "StandardVersion: %02x, VendorExtensionID: %2x\r\n", PTP_DeviceInfo.StandardVersion, PTP_DeviceInfo.VendorExtensionID );

    /* Status Phase */
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      USB_PTP_GetStorageIds
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetStorageIds( void )
{
    uint8_t  s, i, flag;
    uint32_t len, temp;

    /* Command Phase */
    PTP_Container.Code = PTP_OC_GetStorageIDs;
    PTP_Container.Nparam = 0;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }
    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;

    /* Data Phase */
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 ) // First packet of data
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;

            DUG_PRINTF( "temp: %02x\r\n", temp );
        }
        if( ( len < PTP_Handle.data_packet ) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            break;
        }
        PTP_Handle.data_ptr += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }

    PTP_Handle.data_ptr = pMTP_Data_Buf + 12;
    PTP_StorageIDs.n = LE32( PTP_Handle.data_ptr );
    PTP_Handle.data_ptr += 4;
    DUG_PRINTF( "PTP_StorageIDs[ usb_port ].n: %02x\r\n", PTP_StorageIDs.n );

    for( i = 0; i != PTP_StorageIDs.n; i++ )
    {
        PTP_StorageIDs.Storage[ i ] = LE32( PTP_Handle.data_ptr + i * 4 );
    }

    /* Status Phase */
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      PTP_GetString
 *
 * @brief
 *
 * @para
 *
 * @return
 */
void PTP_GetString( uint8_t *str, uint8_t *data, uint32_t *len )
{
    uint32_t i, strlength;

    *len = data[ 0 ];
    strlength =  2 * ( *len );
    data++; /* Adjust the offset ignoring the String Len */

    for( i = 0; i < strlength; i++ )
    {
        /* Copy Only the string and ignore the UNICODE ID, hence add the src */
        *str = data[ i ];
        str++;
    }
    *str = 0; /* mark end of string */
}

/*********************************************************************
 * @fn      USBH_PTP_GetDeviceInfo
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetStorageInfo( uint32_t storage_id )
{
    uint8_t  s, flag;
    uint32_t len, temp;

    /* Command Phase */
    PTP_Container.Code = PTP_OC_GetStorageInfo;
    PTP_Container.Param1 = storage_id; // dev_id.Storage[0];
    PTP_Container.Nparam = 0x01;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }
    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;

    /* Data Phase */
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 ) // First packet of data
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;
        }
        if( ( len < PTP_Handle.data_packet ) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            break;
        }
        PTP_Handle.data_ptr += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }
    PTP_Handle.data_ptr = pMTP_Data_Buf + 12;
    PTP_StorageInfo.StorageType = LE16( PTP_Handle.data_ptr + PTP_si_StorageType );
    PTP_StorageInfo.FilesystemType = LE16( PTP_Handle.data_ptr + PTP_si_FilesystemType );
    PTP_StorageInfo.AccessCapability = LE16( PTP_Handle.data_ptr + PTP_si_AccessCapability );
    PTP_StorageInfo.MaxCapability = LE64( PTP_Handle.data_ptr + PTP_si_MaxCapability );
    PTP_StorageInfo.FreeSpaceInBytes = LE64( PTP_Handle.data_ptr + PTP_si_FreeSpaceInBytes );
    PTP_StorageInfo.FreeSpaceInImages = LE32( PTP_Handle.data_ptr + PTP_si_FreeSpaceInImages );
    PTP_GetString( PTP_StorageInfo.StorageDescription, (uint8_t *)( &PTP_Handle.data_ptr + PTP_si_StorageDescription ), &len );
    PTP_GetString( PTP_StorageInfo.VolumeLabel, (uint8_t *)( &PTP_Handle.data_ptr + ( PTP_si_StorageDescription + ( len * 2 ) + 1 ) ), &len );

    /* Status Phase */
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      USBH_PTP_GetNumObjects
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetNumObjects( uint32_t storage_id, uint32_t objectformatcode, uint32_t objecthandle, uint32_t *numobs )
{
    uint8_t  s;

    PTP_Container.Code = PTP_OC_GetNumObjects;
    PTP_Container.Param1 = storage_id;
    PTP_Container.Param2 = objectformatcode;
    PTP_Container.Param3 = objecthandle;
    PTP_Container.Nparam = 3;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }

    PTP_Container.Transaction_ID++; // Increment the transfer ID after the transfer is complete
    s = USBH_PTP_GetResponse( );
    if( s == PTP_OK )
    {
        *numobs = PTP_RespContainer.param1; // The number of this file type
    }

    return s;
}

/*********************************************************************
 * @fn      USBH_PTP_GetNumObjects
 *
 * @brief
 *
 * @para
 *
 * @return
 */
void ObjectHandles_Process( uint8_t *pdata, uint32_t len )
{
    uint32_t i;

    if( MTP_Para.mtp_handle.flag )
    {
        return;
    }

    for( i = 0; i != len / 4; i++ )
    {
        if( MTP_Para.mtp_handle.Handle_cur_num == MTP_Para.mtp_handle.Handle_locate_num )
        {
            MTP_Para.mtp_handle.Handle_num = LE32( pdata );
            MTP_Para.mtp_handle.flag = 1;
        }
        MTP_Para.mtp_handle.Handle_cur_num++;
        pdata += 4;
    }
}

/*********************************************************************
 * @fn      USBH_MTP_GetObjectHandles
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_MTP_GetObjectHandles( uint32_t storage_id, uint32_t objectformatcode, uint32_t objecthandle )
{
    uint8_t  s, flag;
    uint32_t len, temp;

    /* Command Phase */
    PTP_Container.Code = PTP_OC_GetObjectHandles;
    PTP_Container.Param1 = storage_id;
    PTP_Container.Param2 = objectformatcode;
    PTP_Container.Param3 = objecthandle;
    PTP_Container.Nparam = 3;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }

    /* Data Phase */
    PTP_Container.Transaction_ID++; // Increment the transfer ID after the transfer is complete
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 )
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;
            MTP_Para.mtp_handle.Handle_total_num = LE32( PTP_Handle.data_ptr + 12 ); // Total
            ObjectHandles_Process( PTP_Handle.data_ptr + 16, len - 16 );
        }
        else
        {
            ObjectHandles_Process( PTP_Handle.data_ptr, len );
        }
        if( ( len < PTP_Handle.data_packet ) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            break;
        }
        PTP_Handle.data_ptr = pMTP_Data_Buf;
        PTP_Handle.data_length += len;
        temp -= len;
    }
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      USBH_MTP_GetObjectInfo
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_MTP_GetObjectInfo( uint32_t objecthandle )
{
    uint8_t  s, flag;
    uint32_t len,temp;

    PTP_Container.Code = PTP_OC_GetObjectInfo;
    PTP_Container.Param1 = objecthandle;
    PTP_Container.Nparam = 1;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }

    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 )
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;
        }
        if( ( len < PTP_Handle.data_packet) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            PTP_Handle.data_ptr = pMTP_Data_Buf + 12;
            PTP_ObjectInfo.ObjectFormat = LE16( PTP_Handle.data_ptr + PTP_oi_ObjectFormat );

            /* For Samsung Galaxy */
            if( ( *( PTP_Handle.data_ptr + PTP_oi_filenamelen ) == 0 ) && ( *( PTP_Handle.data_ptr + PTP_oi_filenamelen + 4 ) != 0 ) )
            {
                PTP_Handle.data_ptr += 4;
            }
            PTP_GetString( PTP_ObjectInfo.Filename, PTP_Handle.data_ptr + PTP_oi_filenamelen, &len );
            PTP_ObjectInfo.FileName_len = len << 1;
            temp = len + PTP_oi_Filename;
            PTP_GetString( PTP_ObjectInfo.Data_Creat_Time, PTP_Handle.data_ptr + temp, &len );
            PTP_ObjectInfo.Data_Creat_len = len << 1;
            temp += len + 1;
            PTP_GetString( PTP_ObjectInfo.Data_Modified_Time, PTP_Handle.data_ptr + temp, &len );
            PTP_ObjectInfo.Data_Modified_len = len << 1;
            break;
        }
        PTP_Handle.data_ptr += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }

    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      PTP_GetDevicePropValue
 *
 * @brief
 *
 * @para
 *
 * @return
 */
void PTP_GetDevicePropValue( uint8_t *data, uint32_t *offset, uint16_t datatype )
{
    uint32_t len;

    switch( datatype )
    {
        case PTP_DTC_INT8:
            PTP_PropertyValue.i8 = *(int8_t *)(void *) & ( data[ *offset ] );
            *offset += 1;
            break;
        case PTP_DTC_UINT8:
            PTP_PropertyValue.u8 = *(uint8_t *) & ( data[ *offset ] );
            *offset += 1;
            break;
        case PTP_DTC_INT16:
            PTP_PropertyValue.i16 = *(int16_t *)(void *) & ( data[ *offset ] );
            *offset += 2;
            break;
        case PTP_DTC_UINT16:
            PTP_PropertyValue.u16 = LE16( &( data[ *offset ] ) );
            *offset += 2;
            break;
        case PTP_DTC_INT32:
            PTP_PropertyValue.i32 = *(int32_t *)(void *)( &( data[ *offset ] ) );
            *offset += 4;
            break;
        case PTP_DTC_UINT32:
            PTP_PropertyValue.u32 = LE32( &( data[ *offset ] ) );
            *offset += 4;
            break;
        case PTP_DTC_INT64:
            PTP_PropertyValue.i64 = *(int64_t *)(void *)( &( data[ *offset ] ) );
            *offset += 8;
            break;
        case PTP_DTC_UINT64:
            PTP_PropertyValue.u64 = LE64( &( data[ *offset ] ) );
            *offset += 8;
            break;
        case PTP_DTC_UINT128:
            *offset += 16;
            break;
        case PTP_DTC_INT128:
            *offset += 16;
            break;
        case PTP_DTC_STR:
            PTP_GetString( (uint8_t *)(void *)PTP_PropertyValue.str, (uint8_t *) & ( data[ *offset ] ), &len );
            *offset += (uint32_t)( len * 2 ) + 1;
            break;
        default:
            break;
  }
}

/*********************************************************************
 * @fn      PTP_GetObjectPropList
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint32_t PTP_GetObjectPropList( uint8_t *data, uint32_t len, uint16_t propval )
{
    uint32_t prop_count;
    uint32_t offset = 0, i;
    uint16_t datatype, propval1;

    prop_count = LE32( data );
    if( prop_count == 0 )
    {
        return 0;
    }
    data += sizeof( uint32_t );
    len -= sizeof( uint32_t );

    for( i = 0; i < prop_count; i++ )
    {
        if( len <= 0 )
        {
          return 0;
        }

        data += sizeof( uint32_t );
        len -= sizeof( uint32_t );

        propval1 = LE16( data );
        data += sizeof( uint16_t );
        len -= sizeof( uint16_t );

        datatype = LE16( data );

        data += sizeof( uint16_t );
        len -= sizeof( uint16_t );

        offset = 0;

        PTP_GetDevicePropValue( data, &offset, datatype );

        data += offset;
        len -= offset;
        if( propval1 == propval )
        {
            break;
        }
    }

    return prop_count;
}

/*********************************************************************
 * @fn      USBH_MTP_GetObjectSize
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_MTP_GetObjectSize( uint32_t objecthandle )
{
    uint8_t  s, flag;
    uint32_t len, temp;

    PTP_Container.Code = PTP_OC_GetObjPropList;
    PTP_Container.Param1 = objecthandle;
    PTP_Container.Param2 = 0x00000000;
    PTP_Container.Param3 = PTP_OPC_ObjectSize;
    PTP_Container.Param4 = 0x00000000;
    PTP_Container.Param5 = 0x00000000;
    PTP_Container.Nparam = 5;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }

    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    PTP_Handle.data_ptr = pMTP_Data_Buf;
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( PTP_Handle.data_ptr, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 )
        {
            temp = LE32( PTP_Handle.data_ptr );
            flag = 1;
        }
        if( ( len < PTP_Handle.data_packet) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            PTP_Handle.data_ptr = pMTP_Data_Buf + 12;
            PTP_GetObjectPropList( PTP_Handle.data_ptr, PTP_Handle.data_length - 12, PTP_OPC_ObjectSize );
            PTP_ObjectInfo.File_Size = PTP_PropertyValue.u64;
            break;
        }
        PTP_Handle.data_ptr += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      USBH_PTP_GetPartialObject
 *
 * @brief
 *
 * @para
 *
 * @return
 */
uint8_t USBH_PTP_GetPartialObject( void )
{
    uint8_t  s, flag;
    uint32_t len, temp;

    PTP_Container.Code = PTP_OC_GetPartialObject;
    PTP_Container.Param1 = MTP_Para.get_data.Handle_num;
    PTP_Container.Param2 = MTP_Para.get_data.Handle_locate;
    PTP_Container.Param3 = MTP_Para.get_data.Handle_maxlen;
    PTP_Container.Nparam = 3;
    s = USBH_PTP_SendRequest( );
    if( s != PTP_OK )
    {
        return s;
    }

    PTP_Container.Transaction_ID++;
    PTP_Handle.data_length = 0;
    flag = 0;
    while( 1 )
    {
        s = MTP_RecvData( MTP_Para.get_data.data_str.pdata, &len );
        if( s != PTP_OK )
        {
            return s;
        }

        if( flag == 0 )
        {
            temp = LE32( MTP_Para.get_data.data_str.pdata );
            flag = 1;
        }
        if( ( len < PTP_Handle.data_packet) || ( temp == len ) )
        {
            PTP_Handle.data_length += len;
            MTP_Para.get_data.Handle_maxlen = PTP_Handle.data_length - 12; // Actual file size
            break;
        }
        MTP_Para.get_data.data_str.pdata += len;
        PTP_Handle.data_length += len;
        temp -= len;
    }
    return USBH_PTP_GetResponse( );
}

/*********************************************************************
 * @fn      MTP_Init_Process
 *
 * @brief
 *
 * @para
 *
 * @return
 */
void MTP_Init_Process( void )
{
    uint32_t i, total_num, s;

    MTP_PTP_ParaInit( );
    USBH_PTP_OpenSession( 0x01 );
    USBH_PTP_GetDeviceInfo( );
    USBH_PTP_GetStorageIds( ); // Get how many MTP devices there are
    if( PTP_StorageIDs.n ) // Indicates that the device is mounted
    {
        USBH_PTP_GetStorageInfo( PTP_StorageIDs.Storage[ 0 ] ); // Get disk capacity, including remaining capacity
        DUG_PRINTF( "MaxCapability = %08x, %08x\r\n", (uint32_t)PTP_StorageInfo.MaxCapability,
                                                    (uint32_t)( PTP_StorageInfo.MaxCapability >> 32 ) );
        DUG_PRINTF( "FreeSpaceInBytes = %08x, %08x\r\n", (uint32_t)PTP_StorageInfo.FreeSpaceInBytes,
                                                       (uint32_t)( PTP_StorageInfo.FreeSpaceInBytes >> 32 ) );

        /* Enumerate all files under the root directory */
        USBH_PTP_GetNumObjects( PTP_StorageIDs.Storage[ 0 ], 0x00000000, 0xFFFFFFFF, &total_num ); // Get the number of files under the root directory
        DUG_PRINTF( " total_num = %02x\r\n", total_num );
        for( i = 0; i != total_num; i++ )
        {
            MTP_Para.mtp_handle.Handle_cur_num = 0;
            MTP_Para.mtp_handle.Handle_locate_num = i;
            MTP_Para.mtp_handle.flag = 0;
            USBH_MTP_GetObjectHandles( PTP_StorageIDs.Storage[ 0 ], 0x00000000, 0xFFFFFFFF );
            DUG_PRINTF( "handle = %08x\r\n", MTP_Para.mtp_handle.Handle_num );
            USBH_MTP_GetObjectInfo( MTP_Para.mtp_handle.Handle_num );
            DUG_PRINTF( "obj_info.ObjectFormat = %02x\r\n", PTP_ObjectInfo.ObjectFormat );
            DUG_PRINTF( "obj_info.FileName:" );
            for( s = 0; s != PTP_ObjectInfo.FileName_len; s++ )
            {
                DUG_PRINTF( "%02x ", PTP_ObjectInfo.Filename[ s ] );
            }
            DUG_PRINTF( "\r\n" );
            DUG_PRINTF( "obj_info.Data_Creat:" );
            for( s = 0; s != PTP_ObjectInfo.FileName_len; s++ )
            {
                DUG_PRINTF( "%02x ", PTP_ObjectInfo.Data_Creat_Time[ s ] );
            }
            DUG_PRINTF( "\r\n" );
            DUG_PRINTF( "obj_info.Data_Modified:" );
            for( s = 0; s != PTP_ObjectInfo.FileName_len; s++ )
            {
                DUG_PRINTF( "%02x ", PTP_ObjectInfo.Data_Modified_Time[ s ] );
            }
            DUG_PRINTF( "\r\n" );

            USBH_MTP_GetObjectSize( MTP_Para.mtp_handle.Handle_num );
            DUG_PRINTF( "obj_info.File_Size = %08lx\r\n", (uint32_t)PTP_ObjectInfo.File_Size );
        }

        /* The following demonstrates how to read data */
        MTP_Para.get_data.Handle_locate = 0;
        MTP_Para.get_data.Handle_maxlen = 0;
        while( 1 )
        {
            MTP_Para.get_data.Handle_num = 0x16; // Handle number
            MTP_Para.get_data.Handle_locate += MTP_Para.get_data.Handle_maxlen; // Locate
            MTP_Para.get_data.Handle_maxlen = 8192; // File size to read
            MTP_Para.get_data.data_str.pdata = Com_Buf; // Read data, the data starts from the Com_Buf[12] position, and the front is the packet header
            s = USBH_PTP_GetPartialObject( );
            DUG_PRINTF( "s = %02x\r\n", s );
            DUG_PRINTF( "MTP_Para.get_data.Handle_maxlen=%d\r\n", MTP_Para.get_data.Handle_maxlen );
            if( MTP_Para.get_data.Handle_maxlen < 8192 )
            {
                break;
            }
       }
       DUG_PRINTF( "end\r\n" );
    }
}

/*********************************************************************
 * @fn      USBH_MainDeal
 *
 * @brief   Provide a simple enumeration process for USB devices and
 *          xxxxxxxxxxx.
 *
 * @return  none
 */
void USBH_MainDeal( void )
{
    uint8_t  s;
    uint8_t  index;

    s = USBFSH_CheckRootHubPortStatus( RootHubDev.bStatus ); // Check USB device connection or disconnection
    if( s == ROOT_DEV_CONNECTED )
    {
        DUG_PRINTF( "USB Port Dev In.\r\n" );

        /* Set root device state parameters */
        RootHubDev.bStatus = ROOT_DEV_CONNECTED;
        RootHubDev.DeviceIndex = DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL;

        /* Enumerate root device */
        s = USBH_EnumRootDevice( );
        if( s == ERR_SUCCESS )
        {
            if( RootHubDev.bType == USB_DEV_CLASS_IMAGE ) // Further enumerate
            {
                DUG_PRINTF("Root Device Is Image. ");

                s = USBH_EnumImageDevice( RootHubDev.DeviceIndex );
                DUG_PRINTF( "Further Enum Result: " );
                if( s == ERR_SUCCESS )
                {
                    DUG_PRINTF( "OK\r\n" );

                    /* Set the connection status of the device  */
                    RootHubDev.bStatus = ROOT_DEV_SUCCESS;

                    MTP_Init_Process( );
                }
                else if( s != ERR_USB_DISCON )
                {
                    DUG_PRINTF( "Err(%02x)\r\n", s );

                    RootHubDev.bStatus = ROOT_DEV_FAILED;
                }
            }
            else // Detect that this device is a non-image device
            {
                DUG_PRINTF( "Root Device Is " );
                switch( RootHubDev.bType )
                {
                    case USB_DEV_CLASS_HID:
                        DUG_PRINTF("Hid. ");
                        break;
                    case USB_DEV_CLASS_STORAGE:
                        DUG_PRINTF("Storage. ");
                        break;
                    case USB_DEV_CLASS_PRINTER:
                        DUG_PRINTF("Printer. ");
                        break;
                    case USB_DEV_CLASS_HUB:
                        DUG_PRINTF("Hub. ");
                        break;
                    case DEF_DEV_TYPE_UNKNOWN:
                        DUG_PRINTF("Unknown. ");
                        break;
                }
                DUG_PRINTF( "End Enum.\r\n" );

                RootHubDev.bStatus = ROOT_DEV_SUCCESS;
            }
        }
        else if( s != ERR_USB_DISCON )
        {
            /* Enumeration failed */
            DUG_PRINTF( "Enum Fail with Error Code:%x\r\n",s );
            RootHubDev.bStatus = ROOT_DEV_FAILED;
        }
    }
    else if( s == ROOT_DEV_DISCONNECT )
    {
        DUG_PRINTF( "USB Port Dev Out.\r\n" );

        /* Clear parameters */
        index = RootHubDev.DeviceIndex;
        memset( &RootHubDev.bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
        memset( &HostCtl[ index ].InterfaceNum, 0, sizeof( HOST_CTL ) );
    }
}
