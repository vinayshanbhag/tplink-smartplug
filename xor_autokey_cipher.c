#include <stdio.h>
#include <string.h>
#include <inttypes.h>
union
{
    uint32_t integer;
    uint8_t byte[4];
} int32bit;

void encrypt(uint8_t input[], uint8_t output[], int sz)
{
    uint8_t xorKey = 171;
    int32bit.integer = sz;
    int headerSize = sizeof(int32bit.integer);
    for(int i = 0; i < headerSize; i++){
        output[i] = int32bit.byte[headerSize-1-i];
    }
    for (int i = 0; i < sz; i++)
    {
        output[i+headerSize] = input[i] ^ xorKey;
        xorKey = output[i+headerSize];
    }
}

void decrypt(uint8_t input[], uint8_t output[], int sz)
{
    uint8_t xorKey = 171;
    for (int i = 0; i < sz; i++)
    {
        output[i] = input[i] ^ xorKey;
        xorKey = input[i];
    }
}

void print(uint8_t *buf, int sz, const char* fmt){
    for(int i = 0; i < sz; i++){
        printf(fmt,buf[i]);
    }
    printf("\n");
}

int main()
{
    char* input;
    input = "{\"system\":{\"set_relay_state\":{\"state\":0}}}";
    int inputLen = strlen(input);
    int outputLen = inputLen + sizeof(int32bit.integer);
    uint8_t inputBuf[inputLen];
    uint8_t outputBuf[outputLen];
    
    printf("SmartPlug command: \n%s\n",input);
    
    for(int i = 0; i < inputLen; i++){
        inputBuf[i] = input[i];
    }
    encrypt(inputBuf, outputBuf, inputLen);
    printf("Encrypted SmartPlug command: \n");
    print(outputBuf,outputLen,"%02x ");

    uint8_t encryptedInput[inputLen];
    uint8_t decryptedOutput[inputLen];
    for(int i = 0; i < inputLen; i++){
        encryptedInput[i] = outputBuf[i+sizeof(int32bit.integer)];
    }
    printf("Decrypted SmartPlug command: \n");
    decrypt(encryptedInput, decryptedOutput, inputLen);
    print(decryptedOutput, inputLen, "%c");
    
    return 0;
}
