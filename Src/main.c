/* USER CODE BEGIN Header */
/**
/*
 * Copyright (C)  2019 Hamid Najafi <hamid.najafi@email.com>
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "eeprom.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

#define Wifi_Init 0

#define IndexPageCharCount 731
unsigned char IndexPage[IndexPageCharCount] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:15px 32px;border:2px solid #857f7f;border-radius:15px}.btn{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:20px 100px;border-radius:8px;width:100%}.btn:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">WiFi SMS Panel</h1><div><form action=\"/contact\"><button class=\"btn\" id=contact type=\"submit\" onclick=\"contact()\">Goto Contact Page</button></form><form action=\"/sms\"><button class=\"btn\" id=sms type=\"submit\" onclick=\"sms()\">Goto SMS Page</button></form><form action=\"/call\"><button class=\"btn\" id=sms type=\"submit\" onclick=\"sms()\">Goto Call Page</button></form></div>";
#define ContactPageCharCount1 1139
char ContactPage1[ContactPageCharCount1] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.style{background-color:rgb(21,34,36);border:1px solid #000;color:white;text-align:center;border-radius:8px;padding:10px 70px}.style2{background-color:rgb(21,34,36);border:1px solid #000;color:white;text-align:center;border-radius:8px;padding:10px 80px}.style2:hover,.style:hover{background-color:hsl(315,42%,30%)}table{font-family:arial,sans-serif;border-collapse:collapse;width:100%;text-align:center}td,th{border:1px solid #ddd;padding:8px}tr:nth-child(even){background-color:#dbd3d3}</style><h1 class=\"title\" align=\"center\">Add Contact</h1><div align=\"center\"><form name=\"myform\" method=\"GET\"><br><label>Name</label><input class=\"style\" type=\"text\" name=\"name\"><label>Number</label><input class=\"style\" type=\"text\" name=\"number\"><br><br><button class=\"style\" type=\"submit\">Save Contact</button></div></form><div align=\"center\"><form action=\"/erase\"><button class=\"style\" type=\"submit\">Mass Erase</button></form><table style=\"width:30%\"><tr><th>Name</th><th>Number</th></tr>";
#define ContactPageCharCount2 329
char ContactPage2[ContactPageCharCount2] =
    "</table><br><form action=\"/index\"><div align=\"center\"><button class=\"style\" type=\"submit\">Return to Main Page</button></form><br><br><form action=\"/sms\"><button class=\"style2\" type=\"submit\">Go to SMS Page</button></form><form action=\"/call\"><button class=\"style2\" type=\"submit\">Go to Call Page</button></form>";
#define SuccessPageCharCount 896
unsigned char SuccessPage[SuccessPageCharCount] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.btn{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:12px 60px;width:20%;border-radius:8px}.btn:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">Successfully done</h1><div align=\"center\"><h2>Now you can return. </h2></div><form action=\"/index\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Main Page</button></form><br><br><form action=\"/contact\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Contact Page</button></form><br><br><form action=\"/sms\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to SMS Page</button></form><br><br><form action=\"/call\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Call Page</button></form>";
#define SMSPageCharCount 1066
unsigned char SMSPage[SMSPageCharCount] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;-webkit-border-radius:5px;border-radius:5px}.text{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:70px 100px;border-radius:8px}.style{background-color:rgb(21,34,36);border:1px solid #000;color:white;text-align:center;border-radius:8px;padding:10px 70px}.text:hover,.style:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">Send Text Message</h1><form name=\"myform\" method=\"GET\"><div align=\"center\"><br><br><label>Contact</label><input class=\"style\" type=\"text\" name=\"name\" size=\"25\"><br><br><label>Text Mg</label><input class=\"text\" type=\"text\" name=\"text\" size=\"25\"><br><br><button class=\"style\" type=\"submit\">Send</button></div></form></form><form action=\"/index\"><div align=\"center\"><button class=\"style\" type=\"submit\">Return to Main Page</button></form><br><br><form action=\"/contact\"><button class=\"style\" type=\"submit\">Go to Contact Page</button></form>";
#define ErrorPageCharCount1 429
unsigned char ErrorPage1[ErrorPageCharCount1] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.btn{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:12px 60px;width:20%;border-radius:8px}.btn:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">An error occurred</h1><div align=\"center\"><h2>Sorry,something went wrong! </h2><h3>Description:";
#define ErrorPageCharCount2 496
unsigned char ErrorPage2[ErrorPageCharCount2] =
    "</h3><form action=\"/index\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Main Page</button></form><br><br><form action=\"/contact\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Contact Page</button></form><br><br><form action=\"/sms\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to SMS Page</button></form><br><br><form action=\"/call\"><div align=\"center\"><button class=\"btn\" type=\"submit\">Go to Call Page</button></form></div>";
// #define ErrorPageCharCount 509
// unsigned char ErrorPage[ErrorPageCharCount] =
//     "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.btn{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:12px 60px;border-radius:8px}.btn:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">Error page</h1><div align=\"center\"><h2>Sorry,something went wrong! </h2><br><form action=\"/index\"><button class=\"btn\" type=\"submit\">Goto to Main Page</button></form></div><br><br>";

#define CallPageCharCount 797
unsigned char CallPage[CallPageCharCount] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.style{background-color:rgb(21,34,36);border:1px solid #000;color:white;text-align:center;border-radius:8px;padding:10px 80px}.style:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">Call</h1><form name=\"myform\" method=\"GET\"><div align=\"center\"><br><label>Contact</label><input class=\"style\" type=\"text\" name=\"name\"><br><br><button class=\"style\" type=\"submit\">Make Call</button></form></div><form action=\"/index\"><div align=\"center\"><button class=\"style\" type=\"submit\">Return to Main Page</button></form><br><br><form action=\"/contact\"><button class=\"style\" type=\"submit\">Go to Contact Page</button></form>";
#define TalkingPageCharCount 449
unsigned char TalkingPage[TalkingPageCharCount] =
    "<style type=\"text/css\">body{background-color:rgb(162,168,170)}.title{padding:5px;border:2px solid #857f7f;border-radius:5px}.btn{background-color:rgb(21,34,36);border:1px solid #000;color:white;padding:12px 60px;border-radius:8px}.btn:hover{background-color:hsl(315,42%,30%)}</style><h1 class=\"title\" align=\"center\">Call</h1><br><br><form action=\"/endcall\"><div align=\"center\"><button class=\"btn\" type=\"submit\">End Call</button></form>";

#define NB_OF_VAR 3
#define ContactCountROMAddr 0x5000
#define ContactNameROMAddr 0x6000
#define ContactNumberROMAddr 0x7000

#define ContactCountROMAddrIndex 0
#define ContactNameROMAddrIndex 1
#define ContactNumberROMAddrIndex 2

uint16_t VirtAddVarTab[NB_OF_VAR] = {ContactCountROMAddr, ContactNameROMAddr, ContactNumberROMAddr};
uint16_t WriteAddr;
uint16_t ReadAddr;

unsigned char buffer[60];

uint16_t ContactCount = 0;

#define ContactNameMaxLength 10
unsigned char ContactName[ContactNameMaxLength];
unsigned char ContactNameTemp[ContactNameMaxLength];
uint8_t ContactNameLength;

#define ContactNumberLength 11
unsigned char ContactNumber[ContactNumberLength];
unsigned char ContactNumberTemp[ContactNumberLength];

#define TextMaxLength 50
unsigned char TextMessage[TextMaxLength];
uint8_t TextLength;

#define ErrorLength 60
unsigned char ErrorText[ErrorLength];
uint8_t ErrorTextLength;

unsigned char EspRxBuffer[1];
unsigned char EspRxArray[400];
#define MAXRequestHeaderSize 50
uint8_t rxCounter = 0;
unsigned char RequestHeader[MAXRequestHeaderSize];
uint8_t request = 0;
uint8_t ready = 0;
uint8_t RequestHeaderCounter = 0;
uint8_t clientNumber = 5;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
void StartDefaultTask(void const *argument);

/* USER CODE BEGIN PFP */
// void SIMCOM_Init();
void SIMCOM_SendSMS();
void SIMCOM_MakeCall();
void SIMCOM_EndCall();
void EEPROM_Init();
void EEPROM_ListContacts();
int EEPROM_AddContact();
int EEPROM_GetName();
int EEPROM_GetNumber();
void ESP_Init(int FullInit);
void ESP_SendPage();
void ESP_HandleRequset();
int ESP_HandleHeaderParam();
void ESP_SendIndex();
void ESP_SendContact();
//void ESP_SendContactSaved();
void ESP_SendSMS();
//void ESP_SendSMSSent();
void ESP_SendCall();
void ESP_SendTalking();
void ESP_SendError();
void ESP_HandleContact();
void ESP_HandleSMS();
void ESP_HandleEndCall();
void ESP_HandleCall();
void ESP_HandleErase();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SIMCOM_Init()
{
  // sprintf(buffer, "AT\r\n");
  // HAL_UART_Transmit(&huart2, buffer, 4, HAL_MAX_DELAY);
  // HAL_Delay(1000);
  sprintf(buffer, "AT+CNMI=1, 2, 0, 0, 0\r\n");
  HAL_UART_Transmit(&huart2, buffer, 23, HAL_MAX_DELAY);
  HAL_Delay(1000);
  sprintf(buffer, "AT+CBAND=\"ALL_BAND\"\r\n");
  HAL_UART_Transmit(&huart2, buffer, 21, HAL_MAX_DELAY);
  //HAL_Delay(5000);
}

