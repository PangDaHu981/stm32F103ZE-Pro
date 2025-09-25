#include "main.h"
#include "AT24C02.h"
#include "Board_Flash.h"
#include "Task_LED.h"
#include "W25Q64.h"
#include "boot.h"
#include "delay.h"
#include "iic.h"
#include "stm32F10x.h"
#include "usart.h"

OTA_InfoCB OTA_Info;            // ������24C02�ڵ�OTA��Ϣ��صĽṹ��
UpdateA_CB UpdateA;             // A�������õ��Ľṹ��
u32        BootStateFlag = 0;   // ��¼ȫ��״̬��־λ��ÿλ��ʾ1��״̬

int main(void) {
    uint8_t i;
    LedGpioInit();       // LED���ų�ʼ��
    delay_Init();        // ��ʱ��ʼ��
    USART1_Init(9600);   // ����1��ʼ��
    u1_printf("\r\n����1��ʼ���ɹ�\r\n");
    IIC_Init();              // IIC��ʼ��
    W25Q64_Init();           // W25Q64��ʼ��
    
    AT24C02_ReadOTAInfo();   // ��24C02��ȡ���ݵ�OTA_Info�ṹ��
    BootLoader_Branch();     // ��֧�ж�
    while (1) {
        LED_Task();
        delay_ms(10);
        // ����1����
        if (U1_CB.URxDataOUT != U1_CB.URxDataIN) {                                                                      // ��鴮�ڽ��ջ������Ƿ���������
            Bootloader_Event_Process(U1_CB.URxDataOUT->start, (U1_CB.URxDataOUT->end - U1_CB.URxDataOUT->start + 1));   // ���������ݴ����Ӧ����
            U1_CB.URxDataOUT++;                                                                                         // ��ָ�����
            if (U1_CB.URxDataOUT == U1_CB.URxDataEND) {
                U1_CB.URxDataOUT = &U1_CB.URxDataPtr[0];   // ����Ѿ�������ĩβ�������»ص�������ͷ��
            }
        }
        /*����C*/
        if (BootStateFlag & IAP_XModem_C_FLAG) {   // ���ڷ���XModemЭ�����ʼC
            if (UpdateA.XModem_Timer_Count >= 100) {
                u1_printf("C");
                UpdateA.XModem_Timer_Count = 0;
            }
            UpdateA.XModem_Timer_Count++;
        }
        /*����W25Q64���ݰ���stm32 flash*/
        // ��֮ǰӦ���Ѿ���APP�����ŵ�W25Q64����
        /*--------------------------------------------------*/
        /*        UPDATA_A_FLAG��λ��������Ҫ����A��        */
        /*--------------------------------------------------*/
        if (BootStateFlag & UPDATA_A_FLAG) {
            u1_printf("������Ҫ���µĴ�С��%d�ֽ�\r\n", OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num]);
            if ((OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num] % 4) == 0) {   // �жϳ����Ƿ���4�ֽڣ�32λ�������������ǵĻ�������д�룬����if
                STM32_EraseFlash(STM32_A_START_PAGE, STM32_A_PAGE_NUM);                // ��A���ռ����
                u1_printf("A���Ѳ���\r\n");
                /* ��W25Q64�ж�ȡ1k��д��Ƭ��Flash */
                for (i = 0; i < (OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num] / STM32_PAGE_SIZE); i++) {                             // ��д������1k�ֽ�
                    W25Q64_Read(UpdateA.Updata_A_Buff, i * STM32_PAGE_SIZE + 64 * 1024 * UpdateA.Updata_A_from_W25Q64_Num, STM32_PAGE_SIZE);   // ��ȡ��1k����

                    STM32_WriteFlash(STM32_A_START_ADDR + i * STM32_PAGE_SIZE, (u32*) UpdateA.Updata_A_Buff, STM32_PAGE_SIZE);   // ��A��д��Ƭ��Flashλ��
                }
                // ����д����ǰ�������ļ�k���ݣ�����д����1k������
                if ((OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num] % 1024) != 0) {
                    memset(UpdateA.Updata_A_Buff, 0, STM32_PAGE_SIZE);

                    W25Q64_Read(UpdateA.Updata_A_Buff, i * 1024 + UpdateA.Updata_A_from_W25Q64_Num * 64 * 1024, OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num] % 1024);

                    u1_printf("\r\n");
                    STM32_WriteFlash(STM32_A_START_ADDR + i * STM32_PAGE_SIZE, (u32*) UpdateA.Updata_A_Buff, (OTA_Info.FirmwareLen[UpdateA.Updata_A_from_W25Q64_Num] % 1024));   // ��A��д��Ƭ��Flashλ��
                }
                if (UpdateA.Updata_A_from_W25Q64_Num == 0) {
                    // ������ɣ����OTA��־λ
                    OTA_Info.OTA_flag = 0;
                    AT24C02_WriteOTAInfo();
                }
                u1_printf("A��������ɣ���������ϵͳ��\r\n");
                delay_ms(100);
                NVIC_SystemReset();
            } else {
                u1_printf("������APP���ȴ���\r\n");
                BootStateFlag &= ~(UPDATA_A_FLAG);
            }
        }
    };
}
