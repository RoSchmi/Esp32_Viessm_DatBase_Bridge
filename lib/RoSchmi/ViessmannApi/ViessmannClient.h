

#include <Arduino.h>
#include "WiFiClientSecure.h"
#include "HTTPClient.h"
#include "DateTime.h"

#include "mbedtls/md.h"
#include "mbedtls/md5.h"
#include "mbedtls/base64.h"
#include "mbedtls/sha256.h"
#include "azure/core/az_http.h"

#include "ViessmannApiAccount.h"

#include "ArduinoJson.h"

#ifndef _VIESSMANNCLIENT_H_
#define _VIESSMANNCLIENT_H_

/*
#define MAX_TABLENAME_LENGTH 50

#define RESPONSE_BUFFER_LENGTH 1000

#define REQUEST_BODY_BUFFER_LENGTH 900
#define PROPERTIES_BUFFER_LENGTH 300
#define AUTH_HEADER_BUFFER_LENGTH 100
#define REQUEST_PREPARE_PTR_BUFFER_LENGTH 500
*/

class ViessmannClient
{

public:
     
    //ViessmannClient(CloudStorageAccount *account, const char * caCert, HTTPClient *httpClient, WiFiClient * wifiClient, uint8_t * bufferStore);
    
    ViessmannClient(ViessmannApiAccount * account, const char * caCert, HTTPClient *httpClient, WiFiClient * wifiClient, uint8_t * bufferStore);
    ~ViessmannClient();

    int GetUser(uint8_t * reponsePtr, const uint16_t reponseBufferLength);
    int GetEquipment(uint8_t* responseBuffer, const uint16_t reponseBufferLength);
    int GetFeatures(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const uint32_t data_0_id, const char * gateways_0_serial, const char * gateways_0_devices_0_id);
    
    /*
    az_http_status_code CreateTable(const char * tableName, DateTime pDateTimeUtcNow, ContType pContentType = ContType::contApplicationIatomIxml, AcceptType pAcceptType = AcceptType::acceptApplicationIjson, ResponseType pResponseType = ResponseType::returnContent, bool useSharedKeyLight = false);
    az_http_status_code InsertTableEntity(const char * tableName, DateTime pDateTimeUtcNow, TableEntity pEntity, char* out_ETAG, DateTime * outResonseHeaderDate, ContType pContentType, AcceptType pAcceptType, ResponseType pResponseType, bool useSharedKeyLite = false);   
    void CreateTableAuthorizationHeader(const char * content, const char * canonicalResource, const char * ptimeStamp, const char * pHttpVerb, az_span pConentType, char * pMd5Hash, char pAutorizationHeader[], bool useSharedKeyLite = false);
    int32_t dow(int32_t year, int32_t month, int32_t day);
    */
   
};
#endif