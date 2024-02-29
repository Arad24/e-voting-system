import { createGlobalState } from 'react-hooks-global-state';

// Initialize storedUsername and storedUid from localStorage
let storedUsername = '';
let storedUid = '';

if (typeof window !== "undefined") {
  storedUsername = localStorage.getItem("username") || '';
}

const initialState = {
  username: storedUsername,
  uid: storedUid,
};

const { useGlobalState, setGlobalState } = createGlobalState(initialState);

export { useGlobalState, setGlobalState };