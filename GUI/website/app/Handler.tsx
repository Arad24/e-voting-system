const codeLen = 3;

function removeCodeFromMsg(msg : string)
{
    return msg.substring(codeLen, msg.length);
}

function getMsgCode(msg : string)
{
    return msg.substring(0, codeLen);
}

function isRequestRelevant(req : string)
{
    return getMsgCode(req) >= '100'/*Code*/ && getMsgCode(req) <= '100'/*Code*/;
}

export function handleRequest(req : string)
{
    if (isRequestRelevant(req))
    {
        let reqCode = getMsgCode(req);
        if (reqCode == '100'/*Login code*/)
        {
            handleLogin(req);
        }
    }
    else
    {
        // Close socket
    }

}
export function handleLogin(msg : string)
{
    /*
        TODO:
        - Check if user exist in db
        - if exist: add peers into user db & return ok msg to socket
        - if not exist: return eror msg to socket
    */
}