void SIMCOM_SendSMS()
{
  sprintf(buffer, "AT+CMGF=1\r\n");
  HAL_UART_Transmit(&huart2, buffer, 11, HAL_MAX_DELAY);
  HAL_Delay(1000);

  sprintf(buffer, "AT+CMGS=\"%s\"\r\n", ContactNumberTemp);
  HAL_UART_Transmit(&huart2, buffer, 23, HAL_MAX_DELAY);
  HAL_Delay(2000);

  sprintf(buffer, "%s\r\n", TextMessage);
  HAL_UART_Transmit(&huart2, buffer, TextLength + 2, HAL_MAX_DELAY);
  HAL_Delay(1000);
  sprintf(buffer, "%c", 0x1a);
  HAL_UART_Transmit(&huart2, buffer, 1, HAL_MAX_DELAY);
  HAL_Delay(1000);
}
void SIMCOM_MakeCall()
{
  sprintf(buffer, "ATD%s;\r\n", ContactNumberTemp);
  HAL_UART_Transmit(&huart2, buffer, 17, HAL_MAX_DELAY);
}
void SIMCOM_EndCall()
{
  sprintf(buffer, "ATH\r\n");
  HAL_UART_Transmit(&huart2, buffer, 5, HAL_MAX_DELAY);
}
void EEPROM_Init()
{
  HAL_FLASH_Unlock();
  EE_Init();
  HAL_FLASH_Lock();

  HAL_FLASH_Unlock();
  //get count of contacts in eeprom
  ReadAddr = VirtAddVarTab[ContactCountROMAddrIndex];
  WriteAddr = VirtAddVarTab[ContactCountROMAddrIndex];
  if (EE_ReadVariable(ReadAddr, &ContactCount) != HAL_OK)
  {
    HAL_UART_Transmit(&huart6, "Full erase detected!\r\n", 22, HAL_MAX_DELAY);
    //so if not defined set ContactCount to 0
    HAL_FLASH_Unlock();
    EE_WriteVariable(WriteAddr, ContactCount);
    HAL_FLASH_Lock();
  }
  else
  {
    // sprintf(buffer, "Number of saved Contacts: %d\r\n", ContactCount);
    // HAL_UART_Transmit(&huart6, buffer, 29, HAL_MAX_DELAY);
  }
  HAL_FLASH_Lock();
}
void EEPROM_ListContacts()
{
  if (ContactCount == 0)
  {
    HAL_UART_Transmit(&huart6, "No Contact\r\n", 12, HAL_MAX_DELAY);
    return;
  }

  sprintf(buffer, "Contact List (%d):\r\n", ContactCount);
  HAL_UART_Transmit(&huart6, buffer, 19, HAL_MAX_DELAY);

  unsigned char tempName[ContactNameMaxLength];
  uint16_t tempNameLength = 0;
  unsigned char tempNumber[ContactNumberLength];
  uint16_t temp;

  // For each contact
  for (int i = 0; i < ContactCount; i++)
  {
    // Read Number
    ReadAddr = VirtAddVarTab[ContactNumberROMAddrIndex] + (i * ContactNumberLength);
    for (int j = 0; j < ContactNumberLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      EE_ReadVariable(ReadAddr, &temp);
      HAL_FLASH_Lock();
      tempNumber[j] = temp;
    }

    // Read Name
    memset(tempName, 0, sizeof(tempName));
    tempNameLength = 0;
    ReadAddr = VirtAddVarTab[ContactNameROMAddrIndex] + (i * ContactNameMaxLength);
    for (int j = 0; j < ContactNameMaxLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      if (EE_ReadVariable(ReadAddr, &temp) != HAL_OK)
      {
        break; // Its not because an error, the name ENDS if it cant read.
      }
      HAL_FLASH_Lock();

      if ((temp >= 0x41 && temp <= 0x5A) || (temp >= 0x61 && temp <= 0x7a))
      {
        tempNameLength++;
        tempName[j] = temp;
      }
    }
    // sprintf(buffer, "Contact Name Length:%d\r\n", tempNameLength);
    // HAL_UART_Transmit(&huart6, buffer, 23, 10000);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%d- %s, %s", i + 1, tempName, tempNumber);
    HAL_UART_Transmit(&huart6, buffer, 5 + tempNameLength + ContactNumberLength, 10000);
    HAL_UART_Transmit(&huart6, "\r\n", 2, 10000);
  }
}

