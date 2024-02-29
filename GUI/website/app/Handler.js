import {
    doesUserExist,
    doesPasswordMatch,
    addUserToSurvey,
    getUidByUsername,
    getUsersList,
    getNodeListForSurvey,
    doesSurveyExist,
    doesSurveyIdExist,
    doesUserIdExist,
    addNewUser,
    createNewSurvey,
    isNewSurveyByUid,
    getPeerByUid,
    getPeersList,
    changePeerByUid,
    generateUniqueSurveyId,
    generateUniqueUserId
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
           (req.id >= process.env.SUCCESS_ADD_VOTE && req.id <= process.env.SUCCESS_COUNT_VOTES);
}

export function handleRequest(req)
{
    if (isRequestRelevant(req))
    {
        let reqCode = getMsgCode(req);
        if (reqCode == process.env.LOGIN_CODE)
        {
            handleLogin(req);
        }
        else if(reqCode = '102')
        {
            handleGetPeers(req);
        }
    }

}
async function handleLogin(msg) {
    try {
        const { username, password, peer_address } = JSON.parse(removeCodeFromMsg(msg));

        const userExists = await doesUserExist(username);
        if (!userExists) {
            return '404{}';
        }

        const passwordMatches = await doesPasswordMatch(username, password);
        if (!passwordMatches) {
            return '404{}';
        }

        const uid = await getUserUid(username, password);
        if (!uid) {
            return '404{}';
        }

        await changePeerByUid(uid, peer_address);
        return `204{"uid": "${uid}"}`;
    } catch (error) {
        console.error('Error during login:', error);
        return '404{}';
    }
}

async function handleGetPeers(msg) {
    try {
        const list = await getPeersList();
        return `202${JSON.stringify({ peers: list })}`;
    } catch (error) {
        console.error('Error getting peers:', error);
        return '404{}';
    }
}
