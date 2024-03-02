import sqlite3 from "sqlite3";
import { open } from "sqlite";

const DB_NAME = 'election.db';

const CREATE_USERS = `CREATE TABLE IF NOT EXISTS USERS(
                      NAME TEXT PRIMARY KEY NOT NULL,
                      PASSWORD TEXT NOT NULL,
                      UID TEXT NOT NULL,
                      ADDRESSES TEXT NOT NULL
                      );`;

const CREATE_SURVEYS = `CREATE TABLE IF NOT EXISTS SURVEYS(
                        SURVEY_ID TEXT PRIMARY KEY NOT NULL,
                        SURVEY_NAME TEXT NOT NULL,
                        SURVEY_OPTIONS TEXT NOT NULL
                        );`;



let db;

export async function openDatabase() {
    try {
        const database = await open({
            filename: DB_NAME,
            driver: sqlite3.Database,
        });
        
        // Execute the CREATE TABLE statements for USERS and SURVEYS
        await database.exec(CREATE_USERS);
        await database.exec(CREATE_SURVEYS);
        
        return database;
    } catch (error) {
        console.error('Error opening database:', error);
        throw error;
    }
}


export async function POST(req, res) {
    // Open the database connection
    if (!db) db = await openDatabase();
  
    try {
        const { query } = await req.json();
        console.log(query);
        
        const result = await getFromSql(query);
        
        console.log(result)
        if (result.error) {
            return new Response(JSON.stringify({ error: result.error }), {
                headers: { "Content-Type": "application/json" },
                status: 500,
            });
        } else {
            return new Response(JSON.stringify({ result: result.res }), 
            {
                headers: { "Content-Type": "application/json" },
                status: 200,
            });
        }
    } catch (error) {
        console.error('Error:', error);
        return new Response(JSON.stringify({ error: "Internal Server Error" }), {
            headers: { "Content-Type": "application/json" },
            status: 500,
        });
    }
}

async function getFromSql(query) {
    try {
        const result = await db.all(query);
        console.log(result)
        return { res: result };
    } catch (error) {
        return { error: error.message };
    }
}