int EEPROM_AddContact()
{
  HAL_UART_Transmit(&huart6, "Adding Contact in EEPROM\r\n", 26, HAL_MAX_DELAY);

  if (EEPROM_GetName() != -1)
  {
    sprintf(ErrorText, "contact with this number exists, its name is: %s", ContactNameTemp);
    ErrorTextLength = 46 + ContactNameLength;
    return -1;
  }

  if (EEPROM_GetNumber() != -1)
  {
    sprintf(buffer, "contact with this name exists, its number is: %s", ContactNumberTemp);
    ErrorTextLength = 46 + ContactNumberLength;
    return -1;
  }

  // Write Number
  HAL_FLASH_Unlock();
  //add contact to the end of list
  WriteAddr = VirtAddVarTab[ContactNumberROMAddrIndex] + (ContactCount * ContactNumberLength);

  for (int i = 0; i < ContactNumberLength; i++, WriteAddr++)
  {
    if (ContactNumber[i] >= 0x30 && ContactNumber[i] <= 0x39)
      EE_WriteVariable(WriteAddr, ContactNumber[i]);
  }
  HAL_FLASH_Lock();

  // Write Name
  HAL_FLASH_Unlock();
  WriteAddr = VirtAddVarTab[ContactNameROMAddrIndex] + (ContactCount * ContactNameMaxLength);
  //Note : Writeadd use MAX length but FOR use Lenght from HTTP Header
  for (int i = 0; i < ContactNameMaxLength; i++, WriteAddr++)
  {
    if ((ContactName[i] >= 0x41 && ContactName[i] <= 0x5A) || (ContactName[i] >= 0x61 && ContactName[i] <= 0x7a))
    {
      EE_WriteVariable(WriteAddr, ContactName[i]);
    }
    else
    {
      break;
    }
  }
  HAL_FLASH_Lock();

  // Write Contact Count
  ContactCount = ContactCount + 1;
  HAL_FLASH_Unlock();
  WriteAddr = VirtAddVarTab[ContactCountROMAddrIndex];
  EE_WriteVariable(WriteAddr, ContactCount);
  HAL_FLASH_Lock();

  // memset(buffer, 0, sizeof(buffer));
  // sprintf(buffer, "%s, %s Added Successfully\r\n", ContactName, ContactNumber);
  // HAL_UART_Transmit(&huart6, buffer, 23 + ContactNameLength + ContactNumberLength, 10000);

  EEPROM_ListContacts();

  return 0;
}

