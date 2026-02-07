
export type UserRole = 'admin' | 'police' | 'health';

export interface User {
  email: string;
  role: UserRole;
}

export interface AuthState {
  user: User | null;
  token: string | null;
  isAuthenticated: boolean;
  isLoading: boolean;
}

export interface VictimRequest {
  id: string;
  status: 'CRITICAL' | 'URGENT' | 'SAFE';
  needs: string;
  peopleCount: string;
  loc: string;
  timestamp: string;
}

export interface InventoryItem {
  name: string;
  quantity: number;
  unit: string;
}

export interface HealthInventoryItem {
  name: string;
  available: number;
  needed: number;
  unit: string;
}

export interface ChatMessage {
  from: 'admin' | 'police';
  text: string;
  timestamp: string;
}
