const sqlite3 = require('sqlite3');
const { access } = require('fs').promises;
const StringUtils = require('./StringUtils');

const DB_NAME = 'election.db';

const CREATE_USERS = `CREATE TABLE IF NOT EXISTS USERS(
                      NAME TEXT PRIMARY KEY NOT NULL,
                      PASSWORD TEXT NOT NULL,
                      UID TEXT NOT NULL,
                      ADDRESSES TEXT NOT NULL
                      );`;

const CREATE_SURVEYS = `CREATE TABLE IF NOT EXISTS SURVEYS(
                        SURVEY_ID TEXT PRIMARY KEY NOT NULL,
                        USERS TEXT NOT NULL
                        );`;

async function openDatabase() {
    try {
        await access(DB_NAME);
        await execAsync(CREATE_USERS);
        await execAsync(CREATE_SURVEYS);
        return new sqlite3.Database(DB_NAME);
    } catch (err) {
        console.error("Error accessing or creating DB:", err);
        return null;
    }
}

function closeDatabase(db) {
    try {
        db.close();
        return true;
    } catch (e) {
        console.error("Error closing DB:", e);
        return false;
    }
}

async function doesUserExist(db, name) {
    const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}';`;
    const user = await sendQueryAndGetAns(db, sqlStatement);
    return user !== "";
}

async function doesPasswordMatch(db, name, password) {
    const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}' AND PASSWORD = '${password}';`;
    return await sendQueryAndGetAns(db, sqlStatement) !== "";
}

async function addNewUser(db, name, password, uid, addresses) {
    const usersStatement = `INSERT INTO USERS(NAME, PASSWORD, UID, ADDRESSES) VALUES('${name}', '${password}', '${uid}', '${addresses}');`;
    return sendQuery(db, usersStatement);
}

async function addUserToSurvey(db, surveyId, user) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
    let existingUsers = await sendQueryAndGetAns(db, sqlStatement);
    existingUsers += `,${user}`;
    const updateStatement = `UPDATE SURVEYS SET USERS = '${existingUsers}' WHERE SURVEY_ID = '${surveyId}';`;
    return sendQuery(db, updateStatement);
}

async function getUidByUsername(db, username) {
    try {
        const sqlStatement = `SELECT UID FROM USERS WHERE NAME = '${username}';`;
        const result = await sendQueryAndGetAns(db, sqlStatement);
        return result !== "" ? result : null;
    } catch (error) {
        console.error('Error getting UID:', error);
        return null;
    }
}

async function getUsersList(db) {
    const sqlStatement = "SELECT NAME FROM USERS;";
    return new Promise((resolve, reject) => {
        if (!db) {
            reject("Database handle is null");
            return;
        }
        db.all(sqlStatement, [], (err, rows) => {
            if (err) {
                console.error("Error executing SQL statement:", err);
                reject(err);
            } else {
                const users = rows.map(row => row.NAME);
                resolve(users);
            }
        });
    });
}
async function getNodeListForSurvey(db, surveyId) {
    const sqlStatement = `SELECT P2P_NODES FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
    return await sendQueryAndGetAns(db, sqlStatement);
}

async function doesSurveyExist(db, surveyName) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_NAME = '${surveyName}';`;
    const survey = await sendQueryAndGetAns(db, sqlStatement);
    return survey !== "";
}

async function doesIdExist(db, surveyId) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
    const survey = await sendQueryAndGetAns(db, sqlStatement);
    return survey !== "";
}

async function createNewSurvey(db, users) {
    const string = generateUniqueSurveyId(db);
    const surveyStatement = `INSERT INTO SURVEYS(SURVEY_ID, USERS) VALUES('${string}', '${users}');`;
    return sendQuery(db, surveyStatement);
}

async function isNewSurveyByUid(db, uid) {
    const sqlStatement = `SELECT * FROM SURVEYS WHERE USERS LIKE '%${uid}%';`;
    const survey = await sendQueryAndGetAns(db, sqlStatement);
    return survey !== "";
}

async function getPeerByUid(db, uid) {
    const sqlStatement = `SELECT ADDRESSES FROM USERS WHERE UID = '${uid}';`;
    return await sendQueryAndGetAns(db, sqlStatement);
}

async function getPeersList(db) {
    try {
        const sqlStatement = "SELECT ADDRESSES FROM USERS;";
        const peers = await sendQueryAndGetAns(db, sqlStatement);
        const peersArray = peers.split(',');
        let formattedPeers = '';
        for (let i = 0; i < peersArray.length; i++) {
            formattedPeers += `${i + 1}: "${peersArray[i]}", `;
        }
        formattedPeers = formattedPeers.slice(0, -2);
        return formattedPeers;
    } catch (error) {
        console.error('Error getting peers list:', error);
        return '';
    }
}

async function changePeerByUid(db, uid, newPeer) {
    if (!isValidPeerFormat(newPeer)) {
        console.error("Invalid format for the new peer.");
        return false;
    }
    const sqlStatement = `UPDATE USERS SET ADDRESSES = '${newPeer}' WHERE UID = '${uid}';`;
    return sendQuery(db, sqlStatement);
}

function sendQuery(db, sqlStatement) {
    try {
        db.exec(sqlStatement);
        return true;
    } catch (err) {
        console.error(err.message);
        return false;
    }
}


async function sendQueryAndGetAns(db, sqlStatement) {
    let ans = "";
    return new Promise((resolve, reject) => {
        db.all(sqlStatement, [], (err, rows) => {
            if (err) {
                console.error("Error with sendQueryAndGetAns:", err);
                reject(err);
            } else {
                for (const row of rows) {
                    for (const value of Object.values(row)) {
                        ans += value;
                    }
                }
                resolve(ans);
            }
        });
    });
}

async function generateUniqueSurveyId(db) {
    let surveyId;
    do {
        surveyId = generateRandomSurveyId();
    } while (await doesIdExist(db, surveyId));
    return surveyId;
}

function generateRandomSurveyId() {
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    let result = '';
    for (let i = 0; i < 5; i++) {
        result += characters.charAt(Math.floor(Math.random() * characters.length));
    }
    return result;
}

async function execAsync(db, sqlStatement) {
    return new Promise((resolve, reject) => {
        db.exec(sqlStatement, (err) => {
            if (err) reject(err);
            else resolve();
        });
    });
}

function isValidPeerFormat(peer) {
    const pattern = /^\(\d+:\d+,\s*\d+:\d+\)$/;
    return pattern.test(peer);
}

module.exports = {
    openDatabase,
    closeDatabase,
    doesUserExist,
    doesPasswordMatch,
    addNewUser,
    addUserToSurvey,
    getUidByUsername,
    getUsersList,
    getNodeListForSurvey,
    doesSurveyExist,
    doesIdExist,
    createNewSurvey,
    isNewSurveyByUid,
    getPeerByUid,
    getPeersList,
    changePeerByUid
};