int EEPROM_GetName()
{

  // We have Number => find number index => find index => find name
  HAL_UART_Transmit(&huart6, "Checking for Number in EEPROM\r\n", 31, HAL_MAX_DELAY);

  int index = -1;
  uint16_t temp;
  unsigned char tempNumber[ContactNumberLength];

  // For each contact
  for (int i = 0; i < ContactCount; i++)
  {
    // Read Number
    ReadAddr = VirtAddVarTab[ContactNumberROMAddrIndex] + (i * ContactNumberLength);
    for (int j = 0; j < ContactNumberLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      EE_ReadVariable(ReadAddr, &temp);
      HAL_FLASH_Lock();
      tempNumber[j] = temp;
    }
    for (int j = 0; j < ContactNumberLength; j++)
    {
      if (tempNumber[j] != ContactNumber[j])
        break;
      else if (j == (ContactNumberLength - 1))
        index = i;
    }
  }

  if (index != -1)
  {
    memset(ContactName, 0, sizeof(ContactName));

    // Read Name
    ContactNameLength = 0;
    ReadAddr = VirtAddVarTab[ContactNameROMAddrIndex] + (index * ContactNameMaxLength);
    for (int j = 0; j < ContactNameMaxLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      if (EE_ReadVariable(ReadAddr, &temp) != HAL_OK)
      {
        break; // Its not because an error, the name ENDS if it cant read.
      }
      HAL_FLASH_Lock();

      if ((temp >= 0x41 && temp <= 0x5A) || (temp >= 0x61 && temp <= 0x7a))
      {
        ContactNameLength++;
        ContactNameTemp[j] = temp;
      }
    }
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "Found Contact with this Number, Name is: %s\r\n", ContactNameTemp);
    HAL_UART_Transmit(&huart6, buffer, 43 + ContactNameLength, HAL_MAX_DELAY);
  }
  else
  {
    sprintf(buffer, "Contact Number doesnt exist\r\n");
    HAL_UART_Transmit(&huart6, buffer, 29, 10000);
    index = -1;
  }
  HAL_FLASH_Lock();

  return index;
}
int EEPROM_GetNumber()
{
  // We have name => find name index => find index
  HAL_UART_Transmit(&huart6, "Checking for Name in EEPROM\r\n", 29, HAL_MAX_DELAY);

  int index = -1;
  uint16_t temp;
  unsigned char tempName[ContactNameMaxLength];
  uint16_t tempNameLength = 0;

  // For each contact
  for (int i = 0; i < ContactCount; i++)
  {
    // Read Name
    memset(tempName, 0, sizeof(tempName));
    tempNameLength = 0;
    ReadAddr = VirtAddVarTab[ContactNameROMAddrIndex] + (i * ContactNameMaxLength);
    for (int j = 0; j < ContactNameMaxLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      if (EE_ReadVariable(ReadAddr, &temp) != HAL_OK)
      {
        break; // Its not because an error, the name ENDS if it cant read.
      }
      HAL_FLASH_Lock();

      if ((temp >= 0x41 && temp <= 0x5A) || (temp >= 0x61 && temp <= 0x7a))
      {
        tempNameLength++;
        tempName[j] = temp;
      }
    }

    for (int j = 0; j < ContactNameLength; j++)
    { // upper and lower case check
      if (tempName[j] >= 0x41 && tempName[j] <= 0x5A && ContactName[j] >= 0x61 && ContactName[j] <= 0x7a)
      {
        ContactName[j] -= 0x20;
      }
      if (tempName[j] >= 0x61 && tempName[j] <= 0x7a && ContactName[j] >= 0x41 && ContactName[j] <= 0x5A)
      {
        tempName[j] -= 0x20;
      }
      if (tempName[j] != ContactName[j])
        break;
      else if ((j == (ContactNameLength - 1)) && (ContactNameLength == tempNameLength))
        index = i;
    }
  }

  if (index != -1)
  {

    // Read Number
    ReadAddr = VirtAddVarTab[ContactNumberROMAddrIndex] + (index * ContactNumberLength);
    for (int j = 0; j < ContactNumberLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      if (EE_ReadVariable(ReadAddr, &temp) != HAL_OK)
      {
        break; // Its not because an error, the name ENDS if it cant read.
      }
      HAL_FLASH_Lock();
      ContactNumberTemp[j] = temp;
    }

    sprintf(buffer, "Found Contact with this Name, Number is: %s\r\n", ContactNumberTemp);
    HAL_UART_Transmit(&huart6, buffer, 43 + ContactNumberLength, HAL_MAX_DELAY);
  }

  else
  {
    sprintf(buffer, "Contact Name doesnt exist\r\n");
    HAL_UART_Transmit(&huart6, buffer, 27, 10000);
  }

  return index;
}

