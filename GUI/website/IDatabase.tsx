// IDatabase.d.ts

interface IDatabase {
    open(): Promise<boolean>;
    close(): boolean;
    doesUserExist(name: string): boolean;
    doesPasswordMatch(name: string, password: string): boolean;
    addNewUser(name: string, password: string, uid: string, addresses: string): boolean;
    addUserToSurvey(surveyId: string, user: string): boolean;
    getUsersList(): Promise<string[]>;
    getNodeListForSurvey(surveyId: string): string;
    doesSurveyExist(surveyName: string): boolean;
    doesIdExist(surveyId: string): boolean;
    createNewSurvey(surveyId: string, users: string): Promise<boolean>;
    isNewSurveyByUid(uid: string): Promise<boolean>;
    getPeerByUid(uid: string): Promise<string>;
    changePeerByUid(uid: string, newPeer: string): Promise<boolean>;
}


export default IDatabase;
