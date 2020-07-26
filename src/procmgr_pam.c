#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <security/pam_appl.h>  

#include <procmgr_error.h>
#include <procmgr_debug.h>

struct pam_response *reply;  

// //function used to get user input  
int function_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)  
{  
    *resp = reply;  
    return PAM_SUCCESS;  
}  

pmerr
procmgr_authenticate_system (const char *username,
                             const char *password)   
{  
    PROCMGR_LOG_DEBUG("Authentication transaction [START] for user: %s", username);
    pmerr pe = PM_EOK;
    const struct pam_conv local_conversation = { function_conversation, NULL };  
    pam_handle_t *local_auth_handle = NULL; // this gets set by pam_start  

    int retval;  
    retval = pam_start("su", username, &local_conversation, &local_auth_handle);  

    if (retval != PAM_SUCCESS)  
    {  
            PROCMGR_LOG_DEBUG("Failed to start authentication process");
            return retval;
    }  

    reply = (struct pam_response *) malloc(sizeof(struct pam_response));  

    reply[0].resp = strdup(password);  
    reply[0].resp_retcode = 0;  

    retval = pam_authenticate(local_auth_handle, 0);  

    if (retval != PAM_SUCCESS)  
    {  
        PROCMGR_LOG_DEBUG("Authentication Failure #1");
        pe = EINVAL;  
    }  
    

    retval = pam_end(local_auth_handle, retval);  

    if (retval != PAM_SUCCESS)  
    {  
        PROCMGR_LOG_DEBUG("Authentication Failure #2\n");  
        pe = EBADF; 
    }  
    PROCMGR_LOG_DEBUG("Authentication transaction [END] for user: %s", username);
    return pe;  
}