void ESP_Init(int FullInit)
{

  // HAL_UART_Transmit(&huart3, "AT\r\n", sizeof("AT\r\n"), 1000);
  // HAL_Delay(1000);

  if (FullInit != 0)
  {
    HAL_UART_Transmit(&huart3, "AT+CWMODE=1\r\n", 13, 1000);
    HAL_Delay(1000);

    HAL_UART_Transmit(&huart3, "AT+CWLAP\r\n", 10, 1000);
    HAL_Delay(5000);

    //Hamid's iPhone
    //sprintf(buffer, "AT+CWJAP=\"Hamid%c%c%cs iPhone\",\"51176915\"\r\n", 0xE2, 0X80, 0X99);
    //HAL_UART_Transmit(&huart3, buffer, 40, 1000);

    // ZMI-MF885
    //HAL_UART_Transmit(&huart3, "AT+CWJAP=\"ZMI-MF885\",\"51176915\"\r\n", sizeof("AT+CWJAP=\"ZMI-MF885\",\"51176915\"\r\n"), 1000);

    //Linksys-XAC1900
    //HAL_UART_Transmit(&huart3, "AT+CWJAP=\"Linksys-XAC1900\",\"Mypass.97\"\r\n", sizeof("AT+CWJAP=\"Linksys-XAC1900\",\"Mypass.97\"\r\n"), 1000);

    //WiFi-FUM
    //HAL_UART_Transmit(&huart3, "AT+CWJAP=\"WiFi-FUM\",\"ferdowsi\"\r\n",sizeof("AT+CWJAP=\"WiFi-FUM\",\"ferdowsi\"\r\n"),1000);

    HAL_Delay(10000);
  }

  HAL_UART_Transmit(&huart3, "AT+CIPSTA=\"192.168.21.120\",\"192.168.21.1\",\"255.255.255.0\"\r\n", 59, 1000);
  //HAL_UART_Transmit(&huart3, "AT+CIPSTA=\"192.168.1.120\",\"192.168.1.1\",\"255.255.255.0\"\r\n", 58, 1000);
  HAL_Delay(1000);

  HAL_UART_Transmit(&huart3, "AT+CWJAP=\"ZMI-MF885\",\"51176915\"\r\n", 33, 1000);
  //HAL_UART_Transmit(&huart3, "AT+CWJAP=\"Linksys-XAC1900\",\"Mypass.97\"\r\n", sizeof("AT+CWJAP=\"Linksys-XAC1900\",\"Mypass.97\"\r\n"), 1000);
  HAL_Delay(10000);

  HAL_UART_Transmit(&huart3, "AT+CIPMUX=1\r\n", 13, 1000);
  HAL_Delay(1000);

  HAL_UART_Transmit(&huart3, "AT+CIPSERVER=1,80\r\n", 19, 1000);
  //HAL_Delay(1000);

  //HAL_UART_Transmit(&huart3, "AT+CIFSR\r\n", sizeof("AT+CIFSR\r\n"), 1000);
  //HAL_Delay(1000);

  HAL_UART_Receive_IT(&huart3, EspRxBuffer, 1);
}
void ESP_HandleRequset()
{
  //Debug
  HAL_UART_Transmit(&huart6, "\r\n====================\r\n", 24, 1000);
  sprintf(buffer, "Client %d Connected\r\n", clientNumber);
  HAL_UART_Transmit(&huart6, buffer, 20, 1000);
  sprintf(buffer, "Request is %s\r\n", RequestHeader); // Have 13 word
  HAL_UART_Transmit(&huart6, buffer, 13 + RequestHeaderCounter, 1000);
  //Debug

  memset(ContactName, 0, sizeof(ContactName));
  memset(ContactNumber, 0, sizeof(ContactNumber));
  memset(TextMessage, 0, sizeof(TextMessage));

  if (RequestHeader[0] == 'c' && RequestHeader[1] == 'o' && RequestHeader[2] == 'n' && RequestHeader[3] == 't' && RequestHeader[4] == 'a' && RequestHeader[5] == 'c' && RequestHeader[6] == 't')
    ESP_HandleContact();

  else if (RequestHeader[0] == 's' && RequestHeader[1] == 'm' && RequestHeader[2] == 's')
    ESP_HandleSMS();

  else if (RequestHeader[0] == 'e' && RequestHeader[1] == 'n' && RequestHeader[2] == 'd' && RequestHeader[3] == 'c' && RequestHeader[4] == 'a' && RequestHeader[5] == 'l' && RequestHeader[6] == 'l')
    ESP_HandleEndCall();

  else if (RequestHeader[0] == 'c' && RequestHeader[1] == 'a' && RequestHeader[2] == 'l' && RequestHeader[3] == 'l')
    ESP_HandleCall();
  else if (RequestHeader[0] == 'e' && RequestHeader[1] == 'r' && RequestHeader[2] == 'a' && RequestHeader[3] == 's' && RequestHeader[4] == 'e')
    ESP_HandleErase();

  else
    ESP_SendIndex();
}
void ESP_HandleErase()
{
  HAL_UART_Transmit(&huart6, "Handle Erase Req ....\r\n", 22, 1000);
  HAL_UART_Transmit(&huart6, "======Removing======\r\n", 22, 1000);
  EEPROM_ListContacts();
  ContactCount = 0;
  HAL_FLASH_Unlock();
  WriteAddr = VirtAddVarTab[ContactCountROMAddrIndex];
  EE_WriteVariable(WriteAddr, ContactCount);
  HAL_FLASH_Lock();
  HAL_UART_Transmit(&huart6, "========Done========\r\n", 22, 1000);
  ESP_SendSuccess();
}
void ESP_HandleCall()
{
  HAL_UART_Transmit(&huart6, "Handle Call Req ....\r\n", 22, 1000);
  int haveReq = 0;

  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
    if (RequestHeader[i - 5] == 'n' && RequestHeader[i - 4] == 'a' && RequestHeader[i - 3] == 'm' && RequestHeader[i - 2] == 'e')
      haveReq = 1;

  if (haveReq)
  {

    if ((ESP_HandleHeaderParam() == 0) && (EEPROM_GetNumber() != -1))
    {
      HAL_UART_Transmit(&huart6, buffer, 13 + ContactNumberLength, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart6, "Making Call\r\n", 13, 1000);
      SIMCOM_MakeCall();
      ESP_SendTalking();
    }

    else
    {
      ESP_SendError(); // please enter valid name and number
    }
  }
  else
  {
    ESP_SendCall();
  }
}
void ESP_HandleEndCall()
{
  HAL_UART_Transmit(&huart6, "Handle Hang up Req ....\r\n", 25, 1000);
  HAL_UART_Transmit(&huart6, "Ending Call ....\r\n", 18, 1000);
  SIMCOM_EndCall();
  ESP_SendIndex();
}
void ESP_HandleContact()
{
  HAL_UART_Transmit(&huart6, "Handle Contact Req ....\r\n", 25, 1000);
  int haveReq = 0;

  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
    if (RequestHeader[i - 5] == 'n' && RequestHeader[i - 4] == 'a' && RequestHeader[i - 3] == 'm' && RequestHeader[i - 2] == 'e')
      haveReq = 1;

  if (haveReq)
  {
    if (ESP_HandleHeaderParam() == 0 && (EEPROM_AddContact() == 0))
    {
      //ESP_SendContactSaved();
      ESP_SendSuccess();
    }
    else
    {
      ESP_SendError(); // please enter valid name and number
    }
  }
  else
  {
    ESP_SendContact();
  }
}

