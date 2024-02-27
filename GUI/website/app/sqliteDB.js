import IDatabase from './IDatabase'; // Assuming you have IDatabase interface in a separate file
import sqlite3 from 'sqlite3';
import { access } from 'fs/promises'; // Importing from the promise-based version of fs
import { promisify } from 'util'; // Importing promisify from util
import 'StringUtils';

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

interface UserRow {
  NAME: string;
}


class SqliteDatabase implements IDatabase {
    private dbHandle: sqlite3.Database;

    constructor() {
      this.dbHandle = new sqlite3.Database(DB_NAME);
      this.open();
  }

  async open(): Promise<boolean> {
    try {
        await access(DB_NAME); // Await the result of the access function
        await this.execAsync(CREATE_USERS);
        await this.execAsync(CREATE_SURVEYS);
        return true;
    } catch (err) {
        console.error("Error accessing or creating DB:", err); // Log any errors during file access
        return false; // Return false if an error occurred
    }
}

    close(): boolean {
        try {
            this.dbHandle.close();
            return true;
        } catch (e) {
            console.error("Error closing DB:", e);
            return false;
        }
    }

    doesUserExist(name: string): boolean {
      const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}';`;
      const user = this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
      return user !== "";
    }

    doesPasswordMatch(name: string, password: string): boolean {
      const sqlStatement = `SELECT * FROM USERS WHERE NAME = '${name}' AND PASSWORD = '${password}';`;
      return this.sendQueryAndGetAns(this.dbHandle, sqlStatement) !== "";
    }

    addNewUser(name: string, password: string, uid: string, addresses: string): boolean {
      const usersStatement = `INSERT INTO USERS(NAME, PASSWORD, UID, ADDRESSES) VALUES('${name}', '${password}', '${uid}', '${addresses}');`;
      return this.sendQuery(this.dbHandle, usersStatement);
    }

    addUserToSurvey(surveyId: string, user: string): boolean {
      const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
      let existingUsers = this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
      existingUsers += `,${user}`;
      const updateStatement = `UPDATE SURVEYS SET USERS = '${existingUsers}' WHERE SURVEY_ID = '${surveyId}';`;
      return this.sendQuery(this.dbHandle, updateStatement);
    }

    async getUserUid(username, password): Promise<string | null> {
      try {
          const sqlStatement = `SELECT UID FROM USERS WHERE NAME = '${username}' AND PASSWORD = '${password}';`;
          const result = await this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
          return result !== "" ? result : null; // Return the UID if found, otherwise null
      } catch (error) {
          console.error('Error getting UID:', error);
          return null; // Return null if there was an error getting the UID
      }
  }

getUsersList(): Promise<string[]> {
  const sqlStatement = "SELECT NAME FROM USERS;";
  return new Promise((resolve, reject) => {
    if (!this.dbHandle) {
      reject("Database handle is null");
      return;
    }
    this.dbHandle.all(sqlStatement, [], (err, rows: UserRow[]) => {
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


getNodeListForSurvey(surveyId: string): string {
  const sqlStatement = `SELECT P2P_NODES FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
  return this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
}


doesSurveyExist(surveyName: string): boolean {
  const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_NAME = '${surveyName}';`;
  const survey = this.sendQueryAndGetAns(this.dbHandle, sqlStatement);

  return survey !== "";
}


doesIdExist(surveyId: string): boolean {
  const sqlStatement = `SELECT * FROM SURVEYS WHERE SURVEY_ID = '${surveyId}';`;
  const survey = this.sendQueryAndGetAns(this.dbHandle, sqlStatement);

  return survey !== "";
}


async createNewSurvey(users: string): Promise<boolean> {
  const string = this.generateUniqueSurveyId();
  const surveyStatement = `INSERT INTO SURVEYS(SURVEY_ID, USERS) VALUES('${string}', '${users}');`;
  return this.sendQuery(this.dbHandle, surveyStatement);
}

async isNewSurveyByUid(uid: string): Promise<boolean> {
  const sqlStatement = `SELECT * FROM SURVEYS WHERE USERS LIKE '%${uid}%';`;
  const survey = await this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
  return survey !== "";
}

async getPeerByUid(uid: string): Promise<string> {
  const sqlStatement = `SELECT ADDRESSES FROM USERS WHERE UID = '${uid}';`;
  return await this.sendQueryAndGetAns(this.dbHandle, sqlStatement);
}

async getPeersList(): Promise<string> {
  try {
      const sqlStatement = "SELECT ADDRESSES FROM USERS;";
      const peers = await this.sendQueryAndGetAns(this.dbHandle, sqlStatement);

      // Split the peers string into an array
      const peersArray = peers.split(',');

      // Create a formatted string with the peers
      let formattedPeers = '';
      for (let i = 0; i < peersArray.length; i++) {
          formattedPeers += `${i + 1}: "${peersArray[i]}", `;
      }

      // Remove the trailing comma and space
      formattedPeers = formattedPeers.slice(0, -2);

      return formattedPeers;
  } catch (error) {
      console.error('Error getting peers list:', error);
      return ''; // Return an empty string if there was an error
  }
}

async changePeerByUid(uid: string, newPeer: string): Promise<boolean> {
  // Check if the new peer string is in the correct format
  if (!this.isValidPeerFormat(newPeer)) {
      console.error("Invalid format for the new peer.");
      return false;
  }

  const sqlStatement = `UPDATE USERS SET ADDRESSES = '${newPeer}' WHERE UID = '${uid}';`;
  return this.sendQuery(this.dbHandle, sqlStatement);
}

sendQuery(db: sqlite3.Database, sqlStatement: string): boolean {
  try {
      db.exec(sqlStatement);
      return true; // Return true if the query is executed successfully
  } catch (err: any) {
      console.error(err.message);
      return false; // Return false if there's an error
  }
}


sendQueryAndGetAns(db: sqlite3.Database, sqlStatement: string): string {
  let ans = "";

  // Execute the SQL statement
  db.all(sqlStatement, [], (err: Error | null, rows: any[]) => {
      if (err) {
          console.error("Error with sendQueryAndGetAns:", err);
          return;
      }

      // Process the rows returned by the query
      for (const row of rows) {
          for (const value of Object.values(row)) {
              ans += value;
          }
      }
  });

  return ans;
}

async generateUniqueSurveyId(): Promise<string> {
  let surveyId: string;
  do {
      surveyId = this.generateRandomSurveyId();
  } while (await this.doesIdExist(surveyId));
  return surveyId;
}

generateRandomSurveyId(): string {
  const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  let result = '';
  for (let i = 0; i < 5; i++) {
      result += characters.charAt(Math.floor(Math.random() * characters.length));
  }
  return result;
}



private isValidPeerFormat(peer: string): boolean {
  // Define a regex pattern to match the required format
  const pattern = /^\(\d+:\d+,\s*\d+:\d+\)$/;

  // Test if the peer string matches the pattern
  return pattern.test(peer);
}

private async execAsync(sqlStatement: string): Promise<void> {
  return new Promise((resolve, reject) => {
      this.dbHandle.exec(sqlStatement, (err) => {
          if (err) reject(err);
          else resolve();
      });
  });
}



}
export default SqliteDatabase;
