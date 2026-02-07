
import React, { useState } from 'react';
import { useAuth } from './context/AuthContext';
import LoginPage from './pages/LoginPage';
import SignupPage from './pages/SignupPage';
import VerifyEmailPage from './pages/VerifyEmailPage';
import DisasterDashboard from './pages/DisasterDashboard';
import PoliceDashboard from './pages/PoliceDashboard';
import HealthDashboard from './pages/HealthDashboard';

type AuthView = 'login' | 'signup' | 'verify';

const App: React.FC = () => {
  const { isAuthenticated, user, isLoading } = useAuth();
  const [view, setView] = useState<AuthView>('login');
  const [verifyEmail, setVerifyEmail] = useState('');

  const navigateTo = (newView: AuthView, email?: string) => {
    setView(newView);
    if (email) setVerifyEmail(email);
  };

  if (isLoading) {
    return (
      <div className="h-screen w-screen flex flex-col items-center justify-center bg-slate-900 gap-6">
        <div className="w-16 h-16 border-4 border-blue-500 border-t-transparent rounded-full animate-spin"></div>
        <div className="text-center">
            <h1 className="text-white font-black text-xl tracking-tight">SENTINEL-VAULT</h1>
            <p className="text-blue-500 text-[10px] font-bold uppercase tracking-widest mt-1">Establishing Secure Connection...</p>
        </div>
      </div>
    );
  }

  // Handle Authenticated Users
  if (isAuthenticated && user) {
    switch (user.role) {
      case 'admin':
        return <DisasterDashboard />;
      case 'police':
        return <PoliceDashboard />;
      case 'health':
        return <HealthDashboard />;
      default:
        return <LoginPage onNavigate={navigateTo} />;
    }
  }

  // Handle Unauthenticated Views
  switch (view) {
    case 'signup':
      return <SignupPage onNavigate={navigateTo} />;
    case 'verify':
      return <VerifyEmailPage email={verifyEmail} onNavigate={() => setView('login')} />;
    case 'login':
    default:
      return <LoginPage onNavigate={navigateTo} />;
  }
};

export default App;