void ESP_HandleSMS()
{
  HAL_UART_Transmit(&huart6, "Handle SMS Req ....\r\n", 21, 1000);
  int haveReq = 0;
  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
    if (RequestHeader[i - 5] == 'n' && RequestHeader[i - 4] == 'a' && RequestHeader[i - 3] == 'm' && RequestHeader[i - 2] == 'e')
      haveReq = 1;

  if (haveReq)
  {
    if ((ESP_HandleHeaderParam() == 0) && (EEPROM_GetNumber() != -1))
    {
      HAL_UART_Transmit(&huart6, "Sending SMS\r\n", 13, 1000);
      SIMCOM_SendSMS();
      //ESP_SendSMSSent();
      ESP_SendSuccess();
    }
    else
    {
      ESP_SendError();
      return;
    }
  }
  else
  {
    ESP_SendSMS();
  }
}

int ESP_HandleHeaderParam()
{
  uint8_t numberIndex;
  uint8_t nameIndex;
  uint8_t numberEndIndex;
  uint8_t nameEndIndex;
  uint8_t textIndex;
  uint8_t textEndIndex;

  uint8_t haveName = 0;
  uint8_t haveText = 0;
  uint8_t haveNumber = 0;

  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
  {
    if (RequestHeader[i - 7] == 'n' && RequestHeader[i - 6] == 'u' && RequestHeader[i - 5] == 'm' && RequestHeader[i - 4] == 'b' && RequestHeader[i - 3] == 'e' && RequestHeader[i - 2] == 'r')
    {
      numberIndex = i;
      numberEndIndex = RequestHeaderCounter - 1;
      //ContactNumberLength = numberEndIndex - numberIndex = 11
      haveNumber = 1;
      break;
    }
  }
  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
  {
    if (RequestHeader[i - 5] == 't' && RequestHeader[i - 4] == 'e' && RequestHeader[i - 3] == 'x' && RequestHeader[i - 2] == 't')
    {
      textIndex = i;
      textEndIndex = RequestHeaderCounter - 1;
      TextLength = textEndIndex - textIndex + 1;
      haveText = 1;
      break;
    }
  }
  for (uint8_t i = 0; i < RequestHeaderCounter; i++)
  {
    if (RequestHeader[i - 5] == 'n' && RequestHeader[i - 4] == 'a' && RequestHeader[i - 3] == 'm' && RequestHeader[i - 2] == 'e')
    {
      nameIndex = i;

      if (haveNumber == 1)
      {
        nameEndIndex = numberIndex - 9;
      }
      if (haveText == 1)
      {
        nameEndIndex = textIndex - 7;
      }
      else if (haveNumber == 0 && haveText == 0)
      {
        nameEndIndex = RequestHeaderCounter - 1;
      }
      ContactNameLength = nameEndIndex - nameIndex + 1;
      haveName = 1;
      break;
    }
  }
  if (haveName)
  {
    for (uint8_t i = 0; i < ContactNameLength; i++)
    {
      ContactName[i] = RequestHeader[nameIndex + i];
    }
    sprintf(buffer, "NameIndex= %d, NameEndIndex= %d\r\n", nameIndex, nameEndIndex);
    HAL_UART_Transmit(&huart6, buffer, 32, 1000);

    if (nameEndIndex - nameIndex < 1)
    {
      sprintf(ErrorText, "wrong name, at least type 1 character!");
      ErrorTextLength = 38;
      HAL_UART_Transmit(&huart6, "wrong Name\r\n", 12, 1000);
      return -1;
    }
    for (int i = nameIndex; i <= nameEndIndex; i++)
    {
      if (!((RequestHeader[i] >= 0x41 && RequestHeader[i] <= 0x5A) || (RequestHeader[i] >= 0x61 && RequestHeader[i] <= 0x7a)))
      {
        sprintf(ErrorText, "wrong name, A-Z, a-z are allowed");
        ErrorTextLength = 32;
        HAL_UART_Transmit(&huart6, "wrong Name\r\n", 12, 1000);
        return -1;
      }
    }
    sprintf(buffer, "Name: %s\r\n", ContactName);
    HAL_UART_Transmit(&huart6, buffer, 8 + ContactNameLength, 1000);
    sprintf(buffer, "NameLength: %d\r\n", ContactNameLength);
    HAL_UART_Transmit(&huart6, buffer, 15, 1000);
  }
  if (haveNumber)
  {
    for (uint8_t i = 0; i < ContactNumberLength; i++)
    {
      ContactNumber[i] = RequestHeader[numberIndex + i];
    }
    sprintf(buffer, "NumberIndex= %d, NumberEndIndex= %d\r\n", numberIndex, numberEndIndex);
    HAL_UART_Transmit(&huart6, buffer, 37, 1000);

    if (numberEndIndex - numberIndex != 10)
    {
      sprintf(ErrorText, "wrong number, it must be 10 number!");
      ErrorTextLength = 35;
      HAL_UART_Transmit(&huart6, "wrong Number\r\n", 14, 1000);
      return -1;
    }

    for (int i = numberIndex; i <= numberEndIndex; i++)
    {
      if (!(RequestHeader[i] >= 0x30 && RequestHeader[i] <= 0x39))
      {
        sprintf(ErrorText, "wrong number, numbers are just number!");
        ErrorTextLength = 38;
        HAL_UART_Transmit(&huart6, "wrong Number\r\n", 14, 1000);
        return -1;
      }
    }
    sprintf(buffer, "Number: %s\r\n", ContactNumber);
    HAL_UART_Transmit(&huart6, buffer, 10 + (numberEndIndex - numberIndex + 1), 1000);
  }
  if (haveText)
  {
    for (uint8_t i = 0; i < TextLength; i++)
    {
      TextMessage[i] = RequestHeader[textIndex + i];
    }
    sprintf(buffer, "TextIndex= %d, TextEndIndex= %d\r\n", textIndex, textEndIndex);
    HAL_UART_Transmit(&huart6, buffer, 33, 1000);
    if (textEndIndex - textIndex < 1)
    {
      sprintf(ErrorText, "wrong text, at least type 1 character!");
      ErrorTextLength = 38;
      HAL_UART_Transmit(&huart6, "No Input Text\r\n", 15, 1000);
      return -1;
    }
    sprintf(buffer, "Text: %s\r\n", TextMessage);
    HAL_UART_Transmit(&huart6, buffer, 8 + TextLength, 1000);
    sprintf(buffer, "TextLength: %d\r\n", TextLength);
    HAL_UART_Transmit(&huart6, buffer, 15, 1000);
  }
  return 0;
}
void ESP_SendIndex()
{
  HAL_UART_Transmit(&huart6, "Sending Index page ....\r\n", 25, 1000);
  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, IndexPageCharCount);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, IndexPage, IndexPageCharCount, 1000);
  HAL_Delay(200);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}
