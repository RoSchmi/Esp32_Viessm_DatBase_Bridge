#include <Arduino.h>

#ifndef _VIESSMANNAPIACCOUNT_H_
#define _VIESSMANNAPIACCOUNT_H_

#define MAX_CLIENT_ID_LENGTH 50
#define MAX_TOKEN_LENGTH     1000
#define MAX_USER_URI_LENGTH  60
#define MAX_IOT_URI_LENGTH   60

class ViessmannApiAccount
{
public:
    ViessmannApiAccount();
    ViessmannApiAccount(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps);
    ~ViessmannApiAccount();

    void ChangeAccountParams(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps);
    
    String ClientId;
    String AccessToken;
    String ApiIotUrl;
    String ApiUserUrl;
    String UriEndPointIot;
    String UriEndPointUser;

    /*
    String AccountName;
    String AccountKey;
    String UriEndPointTable;
    String HostNameTable;
    */
};


#endif  // _CLOUDSTORAGEACCOUNT_H_