import {
    doesUserExist,
    doesPasswordMatch,
    getUidByUsername,
    getPeersList,
    changePeerByUid,
  } from './dbApiConnector.js';

const codeLen = 3;

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
    return (req.id >= process.env.ADD_BLOCK_CODE && req.id <= process.env.LOGIN_CODE) ||
           (req.id >= process.env.PEERS_LIST_SUCCEEDED_CODE && req.id <= process.env.LOGIN_SUCCEEDED_CODE) ||
           (req.id >= process.env.ADD_VOTE_CODE && req.id <= process.env.COUNT_VOTES_CODE) ||
           (req.id >= process.env.SUCCESS_ADD_VOTE && req.id <= process.env.SUCCESS_COUNT_VOTES) || true;
}

export async function handleRequest(req) {
    try {
        if (isRequestRelevant(req)) {
            const reqCode = getMsgCode(req);
            if (reqCode === '104') {
                return handleLogin(req);
            } else if (reqCode === '102') {
                return handleGetPeers(req);
            }
        }
        return '404{}'; // Request is not relevant
    } catch (error) {
        console.error('Error handling request:', error);
        return '404{}'; // Generic error response
    }
}

async function handleLogin(msg) {
    try {
        const jsonMsg = JSON.parse(removeCodeFromMsg(msg));
        const { username, password, peer_address } = jsonMsg;

        const userExists = await doesUserExist(username);
        if (!userExists) {
            return '404{}'; // User not found
        }

        const passwordMatches = await doesPasswordMatch(username, password);
        if (!passwordMatches) {
            return '404{}'; // Password mismatch
        }

        const uid = await getUidByUsername(username);
        if (!uid) {
            return '404{}'; // UID not found
        }

        await changePeerByUid(uid, peer_address);
        return `204{"uid": "${uid}"}`;
    } catch (error) {
        console.error('Error during login:', error);
        return '404{}'; // Generic error response
    }
}

async function handleGetPeers(msg) 
{
    try {
        const list = await getPeersList();
        const jsonList = JSON.stringify({ peers: list });
        return `202${jsonList}`;
    } catch (error) {
        console.error('Error getting peers:', error);
        return '404{}';
    }
}