void ESP_SendSMS()
{
  HAL_UART_Transmit(&huart6, "Sending SMS Page ....\r\n", 23, 1000);
  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, SMSPageCharCount);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, SMSPage, SMSPageCharCount, 1000);
  HAL_Delay(200);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}

void ESP_SendContact()
{
  HAL_UART_Transmit(&huart6, "Sending Contact Page ....\r\n", 27, 1000);

  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, ContactPageCharCount1);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, ContactPage1, ContactPageCharCount1, 1000);
  HAL_Delay(200);

  unsigned char tempName[ContactNameMaxLength];
  uint16_t tempNameLength = 0;
  unsigned char tempNumber[ContactNumberLength];
  uint16_t temp;

  // For each contact
  for (int i = 0; i < ContactCount; i++)
  {
    // Read Number
    ReadAddr = VirtAddVarTab[ContactNumberROMAddrIndex] + (i * ContactNumberLength);
    for (int j = 0; j < ContactNumberLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      EE_ReadVariable(ReadAddr, &temp);
      HAL_FLASH_Lock();
      tempNumber[j] = temp;
    }

    // Read Name
    memset(tempName, 0, sizeof(tempName));
    tempNameLength = 0;
    ReadAddr = VirtAddVarTab[ContactNameROMAddrIndex] + (i * ContactNameMaxLength);
    for (int j = 0; j < ContactNameMaxLength; j++, ReadAddr++)
    {
      HAL_FLASH_Unlock();
      if (EE_ReadVariable(ReadAddr, &temp) != HAL_OK)
      {
        break; // Its not because an error, the name ENDS if it cant read.
      }
      HAL_FLASH_Lock();

      if ((temp >= 0x41 && temp <= 0x5A) || (temp >= 0x61 && temp <= 0x7a))
      {
        tempNameLength++;
        tempName[j] = temp;
      }
    }
  
    unsigned char buffer3[tempNameLength + ContactNumberLength + 27];
    sprintf(buffer3, "<tr><td>%s</td><td>%s</td></tr>", tempName, tempNumber);
    sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, sizeof(buffer3));
    HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart3, buffer3, sizeof(buffer3), 1000);
    HAL_Delay(300);

  }
  if(ContactCount ==0)
    {
    unsigned char buffer3[29];
    sprintf(buffer3, "<tr><td>-</td><td>-</td></tr>");
    sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, sizeof(buffer3));
    HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart3, buffer3, sizeof(buffer3), 1000);
    HAL_Delay(300);
    }

  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, ContactPageCharCount2);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, ContactPage2, ContactPageCharCount2, 1000);
  HAL_Delay(300);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}
