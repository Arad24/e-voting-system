const codeLen = 3;
//const db = openDatabase();

function removeCodeFromMsg(msg)
{
    return msg.substring(codeLen, msg.length);
}

function getMsgCode(msg)
{
    return msg.substring(0, codeLen);
}

function isRequestRelevant(req)
{
    var code = getMsgCode(req)
    return (req.id >= ADD_BLOCK_CODE && req.id <= LOGIN_CODE) ||
           (req.id >= PEERS_LIST_SUCCEEDED_CODE && req.id <= LOGIN_SUCCEEDED_CODE) ||
           (req.id >= ADD_VOTE_CODE && req.id <= COUNT_VOTES_CODE) ||
           (req.id >= SUCCESS_ADD_VOTE && req.id <= SUCCESS_COUNT_VOTES);
}

export function handleRequest(req)
{
    if (isRequestRelevant(req))
    {
        let reqCode = getMsgCode(req);
        if (reqCode == '100'/*Login code*/)
        {
            //handleLogin(req);
        }
        else if(reqCode = '102')
        {
            //handleGetPeers(req);
        }
    }
    else
    {
        // Close socket
    }

}
/*async function handleLogin(msg) {
    try {
        const { username, password, peer_address } = JSON.parse(removeCodeFromMsg(msg));

        const userExists = await doesUserExist(db, username);
        if (!userExists) {
            return '404{}';
        }

        const passwordMatches = await doesPasswordMatch(db, username, password);
        if (!passwordMatches) {
            return '404{}';
        }

        const uid = await getUserUid(db, username, password);
        if (!uid) {
            return '404{}';
        }

        await changePeerByUid(db, uid, peer_address);
        return `204{"uid": "${uid}"}`;
    } catch (error) {
        console.error('Error during login:', error);
        return '404{}';
    }
}

async function handleGetPeers(msg) {
    try {
        const list = await getPeersList(db);
        return `202{${list}}`;
    } catch (error) {
        console.error('Error getting peers:', error);
        return '404{}';
    }
}

export async function handleAddUser(msg) {
    try {
        const { username, password} = JSON.parse(removeCodeFromMsg(msg));

        const userExists = await doesUserExist(db, username);
        if (userExists) {
            return '404{}';
        }

        const userAdded = await addNewUser(db, username, password, generateUid(), 'None');
        if (userAdded) {
            return '204{}';
        } else {
            return '404{}';
        }
    } catch (error) {
        console.error('Error adding user:', error);
        return '404{}';
    }
}
*/