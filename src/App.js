import {BrowserRouter as Router,Switch,Route} from 'react-router-dom';
import './App.scss';
import DashboardContainer from './containers/dashboard/dashboard';

function App() {
  return (
    <div className="App">
        
        <Router>
          <div className = "content">
            <Switch>
              <Route exact path="/" render={() => <DashboardContainer />}></Route>
            </Switch>
          </div>
        </Router>
    </div>
  );
}

export default App;
