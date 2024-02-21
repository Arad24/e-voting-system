import IDatabase from './IDatabase';
import SqliteDatabase from './sqliteDB';

const codeLen = 3;
const db: IDatabase = new SqliteDatabase();

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
        else if(reqCode = '102')
        {
            handleGetPeers(req);
        }
    }
    else
    {
        // Close socket
    }

}
async function handleLogin(msg: string) {
    try {
        const { username, password, peer_address } = JSON.parse(removeCodeFromMsg(msg));

        const userExists = db.doesUserExist(username);
        if (!userExists) {
            return '404{}';
        }

        const passwordMatches = db.doesPasswordMatch(username, password);
        if (!passwordMatches) {
            return '404{}';
        }

        const uid = await db.getUidByUsername(username);
        if (!uid) {
            return '404{}';
        }

        await db.changePeerByUid(uid, peer_address);
        return `204{"uid": "${username}"}`;
    } catch (error) {
        console.error('Error during login:', error);
        return '404{}';
    }
}

async function handleGetPeers(msg: string) {
    try {
        const list = await db.getPeersList();
        return `202{${list}}`;
    } catch (error) {
        console.error('Error getting peers:', error);
        return '404{}';
    }
}

export async function handleAddUser(msg: string) {
    try {
        const { username, password, uid, addresses } = JSON.parse(removeCodeFromMsg(msg));

        const userExists = db.doesUserExist(username);
        if (userExists) {
            return '404{}';
        }

        const userAdded = db.addNewUser(username, password, uid, addresses);
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