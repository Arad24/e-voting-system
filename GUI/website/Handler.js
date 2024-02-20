const codeLen = 3;

const loginCode = process.env.LOGIN_CODE_REQ;

function removeCodeFromMsg(msg)
{
    return msg.substring(codeLen);
}

function getMsgCode(msg)
{
    return msg.substring(0, codeLen);
}

function isRequestRelevant(req)
{
    var code = getMsgCode(req)
    return code >= '100'/*Code*/ && code <= '100'/*Code*/;
}

export function handleRequest(req)
{
    if (isRequestRelevant(req))
    {
        let reqCode = getMsgCode(req);
        if (reqCode == loginCode)
        {
            handleLogin(req);
        }
    }
    else
    {
        // Close socket
        return false;
    }

}
export function handleLogin(msg)
{
    /*
        TODO:
        - Check if user exist in db
        - if exist: add peers into user db & return ok msg to socket
        - if not exist: return eror msg to socket
    */
   console.log(1)
}

