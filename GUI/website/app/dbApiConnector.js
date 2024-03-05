export async function doesUserExist(name) {
    const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking user existence:', error);
        return false;
    }
}

export async function doesPasswordMatch(name, password) {
    const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}' AND PASSWORD = '${password}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking password match:', error);
        return false;
    }
}

export async function doesSurveyExist(surveyName) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_NAME = '${surveyName}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking survey existence:', error);
        return false;
    }
}

export async function addNewSurvey(surveyId, surveyName, surveyOptions) {
    const sqlStatement = `INSERT INTO SURVEYS(SURVEY_UID, SURVEY_NAME, SURVEY_OPTIONS) VALUES ('${sanitizeInput(surveyId)}', '${sanitizeInput(surveyName)}', '${sanitizeInput(JSON.stringify(surveyOptions))}');`;
    try {
        await sendQueryAndGetRes(sqlStatement);
        return true;
    } catch (error) {
        console.error('Error adding new survey:', error);
        return false;
    }
}

function sanitizeInput(input)
{
    return input.replace(/'/g, "''");
}

export async function getSurveyOptions(surveyId) {
    const sqlStatement = `SELECT SURVEY_OPTIONS FROM SURVEYS WHERE SURVEY_UID = '${surveyId}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0 ? JSON.parse(response.result[0].SURVEY_OPTIONS) : null;
    } catch (error) {
        console.error('Error getting survey options:', error);
        return null;
    }
}

export async function updateSurveyOptions(surveyId, surveyOptions) {
    const sqlStatement = `UPDATE SURVEYS SET SURVEY_OPTIONS = '${JSON.stringify(surveyOptions)}' WHERE SURVEY_UID = '${surveyId}';`;
    try {
        await sendQueryAndGetRes(sqlStatement);
        return true;
    } catch (error) {
        console.error('Error updating survey options:', error);
        return false;
    }
}

export async function getUidByUsername(username) {
    const sqlStatement = `SELECT UID FROM USERS WHERE NAME = '${username}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        console.log(response)
        if (response.result.length > 0 && response.result) {
            return response.result[0].UID;
        } else {
            return null;
        }
    } catch (error) {
        console.error('Error getting UID by username:', error);
        return null;
    }
}

export async function getUsersList() {
    const sqlStatement = "SELECT NAME FROM USERS;";
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.map(row => row.NAME);
    } catch (error) {
        console.error('Error getting users list:', error);
        return [];
    }
}

export async function doesSurveyIdExist(surveyId) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_UID = '${sanitizeInput(surveyId)}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking survey ID existence:', error);
        return false;
    }
}

export async function getSocketByUid(userUid) 
{
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_UID = '${sanitizeInput(surveyId)}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking survey ID existence:', error);
        return false;
    }
}

export async function getAllSurveys() 
{
    const sqlStatement = `SELECT * FROM SURVEYS;`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result;
    } catch (error) {
        console.error('Error checking survey ID existence:', error);
        return false;
    }
}

export async function doesUserIdExist(userUid) {
    const sqlStatement = `SELECT * FROM USERS WHERE UID = '${userUid}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0;
    } catch (error) {
        console.error('Error checking user ID existence:', error);
        return false;
    }
}

export async function addNewUser(name, password, uid, addresses) {
    const sqlStatement = `INSERT INTO USERS(NAME, PASSWORD, UID, ADDRESSES) VALUES ('${name}', '${password}', '${uid}', '${addresses}');`;
    try {
        await sendQueryAndGetRes(sqlStatement);
        return true;
    } catch (error) {
        console.error('Error adding new user:', error);
        return false;
    }
}

export async function getSurveyByUid(uid) 
{
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_UID LIKE '${sanitizeInput(uid)}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result;
    } catch (error) {
        console.error('Error checking if survey is new by UID:', error);
        return [];
    }
}

export async function getPeerByUid(uid) {
    const sqlStatement = `SELECT ADDRESSES FROM USERS WHERE UID = '${uid}';`;
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        return response.result.length > 0 ? response.result[0].ADDRESSES : null;
    } catch (error) {
        console.error('Error getting peer by UID:', error);
        return null;
    }
}

export async function getPeersList() {
    const sqlStatement = "SELECT ADDRESSES FROM USERS WHERE ADDRESSES NOT LIKE 'None';";
    try {
        const response = await sendQueryAndGetRes(sqlStatement);
        const addresses = response.result.map(row => row.ADDRESSES);
        return addresses;
    } catch (error) {
        console.error('Error getting peers list:', error);
        return []; // Return an empty array on error
    }
}

export async function changePeerByUid(uid, newPeer) {
    if (!isValidPeerFormat(newPeer)) {
        console.error("Invalid format for the new peer.");
        return false;
    }
    const sqlStatement = `UPDATE USERS SET ADDRESSES = '${newPeer}' WHERE UID = '${uid}';`;
    try {
        await sendQueryAndGetRes(sqlStatement);
        return true;
    } catch (error) {
        console.error('Error changing peer by UID:', error);
        return false;
    }
}

function isValidPeerFormat(peer) {
    // Regular expression to match IP address and port combination in the format ip:port
    const ipPortRegex = /^((?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):(\d{1,5})$/;
    return ipPortRegex.test(peer) || peer == 'None';
}

function generateRandomUid(size) {
    const charset = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    let uid = '';

    for (let i = 0; i < size; i++) {
        const randomIndex = Math.floor(Math.random() * charset.length);
        uid += charset[randomIndex];
    }

    return uid;
}

export async function generateUniqueSurveyId() {
    let surveyId;
    try {
        do {
            surveyId = generateRandomUid(5);
        } while (await doesSurveyIdExist(surveyId));
        return surveyId;
    } catch (error) {
        console.error('Error generating unique survey ID:', error);
        return null;
    }
}

export async function generateUniqueUserId() {
    try {
        let userId;
        do {
            userId = await generateRandomUid(8);
        } while (await doesUserIdExist(userId));
        return userId;
    } catch (error) {
        console.error('Error generating unique user ID:', error);
        return null;
    }
}


export async function sendQueryAndGetRes(query) {
    try {
        const response = await handleExecuteQuery(query);
        if (response.error) throw new Error(response.error);
        return response;
    } catch (error) {
        throw new Error(error);
    }
}

async function handleExecuteQuery(query) {
    try {
        const response = await fetch('http://localhost:3000/api/db', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ query }),
        });

        if (!response.ok) {
            throw new Error('Network response was not ok');
        }

        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Error:', error);
        throw error;
    }
}
