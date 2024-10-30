

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

#ifndef _VIESSMANNCLIENT_H_
#define _VIESSMANNCLIENT_H_


class ViessmannClient
{
private:
   char theVie[50] = "";

public:
     
    //ViessmannClient(CloudStorageAccount *account, const char * caCert, HTTPClient *httpClient, WiFiClient * wifiClient, uint8_t * bufferStore);
    
    ViessmannClient(ViessmannApiAccount * account, const char * caCert, HTTPClient *httpClient, WiFiClient * wifiClient, uint8_t * bufferStore);
    ~ViessmannClient();

    az_http_status_code GetUserName();
    
    /*
    az_http_status_code CreateTable(const char * tableName, DateTime pDateTimeUtcNow, ContType pContentType = ContType::contApplicationIatomIxml, AcceptType pAcceptType = AcceptType::acceptApplicationIjson, ResponseType pResponseType = ResponseType::returnContent, bool useSharedKeyLight = false);
    az_http_status_code InsertTableEntity(const char * tableName, DateTime pDateTimeUtcNow, TableEntity pEntity, char* out_ETAG, DateTime * outResonseHeaderDate, ContType pContentType, AcceptType pAcceptType, ResponseType pResponseType, bool useSharedKeyLite = false);   
    void CreateTableAuthorizationHeader(const char * content, const char * canonicalResource, const char * ptimeStamp, const char * pHttpVerb, az_span pConentType, char * pMd5Hash, char pAutorizationHeader[], bool useSharedKeyLite = false);
    int32_t dow(int32_t year, int32_t month, int32_t day);
    */
   
};
#endif