import { create } from 'zustand';

interface UsernameState {
  global_username: string | null;
  setUsername: (username: string | null) => void;
}

export const useGlobalStore = create<UsernameState>((set) => {
  // Check if localStorage is available (client-side)
  const initialUsername = typeof window !== 'undefined' ? localStorage.getItem("username") : null;

  return {
    global_username: initialUsername,
    setUsername: (newUsername) => {
      // Set localStorage only if it's available
      if (typeof window !== 'undefined') {
        localStorage.setItem("username", newUsername || ''); // Ensure newUsername is not null
      }
      set({ global_username: newUsername });
    },
  };
});