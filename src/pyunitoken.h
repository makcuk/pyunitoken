 /*
  * This code is licensed under Creative Commons CC-BY-NC 3.0
  * http://creativecommons.org/licenses/by-nc/3.0/
  * (c) 2012 Max Speransky <speransky@gmail.com> 
  */

#include "UniToken.h"

struct {
    char *name;
    unsigned long value;
} constants[] = {
{(char *)"UT_MAX_PIN_LEN",UT_MAX_PIN_LEN},
{(char *)"UT_MAX_ID_LEN",UT_MAX_ID_LEN},
{(char *)"MAX_SLOT_NUMBER",MAX_SLOT_NUMBER},
{(char *)"FS_MEMORY",FS_MEMORY},
{(char *)"TOKEN_NO_EVENT",TOKEN_NO_EVENT},
{(char *)"TOKEN_INSERT",TOKEN_INSERT},
{(char *)"TOKEN_REMOVE",TOKEN_REMOVE},
{(char *)"UT_USER_LEVEL_GUEST",UT_USER_LEVEL_GUEST},
{(char *)"UT_USER_LEVEL_USER",UT_USER_LEVEL_USER},
{(char *)"UT_USER_LEVEL_ADMIN",UT_USER_LEVEL_ADMIN},
{(char *)"UT_CHANGE_PIN_MODE_BYSELF",UT_CHANGE_PIN_MODE_BYSELF},
{(char *)"UT_CHANGE_PIN_MODE_ADMIN_CHANGE_USER",UT_CHANGE_PIN_MODE_ADMIN_CHANGE_USER},
{(char *)"LED_ON",LED_ON},
{(char *)"LED_OFF",LED_OFF},
{(char *)"DES_KEY_LEN_8_BYTE",DES_KEY_LEN_8_BYTE},
{(char *)"DES3_KEY_LEN_24_BYTE",DES3_KEY_LEN_24_BYTE},
{(char *)"AES_KEY_LEN_128_BIT",AES_KEY_LEN_128_BIT},
{(char *)"AES_KEY_LEN_192_BIT",AES_KEY_LEN_192_BIT},
{(char *)"RSA_MODULUS_LEN_1024_BIT",RSA_MODULUS_LEN_1024_BIT},
{(char *)"RSA_MODULUS_LEN_2048_BIT",RSA_MODULUS_LEN_2048_BIT},
{(char *)"RSA_PUBEXP_LEN_65537_BIT",RSA_PUBEXP_LEN_65537_BIT},
{(char *)"DES_DATA_LEN_8_BYTE",DES_DATA_LEN_8_BYTE},
{(char *)"AES_DATA_LEN_16_BYTE",AES_DATA_LEN_16_BYTE},
{(char *)"RSA_DATA_LEN_1024_BIT",RSA_DATA_LEN_1024_BIT},
{(char *)"RSA_DATA_LEN_2048_BIT",RSA_DATA_LEN_2048_BIT},
{(char *)"RSA_IMPORT_NEW",RSA_IMPORT_NEW},
{(char *)"UT_FILE_NAME_LENGTH",UT_FILE_NAME_LENGTH},
{(char *)"FILE_PERMISSION_GUEST",FILE_PERMISSION_GUEST},
{(char *)"FILE_PERMISSION_USER",FILE_PERMISSION_USER},
{(char *)"FILE_PERMISSION_ADMIN",FILE_PERMISSION_ADMIN},
{0,0}
};

struct {
    unsigned long value;
    char *name;
} error_codes[] = {
{ UT_NOT_INITIALIZE, (char *)"Library not initialized"}, 
{ UT_BUFFER_TOO_SMALL, (char *)"Buffer is not big enough"}, 
{ UT_ARGUMENTS_BAD, (char *)"Invalid parameter"}, 
{ UT_NO_PERMISSION, (char *)"No permission"}, 
{ UT_NO_TOKEN, (char *)"token No token available"}, 
{ UT_TOKEN_HANDLE_INVALID, (char *)"Invalid token handle"}, 
{ UT_SLOT_FULL, (char *)"token/Slot is full so no more token can be added"}, 
{ UT_SLOT_ID_INVALID, (char *)"Invalid slotID"}, 
{ UT_DEVICE_ERROR, (char *)"Device error, mainly used for virtual devices"}, 
{ UT_ALREADY_INITIALIZE, (char *)"Memory not enough"}, 
{ UT_DEVICE_TX, (char *)"Communication error"}, 
{ UT_PIN_ERROR, (char *)"Password error"}, 
{ UT_USER_LOCKED, (char *)"User has been locked"}, 
{ UT_USER_FULL, (char *)"No more users can logon"}, 
{ UT_NO_USER, (char *)"No such user available"}, 
{ UT_DEVICE_NEED_OPEN, (char *)"Device is not open yet"}, 
{ UT_KEY_EXIST, (char *)"The key already existed"}, 
{ UT_NO_KEY, (char *)"No key available"}, 
{ UT_KEY_FULL, (char *)"KEY The key is full and can't generate keys"}, 
{ UT_KEY_HANDLE_INVALID, (char *)"Key handle error"}, 
{ UT_KEY_GENERATE, (char *)"Key generation failed"}, 
{ UT_NOT_ALLOWED, (char *)"Not allow operation"}, 
{ UT_OPERATE_INVALID, (char *)"Invalid operation"}, 
{ UT_FS_INITIALIZE_FAILED, (char *)"File initialization failed"}, 
{ UT_FS_NOSPACE, (char *)"Not enough file system storage"}, 
{ UT_FS_OFFSET_INVALID, (char *)"Designated file offset is invalid"}, 
{ UT_FS_DATA_LEN_RANGE, (char *)"Beyond data range"}, 
{ UT_FS_NO_OPEN, (char *)"No opened file is available"}, 
{ UT_FS_NO_FILE, (char *)"No file"}, 
{ UT_FS_LIST_END, (char *)"End of file list"}, 
{ UT_FS_NAME_LEN_RANGE, (char *)"File name is too long"}, 
{ UT_FS_FILE_EXIST, (char *)"File already existed"}, 
{ 0, 0 }
};
     
