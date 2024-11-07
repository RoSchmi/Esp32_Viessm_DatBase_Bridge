#include <ViessmannApiAccount.h>

/**
 * constructor
 */
ViessmannApiAccount::ViessmannApiAccount(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps )
{
    ChangeAccountParams(clientId, accessToken, apiIot, apiUser, useHttps);   
}

void ViessmannApiAccount::ChangeAccountParams(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps)
{
    ClientId = clientId;
    ClientId = (clientId.length() <= MAX_CLIENT_ID_LENGTH) ? clientId : clientId.substring(0, MAX_CLIENT_ID_LENGTH);
    AccessToken = accessToken;
    ApiIotUrl = apiIot;
    ApiUserUrl = apiIot;
    
    const char * insert = (char *)useHttps ? "s" : "";
    int baseLength = apiIot.length() > apiUser.length() ? apiIot.length() : apiUser.length();
    char strData[baseLength + 30];
    
    sprintf(strData, "http%s://%s", insert, apiIot.c_str());
    UriEndPointIot = String(strData);
    
    sprintf(strData, "http%s://%s", insert, apiUser.c_str());
    UriEndPointUser = String(strData);
}

/**
 * destructor
 */
ViessmannApiAccount::~ViessmannApiAccount()
{}

