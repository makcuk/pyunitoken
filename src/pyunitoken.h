#include "UniToken.h"

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
     