void ESP_SendSuccess()
{
  HAL_UART_Transmit(&huart6, "Sending Success Page ....\r\n", 27, 1000);
  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, SuccessPageCharCount);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, SuccessPage, SuccessPageCharCount, 1000);
  HAL_Delay(200);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}

void ESP_SendCall()
{
  HAL_UART_Transmit(&huart6, "Sending Call Page ....\r\n", 18, 1000);
  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, CallPageCharCount);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, CallPage, CallPageCharCount, 1000);
  HAL_Delay(200);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}
void ESP_SendTalking()
{
  HAL_UART_Transmit(&huart6, "Sending Talking Page ....\r\n", 27, 1000);
  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, TalkingPageCharCount);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, TalkingPage, TalkingPageCharCount, 1000);
  HAL_Delay(200);
  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}
void ESP_SendError()
{
  HAL_UART_Transmit(&huart6, "Sending Error page ....\r\n", 25, 1000);

  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, ErrorPageCharCount1);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(500);
  HAL_UART_Transmit(&huart3, ErrorPage1, ErrorPageCharCount1, 1000);
  HAL_Delay(200);

  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, ErrorTextLength);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(500);
  HAL_UART_Transmit(&huart3, ErrorText, ErrorTextLength, 1000);
  HAL_Delay(200);

  sprintf(buffer, "AT+CIPSEND=%d,%d\r\n", clientNumber, ErrorPageCharCount2);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPSEND=%d,%d\r\n"), 1000);
  HAL_Delay(500);
  HAL_UART_Transmit(&huart3, ErrorPage2, ErrorPageCharCount2, 1000);
  HAL_Delay(200);

  sprintf(buffer, "AT+CIPCLOSE=%d\r\n", clientNumber);
  HAL_UART_Transmit(&huart3, buffer, sizeof("AT+CIPCLOSE=%d\r\n"), 1000);
  HAL_Delay(300);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  //========================================= Init ========================================= //
  HAL_UART_Transmit(&huart6, "\r\n========Debug=======\r\n", 24, 1000);
  HAL_UART_Transmit(&huart6, "EEPROM Init", 11, 1000);
  EEPROM_Init();
  HAL_UART_Transmit(&huart6, ": OK\r\nSimCom Init", 17, 1000);
  SIMCOM_Init();
  HAL_UART_Transmit(&huart6, ": OK\r\nESPMOD Init", 17, 1000);
  ESP_Init(Wifi_Init);
  HAL_UART_Transmit(&huart6, ": OK\r\n====================\r\n", 28, 1000);
  EEPROM_ListContacts();
  HAL_GPIO_WritePin(GPIOD, LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin, GPIO_PIN_SET);
  HAL_Delay(800);
  HAL_GPIO_WritePin(GPIOD, LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin, GPIO_PIN_RESET);
  HAL_UART_Transmit(&huart6, "========Ready=======\r\n", 22, 1000);
  //========================================= Init ========================================= //
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {ƒ
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin */
  GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {
    if (request == 1)
    {
      if (EspRxBuffer[0] == ' ' || RequestHeaderCounter == sizeof(RequestHeader))
      {
        //RequestHeaderCounter = 0; we need this to calc tx buffer size
        request = 0;
        ready = 1;
      }
      else
      {
        RequestHeader[RequestHeaderCounter] = EspRxBuffer[0];
        RequestHeaderCounter++;
      }
    }

    EspRxArray[rxCounter] = EspRxBuffer[0];
    if (EspRxArray[rxCounter - 4] == 'I' && EspRxArray[rxCounter - 3] == 'P' && EspRxArray[rxCounter - 2] == 'D')
    {
      clientNumber = EspRxBuffer[0] - 48;
    }
    if (EspRxArray[rxCounter - 4] == 'G' && EspRxArray[rxCounter - 3] == 'E' && EspRxArray[rxCounter - 2] == 'T')
    // &&  EspRxArray[rxCounter - 1] == ' ' && EspRxArray[rxCounter] == '/'
    {
      request = 1;
      // Make ESP Rx Buffer to Zero Byte
      rxCounter = 0;
      memset(EspRxArray, 0, sizeof(EspRxArray));
      memset(RequestHeader, 0, sizeof(RequestHeader));
      RequestHeaderCounter = 0;
      HAL_UART_Receive_IT(&huart3, EspRxBuffer, 1);
    }
    else
    {
      HAL_UART_Receive_IT(&huart3, EspRxBuffer, 1);
    }
    rxCounter++;
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {

    if (ready)
    {
      ESP_HandleRequset();
      //SIM800_HandleRequset();
      ready = 0;
      HAL_UART_Receive_IT(&huart3, EspRxBuffer, 1);
    }
    //osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_UART_Transmit(&huart6, "\r\n====================", 22, 1000);
  HAL_UART_Transmit(&huart6, "\r\nAn Error occured\r\n", 20, 1000);
  HAL_UART_Transmit(&huart6, "====================\r\n", 22, 1000);
  while (1)
  {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
