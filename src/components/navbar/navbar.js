import React from 'react';
import { Row, Col, Container } from 'react-bootstrap';
import './navbar.scss';
import Button from '@material-ui/core/Button';
import { Link } from 'react-router-dom'
import logo from '../../assets/simple_logo.png';
const Navbar = ({ user, setUser }) => {

  // console.log(logo);
  return (
    <Container>
        <Row className='Navbar'>
            <Col md={8} className='Slogan'>
                {/* <Button><Link to='/' className='navbar-text'></Link></Button> */}
                <a href ="/">
                    <img src={logo} alt='LOGO.PNG' width="50" height="50">
                    </img>
                </a>
            </Col>
            <Col className='Client'>
                <Button><Link to='/' className='navbar-text'>Homepage</Link></Button>
            </Col>
            <Col >
                <Button><Link to='/dashboard' className='navbar-text'>Dashboard</Link></Button>
            </Col>
            <Col >
                <Button ><Link to='/aboutUs' className='navbar-text' >About us</Link></Button>
            </Col>
        </Row>
    </Container>
  )
}
export default Navbar;